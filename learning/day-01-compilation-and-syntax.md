# Day 1 — Compilation and syntax

## Goal

Compile a C++ file manually and explain `main`, variables, functions, branches,
loops, namespaces, strings, output, and the program's exit code.

```cpp
#include <iostream>  // Declaration for std::cout.
#include <string>    // Declaration for std::string.

double nauticalMilesToKm(double nauticalMiles) {
    return nauticalMiles * 1.852;
}

int main() {
    std::string sensorName = "Radar-01";
    double rangeKm = nauticalMilesToKm(67.76);
    if (rangeKm > 100.0) {
        std::cout << sensorName << ": long range\n";
    }
    return 0; // Zero conventionally means success.
}
```

Save it as `/tmp/day1.cpp`, then deliberately avoid an IDE build button:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -g /tmp/day1.cpp -o /tmp/day1
/tmp/day1
echo $?
```

The compiler translates source into machine code. For a single file, the
driver also invokes the linker to produce `/tmp/day1`. `std::` selects names
from the C++ Standard Library namespace. `\n` is a newline character.

## Project connection

Read `planner-core/src/main.cpp`. Find its includes, local variables, branch,
output streams, return codes, and exception handler.

## Practice

1. Print targets numbered 1 through 5 with a `for` loop.
2. Write `bool isValidLatitude(double)` and test `-90`, `34.7`, and `91`.
3. Introduce a missing semicolon and learn where the compiler points.
4. Change the return value to `7`; inspect it with `echo $?`.

Checklist: I can compile, run, change, and intentionally break one source file.
