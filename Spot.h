const int numColumns = 8;
const int columnSpan = 24 / numColumns;

//Define a spot (or location)
typedef struct {
  String name;                   // The name of the spot
  int swellValues[numColumns];   // The column values for the swell
  int windValues[numColumns];    // The column values for wind
  int tideValues[numColumns];    // The column values for the tide
} Spot;
