#include "lis2de12_reg.h"

// must implement these functions in order to use lis2de12 library!

/*
 * @brief  Write generic device register (platform dependent)
 *
 * @param  handle    customizable argument. In this examples is used in
 *                   order to select the correct sensor bus handler.
 * @param  reg       register to write
 * @param  bufp      pointer to data to write in register reg
 * @param  len       number of consecutive register to write
 *
 */

int32_t platform_write(void *handle, uint8_t Reg, const uint8_t *Bufp, uint16_t len) {
  // write to registers over SPI, TODO
  reg |= 0x40;
  HAL_GPIO_WritePin(CS_up_GPIO_Port, CS_up_Pin, GPIO_PIN_RESET);
  HAL_SPI_Transmit(handle, &reg, 1, 1000);
  HAL_SPI_Transmit(handle, (uint8_t*) bufp, len, 1000);
  HAL_GPIO_WritePin(CS_up_GPIO_Port, CS_up_Pin, GPIO_PIN_SET);
}
int32_t platform_read(void *handle, uint8_t Reg, uint8_t *Bufp, uint16_t len) {
  // read from registers using SPI, TODO
  reg |= 0xC0;
  HAL_GPIO_WritePin(CS_up_GPIO_Port, CS_up_Pin, GPIO_PIN_RESET);
  HAL_SPI_Transmit(handle, &reg, 1, 1000);
  HAL_SPI_Receive(handle, bufp, len, 1000);
  HAL_GPIO_WritePin(CS_up_GPIO_Port, CS_up_Pin, GPIO_PIN_SET);
}

stmdev_ctx_t hardware_ctx; // object storing hardware-specific read/write functions
hardware_ctx.write_reg = platform_write;
hardware_ctx.read_reg = platform_read;


void setup() {
  // put your setup code here, to run once:

}

/* GLOBAL VARIABLES */
uint8_t state = 0;
uint32_t recordDuration = 0;


typedef enum {
  NOT_INIT,
  READY,
  RUNNING,
  DONE
} states_t;

/* states:
 *  - NOT_INIT = just turned on, not yet initialized, or not properly init.
 *  - READY = initialized, ready for acq
 *  - RUNNING = acq triggered, currently collecting data
 *  - DONE = acq finished, ready to upload results
 */

void loop() {
  checkI2C();
  switch(state) {
    case NOT_INIT:
      // only end up here in loop as an error state
      break;
    case READY:
      // check if we've received an I2C message
        // ask for readiness message
        // change settings broadcast
          updateSettings(duration, frequency);
        // start acquisition broadcast
          updateState(RUNNING);
      break;
    case RUNNING:
      // grab data from accel and write to mem as fast as possible

      // ask accelerometer for new data
      [IF FIFO AVAILABLE > 0]
        lis2de12_acceleration_raw_get(hardware_ctx, [ARRAY_PTR]);
        // write all new data to memory and update counter
        // during this, check if we've reached end of acq, if so
          updateState(DONE);
      break;
    case DONE:
      // check for i2c message
        // upload data over i2c
        // checksum
        updateState(READY);
      break;
  }
}

void updateState(uint8_t newState) {
  [GLOBAL] state = newState;
  updateLedState(state);
}

void updateLedState(uint8_t state) {
  switch(state) {
    case NOT_INIT:
      setLedColor(0xff0000);
      break;
    case READY:
      setLedColor(0xff00ff);
      break;
    case RUNNING:
      setLedColor(0x0000ff);
      break;
    case DONE:
      setLedColor(0x00ff00);
      break;
  }
}

void setLedColor(uint32_t color) {
  // take color of format 0xRRGGBB and send to addressable LED TODO
}

void updateSettings(uint32_t newDuration, uint8_t frequency = 0x09, uint8_t scale = 3) {
  // duration is acquisition length in # of samples
  // frequency is a 1-byte value corresponding to sample freq (see line 754 of lis2de12_reg.h)
  // - 0x00 powerdown, 0x01 1 Hz, 0x09 5.xxx kHz
  // scale is a 1-byte value corresponding to scale (2g = 0, 4g = 1, 8g = 2, 16g = 3) 
  // TODO input validation, if program has room. Otherwise verification must be done at higher level
  recordDuration = newDuration;
  // change accelerometer params
  lis2de12_data_rate_set(hardware_ctx, frequency);
  lis2de12_full_scale_set(hardware_ctx, scale);
}
