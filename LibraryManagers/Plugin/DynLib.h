#pragma once
#include <string>
/*!
 * @brief 插件类，负责插件的加载和卸载.仅供PluginManager调用
 *
 * @author wanggaoliang
 * @date 2021/06/18
 */
class Plugin;
class QLibrary;
class DynLib
{
public:
	DynLib(const std::string& name,const std::string& path);
	~DynLib();
	//加载一个动态库 
	bool load();
	//卸载动态库
	bool unload();
	//获取动态库的名字
	const std::string& getName(void) const;
	//获取DllInterface的指针，用户可强制转换成自己对应的指针，以调用相应的接口
	Plugin* getPlugin() const;
private:
	std::string m_name;/*!<动态库的名字,和模块的名字一致*/
	std::string m_path;/*!<动态库所在的全路径*/
	QLibrary* m_lib{ nullptr };/*!<动态库加载完成后的句柄，在卸载时会使用该句柄*/
	Plugin* m_plugin{ nullptr };/*!<插件的地址，用户可以通过插件管理器获取插件的地址.一个动态库对应一个插件*/
};
