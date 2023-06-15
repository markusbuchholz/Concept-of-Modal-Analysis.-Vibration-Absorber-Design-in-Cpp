// g++ system_motion.cpp -o t -I/usr/include/python3.8 -lpython3.8

#include <iostream>
#include <tuple>
#include <vector>
#include <math.h>

#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

//----------- system dynamic parameters --------------------
float m1 = 5.0f;
float m2 = 0.5f;
float b1 = 1.0f;
float b2 = 1.0f;
float k1 = 500.0f;
float g = 9.81f; // 9.81f;
float F0 = 2 * 15.0f;
float omega = 15.0f;
float L1 = 0.2f;
float L2 = 0.3f;

float tx = 300.0f; // anchor point
float ty = 10.0f;  // anchor point
float dt = 0.001f;

float k2 = 1.0 * m2 * omega * omega; // 5.0f;
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

std::tuple<std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>> methodRK4_TwoMass()
{

	std::vector<float> diffEq1;
	std::vector<float> diffEq2;
	std::vector<float> diffEq3;
	std::vector<float> diffEq4;

	std::vector<float> time;

	// init values
	float x1 = 0.1f; //
	float x2 = 0.0f; //
	float x3 = 0.0;	 // theta1_dot
	float x4 = 0.0;	 // theta2_dot
	float t = 0.0;	 // init time

	diffEq1.push_back(x1);
	diffEq2.push_back(x2);
	diffEq3.push_back(x3);
	diffEq4.push_back(x4);
	time.push_back(t);

	for (int ii = 0; ii < 20000; ii++)
	{
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

		diffEq1.push_back(x1);
		diffEq2.push_back(x2);
		diffEq3.push_back(x3);
		diffEq4.push_back(x4);
		time.push_back(t);
	}

	return std::make_tuple(diffEq1, diffEq2, diffEq3, diffEq4, time);
}

//---------------------------------------------------------------

void plot2D2D(std::tuple<std::vector<float>, std::vector<float>> data1, std::tuple<std::vector<float>, std::vector<float>> data2)
{

	std::vector<float> xX1 = std::get<0>(data1);
	std::vector<float> yY1 = std::get<1>(data1);

	std::vector<float> xX2 = std::get<0>(data2);
	std::vector<float> yY2 = std::get<1>(data2);

	plt::title("System motion");
	plt::named_plot("pos m1", xX1, yY1);
	plt::named_plot("pos m2", xX2, yY2);
	plt::xlabel("time");
	plt::ylabel("A");
	plt::legend();

	// plt::plot(xX1, yY1);
	// plt::plot(xX2, yY2);
	// plt::xlabel("time[s]");
	// plt::ylabel("A[m]");
	plt::show();
}

//---------------------------------------------------------------

void plotDisplacement()
{

	std::vector<float> y;
	std::vector<float> w;

	auto wp2 = k1 / m1;
	auto u = 0.1;
	auto w_star = 15.0f;

	for (float i = 0.0f; i < 20.0f; i = i + 0.1f)
	{

		auto yi = (F0 / m1) * (i * i - w_star * w_star) / ((i * i - w_star * w_star) * (wp2 - i * i) + u * w_star * w_star * i * i);

		y.push_back(std::abs(yi));
		w.push_back(i);

	}


	plt::title("Aplitude");
	plt::plot(w, y);
	plt::xlabel("w");
	plt::ylabel("A");
	plt::legend();

	plt::show();
}

//---------------------------------------------------------------
int main()
{

	std::tuple<std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>> dyn2mass = methodRK4_TwoMass();

	std::tuple<std::vector<float>, std::vector<float>> mass1t = std::make_tuple(std::get<4>(dyn2mass), std::get<0>(dyn2mass));
	std::tuple<std::vector<float>, std::vector<float>> mass2t = std::make_tuple(std::get<4>(dyn2mass), std::get<1>(dyn2mass));

	// plot2D(mass12);
	plot2D2D(mass1t, mass2t);
	plotDisplacement();
}