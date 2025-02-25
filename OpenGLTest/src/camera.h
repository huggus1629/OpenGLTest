#pragma once
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <cglm/struct.h>
#include <GLFW/glfw3.h>

typedef struct mouse_data
{
	float xLast;
	float yLast;
	float xOffset;
	float yOffset;
	char firstMouse;
} CamMouseData;

typedef struct camera
{
	vec3s pos;
	vec3s front;
	vec3s right;
	vec3s up;
	vec3s worldUp;

	float yaw_deg;
	float pitch_deg;

	float mvmtSpeed;
	float lookSens;

	float t_current;
	float t_last;
	float dt;

	CamMouseData mouseData;
} Camera;


typedef enum mvmt_dir
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
} CamMovementDirection;

void InitCameraYp(Camera* cam, GLFWwindow* window, float w_mid, float h_mid, vec3s pos, float y_deg, float p_deg, vec3s wUp, float spd, float sens);

mat4s GetViewMatrix(Camera* cam);

void CamSetYaw(Camera* cam, float y_deg);

void CamProcessKB(Camera* cam, CamMovementDirection dir);

void CamProcessMouse(Camera* cam);

void CamUpdateDT(Camera* cam);

#endif
