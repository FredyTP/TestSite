#pragma once
#include <bg/bg2e.hpp>
#include <ctime>
#include <iostream>
#include <time.h>

class AppComponent : public bg::scene::Component
{
public:
	AppComponent();
	bg::scene::Component* clone() { return new AppComponent; }
	void frame(float delta);
protected:

};
class LocalTime
{
public:
	LocalTime();
	void update();
	uint16_t year() const { return (local_time.tm_year + 1900); }
	uint16_t month() const { return (local_time.tm_mon + 1); }
	uint16_t monthDay() const { return local_time.tm_mday; }
	uint16_t yearDay() const { return local_time.tm_hour; }
	uint16_t weekDay() const { return (local_time.tm_wday + 1); }
	uint16_t hour() const { return local_time.tm_hour; }
	uint16_t minute() const { return local_time.tm_min; }
	uint16_t second() const { return local_time.tm_sec; }
private:
	time_t current_time;
	tm local_time;

};

class sessionTimeCounter
{
public:
	sessionTimeCounter();
	uint64_t sessionTime() const;
protected:
	uint64_t _begin;

};