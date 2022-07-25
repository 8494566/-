#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include "DynLib.h"
#include <QLibrary>
#include <experimental/filesystem>
#include "Plugin.h"

namespace fs = std::experimental::filesystem;
typedef Plugin* (*CreateInterfaceFunc)();
typedef void (*ReleaseInterfaceFunc)();

DynLib::DynLib(const std::string & name, const std::string & path)
	:m_name(name)
	,m_path(path)
	, m_lib(new QLibrary(path.c_str()))
{

}

DynLib::~DynLib()
{
	unload();
	delete m_lib;
}

bool DynLib::load()
{
	if (m_plugin != nullptr)
	{
		return true;
	}

	if (!m_lib->load())
	{
		return false;
	}
	
	auto createInterface = (CreateInterfaceFunc)m_lib->resolve("CreateInterface");
	if (createInterface==nullptr/* || releaseInterface == nullptr*/)
	{
		return false;
	}

	m_plugin = (*createInterface)();
	if (!m_plugin)
	{
		return false;
	}
	if (!m_plugin->initialise())
	{
		return false;
	}
	return true;
}

bool DynLib::unload()
{
	auto releaseInterface = (ReleaseInterfaceFunc)m_lib->resolve("ReleaseInterface");
	if (releaseInterface != nullptr)
	{
		(*releaseInterface)();
	}
	if (m_lib->isLoaded())
	{
		m_lib->unload();
	}
	return true;
}

const std::string& DynLib::getName(void) const
{
	return m_name;
}

Plugin* DynLib::getPlugin() const
{
	return m_plugin;
}

