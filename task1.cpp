#include <iostream>
#include <vector>
#include <tuple>
using namespace std;

int main() {
    vector<string> days = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    vector<double> temperatures = {8.5, 12.0, 15.3, 21.7, 18.2, 25.1, 9.8};

    tuple<string, double> warmestDay = {days[0], temperatures[0]};

    cout << "Weekly Weather Report (Version 1)" << endl;
    cout << "---------------------------------" << endl;

    for (size_t i = 0; i < days.size(); i++) {
        cout << days[i] << ": " << temperatures[i] << "°C - ";

        if (temperatures[i] < 10) {
            cout << "Cold";
        } else if (temperatures[i] <= 20) {
            cout << "Ok";
        } else {
            cout << "Warm";
        }
        cout << endl;

        if (temperatures[i] > get<1>(warmestDay)) {
            warmestDay = {days[i], temperatures[i]};
        }
    }

    cout << "\nWarmest Day: " << get<0>(warmestDay) << " (" << get<1>(warmestDay) << "°C)" << endl;

    return 0;
}
