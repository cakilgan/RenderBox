#pragma once
#include <glfw/glfw3.h>
#include <chrono>
#include <functional>
#include <memory>
class Timer {
private:
    std::chrono::high_resolution_clock::time_point start;
    bool autoReset = false;
public:
    Timer() {
        reset();
    }
    bool isAutoReset() {
        return autoReset;
    }
    bool setAutoReset(bool v0) {
        autoReset = v0;
        return autoReset;
    }
    void reset() {
        start = std::chrono::high_resolution_clock::now();
    }

    double elapsed() const {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = now - start;
        return duration.count(); // saniye cinsinden
    }

    bool hasPassed(double seconds) const {
        return elapsed() >= seconds;
    }
};
class GLFWTimer {
private:
    double start = 0.0;
    bool autoReset = false;
public:
    bool isAutoReset();
    bool setAutoReset(bool v0);
    void reset();

    double elapsed() const;

    bool hasPassed(double seconds) const;
};

namespace TimeUtils {
    void hook(float time, const std::function<void(Timer*)>& func, Timer* timer);
    void hookGLFW(float time, const std::function<void(GLFWTimer*)>& func, GLFWTimer* timer);
    template<int ID>
    void hook_static(float time, const std::function<void(Timer*)>& func) {
        static Timer timer;
        if (timer.hasPassed(time)) {
            func(&timer);
            if (timer.isAutoReset())
                timer.reset();
        }
    }
    template<int ID>
    void hook_staticGLFW(float time, const std::function<void(GLFWTimer*)>& func) {
        static GLFWTimer timer;
        if (timer.hasPassed(time)) {
            func(&timer);
            if (timer.isAutoReset())
                timer.reset();
        }
    }
}