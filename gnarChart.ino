/*
 * The Gnar Chart!
 * Display surf conditions using a mechanical bar graph.  The machine
 * holds 3 locations around Californaia and displays the swell, tide,
 * and wind conditions for the day.  The points are displayed by 
 * moving fabric boxes up and down based on the relative value to display,
 * The swells and tides have a bound set at 10ft while wind has a 
 * upper bound of 20mph.  These are slightly arbitrary, but grounded in 
 * the fact that anything above those values is, ya know, big.
 *
 * The machine makes use of the Pololu servo controller to drive all 
 * servo rotation.
 *
 *
 * NOTE: I wasn't able to get a wifi card, so I'm Wizard of Oz-ing 
 * the JSON data.  BUT - it is actually parsing JSON.  I build some
 * JavaScript (included) to retreive the data and massage it into 
 * the appropriate format.
 */


#include <Servo.h> 
#include <aJSON.h>
#include "pololu.h"
#include "switchTracker.h"
#include "Spot.h"
#include "testData.h"

//The circumference of a servo spindle
#define CIR .785
#define HEIGHT 10

//I/O pins
const int metricPin = 3;
switchTracker metricTrack;
const int spotPin = 4;
switchTracker spotTrack;
const int resetPin = 5;
switchTracker resetTrack;

//General constants for spots and metrics
const String spotNames[] = {"Mavericks", "El Porto", "Lower Trestles"};
Spot spots[NUM_SPOTS];
int curSpot;
int curMetric;

//Initialization code
void setup() {
  Serial.begin(9600);
  
  initSwitchTrack(spotTrack, spotPin);
  initSwitchTrack(metricTrack, metricPin);
  initSwitchTrack(resetTrack, resetPin);
  
  setupSpots();
  
  curSpot = -1;
  curMetric = -1;
}

/*
 * Setup all spots for the chart.
 */
void setupSpots() {
  for (int i = 0; i < NUM_SPOTS; i++) {
    setupSpot(i, spotNames[i]);
  }
}

/*
 * Setup a spot object representing the given spot name.
 * Get the json representation and parse the values into
 * their appropriate arrays.
 */
void setupSpot(int spotIndex, String spotName) {  
  spots[spotIndex].name = spotName;
  
  char* json = getSpotJSON(spotName);
  aJsonObject* root = aJson.parse(json);

  parseMetric(root, "forecast", spotIndex, SWELL);
  parseMetric(root, "tide", spotIndex, TIDE);
  parseMetric(root, "wind", spotIndex, WIND);
  
  aJson.deleteItem(root);
}

/*
 * From the given json object root, parse the given metric into the 
 * array specified by the given pointer.
 */
void parseMetric(aJsonObject* root, char* metric, int spotIndex, int metricIndex) {
  aJsonObject* values = aJson.getObjectItem(root, metric);
  aJsonObject* v = values->child;
  
  //Set upper bounds
  int maxValue = 10;
  if (metricIndex == WIND) {
    maxValue = 20;
  }
  
  int columnIndex = 0;
  while (v != 0 && columnIndex < NUM_COLUMNS) {
    
    float curValue = (float)v->valueint;
    curValue = (curValue / maxValue) * HEIGHT;
    if (curValue < 0) {
      curValue = 0;
    }
    if (curValue > maxValue) {
      curValue = maxValue;
    }
    spots[spotIndex].values[metricIndex][columnIndex] = curValue;
      
    columnIndex++;
    v = v->next;
  }
}

/*
 * Get the full json representation of the given spot.
 * Right now it is hard coded because I dont have a wifi
 * shield.
 */
char* getSpotJSON(String spotName) {
  if (spotName == "Mavericks") {
    return json_mavericks;
  } else if (spotName == "El Porto") {
    return json_porto;
  } else if (spotName == "Lower Trestles") {
    return json_trestles;
  } else {
    return "";
  }
}

/* Main routine (called repeated by from the Arduino framework) 
 * Handle the button presses to reset and cycle between spots and
 * metrics.
 */
void loop() {
  //light LEDs
  
  if (switchValidPress(resetTrack)) {
    transition(-1, -1); 
  }
  
  if (switchValidPress(spotTrack)) {
    //change spotIndex
    int newSpot = curSpot + 1;
    if (newSpot >= NUM_SPOTS) {
      newSpot = 0; 
    }
    if (curMetric < 0) {
       curMetric = 0; 
    }
    transition(newSpot, curMetric);
  }
  
  if (switchValidPress(metricTrack)) {
    //set new metric
    int newMetric = curMetric + 1;
    if (newMetric > WIND) {
      newMetric = SWELL;
    }
    if (curSpot < 0) {
        curSpot = 0;
    }
    transition(curSpot, newMetric);
  }
  
  //every N minutes, setupSpots()
}

/*
 * Transition the chart to the new spot and new metric.
 * For each column, look up the delta in height, set the direction 
 * to spin, and the duration for which to spin.
 *
 * Determine the time based off of the predetermined rotation
 * speed and the size of the spindle in relation to the height delta.
 */
void transition(int newSpot, int newMetric) {  
  //Serial.println("Transition");
  //Serial.println(newSpot);
  //Serial.println(newMetric);
  
  for (int i = 0; i < NUM_COLUMNS; i++) {
    int direc = ROTATE_RIGHT;
    
    //TODO this could get sped up
    float curValue = 0;
    if (curSpot < 0 || curMetric < 0) {
      curValue = 0;
    } else {
      curValue = spots[curSpot].values[curMetric][i];
    }
    
    //Reset to 0s if both are negative
    float newValue = curValue;
    if (newSpot < 0 && newMetric < 0) {
      newValue = 0;
    } else {
      newValue = spots[newSpot].values[newMetric][i];
    }
    
    //Rotate left if you are lowering
    float delta = newValue - curValue;
    if (delta < 0) {
       direc = ROTATE_LEFT;
       delta *= -1;
    } 
    
    float transitionTime = (delta / CIR) * 1500;
    
    servoPut(i, direc);
    delay(transitionTime);
    servoOff(i);
  }
  
  curSpot = newSpot;
  curMetric = newMetric;
}
