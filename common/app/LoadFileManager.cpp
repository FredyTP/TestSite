#include "LoadFileManager.hpp"

LoadFileManager::LoadFileManager()
{
	_running = false;
	this->loaded = false;
}

void LoadFileManager::run()
{
	if (!_running)
	{
		_running = true;
		_thread = std::thread(ThreadFunction, this);
	}

}

void LoadFileManager::stop()
{
	if (_running)
	{
		_running = false;
		_thread.join();
	}

}

void LoadFileManager::openFile(bg::system::Path & path)
{
	std::ifstream file;
	file.open(path.text());
	std::string text;
	if (file.is_open())
	{
		while(!file.eof())
		{
			std::getline(file, text);
			std::cout << text << std::endl;
		}
		
	}
	file.close();
}

void LoadFileManager::ThreadFunction(LoadFileManager * manager)
{
	using namespace std::chrono_literals;

	while (manager->_running)
	{
		bg::system::Path toLoadPath = bg::system::Path::AppDir();
		toLoadPath.addComponent(".toLoadArchive.txt");

		std::ifstream fileToLoad;
		fileToLoad.open(toLoadPath.text());
		if (fileToLoad.is_open())
		{
			
			manager->_mutex.lock();

			std::string fPath;
			std::getline(fileToLoad, fPath);
			manager->path.setText(fPath);
			manager->openFile(manager->path);
			fileToLoad.close();
			toLoadPath.remove();
			manager->loaded = true;
			manager->_mutex.unlock();
		}
		std::this_thread::sleep_for(100ms);
	}

}
