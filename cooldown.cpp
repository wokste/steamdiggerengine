#include "cooldown.h"

Cooldown::Cooldown(){
	time = 0;
}

void Cooldown::operator-=(double timeDelta){
	time -= timeDelta;
}

void Cooldown::set(double newTime){
	if (time < newTime)
		time = newTime;
}

void Cooldown::add(double newTime){
	time += newTime;
}

bool Cooldown::done(){
	return time <= 0;
}
