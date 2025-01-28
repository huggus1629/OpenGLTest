#include "input.h"

void processInput(GLFWwindow* window)
{
	// For exiting with escape key
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS);
}
