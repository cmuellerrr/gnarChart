#include <Servo.h> 
#include <aJSON.h>
#include "pololu.h"
#include "switchTracker.h"
#include "Spot.h"
#include "testData.h"

//The circumference of a servo spindle
#define CIR 3.14
#define HEIGHT 12

//The time it takes for each servo to fully rotate once.
//The servos rotate at different speeds, which is probably an
//issue with how I modified them, but whatever.
const int rotationSpeed[] = {1700, 1400, 1700, 1600, 1700, 1500, 1500, 1500};

//I/O pins
const int spotPin = 3;
switchTracker spotTrack;
const int metricPin = 4;
switchTracker metricTrack;

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
  
  setupSpots();
  
  curSpot = -1;
  curMetric = -1;
  transition(0, SWELL);
}

/*
 * Setup all spots for the chart.
 */
void setupSpots() {
  for (int i = 0; i < NUM_SPOTS; i++) {
    setupSpot(i, spotNames[i]);
  }
  /*
  for (int i = 0; i < NUM_COLUMNS; i++) {
     spots[0].values[SWELL][i] = 3;
     spots[0].values[TIDE][i] = 6;
     spots[0].values[WIND][i] = 9; 
  }
  for (int i = 0; i < NUM_COLUMNS; i++) {
     spots[1].values[SWELL][i] = 9;
     spots[1].values[TIDE][i] = 3;
     spots[1].values[WIND][i] = 6; 
  }
  for (int i = 0; i < NUM_COLUMNS; i++) {
     spots[2].values[SWELL][i] = 6;
     spots[2].values[TIDE][i] = 9;
     spots[2].values[WIND][i] = 3; 
  }*/
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
  //aJson.deleteItem(root);
}

/*
 * From the given json object root, parse the given metric into the 
 * array specified by the given pointer.
 */
void parseMetric(aJsonObject* root, char* metric, int spotIndex, int metricIndex) {
  aJsonObject* values = aJson.getObjectItem(root, metric);
  aJsonObject* v = values->child;
  
  int maxValue = 10;
  if (metric == "wind") maxValue = 20;
  
  int columnIndex = 0;
  while (v != 0 && columnIndex < NUM_COLUMNS) {
    
    float curValue = (float)v->valueint;
    curValue = (curValue / maxValue) * HEIGHT;
    if (curValue < 0) curValue = 0;
    if (curValue > maxValue) curValue = maxValue;
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
  if (spotName == "Mavericks") return json_mavericks;
  else if (spotName == "Lower Trestles") return json_trestles;
  else if (spotName == "El Porto") return json_porto;
  else return "";
}

/* Main routine (called repeated by from the Arduino framework) */
void loop() {
  //light LEDs
  
  if (switchValidPress(spotTrack) && switchValidPress(metricTrack)) {
    transition(-1, -1); 
  }
  
  if (switchValidPress(spotTrack)) {
    //change spotIndex
    int newSpot = curSpot + 1;
    if (newSpot >= NUM_SPOTS) newSpot = 0; 
    transition(newSpot, curMetric);
  }
  
  if (switchValidPress(metricTrack)) {
    //set new metric
    int newMetric = curMetric + 1;
    if (newMetric > TIDE) newMetric = SWELL;
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
  for (int i = 0; i < NUM_COLUMNS; i++) {
    int direc = ROTATE_RIGHT;
    
    //TODO this could get sped up
    float curValue;
    if (curSpot < 0 || curMetric < 0) curValue = 0;
    else curValue = spots[curSpot].values[curMetric][i];
    
    //Reset to 0s if both are negative
    float newValue;
    if (newSpot < 0 && newMetric < 0) newValue = 0;
    else newValue = spots[newSpot].values[newMetric][i];
    
    float delta = newValue - curValue;
    
    if (delta < 0) {
       direc = ROTATE_LEFT;
       delta *= -1;
    } 
    
    float transitionTime = (delta / CIR) * rotationSpeed[i];
    servoPut(i, direc);
    delay(transitionTime);
    servoOff(i);
  }
  
  curSpot = newSpot;
  curMetric = newMetric;
}
