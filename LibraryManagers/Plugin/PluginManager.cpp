#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include "PluginManager.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <experimental/filesystem>
#include "Plugin.h"
#include "DynLib.h"

namespace fs = std::experimental::filesystem;

PluginManager::~PluginManager()
{
	for (auto it = m_dynLibs.begin();it!=m_dynLibs.end();++it)
	{
		delete it->second;
	}
	m_dynLibs.clear();
}

void PluginManager::loadPlugin(const std::string& pluginPath)
{
	if (pluginPath.empty())
	{
		return;
	}
	if (!m_pluginDir.empty())
	{
		return;
	}
	m_pluginDir = pluginPath;
	std::string dir(m_pluginDir + "../");

	std::string includeFileName("_include");
#ifdef _DEBUG
	includeFileName += "_d";
#endif
	includeFileName += ".txt";

	std::ifstream ifs;
	fs::path fsFileName = fs::u8path(m_pluginDir + includeFileName);
	ifs.open(fsFileName, std::ios::in | std::ios::skipws);
	if (!ifs.good())
	{
		return;
	}
	std::string name;
	while (getline(ifs, name))
	{
		//处理\r\n 类型的文本换行符
		if (!name.empty() && *(name.rbegin()) == '\r') 
		{
			name.erase(name.length() - 1, 1);
		}

		if (m_dynLibs.find(name) != m_dynLibs.end())
		{
			continue;
		}
		m_dynLibs[name] = nullptr;
		m_totalPluginNum++;
	}
	ifs.close();

	for(const auto &dynName : m_dynLibs)
	{
		doLoadPlugin(dynName.first);
		m_loadedPluginNum++;
	}
	m_loadFinished = true;
}

void PluginManager::unloadPlugin(const std::string& name)
{
	std::string dummyName(name);
	std::transform(name.begin(), name.end(), dummyName.begin(), ::toupper);
	auto it = m_dynLibs.find(dummyName);
	if (it != m_dynLibs.end())
	{
		DynLib* dynLib = it->second;
		dynLib->unload();
		delete dynLib;
		m_dynLibs.erase(it);
	}
}

Plugin* PluginManager::getPlugin(const std::string& name)
{
	Plugin* p = nullptr;
	std::string dummyName(name);
	std::transform(name.begin(), name.end(), dummyName.begin(), ::toupper);
	auto it = m_dynLibs.find(dummyName);
	if (it == m_dynLibs.end())
	{
		//强制加载插件
		doLoadPlugin(name);
		//强制加载插件依然可能失败，因为插件可能根本不存在，或者插件存在错误
		it = m_dynLibs.find(dummyName);
	}
	if (it != m_dynLibs.end())
	{
		if (it->second)
		{
			p = it->second->getPlugin();
		}
	}
	return p;
}

void PluginManager::doLoadPlugin(std::string name)
{
	std::string dllName(name);
#ifdef _DEBUG
	dllName += "_d";
#endif
	dllName += ".dll";
	std::string dllPath(m_pluginDir + "/" + dllName);
	DynLib* dynLib = new DynLib(name, dllPath);
	if (!dynLib->load())
	{
		delete dynLib;
		return;
	}
	std::transform(name.begin(), name.end(), name.begin(), ::toupper);
	m_dynLibs[name] = dynLib;
}

int PluginManager::totalPluginNum() const
{
	return m_totalPluginNum;
}

int PluginManager::loadedPluginNum() const
{
	return m_loadedPluginNum;
}

bool PluginManager::loadFinished() const
{
	return m_loadFinished;
}

PluginManager* PluginManager::m_pluginManager = new PluginManager;

LIBRAYMANAGER_API PluginManager* PluginManager::GetPluginManager()
{
	return m_pluginManager;
}
