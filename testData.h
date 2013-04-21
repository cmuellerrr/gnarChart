
/*
Mavericks - San Mateo - Spot 122
Lower Trestles - Orange County - Spot 208
El Porto - Los Angeles - Spot 402

tide (from 12AM to 12 AM)
{
	"date":"Saturday Mar 30 2013",
	"day":"Sat",
	"gmt":"2013-3-30 7",
	"hour":"12AM",
	"name":"Orange County",
	"tide":5.5216535517000001,
	"tide_meters":1.6830000000000001
}

forecast (from 12AM to 12AM)
{
	"date":"Saturday Mar 30 2013",
	"day":"Sat",
	"gmt":"2013-3-30 7",
	"hour":"12AM",
	"latitude":36.954087622045307,
	"live":1,
	"longitude":-121.9716900628907,
	"shape":"f",
	"shape_detail":{
		"swell":"Fair",
		"tide":"Fair",
		"wind":"Fair"
	},
	"shape_full":"Fair",
	"size":1,
	"size_ft":0.86193411069995007,
	"spot_id":1,
	"spot_name":"Pleasure Point",
	"warnings":[]
}

wind (from 12AM to 12AM)
{
	"date":"Saturday Mar 30 2013",
	"day":"Sat",
	"direction_degrees":180,
	"direction_text":"S",
	"gmt":"2013-3-30 7",
	"hour":"12AM",
	"name":"Orange County",
	"speed_kts":3,
	"speed_mph":3.4523383499999998
}
*/

char json_mavericks[] = "{\"forecast\":[4,4,4,3,1,3,3,4],\"tide\":[3,3,4,3,1,1,4,5],\"wind\":[16,16,15,15,14,15,20,15]}";

char json_trestles[] = "{\"forecast\":[4,3,3,4,4,4,4,4],\"tide\":[2,3,4,2,1,2,4,3],\"wind\":[3,2,3,7,9,8,7,6]}";
    
char json_porto[] = "{\"forecast\":[3,3,3,3,3,3,3,3],\"tide\":[2,3,4,2,1,2,4,3],\"wind\":[1,2,2,5,11,12,6,2]}";
