// Compile: g++ -std=c++17 -Wall -Wextra -Wpedantic range_exercises.cpp -o ranges
#include <cassert>
#include <vector>

// Exercise 1: return true only for valid latitude (-90 through 90 inclusive).
bool isValidLatitude(double latitude) {
    (void)latitude;
    return false;  // TODO
}

// Exercise 2: return the greatest value without sorting or copying the vector.
double greatestScore(const std::vector<double>& scores) {
    (void)scores;
    return 0.0;  // TODO: decide what an empty vector should do.
}

int main() {
    assert(isValidLatitude(-90.0));
    assert(isValidLatitude(34.7));
    assert(!isValidLatitude(90.1));
    assert(greatestScore({0.2, 0.9, 0.4}) == 0.9);
}
