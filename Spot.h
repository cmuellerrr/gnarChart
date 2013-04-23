/*
 * A structure to represent a surf spot. It has a name and 
 * and array of values corresponding to the machine's columns.
 */
#define NUM_SPOTS 3
#define NUM_METRICS 3
#define NUM_COLUMNS 8

enum {SWELL, TIDE, WIND};
const int columnSpan = 24 / NUM_COLUMNS;


/* Define a spot (aka location)
 * name: the name of the spot
 * values: the metric values for this spot
 *         metric indexes match the enum
 */
typedef struct Spot {
  String name;
  float values[NUM_METRICS][NUM_COLUMNS];
};
