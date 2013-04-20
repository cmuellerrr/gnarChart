#define SPOTS 3
#define COLUMNS 8
const int columnSpan = 24 / COLUMNS;

//Define a spot (or location)
typedef struct Spot{
  String name;                   // The name of the spot
  int swellValues[COLUMNS];   // The column values for the swell
  int windValues[COLUMNS];    // The column values for wind
  int tideValues[COLUMNS];    // The column values for the tide
} ;
