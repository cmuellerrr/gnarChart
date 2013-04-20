#define SPOTS 3
#define METRICS 3
#define COLUMNS 8

enum {SWELL, WIND, TIDE};
const int columnSpan = 24 / COLUMNS;


//Define a spot (aka location)
typedef struct Spot {
  String name;                   // The name of the spot
  int values[METRICS][COLUMNS];  // The metric values for this spot
                                 // Metric indexs match that of the enum
};
