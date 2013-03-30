#pragma once

class Cooldown{
	int timeMs;
public:
	Cooldown();
	void operator-=(int timeMsDelta);
	void set(int newTime);
	bool done();
};
