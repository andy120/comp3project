#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "EnemyBullet.h"

class EnemyJet : public FighterJet
{
private:
	bool off_screen;
	void check_screen_bounds();

public:
	EnemyJet(ALLEGRO_BITMAP *image, ALLEGRO_BITMAP *xplode, float speed, float x, float y, float w, float h);
	EnemyJet(void);
	~EnemyJet(void);

	bool bullet_hit();

	virtual void move();
	virtual Bullet* fire(ALLEGRO_BITMAP *bullet_image);

};

