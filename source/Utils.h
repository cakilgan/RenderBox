#pragma once
#include <glfw/glfw3.h>
class Timer {
private:
    double start = 0.0;
public:
    void reset();

    double elapsed() const;

    bool hasPassed(double seconds) const;
};