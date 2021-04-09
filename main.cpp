#include <iostream>
#include <unordered_map>
#include <vector>
#define MAX_LENGTH 512
#include <algorithm>

#ifdef WINDOWS
#include <direct.h>
#define CurrentSysDir _getcwd
#else
#include <unistd.h>
#define CurrentSysDir getcwd
#endif

using namespace std;

class WeatherTable;
string getCurrentDir ();

/*----------------------------------------------------------------------------------------------------------------------
Struct (to be refactored)
----------------------------------------------------------------------------------------------------------------------*/
struct rideData{

};

struct weatherData{
    float temp;
    string location;
    float clouds;
    float pressure;
    float rain;
    time_t time_stamp;
    float humidity;
    float wind;
};

typedef struct rideData rideData;
typedef struct weatherData weatherData;

// Typedef for unordered_map
typedef unordered_map<string, int>::iterator iter;
typedef pair<iter, bool> insertReturn;

// Comparison operator to make weatherData a comparable data type
bool operator<(weatherData a, weatherData b) {
    return a.time_stamp < b.time_stamp;
}

/*----------------------------------------------------------------------------------------------------------------------
class implementation (Michael)
----------------------------------------------------------------------------------------------------------------------*/
// Class to store all weather data.
class WeatherTable {
    vector<vector<weatherData>> dataTable;
    unordered_map<string, int> locationIndex;
    int sorted = 0;

    public:
        WeatherTable() {
            dataTable = {};
            locationIndex = unordered_map<string, int>();
        }

        void insert(weatherData data) {
            int i = dataTable.size();
            // Check return value of insert() . If insert() succeeds, create a new weatherData vector with the
            // data in the argument as the first element. Otherwise push back the data to the appropriate vector.
            insertReturn p = locationIndex.insert(pair<string, int> (data.location, i));
            if(p.second) {
                dataTable.push_back({data});
                i++;
            }
            else {
                int index = p.first->second;
                dataTable.at(index).push_back(data);
                sorted = 0;
            }
        }

        weatherData getWeather(string location, time_t time) {
            time_t closest = LONG_MAX;
            // sort all weatherData vector (ascending).
            if (!sorted) {
                for (int i = 0; i < dataTable.size(); i++) {
                    sort(dataTable.at(i).begin(), dataTable.at(i).end());
                }
                sorted = 1;
            }

            // Find the index of weatherData vector of the queried location in dataTable
            iter it = locationIndex.find(location);
            if (it == locationIndex.end()) {
                cout << "ERROR: " << location << " not found." << endl;
                throw;
            }
            int i = it->second;
            int j = 0;
            dataTable.at(i);

            // Perform linear search to find the weather data at the closest time_stamp as the queried argument. Since
            // vector is sorted, stop the search once the difference is larger than the previous difference.
            for (; j < dataTable.at(i).size(); j++) {
                time_t diff = (time - dataTable.at(i).at(j).time_stamp);
                if (diff < 0) {
                    diff *= -1;
                }
                if (diff < closest) {
                    closest = diff;
                } else {break;}
            }

            return dataTable.at(i).at(j-1);
        }
};

/*----------------------------------------------------------------------------------------------------------------------
 * Main
----------------------------------------------------------------------------------------------------------------------*/

// return string that contains the directory of the current .cpp file
string getCurrentDir () {
    char buff[512];
    CurrentSysDir(buff, MAX_LENGTH);
    string dir(buff);
    return dir;
}

int main() {
    typedef weatherData weatherData;
    string cabRides = getCurrentDir() + "\\data\\cab_rides.csv";
    string weather = getCurrentDir() + "\\data\\weather.csv";

    WeatherTable weatherTable = WeatherTable();
    /*------------------------------------------------------------------------------------------------------------------
    * file IO (Raff)
    ------------------------------------------------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------------------------------------------------
    * Testing WeatherTable class
    ------------------------------------------------------------------------------------------------------------------*/
//    vector<vector<string>> mockData = {{"42.42", "Back Bay",    "1",    "1012.14", "0.1228", "1545003901", "0.77", "11.25"},
//                                       {"42.43", "Beacon Hill", "1",    "1012.15", "0.1846", "1545003901", "0.76", "11.32"},
//                                       {"43.28", "Back Bay",    "0.81", "990.81",  "null",   "1543347920", "0.71", "8.3"},
//                                       {"42.42", "Back Bay",    "1",    "1012.14", "0.1228", "1545003902", "0.77", "11.25"}};
//
//    for (int i = 0; i < mockData.size(); i++) {
//        if (mockData.at(i).at(4) == "null") {
//            mockData.at(i).at(4) = "0";
//        }
//        weatherData d;
//        d.temp = stof(mockData.at(i).at(0));
//        d.location = (mockData.at(i).at(1));
//        d.clouds = stof(mockData.at(i).at(2));
//        d.pressure = stof(mockData.at(i).at(3));
//        d.rain = stof(mockData.at(i).at(4));
//        d.time_stamp = stoi(mockData.at(i).at(5));
//        d.humidity = stof(mockData.at(i).at(6));
//        d.wind = stof(mockData.at(i).at(7));
//
//        weatherTable.insert(d);
//    }
//    cout << weatherTable.getWeather("Back Bay", 1545003903).time_stamp;
}