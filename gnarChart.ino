#include <Servo.h> 
#include <aJSON.h>
#include "pololu.h"
#include "switchTracker.h"
#include "Spot.h"
#include "testData.h"

//The circumference of a servo spindle
#define CIR 3.14
//The time it takes for each servo to fully rotate once.
//The servos rotate at different speeds, which is probably an
//issue with how I modified them, but whatever.
const int rotationSpeed[] = {1700, 1500, 1500, 1500, 1500, 1500, 1500, 1500};

//I/O pins
const int spotPin = 3;
switchTracker spotTrack;
const int metricPin = 4;
switchTracker metricTrack;

//The info for the metrics shown
int curMetric;
int newMetric;

//General constants
const String spotNames[] = {"Mavericks", "El Porto", "Lower Trestles"};
Spot spots[SPOTS];
int spotIndex;

//char** jsonFilter = {"forecast", "hour", "size", "size_ft", "tide", "tide_meters", "wind", "speed_kts", "speed_mpg", NULL};


//Initialization code
void setup() {
  Serial.begin(9600);
  
  initSwitchTrack(spotTrack, spotPin);
  initSwitchTrack(metricTrack, metricPin);
  
  setupSpots();
  
  spotIndex = 0;
  curMetric = 0;
  newMetric = -1;
}

void setupSpots() {
  for (int i = 0; i < SPOTS; i++) {
    spots[i] = setupSpot(spotNames[i]); 
  }
}

Spot setupSpot(String spotName) {  
  Spot newSpot;
  newSpot.name = spotName;
  
  char* json = getSpotJSON(spotName);
  aJsonObject* root = aJson.parse(json);//, jsonFilter);

  parseMetric(root, "forecast", "size_ft", newSpot.values[SWELL]);
  parseMetric(root, "tide", "tide", newSpot.values[TIDE]);
  parseMetric(root, "wind", "speed_mph", newSpot.values[WIND]);
  
  return newSpot;
}

void parseMetric(aJsonObject* root, char* metric, char* attribute, int* spotArr) {
  aJsonObject* values = aJson.getObjectItem(root, metric);
  aJsonObject* v = values->child;
  
  int maxValue = 0;
  int minValue = 0;
  int columnIndex = 0;
  int counter = 0;
  while (v != NULL && columnIndex < COLUMNS) {
    int curValue = aJson.getObjectItem(v, attribute)->valueint;
    spotArr[columnIndex] += curValue;
    counter++;
    
    if (curValue < minValue) minValue = curValue;
    if (curValue > maxValue) maxValue = curValue;
        
    if (counter >= columnSpan) {
      spotArr[columnIndex] /= columnSpan;
      columnIndex++;
      counter = 0;
    }        

    v = v->next;
  }
}

char* getSpotJSON(String spotName) {
  if (spotName == "Mavericks") return json_mavericks;
  else if (spotName == "Lower Trestles") return json_trestles;
  else if (spotName == "El Porto") return json_porto;
  else return "";
}

/* Main routine (called repeated by from the Arduino framework) */
void loop() {
  //light LEDs
  
  if (switchValidPress(spotTrack)) {
    //change spotIndex
    if (spotIndex++ >= SPOTS) spotIndex = 0; 
    transition();
  }
  
  if (switchValidPress(metricTrack)) {
    //set new metric
    newMetric = curMetric + 1;
    if (newMetric > TIDE) newMetric = SWELL;
    transition();
    curMetric = newMetric;
    newMetric = -1;
  }
  
  //every N minutes, setupSpots()
}

void transition() {
  //in the allotted time
    //for each column
      //set speed/direction of spin based difference btwn current pos and desired pos
  for (int i = 0; i < COLUMNS; i++) {
    int direc = ROTATE_RIGHT;
    
    int curValue = 0;//TODO
    int newValue = 0;//TODO
    
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
}
