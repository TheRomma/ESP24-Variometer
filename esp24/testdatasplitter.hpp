#pragma once

#include <cinttypes>

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

	uint8_t small_counter;
	uint16_t counter;

	TestData get_test_data();
	
};

