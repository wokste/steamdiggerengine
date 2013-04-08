#pragma once

#include <string>
#include <functional>
#include "vector2.h"
#include "../../external/jansson-2.4/src/jansson.h"

class ConfigNode{
	json_t* m_Node; // TODO: manage this pointer

public:
	//ConfigNode();
	ConfigNode(json_t* node);
	~ConfigNode();
	static void load(const std::string& filename, std::function<void (ConfigNode&)> load_func);

	int getInt(const std::string& name){return getInt(name, 0, false);}
	int getInt(const std::string& name, const int defaultValue, const bool useDefault = true);
	double getDouble(const std::string& name){return getDouble(name, 0.0, false);}
	double getDouble(const std::string& name, const double defaultValue, const bool useDefault = true);
	std::string getString(const std::string& name){return getString(name, "", false);}
	std::string getString(const std::string& name, const std::string& defaultValue, const bool useDefault = true);
	bool getBool(const std::string& name){return getBool(name, false, false);}
	bool getBool(const std::string& name, const bool defaultValue, const bool useDefault = true);
	Vector2i getVector2i(const std::string& name, int vecNr = 0);
	Vector2d getVector2d(const std::string& name, int vecNr = 0);
	ConfigNode getNode(const std::string& name);
	void forEachNode(std::function<void (ConfigNode&)> elem_func);
};
