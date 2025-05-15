#include "Utils.h"
void GLFWTimer::reset() {
    start = glfwGetTime();
}

double GLFWTimer::elapsed() const {
    return glfwGetTime() - start;
}

bool GLFWTimer::hasPassed(double seconds) const {
    return elapsed() >= seconds;
}
bool GLFWTimer::isAutoReset() {
    return autoReset;
}
bool GLFWTimer::setAutoReset(bool v0){
    autoReset = v0;
    return autoReset;
}

void TimeUtils::hook(float time, const std::function<void(Timer*)>& func, Timer* timer) {
    if (timer->hasPassed(time)) {
        func(timer);
    }
}
void TimeUtils::hookGLFW(float time, const std::function<void(GLFWTimer*)>& func, GLFWTimer* timer) {
    if (timer->hasPassed(time)) {
        func(timer);
    }
}
