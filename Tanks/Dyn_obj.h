#pragma once
#include "olcPixelGameEngine.h"
class Dyn_obj{
protected:
	int id;
	static int max_id;
public:
	int radius;
	float speed;
	bool to_delete = false;
	bool bullet;
	olc::vf2d dir;
	olc::vf2d pos;
public:
	virtual void Draw_self(float elapsed_t, olc::PixelGameEngine* pge) {

	}
	virtual void Update(float elapsed_t, olc::PixelGameEngine* pge) {

	}
	int get_ID() {
		return id;
	}
	virtual float get_dmg() {
		return 0.0f;
	}
	virtual void Hit(float dmg) {

	}
};
int Dyn_obj::max_id = 0;


