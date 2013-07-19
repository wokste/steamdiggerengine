#pragma once

class Cooldown{
	double time;
public:
	Cooldown();
	void operator-=(double timeDelta);
	void set(double newTime);
	void add(double newTime);
	bool done();
};
