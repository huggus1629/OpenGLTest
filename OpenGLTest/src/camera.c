#include "camera.h"

// private function declarations
static void UpdateCameraVecs(Camera* cam);

void InitCameraYp(Camera* cam, GLFWwindow* window, float w_mid, float h_mid, vec3s pos, float y_deg, float p_deg, vec3s wUp, float spd, float sens)
{
	cam->pos = pos;
	cam->yaw_deg = y_deg;
	cam->pitch_deg = p_deg;
	cam->worldUp = wUp;
	cam->mvmtSpeed = spd;
	cam->lookSens = sens;
	cam->mouseData = (CamMouseData) { 0, 0, w_mid, h_mid, 1 };

	glfwSetWindowUserPointer(window, cam);
	UpdateCameraVecs(cam);
	CamUpdateDT(cam);
}

mat4s GetViewMatrix(Camera* cam)
{
	return glms_lookat(cam->pos, glms_vec3_add(cam->pos, cam->front), cam->up);
}

void CamSetYaw(Camera* cam, float y_deg)
{
	cam->yaw_deg = y_deg;
	UpdateCameraVecs(cam);
}

void CamProcessKB(Camera* cam, CamMovementDirection dir)
{
	float v = cam->mvmtSpeed * cam->dt;

	if (dir == FORWARD)
		cam->pos = glms_vec3_add(cam->pos, glms_vec3_scale(cam->front, v));
	if (dir == BACKWARD)
		cam->pos = glms_vec3_sub(cam->pos, glms_vec3_scale(cam->front, v));
	if (dir == LEFT)
		cam->pos = glms_vec3_sub(cam->pos, glms_vec3_scale(cam->right, v));
	if (dir == RIGHT)
		cam->pos = glms_vec3_add(cam->pos, glms_vec3_scale(cam->right, v));
}

void CamProcessMouse(Camera* cam)
{
	float dx = cam->mouseData.xOffset * cam->lookSens;
	float dy = cam->mouseData.yOffset * cam->lookSens;

	cam->yaw_deg += dx;
	cam->pitch_deg += dy;

	if (cam->pitch_deg < -89.0f)
		cam->pitch_deg = -89.0f;
	else if (cam->pitch_deg > 89.0f)
		cam->pitch_deg = 89.0f;

	UpdateCameraVecs(cam);
}

void CamUpdateDT(Camera* cam)
{
	cam->t_last = cam->t_current;
	cam->t_current = glfwGetTime();
	cam->dt = cam->t_current - cam->t_last;
}

// private func defs
static void UpdateCameraVecs(Camera* cam)
{
	cam->front.x = cos(glm_rad(cam->yaw_deg)) * cos(glm_rad(cam->pitch_deg));
	cam->front.y = sin(glm_rad(cam->pitch_deg));
	cam->front.z = -sin(glm_rad(cam->yaw_deg)) * cos(glm_rad(cam->pitch_deg));
	
	cam->front = glms_vec3_normalize(cam->front);
	cam->right = glms_vec3_normalize(glms_vec3_cross(cam->front, cam->worldUp));
	cam->up = glms_vec3_normalize(glms_vec3_cross(cam->right, cam->front));
}
