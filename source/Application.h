#pragma once
#include <Windows.h>
#include <glad.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <iostream>
#include <iomanip>
#include "Utils.h"
#include <functional>
#include <string>
#include <typeindex>
#include <random>
#include <sstream>
#include <memory>
void setAlphaEnabled();
void setDepthTestEnabled();
void setGLPolygonModeEnabled(GLenum type);

void addLog(const std::string& msg);
void printLogBuffer();
void printAllHistory();
void scrollUp(int lines);
void clearBelowCursor();
void moveCursorUp(int lines);
void moveCursorToStartOfLine();
void clearScreen();
void setStatus(std::string msg);

int getTerminalHeight();

void CALLBACK_ERROR(int error, const char* description);
void CALLBACK_FRAMEBUFFER(GLFWwindow* window, int width, int height);
void CALLBACK_KEYBOARD(GLFWwindow* window, int key, int scancode, int action, int mods);

struct AppConfig {
	int width, height;
	std::string title;
};
struct AppDynamic{
	double delta_time;
	double fps;
	double localtime;
};
enum class AppState {
	RUN, EXCEPTION, EXIT, EMPTY
};
const char* parseState(AppState& state);


struct Event {
	virtual ~Event() = default;
};

struct KeyEvent : public Event {
	int keyCode;
	int actionCode;
	std::string toString() const {
		return "KeyEvent::" + std::to_string(keyCode) + " " + std::to_string(actionCode);
	}
};

struct MouseEvent : public Event {
	int x, y;
	int button;
};

struct EventListenerBase {
	virtual ~EventListenerBase() = default;
};

template<typename T>
struct EventListener : public EventListenerBase {
	std::function<void(const T&)> callback;

	EventListener(std::function<void(const T&)> cb) : callback(cb) {}

	void handle(const Event& event) {
		callback(static_cast<const T&>(event));
	}
};

class Application {
private:
	
	std::string APP_NAME;
	std::string APP_DESC;
	AppConfig APP_CONFIG;
	void init();
	void loop();
	void dispose();


	std::unordered_map<std::type_index, std::vector<std::unique_ptr<EventListenerBase>>> listeners;


	AppState APP_STATE;
	AppDynamic APP_DYNAMIC;

	GLFWwindow* APP_GLFW_WINDOW;
public:
	template<typename EventType>
	void subscribe(std::function<void(const EventType&)> callback) {
		auto listener = std::make_unique<EventListener<EventType>>(callback);
		listeners[typeid(EventType)].push_back(std::move(listener));
	}
	void dispatch(const Event& event);
	double fps();
	double delta_time();
	void setState(AppState dest);
	AppState getState();

	GLFWwindow* getGLFWwindow();
	Application(std::string name,std::string desc,AppConfig cfg):
		APP_NAME(name),
		APP_DESC(desc),
		APP_CONFIG(cfg),
	    APP_DYNAMIC({0,0,0}),
		APP_GLFW_WINDOW(),
	    APP_STATE(AppState::EMPTY){}
	virtual void initialize() = 0;
	virtual void update(double dt)= 0;
	virtual void destroy() = 0;
	void start();
};
bool key_press(int code);
bool key_release(int code);
void addLog(const std::string& log);