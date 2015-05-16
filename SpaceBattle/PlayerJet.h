#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "PlayerBullet.h"


class PlayerJet : public FighterJet
{
private:
	/*must include a reference to an array of enemy get for collision detection purposes*/
	ALLEGRO_SAMPLE *gunfire_sound;

public:
	PlayerJet(ALLEGRO_BITMAP *image, ALLEGRO_BITMAP *xplode, float speed, float x, float y, float w, float h);
	PlayerJet(void);
	~PlayerJet(void);

	void collision_detect(FighterJet * * const enemies, int total_enemies);
	void move_left();
	void move_right();
	void set_gunfire_sample(ALLEGRO_SAMPLE *gun_sample);

	/*inherited virtual funtions*/
	virtual void move();
	virtual Bullet* fire(ALLEGRO_BITMAP *bullet_image);

};

