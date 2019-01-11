#pragma once
#include <string>

//NOT USING//
typedef enum
{
	kT_HP1 = 0, kT_HP2, kT_AMB1, kT_GR1, kT_3V1, kT_3V2, kT_3V3, kT_3V4, kFLOW1, kPRES1
}geoVarEnum;

struct geoVar
{
	std::string tag;
	std::string name;
	std::string unit;
	bool isBool;
	float value;
};
