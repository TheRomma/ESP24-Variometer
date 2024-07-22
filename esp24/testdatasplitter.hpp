#pragma once

#include <cinttypes>
#include <vector>

struct TestData
{
	float x_acc;
	float y_acc;
	float z_acc;
	float x_gyro;
	float y_gyro;
	float z_gyro;
	float barometer;
	float up_acc;
} ;

struct testDataSplitter
{
	testDataSplitter();
	~testDataSplitter();

	std::vector<float> imu_x;
	std::vector<float> imu_y;
	std::vector<float> imu_z;

	std::vector<float> imu_x_gyro;
	std::vector<float> imu_y_gyro;
	std::vector<float> imu_z_gyro;

	std::vector<float> baro_raw;

	std::vector<float> vel_down;

	uint8_t small_counter;
	uint16_t counter;

	TestData get_test_data();
	
};

