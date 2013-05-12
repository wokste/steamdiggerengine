#include "cooldown.h"

Cooldown::Cooldown(){
	time = 0;
}

void Cooldown::operator-=(double timeDelta){
	time -= timeDelta;
	if (time < 0)
		time = 0;
}

void Cooldown::set(double newTime){
	time = newTime;
}

bool Cooldown::done(){
	return time == 0;
}
