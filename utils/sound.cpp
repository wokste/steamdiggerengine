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

#include "src/utils/sound.h"
#include <iostream>
#include <map>

#include "src/utils/filesystem.h"
#include "src/utils/random.h"

namespace SoundSystem{
	constexpr int NUM_SOUNDS = 16;
	sf::Sound* findUnusedSound();
	std::shared_ptr<Sound> findLoadedSound(const std::string& fileName);

	// TODO: Keep a list of all buffers that are currently used for playing the sound.
	sf::Sound soundSlots[NUM_SOUNDS];
	std::map<std::string,std::shared_ptr<Sound>> loadedSounds;
};


sf::Sound* SoundSystem::playSound(std::shared_ptr<Sound> sound){
	if (!sound)
		return nullptr;

	sf::Sound* slot = findUnusedSound();

	if (slot){
		slot->setLoop(false);
		slot->setBuffer(sound->buffer);
		slot->setPitch(g_Random.generate(0.5, 1.5));
		slot->play();
	}
	return slot;
}

sf::Sound* SoundSystem::findUnusedSound(){
	for (auto& slot: soundSlots){
		if (slot.getStatus() == sf::SoundSource::Stopped)
			return &slot;
	}
	return nullptr;
}

std::shared_ptr<Sound> SoundSystem::loadSound(const std::string& fileName){
	std::shared_ptr<Sound> sound = findLoadedSound(fileName);
	if (!sound && fileName != ""){
		sound.reset(new Sound());
		if (sound->buffer.loadFromFile (g_FileSystem.fullpath(fileName))){
			loadedSounds[fileName]=sound;
		} else {
			std::cout << "Could not load sound " << fileName << " \n";
			sound.reset();
		}
	}
	return sound;
}

std::shared_ptr<Sound> SoundSystem::findLoadedSound(const std::string& key){
	auto iterator = loadedSounds.find(key);
	if (iterator == loadedSounds.end()) {
		return std::shared_ptr<Sound>();
	} else {
		return iterator->second;
	}
}
