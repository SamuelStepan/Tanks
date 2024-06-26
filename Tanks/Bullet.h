#pragma once
#include "Dyn_obj.h"
#include "Map_info.h"
#include "Detection.h"
#include <cmath>
class Bullet :
    public Dyn_obj
{
public:
    int team;
    int dmg;
    float angle;
    bool bounce;
    int bounce_n;   //number of possible bounces
    bool invisible;
    Map_info* map_info;
    olc::Decal* bullet_img;
    olc::Pixel color;
public:
    Bullet(olc::vf2d pos, float radius, float angle, int team, float dmg, Map_info* &map_info, olc::Decal* bullet_img, olc::Pixel color) {
        this->angle = angle;
        this->dir = olc::vf2d{ float(sin(angle)), - float(cos(angle)) };
        this->pos = pos + dir * (radius + 7.0f);
        this->team = team;
        this->dmg = dmg;
        this->map_info = map_info;
        this->bullet_img = bullet_img;
        this->color = color;
        this->radius = 2.5f;
        bullet = true;
        speed = 80.0f;
    }
    virtual float get_dmg() override {
        return dmg;
    }
    virtual void Update(float elapsed_t, olc::PixelGameEngine* pge) override {
        olc::vf2d next_pos = pos + dir * speed * elapsed_t;
        if (!ball_vs_ball(this, next_pos, pge, map_info)) to_delete = true;
        if (ball_vs_map(this, map_info, next_pos, pge))pos = next_pos;
        else to_delete = true;
    }
    virtual void Draw_self(float elapsed_t, olc::PixelGameEngine* pge) override {
        /*pge->DrawCircle(pos + map_info->map_start, radius);*/
        pge->DrawRotatedDecal(pos + map_info->map_start, bullet_img, angle, { 11.5f,10.5f }, { 5.0f / 23.0f, 5.0f / 21.0f }, color);
    }

};

