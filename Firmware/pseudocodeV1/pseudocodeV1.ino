void setup() {
  // put your setup code here, to run once:

}


uint8_t state = 0;
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
          [start logging data ASAP]
          updateState(RUNNING);
    case RUNNING:
      // grab data from accel and write to mem as fast as possible

      // ask accelerometer for new data
        // write all new data to memory and update counter
        // during this, check if we've reached end of acq, if so
          updateState(DONE);
    case DONE:
      // check for i2c message
        // upload data over i2c
        // checksum
        updateState(READY);
  }
}

void updateState(uint8_t newState) {
  [GLOBAL] state = newState;
  updateLedState(state);
}

void updateLedState(uint8_t state) {
  switch(state) {
    case ...
  }
}

void updateSettings(uint16_t duration, uint16_t frequency) {
  [check if duration is possible]
    [convert duration to # of samples]
    [save duration to global variable]
  [change freq on accelerometer]
  
}
