#include "EnemyJet.h"

EnemyJet::EnemyJet(ALLEGRO_BITMAP *image, ALLEGRO_BITMAP *xplode, float speed, float x, float y, float w, float h)
	:FighterJet(image, xplode, speed, x, y, w, h)
{
	off_screen = false;
}

EnemyJet::EnemyJet(void)
{
}


EnemyJet::~EnemyJet(void)
{
}

void EnemyJet::move()
{
	y += moving_speed;

	/*if the jet has gone of the screen, it should set is delete_me flag on*/
	check_screen_bounds();
}

Bullet* EnemyJet::fire(ALLEGRO_BITMAP *bullet_image)
{
	EnemyBullet *bullet = new EnemyBullet(bullet_image, 2.0, x + (0.3 * width), y, 13, 12);
	return bullet;
}

void EnemyJet::check_screen_bounds()
{
	if (y > 600)
		delete_me = true;
}

bool  EnemyJet::bullet_hit()
{
	return got_hit;
}

