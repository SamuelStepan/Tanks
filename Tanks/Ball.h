#pragma once
#include "Dyn_obj.h"
#include "Bullet.h"
#include "Map_info.h"
#include "Detection.h"
#include <math.h>
#include <stdexcept>
class Bullet;

class Ball :
    public Dyn_obj{
private:
    int player;
    bool AI;
    int max_shield;
    float recharge_speed;
    int max_ammo;
    int max_hp;
    float reload_time;   
    float hull_dir;
    float rot_speed;
    bool rot_right;
    float time_rotating;
    float time_reloading;
    float time_since_shot;
    float time_since_hit;
    bool was_hit;
public:
    int team;
    float angle;
    int weapon_type;
    int dmg;
    float armor;
    float shield;
    int hp;
    int ammo;
    olc::Pixel color;
    Map_info* map_info;
    olc::Decal* hull_img;
    olc::Decal* turret_img;
    olc::Decal* bullet_img;
public:
    Ball(int player, bool AI, int team, Map_info* &map_info, olc::vi2d starting_pos = {0,0}) {
        id = max_id++;
        this->player = player;
        this->AI = AI;
        this->team = team;
        hull_dir = 0.0f;
        angle = 0.0f;
        rot_speed = 2.0f;
        rot_right = false;
        time_rotating = 0.0f;
        speed = 50.0f;
        max_ammo = 10;
        ammo = max_ammo;
        max_hp = 100;
        hp = max_hp;
        max_shield = 50;
        shield = max_shield;
        recharge_speed = 3.0f;
        armor = 0.0f;
        time_since_shot = 1.0f;
        reload_time = 2.0f;
        weapon_type = 1;
        dmg = 10;
        bullet = false;
        was_hit = false;
        radius = 10;
        dir = { 0.0f,0.0f };
        this->map_info = map_info;
        if (team == 1) {
            if (!AI) {
                pos.x = map_info->square_size / 2;
                pos.y = map_info->square_size / 2;
            }
            color = olc::YELLOW;
        }
        else if (team == 2) {
            if (!AI) {
                pos.x = map_info->map_size.x - this->map_info->square_size / 2;
                pos.y = map_info->square_size / 2;
            }
            color = olc::CYAN;
        }
        else if (team == 3) {
            if (!AI) {
                pos.x = map_info->map_size.x - this->map_info->square_size / 2;
                pos.y = this->map_info->map_size.y - this->map_info->square_size / 2;
            }
            color = olc::GREEN;
        }
        //else if (team == 4) {
        //    pos.x = map_info->square_size / 2;
        //    pos.y = this->map_info->map_size.y - this->map_info->square_size / 2;
        //    color = olc::YELLOW;
        //}
        if(AI) pos = starting_pos;
        hull_img = new olc::Decal(new olc::Sprite("./img/Tank/Tank.png"));
        turret_img = new olc::Decal(new olc::Sprite("./img/Tank/GunTurret.png"));
        bullet_img = new olc::Decal(new olc::Sprite("./img/Tank/Bullet.png"));
    }
    void Move(float elapsed_t, olc::PixelGameEngine* pge) {
        olc::vf2d next_pos;
        next_pos = pos + elapsed_t * speed * dir;
        ball_vs_map(this, map_info, next_pos, pge);
        ball_vs_ball(this, next_pos, pge, map_info);
        pos = next_pos;
        
        
    }
    virtual void Update(float elapsed_t, olc::PixelGameEngine* pge) override {
        if (team == 1 and !AI) {
            if ((pge->GetKey(olc::Key::W).bPressed or pge->GetKey(olc::Key::W).bHeld) and (pge->GetKey(olc::Key::A).bPressed or pge->GetKey(olc::Key::A).bHeld)) dir = olc::vf2d{ -1,-1 };
            else if ((pge->GetKey(olc::Key::W).bPressed or pge->GetKey(olc::Key::W).bHeld) and (pge->GetKey(olc::Key::D).bPressed or pge->GetKey(olc::Key::D).bHeld)) dir = olc::vf2d{ 1,-1 };
            else if ((pge->GetKey(olc::Key::S).bPressed or pge->GetKey(olc::Key::S).bHeld) and (pge->GetKey(olc::Key::A).bPressed or pge->GetKey(olc::Key::A).bHeld)) dir = olc::vf2d{ -1,1 };
            else if ((pge->GetKey(olc::Key::S).bPressed or pge->GetKey(olc::Key::S).bHeld) and (pge->GetKey(olc::Key::D).bPressed or pge->GetKey(olc::Key::D).bHeld)) dir = olc::vf2d{ 1,1 };
            else if (pge->GetKey(olc::Key::W).bPressed or pge->GetKey(olc::Key::W).bHeld) dir = olc::vf2d{ 0,-1 };
            else if (pge->GetKey(olc::Key::S).bPressed or pge->GetKey(olc::Key::S).bHeld) dir = olc::vf2d{ 0,1 };
            else if (pge->GetKey(olc::Key::A).bPressed or pge->GetKey(olc::Key::A).bHeld) dir = olc::vf2d{ -1,0 };
            else if (pge->GetKey(olc::Key::D).bPressed or pge->GetKey(olc::Key::D).bHeld) dir = olc::vf2d{ 1,0 };
            else dir = olc::vf2d{ 0,0 };
            if ((pge->GetKey(olc::Key::C).bPressed or pge->GetKey(olc::Key::C).bHeld)) {
                if (rot_right) time_rotating = 0.0f;
                rot_right = false;
                time_rotating += elapsed_t;
                angle = time_rotating > 0.5f ? angle - 2.0f * rot_speed * elapsed_t : angle - rot_speed * elapsed_t;
            }
            else if ((pge->GetKey(olc::Key::B).bPressed or pge->GetKey(olc::Key::B).bHeld)) {
                if (!rot_right) time_rotating = 0.0f;
                rot_right = true;
                time_rotating += elapsed_t;
                angle = time_rotating > 0.5f ? angle + 2.0f * rot_speed * elapsed_t : angle + rot_speed * elapsed_t;
            }
            else time_rotating = 0.0f;
            if ((pge->GetKey(olc::Key::V).bPressed)) {
                if (ammo > 0 and time_since_shot >= 0.3f) {
                    for (auto& bullet : *map_info->bullets) if (bullet == unique_ptr<Dyn_obj>(nullptr)) {
                        bullet = unique_ptr<Bullet>(new Bullet(pos, radius, angle, team, dmg, map_info, bullet_img, color));
                        break;
                    }
                    ammo -= 1;
                    time_since_shot = 0.0f;
                }
            }
        }
        else if (team == 2 and !AI) {
            if ((pge->GetKey(olc::Key::I).bPressed or pge->GetKey(olc::Key::I).bHeld) and (pge->GetKey(olc::Key::J).bPressed or pge->GetKey(olc::Key::J).bHeld)) dir = olc::vf2d{ -1,-1 };
            else if ((pge->GetKey(olc::Key::I).bPressed or pge->GetKey(olc::Key::I).bHeld) and (pge->GetKey(olc::Key::L).bPressed or pge->GetKey(olc::Key::L).bHeld)) dir = olc::vf2d{ 1,-1 };
            else if ((pge->GetKey(olc::Key::K).bPressed or pge->GetKey(olc::Key::K).bHeld) and (pge->GetKey(olc::Key::J).bPressed or pge->GetKey(olc::Key::J).bHeld)) dir = olc::vf2d{ -1,1 };
            else if ((pge->GetKey(olc::Key::K).bPressed or pge->GetKey(olc::Key::K).bHeld) and (pge->GetKey(olc::Key::L).bPressed or pge->GetKey(olc::Key::L).bHeld)) dir = olc::vf2d{ 1,1 };
            else if (pge->GetKey(olc::Key::I).bPressed or pge->GetKey(olc::Key::I).bHeld) dir = olc::vf2d{ 0,-1 };
            else if (pge->GetKey(olc::Key::K).bPressed or pge->GetKey(olc::Key::K).bHeld) dir = olc::vf2d{ 0,1 };
            else if (pge->GetKey(olc::Key::J).bPressed or pge->GetKey(olc::Key::J).bHeld) dir = olc::vf2d{ -1,0 };
            else if (pge->GetKey(olc::Key::L).bPressed or pge->GetKey(olc::Key::L).bHeld) dir = olc::vf2d{ 1,0 };
            else dir = olc::vf2d{ 0,0 };
            if ((pge->GetKey(olc::Key::LEFT).bPressed or pge->GetKey(olc::Key::LEFT).bHeld)) {
                if (rot_right) time_rotating = 0.0f;
                rot_right = false;
                time_rotating += elapsed_t;
                angle = time_rotating > 0.5f ? angle - 2.0f * rot_speed * elapsed_t : angle - rot_speed * elapsed_t;
            }
            else if ((pge->GetKey(olc::Key::RIGHT).bPressed or pge->GetKey(olc::Key::RIGHT).bHeld)) {
                if (!rot_right) time_rotating = 0.0f;
                rot_right = true;
                time_rotating += elapsed_t;
                angle = time_rotating > 0.5f ? angle + 2.0f * rot_speed * elapsed_t : angle + rot_speed * elapsed_t;
            }
            else time_rotating = 0.0f;
            if ((pge->GetKey(olc::Key::UP).bPressed)) {
                if (ammo > 0 and time_since_shot >= 0.3f) {
                    for (auto& bullet : *map_info->bullets) if (bullet == unique_ptr<Dyn_obj>(nullptr)) {
                        bullet = unique_ptr<Bullet>(new Bullet(pos, radius, angle, team, dmg, map_info, bullet_img, color));
                        break;
                    }
                    ammo -= 1;
                    time_since_shot = 0.0f;
                }
            }
        }
        else if (team == 3 and !AI) {
            if ((pge->GetKey(olc::Key::NP5).bPressed or pge->GetKey(olc::Key::NP5).bHeld) and (pge->GetKey(olc::Key::NP1).bPressed or pge->GetKey(olc::Key::NP1).bHeld)) dir = olc::vf2d{ -1,-1 };
            else if ((pge->GetKey(olc::Key::NP5).bPressed or pge->GetKey(olc::Key::NP5).bHeld) and (pge->GetKey(olc::Key::NP3).bPressed or pge->GetKey(olc::Key::NP3).bHeld)) dir = olc::vf2d{ 1,-1 };
            else if ((pge->GetKey(olc::Key::NP2).bPressed or pge->GetKey(olc::Key::NP2).bHeld) and (pge->GetKey(olc::Key::NP1).bPressed or pge->GetKey(olc::Key::NP1).bHeld)) dir = olc::vf2d{ -1,1 };
            else if ((pge->GetKey(olc::Key::NP2).bPressed or pge->GetKey(olc::Key::NP2).bHeld) and (pge->GetKey(olc::Key::NP3).bPressed or pge->GetKey(olc::Key::NP3).bHeld)) dir = olc::vf2d{ 1,1 };
            else if (pge->GetKey(olc::Key::NP5).bPressed or pge->GetKey(olc::Key::NP5).bHeld) dir = olc::vf2d{ 0,-1 };
            else if (pge->GetKey(olc::Key::NP2).bPressed or pge->GetKey(olc::Key::NP2).bHeld) dir = olc::vf2d{ 0,1 };
            else if (pge->GetKey(olc::Key::NP1).bPressed or pge->GetKey(olc::Key::NP1).bHeld) dir = olc::vf2d{ -1,0 };
            else if (pge->GetKey(olc::Key::NP3).bPressed or pge->GetKey(olc::Key::NP3).bHeld) dir = olc::vf2d{ 1,0 };
            else dir = olc::vf2d{ 0,0 };
            if ((pge->GetKey(olc::Key::NP_DIV).bPressed or pge->GetKey(olc::Key::NP_DIV).bHeld)) {
                if (rot_right) time_rotating = 0.0f;
                rot_right = false;
                time_rotating += elapsed_t;
                angle = time_rotating > 0.5f ? angle - 2.0f * rot_speed * elapsed_t : angle - rot_speed * elapsed_t;
            }
            else if ((pge->GetKey(olc::Key::NP_SUB).bPressed or pge->GetKey(olc::Key::NP_SUB).bHeld)) {
                if (!rot_right) time_rotating = 0.0f;
                rot_right = true;
                time_rotating += elapsed_t;
                angle = time_rotating > 0.5f ? angle + 2.0f * rot_speed * elapsed_t : angle + rot_speed * elapsed_t;
            }
            else time_rotating = 0.0f;
            if ((pge->GetKey(olc::Key::NP_MUL).bPressed)) {
                if (ammo > 0 and time_since_shot >= 0.3f) {
                    for (auto& bullet : *map_info->bullets) if (bullet == unique_ptr<Dyn_obj>(nullptr)) {
                        bullet = unique_ptr<Bullet>(new Bullet(pos, radius, angle, team, dmg, map_info, bullet_img, color));
                        break;
                    }
                    ammo -= 1;
                    time_since_shot = 0.0f;
                }
            }
        }
        dir = (abs(dir.x) + abs(dir.y) > 0 )? dir.norm() : olc::vf2d{ 0.0f, 0.0f };
        if (ammo == 0) {
            time_reloading += elapsed_t;
            if (time_reloading >= reload_time) {
                ammo = max_ammo;
                time_reloading = 0;
            }
        }
        if (time_since_shot < 0.3f) time_since_shot += elapsed_t;
        if (was_hit) time_since_hit += elapsed_t;
        if (time_since_hit > 5.0f) {
            shield += elapsed_t * (float(max_shield) / recharge_speed);
            if (shield > max_shield) {
                shield = max_shield;
                was_hit = false;
                time_since_hit = 0.0f;
            }
        }
        Move(elapsed_t, pge);    
    }
    virtual void Draw_self(float elapsed_t, olc::PixelGameEngine* pge) override{
        if (dir != olc::vf2d{ 0.0f, 0.0f })hull_dir = atan2f(dir.y, dir.x) + 3.14159265 / 2.0f;
        pge->DrawRotatedDecal(pos + map_info->map_start, hull_img, hull_dir, { 91.0f,94.0f }, { 20.0f / 182.0f, 20.0f / 188.0f }, color);
        pge->DrawRotatedDecal(pos + map_info->map_start, turret_img, angle, { 34.0f,105.0f }, { 10.0f / 68.0f, 20.0f / 134.0f }, color);
        pge->FillRect(pos + map_info->map_start - olc::vf2d(radius, 5.0f * radius / 3.0f), olc::vi2d{ int(2.0f * radius*(float(hp)/float(max_hp))),4 }, olc::GREEN);
        pge->DrawRect(pos + map_info->map_start - olc::vf2d(radius, 5.0f * radius / 3.0f), olc::vi2d{ int(2.0f * radius),4 }, olc::DARK_GREEN);
        pge->FillRect(pos + map_info->map_start - olc::vf2d(2.0f * radius / 3.0f, 2.12f * radius), olc::vi2d{ int((4.0f * radius / 3.0f) * (float(shield) / float(max_shield))),3 }, olc::BLUE);
        pge->DrawRect(pos + map_info->map_start - olc::vf2d(2.0f * radius / 3.0f, 2.12f * radius), olc::vi2d{ int((4.0f * radius / 3.0f)),3 }, olc::DARK_BLUE);
        pge->FillRect(pos + map_info->map_start - olc::vf2d(radius, -5.0f * radius / 3.0f), olc::vi2d{ int(2.0f * radius * (float(ammo) / float(max_ammo))),4 }, olc::YELLOW);
        for(int i = 0; i< max_ammo;i++) pge->DrawRect(pos + map_info->map_start - olc::vf2d(radius - i * (2.0f * radius / float(max_ammo)), -5.0f * radius / 3.0f), olc::vi2d{ int(2.0f * (radius / max_ammo)),4 }, olc::DARK_YELLOW);
    }
    virtual void Hit(float dmg)override {
        dmg /= 100.0f / (100.0f - armor);
        if (shield - dmg < 0) {
            shield = 0;
            hp += (shield - dmg);
            if (hp <= 0) to_delete = true;
        }
        else shield -= dmg;
        was_hit = true;
        time_since_hit = 0.0f;
    }
};

