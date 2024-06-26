#pragma once
#include <cmath>
#include <vector>
#include <list>
#include <string>
#include "Dyn_obj.h"
#include "Map_info.h"
#include "olcPixelGameEngine.h"
using namespace std;
class Dyn_obj;

//function that solves out if collision between ball and rectangle happened
bool ball_vs_map(Dyn_obj* ball, Map_info* map_info, olc::vf2d &next_pos, olc::PixelGameEngine* pge) {
	olc::vf2d nearest_p; //nearest point between ball and rectangle
	olc::vf2d vector_to_rect; //vector from middle of ball to rectangle(closest range)
	float over_lap;	
	bool collision = false;
	if (next_pos.x < ball->radius) {
		next_pos.x = ball->radius;
		collision = true;
	}
	else if (next_pos.x > map_info->map_size.x - ball->radius) {
		next_pos.x = map_info->map_size.x - ball->radius;
		collision = true;
	}
	if (next_pos.y < ball->radius) {
		next_pos.y = ball->radius;
		collision = true;
	}
	else if (next_pos.y > map_info->map_size.y - ball->radius) {
		next_pos.y = map_info->map_size.y - ball->radius;
		collision = true;
	}
	if (collision) return false;
	olc::vi2d column_range = { int(floor(ball->pos.x / map_info->square_size) - 1),int(floor(ball->pos.x / map_info->square_size) + 1) };
	olc::vi2d row_range = { int(floor(ball->pos.y / map_info->square_size) - 1),int(floor(ball->pos.y / map_info->square_size) + 1) };
	if (column_range.x < 0) column_range.x = 0;
	else if (column_range.y > map_info->squares_count.x - 1) column_range.y = map_info->squares_count.x - 1;
	if (row_range.x < 0) row_range.x = 0;
	else if (row_range.y > map_info->squares_count.y - 1) row_range.y = map_info->squares_count.y - 1;
	/*if (ball->get_ID() == 0) {
		pge->DrawString(olc::vi2d{ 0,0 }, "columns are " + to_string(column_range.x) + "to " + to_string(column_range.y));
		pge->DrawString(olc::vi2d{ 0,50 }, "rows are " + to_string(row_range.x) + "to " + to_string(row_range.y));
	}*/
	for (int column = column_range.x; column <= column_range.y; column++){
		for (int row = row_range.x; row <= row_range.y; row++){
			/*pge->DrawRect(map_info->map_start + olc::vf2d{ float(column * map_info->square_size), float(row * map_info->square_size) }, olc::vf2d{ float(map_info->square_size), float(map_info->square_size) }, olc::RED);*/
			if (map_info->map[row][column] == 'W') {
				//clamping, calculates the closest point at rectangle to ball
				nearest_p.x = max(float(column*map_info->square_size), min(next_pos.x, float((column + 1) * map_info->square_size)));
				nearest_p.y = max(float(row * map_info->square_size), min(next_pos.y, float((row + 1) * map_info->square_size)));

				vector_to_rect = nearest_p - next_pos;
				over_lap = ball->radius - vector_to_rect.mag();
				if (std::isnan(over_lap)) over_lap = 0;
				//collision happened
				if (over_lap > 0) {
					next_pos = next_pos - ball->dir * (over_lap);	//moves ball back, it touches the rectangle				
					collision = true;
				}
			}
		}
	}
	if (collision) return false;
	return true;
}
bool ball_vs_ball(Dyn_obj* ball, olc::vf2d& next_pos, olc::PixelGameEngine* pge, Map_info* map_info) {
	for (auto& tank : *map_info->tanks) {
		if (ball->bullet) 
			if((next_pos - tank->pos).mag() < ball->radius + tank->radius) {
				next_pos = ball->pos - ball->dir * (ball->radius + tank->radius - (next_pos - tank->pos).mag());
				tank->Hit(ball->get_dmg());
				return false;
		}
		else if (ball->get_ID() != tank->get_ID()) {
			if ((next_pos - tank->pos).mag() < ball->radius + tank->radius) {
				next_pos = ball->pos - ball->dir * (ball->radius + tank->radius - (next_pos - tank->pos).mag());
				return false;
			}
		}
	}
	return true;
}
