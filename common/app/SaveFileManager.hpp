#pragma once

#include <bg/bg2e.hpp>
#include <var/geoVarManager.h>
#include <var/geoVarGraph.h>
#include <thread>
#include <atomic>

namespace app
{
	class SaveFileManager
	{
	public:

		enum FileType
		{
			kGeoVarNow = 0,
			kGeoVarRecord = 1,
			kGeoGraph = 2, 
		};
		SaveFileManager();
		~SaveFileManager();
		void save(bg::system::Path &path, FileType type,geoVarManager * manager);
		void save(bg::system::Path &path, FileType type, geoVarGraph *graph);



	private:
		static void Save(SaveFileManager* fileManager, bg::system::Path path, FileType type, geoVarManager * manager);
		static void SaveGraph(SaveFileManager* fileManager, bg::system::Path path, FileType type, geoVarGraph *graph);

		void addToFile(const bg::system::Path &path, geoVarManager * manager);
		void saveToFile(const bg::system::Path &path, geoVarManager * manager);
		std::vector<std::string> FileExtension;
		std::thread _thread;
		std::atomic<bool> _isSaving;
	};
}
