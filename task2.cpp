#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <vector>

class WeatherStation {
private:
    std::string location;
    float *readings;
    size_t size;
    size_t capacity;

    void ensureCapacity(size_t newCount) {
        if (newCount <= capacity) return;
        size_t newCap = capacity * 2;
        if (newCap < newCount) newCap = newCount;
        float *newArr = new float[newCap];
        for (size_t i = 0; i < size; ++i) {
            newArr[i] = readings[i];
        }
        delete[] readings;
        readings = newArr;
        capacity = newCap;
    }

public:
    WeatherStation(const std::string &loc, size_t initialCap = 10)
        : location(loc), size(0), capacity(initialCap) {
        readings = new float[capacity];
    }

    // copy constructor (deep copy)
    WeatherStation(const WeatherStation &other)
        : location(other.location), size(other.size), capacity(other.capacity) {
        readings = new float[capacity];
        for (size_t i = 0; i < size; ++i) {
            readings[i] = other.readings[i];
        }
    }

    // assignment operator
    WeatherStation& operator=(const WeatherStation &other) {
        if (this != &other) {
            delete[] readings;
            location = other.location;
            size = other.size;
            capacity = other.capacity;
            readings = new float[capacity];
            for (size_t i = 0; i < size; ++i) {
                readings[i] = other.readings[i];
            }
        }
        return *this;
    }

    ~WeatherStation() {
        delete[] readings;
    }

    void addReading(float value) {
        ensureCapacity(size + 1);
        readings[size] = value;
        ++size;
    }

    void printReadings() const {
        std::cout << "Readings:";
        for (size_t i = 0; i < size; ++i) {
            std::cout << " " << readings[i];
        }
        std::cout << "\n";
    }

    template<typename T = double>
    T average() const {
        if (size == 0) {
            throw std::runtime_error("No readings available to compute average");
        }
        T sum = 0;
        for (size_t i = 0; i < size; ++i) {
            sum += readings[i];
        }
        return sum / static_cast<T>(size);
    }

    int countAbove(float threshold) const {
        return std::count_if(readings, readings + size,
                             [threshold](float v){ return v > threshold; });
    }

    void sortReadings() {
        std::sort(readings, readings + size);
    }

    void saveToFile(const std::string &filename) const {
        std::ofstream ofs;
        ofs.exceptions(std::ios::failbit | std::ios::badbit);

        try {
            ofs.open(filename);
            // First line: location
            ofs << location << "\n";
            // Next line: number of readings
            ofs << size << "\n";
            // Next: the readings, one per line
            for (size_t i = 0; i < size; ++i) {
                ofs << readings[i] << "\n";
            }
            ofs.close();
        } catch (const std::ios_base::failure &e) {
            throw std::runtime_error("File I/O error when saving to file: " + std::string(e.what()));
        }
    }

    static WeatherStation loadFromFile(const std::string &filename) {
        std::ifstream ifs;
        ifs.exceptions(std::ios::failbit | std::ios::badbit);

        try {
            ifs.open(filename);
            std::string loc;
            std::getline(ifs, loc);
            if (loc.empty()) {
                throw std::runtime_error("Invalid file format: location missing");
            }
            std::string line;
            std::getline(ifs, line);
            if (line.empty()) {
                throw std::runtime_error("Invalid file format: size missing");
            }
            std::istringstream iss(line);
            size_t count;
            iss >> count;
            WeatherStation ws(loc, count > 0 ? count : 1);

            for (size_t i = 0; i < count; ++i) {
                std::string vline;
                std::getline(ifs, vline);
                if (vline.empty()) {
                    throw std::runtime_error("Invalid file format: missing reading at index " + std::to_string(i));
                }
                std::istringstream viss(vline);
                float v;
                viss >> v;
                ws.addReading(v);
            }

            ifs.close();
            return ws;
        } catch (const std::ios_base::failure &e) {
            throw std::runtime_error("File I/O error when loading from file: " + std::string(e.what()));
        }
    }

    const std::string& getLocation() const {
        return location;
    }
};

int main() {
    try {
        // Create a station for Lahti
        WeatherStation ws("Lahti");
        ws.addReading(22.5f);
        ws.addReading(24.0f);
        ws.addReading(26.3f);
        ws.addReading(21.8f);
        ws.addReading(25.7f);

        std::cout << "Location: " << ws.getLocation() << "\n";

        ws.printReadings();

        // Sort them (if you like)
        ws.sortReadings();
        std::cout << "Sorted "; 
        ws.printReadings();

        double avg = ws.average<double>();
        std::cout << "Average: " << avg << "\n";

        int aboveCount = ws.countAbove(25.0f);
        std::cout << "Readings above 25°C: " << aboveCount << "\n";

        std::string fname = "lahti_readings.txt";
        ws.saveToFile(fname);
        std::cout << "Saved to file: " << fname << "\n";

        // Now read them back into new object
        WeatherStation ws2 = WeatherStation::loadFromFile(fname);
        std::cout << "\n--- After loading from file ---\n";
        std::cout << "Location: " << ws2.getLocation() << "\n";
        ws2.printReadings();
        ws2.sortReadings();
        std::cout << "Sorted (loaded) ";
        ws2.printReadings();
        std::cout << "Average (loaded): " << ws2.average<double>() << "\n";
        std::cout << "Readings above 25°C (loaded): " << ws2.countAbove(25.0f) << "\n";

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
