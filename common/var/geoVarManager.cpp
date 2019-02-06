#include "geoVarManager.h"
#include "GeneralConstants.h"



geoVarManager::geoVarManager()
{
	generateVector();
	staticGeoVars = geoVars;
	_running = false;
	_connected = false;
	connect();
}


geoVarManager::~geoVarManager()
{
}

void geoVarManager::connect()
{
	if (!_running)
	{
		_running = true;
		_thread = std::thread(ThreadFunction, this);
	}
}

void geoVarManager::disconnect()
{
	if (_running)
	{
		_running = false;
		_thread.join();
	}
}

int geoVarManager::init()
{
	this->connect();
	return 0;
}

int geoVarManager::exit()
{
	this->disconnect();
	return 0;
}



float geoVarManager::GetNumber(std::string &file, std::string &tag)
{
	int ind = FindTag(file, tag);
	if (ind >= 0)
		return FindNumber(file, ind);
	else
		return std::numeric_limits<float>::min();
}

int geoVarManager::FindTag(std::string &file, std::string &tag)
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

float geoVarManager::FindNumber(std::string &file, int ind)
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

void geoVarManager::ThreadFunction(geoVarManager * manager)
{
	std::string url = ts::kTestSiteVariablesIp;
	bg::system::Path filePath = bg::system::Path::AppDir();
	filePath.addComponent(ts::kTestDownloadPath);

	//IMPROVE THE FUNC WITH OPTIONS TO CLOSE FASTER
	while (manager->_running)
	{	
		//std::cout << "going to download" << std::endl;
		geoVarManager::DownloadFile(url, filePath.text());
		
		//LOCK MUTEX
		manager->_mutex.lock();
		//std::cout << "going to parse" << std::endl;
		geoVarManager::ParseDoc(manager, filePath.text());
		
		//UNLOCK MUTEX
		manager->_mutex.unlock();
	}
}

int geoVarManager::DownloadFile(const std::string & url, const std::string & dst)
{
	bg::net::Download syncDownload;
	syncDownload.downloadSync(url, dst);

	return 0;
}

int geoVarManager::ParseDoc(geoVarManager * manager, const std::string filePath)
{
	std::string data;
	int result = ReadDocument(filePath, &data);
	//if reading failed means no connection//
	if (result < 0)
	{
		std::cout << "CANT FIND THE DOC" << std::endl;
		manager->_connected = false;
		return -1;
	}
	manager->_connected = true;

	for (int i = 0; i < manager->geoVars.size(); i++)
	{
		if (!manager->_running) return i;
		manager->geoVars[i].value = GetNumber(data, manager->geoVars[i].tag);
	}
	return 0;
}

int geoVarManager::ReadDocument(std::string filePath, std::string * data)
{
	std::ifstream html(filePath);
	if (!html.is_open())
	{
		return -1;
	}
	std::string line;
	//Limpiamos la variable data//(Donde guardamos el documento)
	data->clear();
	while (!html.eof())
	{
		std::getline(html, line);
		*data += line;
		*data += '\n';
	}
	if (data->size() > 10)
	{
		return 1;
	}
	return -1;
	
}



geoVar geoVarManager::get(const std::string tag) const
{
	std::lock_guard<std::mutex> lock(_mutex);
	geoVar result;
	for (int i = 0; i < geoVars.size(); i++)
	{
		if (geoVars[i].name.compare(tag) == 0)
		{
			result = geoVars[i];
		}
	}

	return result;
}

geoVar geoVarManager::get(int ind) const
{
	std::lock_guard<std::mutex> lock(_mutex);
	geoVar result = geoVars[ind];
	return result;
}

int geoVarManager::getIdx(const std::string tag) const
{
	for (int i = 0; i < staticGeoVars.size(); i++)
	{
		if (staticGeoVars[i].name.compare(tag) == 0)
		{
			return i;
		}
	}
	return 0;
}


void geoVarManager::generateVector()
{
	geoVar T_HP1;
	geoVar T_HP2;
	geoVar T_AMB1;
	geoVar T_GR1;
	geoVar T_3V1;
	geoVar T_3V2;
	geoVar T_3V3;
	geoVar T_3V4;
	geoVar FLOW1;
	geoVar PRES1;
	//10

	//VARIABLE BHE1
	geoVar BHE1_1;
	geoVar BHE1_2;
	geoVar BHE1_3;
	geoVar BHE1_4;
	//14

	//VARIABLE BHE2
	geoVar BHE2_1;
	geoVar BHE2_2;
	geoVar BHE2_3;
	geoVar BHE2_4;
	//18

	//VARIABLE BHE3
	geoVar BHE3_1;
	geoVar BHE3_2;
	geoVar BHE3_3;
	geoVar BHE3_4;
	//22

	//VARIABLE BHE4
	geoVar BHE4_1;
	geoVar BHE4_2;
	geoVar BHE4_3;
	geoVar BHE4_4;
	//26

	//VARIABLES ON/OFF

	geoVar HEAT_PUMP;
	geoVar KF1;
	geoVar KF2;
	geoVar KF3;
	geoVar KF4;
	geoVar RES_TANK;

	T_HP1.name += "T_HP1";
	T_HP1.tag += "<td id=\"impulseBC\" class=\"dato\">";
	T_HP1.unit += "°C";
	T_HP1.isBool = false;

	T_HP2.name += "T_HP2";
	T_HP2.tag += "<td id=\"returnBC\" class=\"dato\">";
	T_HP2.unit += "°C";
	T_HP2.isBool = false;

	T_AMB1.name += "T_AMB1";
	T_AMB1.tag += "<td id=\"ambientTp\" class=\"dato\">";
	T_AMB1.unit += "°C";
	T_AMB1.isBool = false;

	//
	T_GR1.name += "T_GR1";
	T_GR1.tag += "<td id=\"groundTp\" class=\"dato\">";
	T_GR1.unit += "°C";
	T_GR1.isBool = false;
	//
	T_3V1.name += "T_3V1";
	T_3V1.tag += "<td id=\"3wVOutput\"  class=\"dato\">";
	T_3V1.unit += "°C";
	T_3V1.isBool = false;
	//
	T_3V2.name += "T_3V2";
	T_3V2.tag += "<td id=\"3wVeInput\" class=\"dato\">";
	T_3V2.unit += "°C";
	T_3V2.isBool = false;

	//
	T_3V3.name += "T_3V3";
	T_3V3.tag += "<td id=\"3wVReturn\" class=\"dato\">";
	T_3V3.unit += "°C";
	T_3V3.isBool = false;
	//
	T_3V4.name += "T_3V4";
	T_3V4.tag += "<td id=\"tankTp\" class=\"dato\">";
	T_3V4.unit += "°C";
	T_3V4.isBool = false;

	FLOW1.name += "FLOW1";
	FLOW1.tag += "<td  id=\"flow1\"  class=\"dato\">";
	FLOW1.unit += "m^3/h";
	FLOW1.isBool = false;

	PRES1.name += "PRES1";
	PRES1.tag += "<td id=\"pressure1\" class=\"dato\">";
	PRES1.unit += "bar";
	PRES1.isBool = false;

	//VARIABLES BHE1


	BHE1_1.name += "BHE1_1";
	BHE1_1.tag += "<td id=\"bhe11\" class=\"dato\">";
	BHE1_1.unit += "°C";
	BHE1_1.isBool = false;

	BHE1_2.name += "BHE1_2";
	BHE1_2.tag += "<td id=\"bhe12\" class=\"dato\">";
	BHE1_2.unit += "°C";
	BHE1_2.isBool = false;


	BHE1_3.name += "BHE1_3";
	BHE1_3.tag += "<td id=\"bhe13\" class=\"dato\">";
	BHE1_3.unit += "°C";
	BHE1_3.isBool = false;


	BHE1_4.name += "BHE1_4";
	BHE1_4.tag += "<td id=\"bhe14\" class=\"dato\">";
	BHE1_4.unit += "°C";
	BHE1_4.isBool = false;


	//VARIABLES BHE2
	BHE2_1.name += "BHE2_1";
	BHE2_1.tag += "<td id=\"bhe21\" class=\"dato\">";
	BHE2_1.unit += "°C";
	BHE2_1.isBool = false;

	BHE2_2.name += "BHE2_2";
	BHE2_2.tag += "<td id=\"bhe22\" class=\"dato\">";
	BHE2_2.unit += "°C";
	BHE2_2.isBool = false;


	BHE2_3.name += "BHE2_3";
	BHE2_3.tag += "<td id=\"bhe23\" class=\"dato\">";
	BHE2_3.unit += "°C";
	BHE2_3.isBool = false;

	BHE2_4.name += "BHE2_4";
	BHE2_4.tag += "<td id=\"bhe24\" class=\"dato\">";
	BHE2_4.unit += "°C";
	BHE2_4.isBool = false;


	//VARIABLES BHE3
	BHE3_1.name += "BHE3_1";
	BHE3_1.tag += "<td id=\"bhe31\" class=\"dato\">";
	BHE3_1.unit += "°C";
	BHE3_1.isBool = false;


	BHE3_2.name += "BHE3_2";
	BHE3_2.tag += "<td id=\"bhe32\" class=\"dato\">";
	BHE3_2.unit += "°C";
	BHE3_2.isBool = false;


	BHE3_3.name += "BHE3_3";
	BHE3_3.tag += "<td id=\"bhe33\" class=\"dato\">";
	BHE3_3.unit += "°C";
	BHE3_3.isBool = false;


	BHE3_4.name += "BHE3_4";
	BHE3_4.tag += "<td id=\"bhe34\" class=\"dato\">";
	BHE3_4.unit += "°C";
	BHE3_4.isBool = false;

	////VARIABLES BHE4

	BHE4_1.name += "BHE4_1";
	BHE4_1.tag += "<td id=\"bhe41\" class=\"dato\">";
	BHE4_1.unit += "°C";
	BHE4_1.isBool = false;


	BHE4_2.name += "BHE4_2";
	BHE4_2.tag += "<td id=\"bhe42\" class=\"dato\">";
	BHE4_2.unit += "°C";
	BHE4_2.isBool = false;

	BHE4_3.name += "BHE4_3";
	BHE4_3.tag += "<td id=\"bhe43\" class=\"dato\">";
	BHE4_3.unit += "°C";
	BHE4_3.isBool = false;


	BHE4_4.name += "BHE4_4";
	BHE4_4.tag += "<td id=\"bhe44\" class=\"dato\">";
	BHE4_4.unit += "°C";
	BHE4_4.isBool = false;



	//ON OFF

	HEAT_PUMP.name += "HEAT_PUMP";
	HEAT_PUMP.tag += "<td id=\"heatPump\" class=\"dato\">";
	HEAT_PUMP.isBool = true;


	KF1.name += "KF1";
	KF1.tag += "<td id=\"coaxialBHEvalve\" class=\"dato\">";
	KF1.isBool = true;


	KF2.name += "KF2";
	KF2.tag += "<td id=\"helixBHEvalve\" class=\"dato\">";
	KF2.isBool = true;

	KF3.name += "KF3";
	KF3.tag += "<td id=\"singleBHEvalve\" class=\"dato\">";
	KF3.isBool = true;

	KF4.name += "KF4";
	KF4.tag += "<td id=\"pilotBHEvalve\" class=\"dato\">";
	KF4.isBool = true;

	RES_TANK.name += "RES_TANK";
	RES_TANK.tag += "<td id=\"tankAux\"  class=\"dato\">";
	RES_TANK.isBool = true;

	geoVars.push_back(T_HP1);
	geoVars.push_back(T_HP2);
	geoVars.push_back(T_AMB1);
	geoVars.push_back(T_GR1);
	geoVars.push_back(T_3V1);
	geoVars.push_back(T_3V2);
	geoVars.push_back(T_3V3);
	geoVars.push_back(T_3V4);
	geoVars.push_back(FLOW1);
	geoVars.push_back(PRES1);

	//VARIABLES BHE1

	geoVars.push_back(BHE1_1);
	geoVars.push_back(BHE1_2);
	geoVars.push_back(BHE1_3);
	geoVars.push_back(BHE1_4);
	//VARIABLES BHE2

	geoVars.push_back(BHE2_1);
	geoVars.push_back(BHE2_2);
	geoVars.push_back(BHE2_3);
	geoVars.push_back(BHE2_4);

	//VARIABLES BHE3

	geoVars.push_back(BHE3_1);
	geoVars.push_back(BHE3_2);
	geoVars.push_back(BHE3_3);
	geoVars.push_back(BHE3_4);

	//VARIABLES BHE4

	geoVars.push_back(BHE4_1);
	geoVars.push_back(BHE4_2);
	geoVars.push_back(BHE4_3);
	geoVars.push_back(BHE4_4);

	//VARIABLES ON/OFF
	geoVars.push_back(HEAT_PUMP);
	geoVars.push_back(KF1);
	geoVars.push_back(KF2);
	geoVars.push_back(KF3);
	geoVars.push_back(KF4);
	geoVars.push_back(RES_TANK);

	for (int i = 0; i < geoVars.size(); i++)
	{
		geoVars[i].value = std::numeric_limits<float>::min();
	}

}