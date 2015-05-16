#include "PlayerBullet.h"

int PlayerBullet::bullet_count = 0;

PlayerBullet::PlayerBullet(ALLEGRO_BITMAP *image, float speed, float x, float y, float w, float h)
	: Bullet(image, speed, x, y, w, h)
{
	PlayerBullet::bullet_count += 1; /*increase the count when this bullet is created*/
}

PlayerBullet::PlayerBullet(void) : Bullet(NULL, 0.0, 0.0, 0.0, 0.0, 0.0)
{
}


PlayerBullet::~PlayerBullet(void)
{
	PlayerBullet::bullet_count -= 1; /*decrease the count when this bullet goes off the screen*/
}

void PlayerBullet::travel()
{
	/*the player bullet travels up the screen in the decreaseing y direction. the x position never changes*/
	y -= travel_speed;
	check_collision();
	check_screen_bounds();
}
