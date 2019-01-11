#include "AppComponents.hpp"
#include <app/App.h>


LocalTime::LocalTime()
{
	this->update();
}

void LocalTime::update()
{
	time(&current_time);
	localtime_s(&local_time, &current_time);
}




sessionTimeCounter::sessionTimeCounter()
{
	_begin = clock();
}

uint64_t sessionTimeCounter::sessionTime() const
{
	uint64_t _now = clock();
	return ((_now - _begin) / CLOCKS_PER_SEC);
}

AppComponent::AppComponent()
{

}

void AppComponent::frame(float delta)
{
	ts::App::Get().localTime()->update();
}
