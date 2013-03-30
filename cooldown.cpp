#include "cooldown.h"

Cooldown::Cooldown(){
	timeMs = 0;
}

void Cooldown::operator-=(int timeMsDelta){
	timeMs -= timeMsDelta;
	if (timeMs < 0)
		timeMs = 0;
}

void Cooldown::set(int newTime){
	timeMs = newTime;
}

bool Cooldown::done(){
	return timeMs == 0;
}
