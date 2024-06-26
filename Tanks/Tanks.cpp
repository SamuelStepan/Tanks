#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Ball.h"
#include "Bullet.h"
#include <vector>
#include <string>
#include <iostream>
#include <list>
using namespace std;
class Tanks : public olc::PixelGameEngine
{
public:
	Tanks(){
		sAppName = "Tanks";
	}
private:
	olc::vi2d map_size = { 510,330 };
	olc::vi2d map_start = { 95,20 };
	int square_size = 30;
	olc::vi2d squares_count;
	vector<string> map;
	list<unique_ptr<Dyn_obj>> tanks;
	vector<unique_ptr<Dyn_obj>> bullets;
	Map_info* map_info;

public:
	bool OnUserCreate() override{
		if (map_size.x % square_size != 0 or map_size.y % square_size != 0) {
			throw invalid_argument("Square size does not match the map size!");
		}
		squares_count = { map_size.x / square_size, map_size.y / square_size };
		map = {
			"#################",
			"##WW####WW###WW##",
			"##W###########W##",
			"#####WWWW########",
			"#W###W######W####",
			"#W###W######W####",
			"#W######W###W####",
			"####WWW##########",
			"#################",
			"##WWW######WWWW##",
			"#######WW########",
			"########W########",
		};
		bullets.resize(100);
		map_info = new Map_info(map_size, map_start, square_size, squares_count, map, tanks, bullets);
		tanks.push_back(unique_ptr<Ball>(new Ball(1, false, 1, map_info)));
		tanks.push_back(unique_ptr<Ball>(new Ball(2, false, 2, map_info)));
		tanks.push_back(unique_ptr<Ball>(new Ball(3, false, 3, map_info)));
		return true;
	}

	bool OnUserUpdate(float elapsed_t) override{
		Clear(olc::BLACK);
		FillRect({ map_start.x - 10, map_start.y - 10 }, { map_size.x + 20,10 }, olc::WHITE);
		FillRect({ map_start.x - 10, map_start.y + map_size.y }, { map_size.x + 20,10 }, olc::WHITE);
		FillRect({ map_start.x - 10, map_start.y }, { 10,map_size.y }, olc::WHITE);
		FillRect({ map_start.x + map_size.x, map_start.y }, { 10,map_size.y }, olc::WHITE);
		for (int i = 0; i < squares_count.y; i++){
			for (int j = 0; j < squares_count.x; j++){
				if (map[i][j] == 'W')FillRect(map_start + olc::vi2d{ j * square_size,i * square_size }, { square_size,square_size }, olc::DARK_GREY);		
			}
		}
		for (list<unique_ptr<Dyn_obj>>::iterator i = tanks.begin(); i != tanks.end(); i++) {
			(*i)->Update(elapsed_t, this);
			(*i)->Draw_self(elapsed_t, this);
			if ((*i)->to_delete) {
				tanks.erase(i);
				break;
			}
		}
		for (auto& bullet : bullets) {
			if (bullet != unique_ptr<Dyn_obj>(nullptr)) {
				bullet->Update(elapsed_t, this);
				bullet->Draw_self(elapsed_t, this);
				if (bullet->to_delete) bullet = unique_ptr<Dyn_obj>(nullptr);
			}
		}
		return true;
	}
};

int main(){
	Tanks Tanks;
	if (Tanks.Construct(700, 400, 2, 2))
		Tanks.Start();
	return 0;
}


