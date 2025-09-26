#include <iostream>
#include <map>
using namespace std;

int main() {
    map<string, pair<double, char>> weatherData = {
        {"Monday", {8.5, 'R'}},
        {"Tuesday", {12.0, 'C'}},
        {"Wednesday", {15.3, 'S'}},
        {"Thursday", {21.7, 'S'}},
        {"Friday", {18.2, 'C'}},
        {"Saturday", {25.1, 'S'}},
        {"Sunday", {9.8, 'R'}}
    };

    double sum = 0.0;
    int count = 0;
    pair<string, double> warmestDay = *weatherData.begin();

    cout << "Weekly Weather Report (Version 2)" << endl;
    cout << "---------------------------------" << endl;

    for (const auto &entry : weatherData) {
        string day = entry.first;
        double temp = entry.second.first;
        char condition = entry.second.second;

        cout << day << ": " << temp << "°C (" << condition << ")" << endl;

        sum += temp;
        count++;

        if (temp > warmestDay.second) {
            warmestDay = {day, temp};
        }
    }

    double average = sum / count;
    cout << "\nAverage Temperature: " << average << "°C" << endl;
    cout << "Warmest Day: " << warmestDay.first << " (" << warmestDay.second << "°C)" << endl;

    return 0;
}
