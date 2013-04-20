/* Structure to track and debounce the state of a push button switch */
typedef struct switchTracker {
  int lastReading;       // last raw value read
  long lastChangeTime;   // last time the raw value changed
  byte pin;              // the pin this is tracking changes on
  byte switchState;      // debounced state of the switch
};


/* Fill in a switchTrack structure to start tracking/debouncing a switch
   on the given pin.
*/
void initSwitchTrack(struct switchTracker &sw, int swPin) {  
  pinMode(swPin, INPUT);
  sw.lastReading = digitalRead(swPin);
  sw.lastChangeTime = millis();
  sw.pin = swPin;
  sw.switchState = sw.lastReading;
}


/*-------------------------------------------*/
/* Using a time based debounce, test whether a switch should be considered to have 
   changed state since the last call to this routine for the switch, and track the 
   current state of the switch in the given switch tracking structure (sw).  The switch 
   will be consided to be in a new state only after it has held that state for debounceTime msec 
   or longer.  sw.switchState will be set to 1 when the switch should be considered pressed 
   and 0 when it should be considered released.  This routine will return true when the 
   switch is in a new state compared to the last call to this routine, false otherwise.
   
   Note: this routine must be called frequently to operate correctly.
   Note: this code is here to make the demo a little more stand alone (nothing to import),
         but in other cases you might want to consider using the code from Bounce.h instead.
*/
boolean switchChange(struct switchTracker &sw) { 
  // switch must stay stable this long (in msec) to register
  const long debounceTime = 100;   
  
  // default to no change until we find out otherwise
  boolean result = false; 
  
  // get the current raw reading from the switch
 int reading = digitalRead(sw.pin);

  // if the raw reading has changed (by noise or actual press/release) reset change time to now
  if (reading != sw.lastReading) sw.lastChangeTime = millis();
  sw.lastReading = reading;
  
  // if time since the last change is longer than the required dwell
  if ((millis() - sw.lastChangeTime) > debounceTime) {
    // note whether we are changing
    result = (reading != sw.switchState);
    // in any case the value has been stable and so the reported state 
    // should now match the current raw reading
    sw.switchState = reading;
  }
  return result;
}

boolean switchValidPress(struct switchTracker &sw) {
  return switchChange(sw) && sw.switchState == 1;
}
