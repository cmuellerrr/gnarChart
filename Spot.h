#define NUM_SPOTS 3
#define NUM_METRICS 3
#define NUM_COLUMNS 8

enum {SWELL, WIND, TIDE};
const int columnSpan = 24 / NUM_COLUMNS;


/* Define a spot (aka location)
 * name: the name of the spot
 * values: the metric values for this spot
 *         metric indexes match the enum
 */
typedef struct Spot {
  String name;
  int values[NUM_METRICS][NUM_COLUMNS];
};
