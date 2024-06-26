#pragma once
#include "olcPixelGameEngine.h"
#include "vector"
#include "string"
using namespace std;
class Dyn_obj;

class Map_info{
public:
	olc::vi2d map_size;
	olc::vi2d map_start;
	int square_size;
	olc::vi2d squares_count;
	vector<string> map;
	list<unique_ptr<Dyn_obj>>* tanks;
	vector<unique_ptr<Dyn_obj>>* bullets;
public:
	Map_info(olc::vi2d &map_size, olc::vi2d &map_start, int &square_size, olc::vi2d &squares_count, vector<string> &map, list<unique_ptr<Dyn_obj>> &tanks, vector<unique_ptr<Dyn_obj>> &bullets) {
		this->map_size = map_size;
		this->map_start = map_start;
		this->square_size = square_size;
		this->squares_count = squares_count;
		this->map = map;
		this->tanks = &tanks;
		this->bullets = &bullets;
	}
};

