#pragma once

#include <cinttypes>


struct testDataSplitter
{
	testDataSplitter();
	~testDataSplitter();

	float coord_x_acc;
	float coord_y_acc;
	float coord_z_acc;
	float coord_x_gyro;
	float coord_y_gyro;
	float coord_z_gyro;
	float vel_d;
	float baro;

	uint16_t counter;
	uint16_t timestamp;

	void get_test_data(*float x_acc, *float y_acc, *float z_cc, *float x_gyro, *float y_gyro, *float z_gyro, *float barometer, *float up_acc, *uint16_t timestamp);
	
};