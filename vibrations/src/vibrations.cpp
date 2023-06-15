// Markus Buchholz, 2023
// https://physics.stackexchange.com/questions/629159/what-are-the-maximum-spring-lengths-of-a-double-spring-pendulum
#include <stdafx.hpp>
#include "imgui_helper.hpp"
#include <tuple>
#include <thread>
#include <chrono>
#include <vector>
#include <math.h>
#include <list>
#include <numeric>
#include <memory>

//----------- system dynamic parameters --------------------
float m1 = 5.0f;
float m2 = 0.005f;
float b1 = 1.0f;
float b2 = 1.0f;
float k1 = 500.0f;
float g = 9.81f; // 9.81f;
float F0 = 15.0f;
float omega = 15.0f;
float L1 = 0.2f;
float L2 = 0.3f;

float tx = 300.0f; // anchor point
float ty = 10.0f;  // anchor point
float dt = 0.01f;

float k2 = m2 * 1.0f * omega * omega; // 5.0f;

/*
 *k2 / m2 = omega * omega
 *
 */

// float m2 =  k2 / (omega * omega);//2.0f;

//-----------------------------------------------------------
// dy1
float function1(float y1, float y2, float dy1, float dy2, float t)
{

	return dy1;
}

//-----------------------------------------------------------
// dy2
float function2(float y1, float y2, float dy1, float dy2, float t)
{

	return dy2;
}

//-----------------------------------------------------------
// ddy1
float function3(float y1, float y2, float dy1, float dy2, float t)
{

	auto ddy1 = [=]()
	{
		
		auto fx = 1 / m1 * (F0 *  std::cos(omega * t) + m1 * g - b1 * dy1 - k1 * (y1 - L1) - k2 * (y1 - y2 - L2));  
		return fx; }();

	return ddy1;
}

//-----------------------------------------------------------
// ddy2
float function4(float y1, float y2, float dy1, float dy2, float t)
{
	auto ddy2 = [=]()
	{
		
		auto fx = 1 / m2 * (m2 * g + k2 * (y1 - y2 - L2) - b2 * dy2);  
		
		return fx; }();

	return ddy2;
}

//-----------------------------------------------------------

int main(int argc, char const *argv[])
{
	ImVec4 clear_color = ImVec4(0.0f / 255.0, 0.0f / 255.0, 0.0f / 255.0, 1.00f);
	ImVec4 white_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 pink_color = ImVec4(245.0f / 255.0, 5.0f / 255.0, 150.0f / 255.0, 1.00f);
	ImVec4 blue_color = ImVec4(0.0f / 255.0, 0.0f / 255.0, 2550.0f / 255.0, 1.00f);
	int flag = true;

	int w = 800;
	int h = 800;
	std::string title = "Spring Forces";
	initImgui(w, h, title);

	float k11{0.0f}, k12{0.0f}, k13{0.0f}, k14{0.0f};
	float k21{0.0f}, k22{0.0f}, k23{0.0f}, k24{0.0f};
	float k31{0.0f}, k32{0.0f}, k33{0.0f}, k34{0.0f};
	float k41{0.0f}, k42{0.0f}, k43{0.0f}, k44{0.0f};

	// init values mass 1
	float x1 = 0.1f;		   // init position of mass in x
	float x2 = 0.1f + L1 + L2; // init position of masss in y
	float x3 = 0.0f;		   // init velocity
	float x4 = 0.0f;		   // init velocity

	float t = 0.0f; // init time
	int ij = 0;

	while (!glfwWindowShouldClose(window) && flag == true)
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();

		ImGuiWindowFlags window_flags = 0;
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_FirstUseEver);
		window_flags |= ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_NoBackground;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

		ImGui::Begin("Simulation", nullptr, window_flags);
		ImDrawList *draw_list = ImGui::GetWindowDrawList();

		t = t + dt;

		float k11 = function1(x1, x2, x3, x4, t);
		float k12 = function2(x1, x2, x3, x4, t);
		float k13 = function3(x1, x2, x3, x4, t);
		float k14 = function4(x1, x2, x3, x4, t);

		float k21 = function1(x1 + dt / 2 * k11, x2 + dt / 2 * k12, x3 + dt / 2 * k13, x4 + dt / 2 * k14, t);
		float k22 = function2(x1 + dt / 2 * k11, x2 + dt / 2 * k12, x3 + dt / 2 * k13, x4 + dt / 2 * k14, t);
		float k23 = function3(x1 + dt / 2 * k11, x2 + dt / 2 * k12, x3 + dt / 2 * k13, x4 + dt / 2 * k14, t);
		float k24 = function4(x1 + dt / 2 * k11, x2 + dt / 2 * k12, x3 + dt / 2 * k13, x4 + dt / 2 * k14, t);

		float k31 = function1(x1 + dt / 2 * k21, x2 + dt / 2 * k22, x3 + dt / 2 * k23, x4 + dt / 2 * k24, t);
		float k32 = function2(x1 + dt / 2 * k21, x2 + dt / 2 * k22, x3 + dt / 2 * k23, x4 + dt / 2 * k24, t);
		float k33 = function3(x1 + dt / 2 * k21, x2 + dt / 2 * k22, x3 + dt / 2 * k23, x4 + dt / 2 * k24, t);
		float k34 = function4(x1 + dt / 2 * k21, x2 + dt / 2 * k22, x3 + dt / 2 * k23, x4 + dt / 2 * k24, t);

		float k41 = function1(x1 + dt * k31, x2 + dt * k32, x3 + dt * k33, x4 + dt * k34, t);
		float k42 = function2(x1 + dt * k31, x2 + dt * k32, x3 + dt * k33, x4 + dt * k34, t);
		float k43 = function3(x1 + dt * k31, x2 + dt * k32, x3 + dt * k33, x4 + dt * k34, t);
		float k44 = function4(x1 + dt * k31, x2 + dt * k32, x3 + dt * k33, x4 + dt * k34, t);

		x1 = x1 + dt / 6.0 * (k11 + 2 * k21 + 2 * k31 + k41);
		x2 = x2 + dt / 6.0 * (k12 + 2 * k22 + 2 * k32 + k42);
		x3 = x3 + dt / 6.0 * (k13 + 2 * k23 + 2 * k33 + k43);
		x4 = x4 + dt / 6.0 * (k14 + 2 * k24 + 2 * k34 + k44);

		ij++;

		draw_list->AddCircleFilled({tx, ty}, 3.0f, ImColor(white_color));
		draw_list->AddCircleFilled({tx, ty+100.0f + (x2 * 600.0f)}, 10.0f, ImColor(pink_color));
		draw_list->AddCircleFilled({tx, ty + 200.0f + (x1 * 600.0f)}, 10.0f, ImColor(blue_color));


		ImGui::End();

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	termImgui();
	return 0;
}