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

#include "src/utils/filesystem.h"

#include <iostream>
#include <cstdlib>
#include <dirent.h>

FileSystem g_FileSystem;

FileSystem::FileSystem() : dataDir("data/"){
}

std::string FileSystem::fullpath(const std::string& resourcename) const{
	return dataDir + resourcename;
}

// Inspired by http://www.gnu.org/savannah-checkouts/gnu/libc/manual/html_node/Simple-Directory-Lister.html
std::vector<std::string> FileSystem::getList(std::string extention) const{
	std::vector<std::string> fileNames;
	DIR *dir;
	dirent *directoryEntry;

	dir = opendir(dataDir.c_str());
	if (dir != nullptr){
		while (directoryEntry = readdir(dir)){
			std::string name = directoryEntry->d_name;
			if (name.length() > extention.length() && name.compare(name.length() - extention.length(),extention.length(),extention) == 0)
				fileNames.push_back(name);
		}
		closedir(dir);
	} else
		std::cerr << "Couldn't open file directory";

	return fileNames;
}
