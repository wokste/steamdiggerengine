/*
The MIT License (MIT)

Copyright (c) 2013, Steven Wokke

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
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

	int getInt(const std::string& name) const {return getInt(name, 0, false);}
	int getInt(const std::string& name, const int defaultValue, const bool useDefault = true) const;
	double getDouble(const std::string& name) const {return getDouble(name, 0.0, false);}
	double getDouble(const std::string& name, const double defaultValue, const bool useDefault = true) const;
	const std::string getString(const std::string& name) const {return getString(name, "", false);}
	const std::string getString(const std::string& name, const std::string& defaultValue, const bool useDefault = true) const;
	bool getBool(const std::string& name) const {return getBool(name, false, false);}
	bool getBool(const std::string& name, const bool defaultValue, const bool useDefault = true) const;
	Vector2i getVector2i(const std::string& name, int vecNr = 0) const;
	Vector2d getVector2d(const std::string& name, int vecNr = 0) const;
	ConfigNode getNode(const std::string& name);
	const ConfigNode getNodeConst(const std::string& name) const;
	void forEachNode(std::function<void (ConfigNode&)> elem_func);
	ConfigNode operator[](int id);
};
