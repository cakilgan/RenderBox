#include "Application.h"
#include <vector>
const char* parseState(AppState& state) {
	switch (state) {
	case AppState::EXCEPTION: return "exception"; break;
	case AppState::EXIT: return "exit"; break;
	case AppState::RUN: return "run"; break;
	default: return "null";
	}
}
void CALLBACK_ERROR(int error, const char* description) {
	std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}
void CALLBACK_FRAMEBUFFER(GLFWwindow* window, int width, int height) {
    addLog("resizing to "+std::to_string(width)+" "+std::to_string(height));
    glViewport(0, 0, width, height);  
}
bool pressKeys[500]{false};
bool releaseKeys[500]{ false };
void CALLBACK_KEYBOARD(GLFWwindow* window, int key, int scancode, int action, int mods){
    Application* Pong = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (Pong) {
        KeyEvent event{};
        event.actionCode = action;
        event.keyCode = key;
        Pong->dispatch(event);
    }
    if (action==GLFW_PRESS)
    {
        pressKeys[key] = true;
        releaseKeys[key] = false;
    }
    if (action==GLFW_RELEASE)
    {
        releaseKeys[key] = true;
        pressKeys[key] = false;
    }
}

bool key_press(int code) {
    return pressKeys[code];
}
bool key_release(int code) {
    return releaseKeys[code];
}
void setAlphaEnabled() {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}
void setDepthTestEnabled() {
    glEnable(GL_DEPTH_TEST);
}
void setGLPolygonModeEnabled(GLenum type) {
    glPolygonMode(GL_FRONT_AND_BACK, type);
}

void Application::dispatch(const Event& event) {
    auto it = listeners.find(typeid(event));
    if (it != listeners.end()) {
        for (auto& listenerBase : it->second) {
            auto* listener = static_cast<EventListener<std::remove_reference_t<decltype(event)>>*>(listenerBase.get());
            if (listener) listener->handle(event);
        }
    }
}
void Application::init() {
    APP_DYNAMIC.localtime = glfwGetTime();
    if (!glfwInit()) {
        std::cerr << "cannot start GLFW!" << std::endl;
       APP_STATE =  AppState::EXCEPTION;
    }
    
    glfwSetErrorCallback(CALLBACK_ERROR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

    APP_GLFW_WINDOW = glfwCreateWindow(APP_CONFIG.width, APP_CONFIG.height, APP_CONFIG.title.c_str(), nullptr, nullptr);
    if (!APP_GLFW_WINDOW) {
        std::cerr << "cannot create Window!" << std::endl;
        glfwTerminate();
        APP_STATE = AppState::EXCEPTION;
    }

    glfwMakeContextCurrent(APP_GLFW_WINDOW);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "cannot load GLAD!" << std::endl;
        APP_STATE = AppState::EXCEPTION;
    }
    glfwSetFramebufferSizeCallback(APP_GLFW_WINDOW,CALLBACK_FRAMEBUFFER);
    glfwSetWindowUserPointer(APP_GLFW_WINDOW, this);
    glfwSetKeyCallback(APP_GLFW_WINDOW, CALLBACK_KEYBOARD);
    glfwSwapInterval(1);
    addLog("initializing application:");
    addLog("app:: "+APP_NAME+" {");
    addLog("description: " + APP_DESC);
    initialize();
    APP_STATE = AppState::RUN;
    APP_DYNAMIC = {0.0,0.0};
}
std::vector<std::string> logHistory;
std::vector<std::string> logBuffer;
int getTerminalHeight() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
    return 25;
}
void addLog(const std::string& msg) {
    logHistory.push_back(msg);

    int maxLogLines = getTerminalHeight() - 1;
    if (maxLogLines < 1) maxLogLines = 1; 

    logBuffer.push_back(msg);

    while (logBuffer.size() > maxLogLines) {
        logBuffer.erase(logBuffer.begin());
    }
}
void printLogBuffer() {
    for (const auto& line : logBuffer) {
        std::cout << line << std::endl;
    }
}
void printAllHistory() {
    for (size_t i = 0; i < logHistory.size(); i++)
    {
        std::cout << logHistory[i] << std::endl;
    }
}

void scrollUp(int lines) {
    for (int i = 0; i < lines; ++i)
        std::cout << "\n";
}
void clearBelowCursor() {
    std::cout << "\033[J";
}
void moveCursorUp(int lines) {
    std::cout << "\033[" << lines << "A";
}
void moveCursorToStartOfLine() {
    std::cout << "\r";
}
void clearScreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD cellCount, count;
    COORD homeCoords = { 0, 0 };

    if (hConsole == INVALID_HANDLE_VALUE) return;

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    FillConsoleOutputCharacter(hConsole, ' ', cellCount, homeCoords, &count);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count);
    SetConsoleCursorPosition(hConsole, homeCoords);
}
std::string statusLog;
void setStatus(std::string msg)
{
    statusLog = msg;
}
    double temp_fps = 0.0;
void Application::loop() {
    GLFWTimer fpsTimer,scrollbackTimer;
    fpsTimer.reset();
    scrollbackTimer.reset();
    double currentTime=0.0,previous = 0.0,elapsed=0.0;
    int logLines = 0;
    bool printedOnce = false;
    while (APP_STATE==AppState::RUN) {
        currentTime = glfwGetTime();
        elapsed = currentTime - previous;
        previous = currentTime;
        APP_DYNAMIC.delta_time = elapsed;
        APP_DYNAMIC.fps = 1.0f / (elapsed);
        update(elapsed);
        TimeUtils::hook_staticGLFW<1>(1.0f, [this](GLFWTimer* timer) {
            temp_fps = APP_DYNAMIC.fps;
            timer->reset();
            });
        double localElapsed = glfwGetTime()-APP_DYNAMIC.localtime;

        int lineCount = std::count(statusLog.begin(), statusLog.end(), '\n') + 1;

        for (int i = 0; i < lineCount; ++i) {
            std::cout << "\33[2K\r";      
            if (i < lineCount - 1)
                std::cout << "\x1b[1A";   
        }

        clearScreen();
        printLogBuffer();
        std::cout << "STATE[" << parseState(APP_STATE) << "] "
            << "FPS[" << std::fixed << std::setprecision(1) << temp_fps << "] "
            << "for " << localElapsed << " seconds {\n"
            << statusLog
            <<"\n}"
            << std::flush;
    }
}
void Application::dispose() {
    glfwDestroyWindow(APP_GLFW_WINDOW);
    glfwTerminate();
    destroy();
    clearScreen();
    printAllHistory();
    std::cout << std::endl;
    std::cout << "}";
}
void Application::start() {
    init();
    loop();
    dispose();
}
AppState Application::getState() {
    return APP_STATE;
}
void Application::setState(AppState dest) {
    APP_STATE = dest;
}
GLFWwindow* Application::getGLFWwindow() {
    return APP_GLFW_WINDOW;
}
double Application::fps() {
    return APP_DYNAMIC.fps;
}
double Application::delta_time() {
    return APP_DYNAMIC.delta_time;
}