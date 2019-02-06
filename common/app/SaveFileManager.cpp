#include "SaveFileManager.hpp"

namespace app
{
	SaveFileManager::SaveFileManager()
	{
		
		FileExtension.resize(3);
		FileExtension[kGeoVarNow] = ".tsvar";
		FileExtension[kGeoVarRecord] = ".tsrvar";
		FileExtension[kGeoGraph] = ".tsgr";
		_isSaving = false;
	}

	SaveFileManager::~SaveFileManager()
	{
		if(_thread.joinable())
			_thread.join();
	}

	void SaveFileManager::save(bg::system::Path & path, FileType type, geoVarManager * manager)
	{
		bg::system::Path _path = path;
		_path.removeExtension();
		_path.addExtension(FileExtension[type]);
		if (!_isSaving)
		{
			if(_thread.joinable())
			{
				_thread.join();
			}
			_thread = std::thread(Save, this, _path, type, manager);

		}
		else
		{
			bg::wnd::MessageBox warning;
			warning.Show(nullptr, "Error", "Wait until the actual document is saved");
			
		}

	}
	void SaveFileManager::save(bg::system::Path & path, FileType type, geoVarGraph * graph)
	{		
		bg::system::Path _path = path;
		_path.removeExtension();
		_path.addExtension(FileExtension[type]);
		if (!_isSaving)
		{
			if (_thread.joinable())
			{
				_thread.join();
			}
			_thread = std::thread(SaveGraph, this, _path, type, graph);

		}
		else
		{
			bg::wnd::MessageBox warning;
			warning.Show(nullptr, "Error", "Wait until the actual document is saved");

		}
	}

	void SaveFileManager::Save(SaveFileManager * fileManager, bg::system::Path path, FileType type, geoVarManager * manager)
	{
		fileManager->_isSaving = true;
		if (type == kGeoVarNow)
		{
			fileManager->saveToFile(path, manager);
		}
		else if (type == kGeoVarRecord)
		{
			fileManager->addToFile(path, manager);
		}
		fileManager->_isSaving = false;
	}

	void SaveFileManager::SaveGraph(SaveFileManager * fileManager, bg::system::Path path, FileType type, geoVarGraph * graph)
	{
		fileManager->_isSaving = true;

		//SAVING LOGIC
		using namespace bg::db::json;

		bg::ptr<Value> data = new Value();
		for (int i = 0; i < graph->size(); i++)
		{
			data->setValue("x_"+std::to_string(i), graph->originalPoints()[i].x());
			data->setValue("y_" + std::to_string(i), graph->originalPoints()[i].y());
		}

		std::ofstream file;
		file.open(path.text(), std::ofstream::trunc);
		if (file.is_open())
		{
			data->writeToStream(file);
		}

		file.close();

		fileManager->_isSaving = false;
	}


	void SaveFileManager::addToFile(const bg::system::Path & path, geoVarManager * manager)
	{
	}

	void SaveFileManager::saveToFile(const bg::system::Path & path, geoVarManager * manager)
	{	
		using namespace bg::db::json;

		bg::ptr<Value> data = new Value();
		for (int i = 0; i < manager->size(); i++)
		{
			if (manager->get(i).isBool)
				data->setValue(manager->get(i).name, manager->get(i).value < 0.1 ? false : true);
			else
			{
				data->setValue(manager->get(i).name, manager->get(i).value);
			}
		}

		std::ofstream file;
		file.open(path.text(), std::ofstream::trunc);
		if (file.is_open())
		{
			data->writeToStream(file);
		}
				
		file.close();
	}




}

