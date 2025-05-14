#include "Utils.h"
void Timer::reset() {
    start = glfwGetTime();
}

double Timer::elapsed() const {
    return glfwGetTime() - start;
}

bool Timer::hasPassed(double seconds) const {
    return elapsed() >= seconds;
}