#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H
#include <string>
#include <map>
#include "LibraryManagersDef.h"

class DynLib;
class Plugin;
class  PluginManager
{
public:
	~PluginManager();
private:
	PluginManager() {};

private:
	void unloadPlugin(const std::string& name);
	int totalPluginNum() const;
	int loadedPluginNum() const;
	bool loadFinished() const;
private:
	void doLoadPlugin(std::string name);
private:
	std::map<std::string, DynLib*> m_dynLibs;
	std::string m_pluginDir;
	int m_totalPluginNum{ 0 };
	int m_loadedPluginNum{ 0 };
	bool m_loadFinished{ false };
public:
	LIBRAYMANAGER_API void loadPlugin(const std::string& pluginDir);
	LIBRAYMANAGER_API Plugin* getPlugin(const std::string& name);
	static LIBRAYMANAGER_API PluginManager* GetPluginManager();
private:
	static PluginManager* m_pluginManager;
};

#endif // PLUGINMANAGER_H
