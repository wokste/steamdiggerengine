#pragma once

class PerlinNoise{
public:
	PerlinNoise(int newSeed, int newOctaves, double newPersistence, double newScale = 1);
	double noise2d(double x, double y) const;
private:
	double random2D(int x,int y) const;
	double interpolate(double value0,double value1,double fraction) const;
	int floorInt(double x) const;
	double noiseIteration2d(double x,double y) const;

	int seed;
	int octaves;
	double persistence;
	double scale;
};
