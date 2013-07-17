#include <iostream>
#include "confignode.h"
#include "assert.h"
#include "../../external/jansson-2.4/src/jansson.h"

ConfigNode::ConfigNode(json_t* node) :
	m_Node(node){}

ConfigNode::~ConfigNode(){
	//json_decref(m_Node);
}

void ConfigNode::load(const std::string& filename, std::function<void (ConfigNode&)> onLoad){
	json_error_t error;
	json_t* node = json_load_file(filename.c_str(), 0, &error);
	ASSERT(node, error.source, "Error: Could not load " + filename + "\n" + error.text);
	auto cnode = ConfigNode(node);
	onLoad(cnode);
	json_decref(node);
}

int ConfigNode::getInt(const std::string& name, const int defaultValue, const bool useDefault) const{
	if (m_Node != nullptr){
		json_t* val = json_object_get(m_Node, name.c_str());
		if (json_is_integer(val))
			return json_integer_value(val);
	}
	ASSERT(useDefault, "??", "Warning: Integer " + name + " undefined");
	return defaultValue;
}

double ConfigNode::getDouble(const std::string& name, const double defaultValue, const bool useDefault) const{
	if (m_Node != nullptr){
		json_t* val = json_object_get(m_Node, name.c_str());
		if (json_is_real(val))
			return json_real_value(val);
		else if (json_is_integer(val))
			return (double)json_integer_value(val);
	}
	ASSERT(useDefault, "??", "Warning: Double " + name + " undefined");
	return defaultValue;
}

const std::string ConfigNode::getString(const std::string& name, const std::string& defaultValue, const bool useDefault) const{
	if (m_Node != nullptr){
		json_t* val = json_object_get(m_Node, name.c_str());
		if (json_is_string(val))
			return std::string(json_string_value(val));
	}
	ASSERT(useDefault, "??", "Warning: String " + name + " undefined");
	return defaultValue;
}

bool ConfigNode::getBool(const std::string& name, const bool defaultValue, const bool useDefault) const{
	if (m_Node == nullptr)
		return defaultValue;

	json_t* val = json_object_get(m_Node, name.c_str());
	if (defaultValue)
		return !json_is_false(val);
	else
		return json_is_true(val);
}

Vector2i ConfigNode::getVector2i(const std::string& name, int vecNr) const{
	if (m_Node != nullptr){
		vecNr *= 2;

		json_t* arr = json_object_get(m_Node, name.c_str());
		if (json_is_array(arr)){
			json_t* v0 = json_array_get(arr, vecNr);
			json_t* v1 = json_array_get(arr, vecNr+1);
			return Vector2i(json_integer_value(v0), json_integer_value(v1));
		}
	}
	return Vector2i(16, 16);
}

Vector2d ConfigNode::getVector2d(const std::string& name, int vecNr) const{
	if (m_Node != nullptr){
		vecNr *= 2;

		json_t* arr = json_object_get(m_Node, name.c_str());
		if (json_is_array(arr)){
			json_t* v0 = json_array_get(arr, vecNr);
			json_t* v1 = json_array_get(arr, vecNr+1);
			return Vector2d(json_real_value(v0), json_real_value(v1));
		}
	}
	return Vector2d(1, 1);
}

ConfigNode ConfigNode::getNode(const std::string& name){
	if (m_Node == nullptr)
		return ConfigNode(nullptr);
	return ConfigNode(json_object_get(m_Node, name.c_str()));
}

const ConfigNode ConfigNode::getNodeConst(const std::string& name) const{
	if (m_Node == nullptr)
		return ConfigNode(nullptr);
	return ConfigNode(json_object_get(m_Node, name.c_str()));
}

void ConfigNode::forEachNode(std::function<void (ConfigNode&)> elem_func){
	if (m_Node == nullptr || json_is_null(m_Node))
		return; // treat nullptr as empty array.

	if (json_is_array(m_Node)){
		unsigned int count = json_array_size(m_Node);
		for (unsigned int i = 0; i < count; ++i){
			ConfigNode childNode(json_array_get(m_Node, i));
			elem_func(childNode);
		}
	} else {
		elem_func(*this);
	}
}
