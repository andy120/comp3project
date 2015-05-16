#pragma once

#include "FighterJet.h"

class PlayerBullet : public Bullet
{
protected:

	FighterJet ** enemies; /*an array of enemies on the screen*/

public:
	static int bullet_count; /*counts how many player bullets are current alive on the screen, the maximum allowed is two*/

	PlayerBullet(ALLEGRO_BITMAP *image, float speed, float x, float y, float w, float h);
	PlayerBullet(void);
	~PlayerBullet(void);

	virtual void travel();
	virtual void check_collision();
	virtual void check_screen_bounds();

	void setEnemies(FighterJet * * const array);
};

