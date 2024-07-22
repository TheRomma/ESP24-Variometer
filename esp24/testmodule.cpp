#include "testmodule.hpp"
#include "fstream"
#include "buzzer.hpp"
#include "display.hpp"

/*
Selftest module for the project by Aleksanteri Sepp�
*/

Buzzer buzzer;
Display display;

TestModule::TestModule(Buzzer b, Display d) {
	buzzer = b;
	display = d;
}

TestModule::~TestModule() {

}

void TestModule::startTests(bool bluetooth, bool serial) {

}

void TestModule::BuzzerDisplayTest() {

}

void TestModule::IMUTest() {

}

void TestModule::barometerTest() {

}

void TestModule::testFlight() {

}