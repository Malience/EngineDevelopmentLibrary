#ifndef GLFW_LIB_H
#define GLFW_LIB_H

#include <stdint.h>
#include "GLFW/glfw3.h"

//inline void initWindow();

//inline void windowHint();

namespace edl {

class GLFW {
public:
    static void createWindow(int width, int height, char* title);
    static void* getWindowHandle();
    static GLFWwindow* getWindow() { return window; }

    static int getKeyPressed(int key);
    static int getKeyReleased(int key);

    static const char** getRequiredExtensions(uint32_t* count);

    static int windowShouldClose();
    static void pollEvents();

    inline static GLFWwindow* window;
};



}

#endif // !GLFW_LIB_H
