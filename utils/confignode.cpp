#include <iostream>
#include "confignode.h"
#include "assert.h"

extern std::string dataDirectory;

ConfigNode::ConfigNode(json_t* node) :
	m_Node(node){}

ConfigNode::~ConfigNode(){
	//json_decref(m_Node);
}

ConfigNode ConfigNode::load(const std::string& filename){
	json_error_t error;
	std::string fullName = dataDirectory + filename;
	auto node = json_load_file(fullName.c_str(), 0, &error);
	ASSERT(node, error.source, "Could not load " + filename + "\n" + error.text);
	return ConfigNode(node);
}

int ConfigNode::getInt(const std::string& name, const int defaultValue, const bool useDefault){
	json_t* val = json_object_get(m_Node, name.c_str());
	if (json_is_integer(val))
		return json_integer_value(val);
	ASSERT(useDefault, "??", "Integer " + name + " undefined");
	return defaultValue;
}

double ConfigNode::getDouble(const std::string& name, const double defaultValue, const bool useDefault){
	json_t* val = json_object_get(m_Node, name.c_str());
	if (json_is_real(val))
		return json_real_value(val);
	else if (json_is_integer(val))
		return (double)json_integer_value(val);
	ASSERT(useDefault, "??", "Double " + name + " undefined");
	return defaultValue;
}

std::string ConfigNode::getString(const std::string& name, const std::string& defaultValue, const bool useDefault){
	json_t* val = json_object_get(m_Node, name.c_str());
	if (json_is_string(val))
		return std::string(json_string_value(val));
	ASSERT(useDefault, "??", "String " + name + " undefined");
	return defaultValue;
}

bool ConfigNode::getBool(const std::string& name, const bool defaultValue, const bool useDefault){
	json_t* val = json_object_get(m_Node, name.c_str());
	if (json_is_boolean(val))
		return (bool)json_integer_value(val);
	ASSERT(useDefault, "??", "Boolean " + name + " undefined");
	return defaultValue;
}

Vector2i ConfigNode::getVector2i(const std::string& name, int vecNr){
	vecNr *= 2;

	json_t* arr = json_object_get(m_Node, name.c_str());
	if (json_is_array(arr)){
		json_t* v0 = json_array_get(arr, vecNr);
		json_t* v1 = json_array_get(arr, vecNr+1);
		return Vector2i(json_integer_value(v0), json_integer_value(v1));
	}

	return Vector2i(16, 16);
}

Vector2d ConfigNode::getVector2d(const std::string& name, int vecNr){
	vecNr *= 2;

	json_t* arr = json_object_get(m_Node, name.c_str());
	if (json_is_array(arr)){
		json_t* v0 = json_array_get(arr, vecNr);
		json_t* v1 = json_array_get(arr, vecNr+1);
		return Vector2d(json_real_value(v0), json_real_value(v1));
	}

	return Vector2d(1, 1);
}

ConfigNode ConfigNode::getNode(const std::string& name){
	return ConfigNode(json_object_get(m_Node, name.c_str()));
}

void ConfigNode::forEachNode(std::function<void (ConfigNode&)> elem_func){
	if (json_is_array(m_Node)){
		unsigned int count = json_array_size(m_Node);
		std::cout << "count " <<  count << "\n";
		for (unsigned int i = 0; i < count; ++i){
			ConfigNode childNode(json_array_get(m_Node, i));
			elem_func(childNode);
		}
	} else {
		std::cout << "not an array\n";
	}
}
