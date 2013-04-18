#include <Servo.h> 
#include <aJSON.h>
#include "testData.h"
#include "Spot.h"

//I/O pins
Servo myServo;
const int servoPin = 2;
const int spotBtn = 3;
const int metricBtn = 4;

//The info for the metrics shown
enum metric {SWELL, WIND, TIDE};
metric curMetric;
metric prevMetric;

//General constants
const int numSpots = 3;
Spot spots[numSpots];
const String spotNames[] = {"Mavericks", "El Porto", "Lower Trestles"};

//char** jsonFilter = {"forecast", "hour", "size", "size_ft", "tide", "tide_meters", "wind", "speed_kts", "speed_mpg", NULL};

const int transitionTime = 1500;
int spotIndex = 0;


//Initialization code
void setup() {
  myServo.attach(servoPin);
  
  pinMode(spotBtn, INPUT);
  pinMode(metricBtn, INPUT);
  
  setupSpots();
}

void setupSpots() {
  for (int i = 0; i < numSpots; i++) {
    spots[i] = setupSpot(spotNames[i]); 
  }
}

Spot setupSpot(String spotName) {  
  Spot newSpot;
  newSpot.name = spotName;
  
  char* json = getSpotJSON(spotName);
  aJsonObject* root = aJson.parse(json);//, jsonFilter);

  parseMetric(root, "forecast", "size_ft", newSpot.swellValues);
  parseMetric(root, "tide", "tide", newSpot.tideValues);
  parseMetric(root, "wind", "speed_mph", newSpot.windValues);
  
  return newSpot;
}

void parseMetric(aJsonObject* root, char* metric, char* attribute, int* spotArr) {
  aJsonObject* values = aJson.getObjectItem(root, metric);
  aJsonObject* v = values->child;
  
  int maxValue = 0;
  int minValue = 0;
  int columnIndex = 0;
  int counter = 0;
  while (v != NULL && columnIndex < numColumns) {
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
  
  //if spotBtn is pressed
    //change spotIndex
    //transition
    
  //if metricBtn is pressed
    //change metric
    //transition
    
  //every N minutes, setupSpots()
}

//coninuous rotation servos range from 
//0 (full speed one way)
//90 (no movement)
//180 (full speed other way)
void transition() {
  //in the allotted time
    //for each column
      //set speed/direction of spin based difference btwn current pos and desired pos
}
