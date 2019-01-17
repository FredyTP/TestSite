#pragma once

//TestSite
#include <var\TestSiteVariables.hpp>
#include <extra\GraphicGenerator.h>

//Bg2e
#include <bg\net\download.hpp>
#include <bg\bg2e.hpp>

//std
#include <thread>
#include <mutex>
#include <map>
#include <time.h>
#include <iostream>

//CLEANED 02/01/2019 20:25

class geoVarManager
{
public:

	geoVarManager();
	~geoVarManager();

	void connect();
	void disconnect();

	int init();
	int exit();
	geoVar get(const std::string tag) const;
	geoVar get(int ind) const;
	int getIdx(const std::string tag) const;
	inline const std::vector<geoVar> & getStaticGeoVars() const { return staticGeoVars; }
	inline size_t size() const { return staticGeoVars.size(); }
	bool isConnected() const { return _connected && _running; }
protected:

	//GENERATE VARIABLE DATA
	void generateVector(); 

	//LOOP FUNCTION THAT PARSES AND DOWNLOAD DATA//
	static void ThreadFunction(geoVarManager *manager);
	static int DownloadFile(const std::string &url, const std::string &dst);
	static int ParseDoc(geoVarManager *manager, const std::string filePath);

	//PARSE FUNCTIONS	
	static float GetNumber(std::string &file, std::string &tag);
	static int FindTag(std::string &file, std::string &tag);
	static float FindNumber(std::string &file, int ind);
	static int ReadDocument(std::string filePath,std::string *data);

	std::vector<geoVar> geoVars; //UPDATED GEOVARS
	std::vector<geoVar> staticGeoVars; //GEOVARS THAT CONTAINS THE BASIC THEY ARE CONSTANT
	//INFORMATION OF THE VARIABLE (name, tag, units..)

	std::thread _thread;
	mutable std::mutex _mutex;
	std::atomic<bool> _running;
	std::atomic<bool> _connected;
	
	
	
};

