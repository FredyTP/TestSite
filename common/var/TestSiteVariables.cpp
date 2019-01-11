#include "TestSiteVariables.hpp"
/*
test_site_Variables::test_site_Variables()
{
	dataLoaded = false;
	//GENERAL VARIABLES
	T_HP1.name += "T_HP1";
	T_HP1.tag += "<td id=\"impulseBC\" class=\"dato\">";
	T_HP1.unit += "°C";
	T_HP1.updated = false;

	T_HP2.name += "T_HP2";
	T_HP2.tag += "<td id=\"returnBC\" class=\"dato\">";
	T_HP2.unit += "°C";
	T_HP2.updated = false;

	T_AMB1.name += "T_AMB1";
	T_AMB1.tag += "<td id=\"ambientTp\" class=\"dato\">";
	T_AMB1.unit += "°C";
	T_AMB1.updated = false;

	//
	T_GR1.name += "T_GR1";
	T_GR1.tag += "<td id=\"groundTp\" class=\"dato\">";
	T_GR1.unit += "°C";
	T_GR1.updated = false;
	//
	T_3V1.name += "T_3V1";
	T_3V1.tag += "<td id=\"3wVOutput\"  class=\"dato\">";
	T_3V1.unit += "°C";
	T_3V1.updated = false;
	//
	T_3V2.name += "T_3V2";
	T_3V2.tag += "<td id=\"3wVeInput\" class=\"dato\">";
	T_3V2.unit += "°C";
	T_3V2.updated = false;

	//
	T_3V3.name += "T_3V3";
	T_3V3.tag += "<td id=\"3wVReturn\" class=\"dato\">";
	T_3V3.unit += "°C";
	T_3V3.updated = false;
	//
	T_3V4.name += "T_3V4";
	T_3V4.tag += "<td id=\"tankTp\" class=\"dato\">";
	T_3V4.unit += "°C";
	T_3V4.updated = false;

	FLOW1.name += "FLOW1";
	FLOW1.tag += "<td  id=\"flow1\"  class=\"dato\">";
	FLOW1.unit += "m^3/s";
	FLOW1.updated = false;

	PRES1.name += "PRES1";
	PRES1.tag += "<td id=\"pressure1\" class=\"dato\">";
	PRES1.unit += "bar";
	PRES1.updated = false;

	//VARIABLES BHE1


	BHE1_1.name += "BHE1_1";
	BHE1_1.tag += "<td id=\"bhe11\" class=\"dato\">";
	BHE1_1.unit += "°C";
	BHE1_1.updated = false;

	BHE1_2.name += "BHE1_2";
	BHE1_2.tag += "<td id=\"bhe12\" class=\"dato\">";
	BHE1_2.unit += "°C";
	BHE1_2.updated = false;


	BHE1_3.name += "BHE1_3";
	BHE1_3.tag += "<td id=\"bhe13\" class=\"dato\">";
	BHE1_3.unit += "°C";
	BHE1_3.updated = false;


	BHE1_4.name += "BHE1_4";
	BHE1_4.tag += "<td id=\"bhe14\" class=\"dato\">";
	BHE1_4.unit += "°C";
	BHE1_4.updated = false;


	//VARIABLES BHE2
	BHE2_1.name += "BHE2_1";
	BHE2_1.tag += "<td id=\"bhe21\" class=\"dato\">";
	BHE2_1.unit += "°C";
	BHE2_1.updated = false;

	BHE2_2.name += "BHE2_2";
	BHE2_2.tag += "<td id=\"bhe22\" class=\"dato\">";
	BHE2_2.unit += "°C";
	BHE2_2.updated = false;


	BHE2_3.name += "BHE2_3";
	BHE2_3.tag += "<td id=\"bhe23\" class=\"dato\">";
	BHE2_3.unit += "°C";
	BHE2_3.updated = false;

	BHE2_4.name += "BHE2_4";
	BHE2_4.tag += "<td id=\"bhe24\" class=\"dato\">";
	BHE2_4.unit += "°C";
	BHE2_4.updated = false;


	//VARIABLES BHE3
	BHE3_1.name += "BHE3_1";
	BHE3_1.tag += "<td id=\"bhe31\" class=\"dato\">";
	BHE3_1.unit += "°C";
	BHE3_1.updated = false;


	BHE3_2.name += "BHE3_2";
	BHE3_2.tag += "<td id=\"bhe32\" class=\"dato\">";
	BHE3_2.unit += "°C";
	BHE3_2.updated = false;


	BHE3_3.name += "BHE3_3";
	BHE3_3.tag += "<td id=\"bhe33\" class=\"dato\">";
	BHE3_3.unit += "°C";
	BHE3_3.updated = false;


	BHE3_4.name += "BHE3_4";
	BHE3_4.tag += "<td id=\"bhe34\" class=\"dato\">";
	BHE3_4.unit += "°C";
	BHE3_4.updated = false;

	////VARIABLES BHE4

	BHE4_1.name += "BHE4_1";
	BHE4_1.tag += "<td id=\"bhe41\" class=\"dato\">";
	BHE4_1.unit += "°C";
	BHE4_1.updated = false;


	BHE4_2.name += "BHE4_2";
	BHE4_2.tag += "<td id=\"bhe42\" class=\"dato\">";
	BHE4_2.unit += "°C";
	BHE4_2.updated = false;

	BHE4_3.name += "BHE4_3";
	BHE4_3.tag += "<td id=\"bhe43\" class=\"dato\">";
	BHE4_3.unit += "°C";
	BHE4_3.updated = false;


	BHE4_4.name += "BHE4_4";
	BHE4_4.tag += "<td id=\"bhe44\" class=\"dato\">";
	BHE4_4.unit += "°C";
	BHE4_4.updated = false;



	//ON OFF

	HEAT_PUMP.name += "HEAT_PUMP";
	HEAT_PUMP.tag += "<td id=\"heatPump\" class=\"dato\">";
	HEAT_PUMP.updated = false;


	KF1.name += "KF1";
	KF1.tag += "<td id=\"coaxialBHEvalve\" class=\"dato\">";
	KF1.updated = false;


	KF2.name += "KF2";
	KF2.tag += "<td id=\"helixBHEvalve\" class=\"dato\">";
	KF2.updated = false;

	KF3.name += "KF3";
	KF3.tag += "<td id=\"singleBHEvalve\" class=\"dato\">";
	KF3.updated = false;

	KF4.name += "KF4";
	KF4.tag += "<td id=\"pilotBHEvalve\" class=\"dato\">";
	KF4.updated = false;

	RES_TANK.name += "RES_TANK";
	RES_TANK.tag += "<td id=\"tankAux\"  class=\"dato\">";
	RES_TANK.updated = false;

	//ASSIGN VECTORS//

	geoVars.push_back(&T_HP1);
	geoVars.push_back(&T_HP2);
	geoVars.push_back(&T_AMB1);
	geoVars.push_back(&T_GR1);
	geoVars.push_back(&T_3V1);
	geoVars.push_back(&T_3V2);
	geoVars.push_back(&T_3V3);
	geoVars.push_back(&T_3V4);
	geoVars.push_back(&FLOW1);
	geoVars.push_back(&PRES1);

	//VARIABLES BHE1

	geoVars.push_back(&BHE1_1);
	geoVars.push_back(&BHE1_2);
	geoVars.push_back(&BHE1_3);
	geoVars.push_back(&BHE1_4);
	//VARIABLES BHE2

	geoVars.push_back(&BHE2_1);
	geoVars.push_back(&BHE2_2);
	geoVars.push_back(&BHE2_3);
	geoVars.push_back(&BHE2_4);

	//VARIABLES BHE3

	geoVars.push_back(&BHE3_1);
	geoVars.push_back(&BHE3_2);
	geoVars.push_back(&BHE3_3);
	geoVars.push_back(&BHE3_4);

	//VARIABLES BHE4

	geoVars.push_back(&BHE4_1);
	geoVars.push_back(&BHE4_2);
	geoVars.push_back(&BHE4_3);
	geoVars.push_back(&BHE4_4);

	//VARIABLES ON/OFF
	boolGeoVars.push_back(&HEAT_PUMP);
	boolGeoVars.push_back(&KF1);
	boolGeoVars.push_back(&KF2);
	boolGeoVars.push_back(&KF3);
	boolGeoVars.push_back(&KF4);
	boolGeoVars.push_back(&RES_TANK);
}


/////////////////////////////////////////////////////////////////////////

void test_site_Variables::parse()
{
	T_HP1.value = getNumber(data, T_HP1.tag);
	T_HP2.value = getNumber(data, T_HP2.tag);
	T_AMB1.value = getNumber(data, T_AMB1.tag);
	T_GR1.value = getNumber(data, T_GR1.tag);
	T_3V1.value = getNumber(data, T_3V1.tag);
	T_3V2.value = getNumber(data, T_3V2.tag);
	T_3V3.value = getNumber(data, T_3V3.tag);
	T_3V4.value = getNumber(data, T_3V4.tag);
	FLOW1.value = getNumber(data, FLOW1.tag);
	PRES1.value = getNumber(data, PRES1.tag);

	//VARIABLES BHE1


	BHE1_1.value = getNumber(data, BHE1_1.tag);
	BHE1_2.value = getNumber(data, BHE1_2.tag);
	BHE1_3.value = getNumber(data, BHE1_3.tag);
	BHE1_4.value = getNumber(data, BHE1_4.tag);

	//VARIABLES BHE2
	BHE2_1.value = getNumber(data, BHE2_1.tag);
	BHE2_2.value = getNumber(data, BHE2_2.tag);
	BHE2_3.value = getNumber(data, BHE2_3.tag);
	BHE2_4.value = getNumber(data, BHE2_4.tag);

	//VARIABLES BHE3
	BHE3_1.value = getNumber(data, BHE3_1.tag);

	BHE3_2.value = getNumber(data, BHE3_2.tag);
	BHE3_3.value = getNumber(data, BHE3_3.tag);
	BHE3_4.value = getNumber(data, BHE3_4.tag);
	////VARIABLES BHE4
	BHE4_1.value = getNumber(data, BHE4_1.tag);
	BHE4_2.value = getNumber(data, BHE4_2.tag);
	BHE4_3.value = getNumber(data, BHE4_3.tag);
	BHE4_4.value = getNumber(data, BHE4_4.tag);

	HEAT_PUMP.value = (bool)getNumber(data, HEAT_PUMP.tag);
	KF1.value = (bool)getNumber(data, KF1.tag);
	KF2.value = (bool)getNumber(data, KF2.tag);
	KF3.value = (bool)getNumber(data, KF3.tag);
	KF4.value = (bool)getNumber(data, KF4.tag);
	RES_TANK.value = (bool)getNumber(data, RES_TANK.tag);
}
void test_site_Variables::parse(int begin, int end)
{

	int _begin = begin;
	int _end = end;
	if (begin < 0)
	{
		begin = 0;
	}
	if (end >= totalVariables)
	{
		_end = totalVariables - 1;
	}
	//USAMOS LAS NUEVAS VARIABLES //
	if (_begin < floatVariables)
	{
		if (_end < floatVariables)
		{
			for (int i = _begin; i <= _end; i++)
			{
				geoVars[i]->value = getNumber(data, geoVars[i]->tag);
				geoVars[i]->updated = true;
			}
		}
		else
		{
			for (int i = _begin; i < floatVariables; i++)
			{
				geoVars[i]->value = getNumber(data, geoVars[i]->tag);
				geoVars[i]->updated = true;
			}
			for (int i = 0; i <= _end - floatVariables; i++)
			{
				boolGeoVars[i]->value = (bool)getNumber(data, boolGeoVars[i]->tag);
				geoVars[i]->updated = true;
			}
		}
	}
	else
	{
		for (int i = totalVariables-_begin; i <= _end - floatVariables; i++)
		{
			boolGeoVars[i]->value = (bool)getNumber(data, boolGeoVars[i]->tag);
			geoVars[i]->updated = true;
		}
	}
}
int test_site_Variables::readDocument(std::string filePath)
{
	dataLoaded = true;
	std::ifstream html(filePath);
	if (!html.is_open())
	{
		std::cerr << "COULDN'T OPEN THE FILE" << std::endl;
		return -1;
	}
	std::string line;
	//Limpiamos la variable data//(Donde guardamos el documento)
	data.clear();
	while (!html.eof())
	{
		std::getline(html, line);
		data += line;
		data += '\n';
	}
	if (data.size() > 10)
	{
		dataLoaded = true;
	}
	return 0;
}
void test_site_Variables::print()
{
	std::cout << "__TEST_SITE_VARIABLES__" << std::endl;
	std::cout << "_______________________________" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "GENERAL VARIABLES" << std::endl;
	std::cout << "_______________________________" << std::endl;
	std::cout << T_HP1.name << " " << T_HP1.value << " " << T_HP1.unit << std::endl;
	std::cout << T_HP2.name << " " << T_HP2.value << " " << T_HP2.unit << std::endl;
	std::cout << T_AMB1.name << " " << T_AMB1.value << " " << T_AMB1.unit << std::endl;
	std::cout << T_GR1.name << " " << T_GR1.value << " " << T_GR1.unit << std::endl;
	std::cout << T_3V1.name << " " << T_3V1.value << " " << T_3V1.unit << std::endl;
	std::cout << T_3V2.name << " " << T_3V2.value << " " << T_3V2.unit << std::endl;
	std::cout << T_3V3.name << " " << T_3V3.value << " " << T_3V3.unit << std::endl;
	std::cout << T_3V4.name << " " << T_3V4.value << " " << T_3V4.unit << std::endl;
	std::cout << FLOW1.name << " " << FLOW1.value << " " << FLOW1.unit << std::endl;
	std::cout << PRES1.name << " " << PRES1.value << " " << PRES1.unit << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

	std::cout << "BHE1 VARIABLES" << std::endl;
	std::cout << "_______________________________" << std::endl;
	std::cout << BHE1_1.name << " " << BHE1_1.value << " " << BHE1_1.unit << std::endl;
	std::cout << BHE1_2.name << " " << BHE1_2.value << " " << BHE1_2.unit << std::endl;
	std::cout << BHE1_3.name << " " << BHE1_3.value << " " << BHE1_3.unit << std::endl;
	std::cout << BHE1_4.name << " " << BHE1_4.value << " " << BHE1_4.unit << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

	std::cout << "BHE2 VARIABLES" << std::endl;
	std::cout << "_______________________________" << std::endl;
	std::cout << BHE2_1.name << " " << BHE2_1.value << " " << BHE2_1.unit << std::endl;
	std::cout << BHE2_2.name << " " << BHE2_2.value << " " << BHE2_2.unit << std::endl;
	std::cout << BHE2_3.name << " " << BHE2_3.value << " " << BHE2_3.unit << std::endl;
	std::cout << BHE2_4.name << " " << BHE2_4.value << " " << BHE2_4.unit << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

	std::cout << "BHE3 VARIABLES" << std::endl;
	std::cout << "_______________________________" << std::endl;
	std::cout << BHE3_1.name << " " << BHE3_1.value << " " << BHE3_1.unit << std::endl;
	std::cout << BHE3_2.name << " " << BHE3_2.value << " " << BHE3_2.unit << std::endl;
	std::cout << BHE3_3.name << " " << BHE3_3.value << " " << BHE3_3.unit << std::endl;
	std::cout << BHE3_4.name << " " << BHE3_4.value << " " << BHE3_4.unit << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

	std::cout << "BHE4 VARIABLES" << std::endl;
	std::cout << "_______________________________" << std::endl;
	std::cout << BHE4_1.name << " " << BHE4_1.value << " " << BHE4_1.unit << std::endl;
	std::cout << BHE4_2.name << " " << BHE4_2.value << " " << BHE4_2.unit << std::endl;
	std::cout << BHE4_3.name << " " << BHE4_3.value << " " << BHE4_3.unit << std::endl;
	std::cout << BHE4_4.name << " " << BHE4_4.value << " " << BHE4_4.unit << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

	std::cout << "VARIABLES ON/OFF" << std::endl;
	std::cout << "_______________________________" << std::endl;
	std::cout << HEAT_PUMP.name << " " << HEAT_PUMP.value << " " << std::endl;
	std::cout << KF1.name << " " << KF1.value << " " << std::endl;
	std::cout << KF2.name << " " << KF2.value << " " << std::endl;
	std::cout << KF3.name << " " << KF3.value << " " << std::endl;
	std::cout << KF4.name << " " << KF4.value << " " << std::endl;
	std::cout << RES_TANK.name << " " << RES_TANK.value << " " << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}
int findTag(std::string file, std::string tag)
{
	int file_it = 0;
	int count = 0;

	while (file_it < file.size())
	{
		if (file[file_it] == tag[0])
		{
			for (int i = 0; i < tag.size(); i++)
			{
				if (file[file_it + i] == tag[i])
				{
					count++;
				}
				else
				{
					count = 0;
					break;

				}
			}
			if (count == tag.size())
			{
				return file_it + (int)tag.size();
			}
		}
		file_it++;
	}
	return -1;
}
float findNumber(std::string file, int ind)
{
	std::string num;
	num.resize(10);
	for (int i = ind; i < ind + 10; i++)
	{
		if (i > file.size() - 10)
		{
			break;
		}
		if (file[i] >= '0' && file[i] <= '9' || file[i] == '.')
		{
			num[i - ind] = file[i];
		}
		else
		{
			break;
		}

	}
	return std::stof(num);
}
float getNumber(std::string file, std::string tag)
{
	int ind = findTag(file, tag);
	if (ind >= 0)
		return findNumber(file, ind);
	else
		return -1;
}

*/