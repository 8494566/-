#ifndef PLUGIN_H
#define PLUGIN_H
#include "LibraryManagersDef.h"
/*!
 * @brief 插件类，开发接口需集成Plugin类,getPlugin接口才能调用
 *
 * @author wanggaoliang
 * @date 2021/06/18 
 */

class LIBRAYMANAGER_API Plugin
{
public:
	virtual bool initialise();
	virtual bool initFromConfig(const char * config);
};


inline bool Plugin::initialise()
{
	return true;
}

inline bool Plugin::initFromConfig(const char* config)
{
	return true;
}

#endif