#pragma once

#include <cinttypes>


struct testDataSplitter
{
	testDataSplitter();
	~testDataSplitter();

	uint8_t small_counter;
	uint16_t counter;

	void get_test_data(float *x_acc, float *y_acc, float *z_cc, float *x_gyro, float *y_gyro, float *z_gyro, float *barometer, float *up_acc, uint16_t *timestamp);
	
};