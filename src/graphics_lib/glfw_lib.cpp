#include "edl/glfw_lib.h"

#include "edl/glfw_defines.h"

#ifdef WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#else
#include "GLFW/glfw3.h"
#endif

namespace edl {

void GLFW::createWindow(int width, int height, char* title) {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, title, NULL, NULL);
}

void* GLFW::getWindowHandle() {
#ifdef WIN32
	return glfwGetWin32Window(window);
#endif
}

int GLFW::getKeyPressed(int key) {
	return glfwGetKey(window, key) == GLFW_PRESS;
}
int GLFW::getKeyReleased(int key) {
	return glfwGetKey(window, key) == GLFW_RELEASE;
}

const char** GLFW::getRequiredExtensions(uint32_t* count) {
	return glfwGetRequiredInstanceExtensions(count);
}

int GLFW::windowShouldClose() { return glfwWindowShouldClose(window); }
void GLFW::pollEvents() { glfwPollEvents(); }

}