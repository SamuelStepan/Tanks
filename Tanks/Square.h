#pragma once
#include "olcPixelGameEngine.h"
#include <vector>
using namespace std;

class Square
{
public:
	olc::vi2d pos;
	int team;
	int health;
	vector<bool> solid;
	olc::Decal img;
};

