#include "input.h"

void ProcessKBInput(GLFWwindow* window, Camera* cam)
{
	// For exiting with escape key
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		CamProcessKB(cam, FORWARD);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		CamProcessKB(cam, LEFT);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		CamProcessKB(cam, BACKWARD);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		CamProcessKB(cam, RIGHT);
}

void Clbk_Mouse(GLFWwindow* window, double xposIn, double yposIn)
{
	Camera* p_cam = (Camera*)glfwGetWindowUserPointer(window);
	CamMouseData* p_md = &(p_cam->mouseData);

	float xpos = (float)xposIn;
	float ypos = (float)yposIn;

	if (p_md->firstMouse)
	{
		p_md->xLast = xpos;
		p_md->yLast = ypos;
		p_md->firstMouse = 0;
	}

	p_md->xOffset = p_md->xLast - xpos;
	p_md->yOffset = p_md->yLast - ypos;

	p_md->xLast = xpos;
	p_md->yLast = ypos;

	CamProcessMouse(p_cam);
}
