#include "EnemyBullet.h"

EnemyBullet::EnemyBullet(ALLEGRO_BITMAP *image, float speed, float x, float y, float w, float h)
	: Bullet(image, speed, x, y, w, h)
{

}

EnemyBullet::EnemyBullet(void) : Bullet(NULL, 0.0, 0, 0, 0, 0)
{
}


EnemyBullet::~EnemyBullet(void)
{
}

void EnemyBullet::travel()
{
	y += travel_speed;
	check_collision();
	check_screen_bounds();
}

void EnemyBullet::check_collision()
{
	float pl_x, pl_y, pl_w, pl_h;
	if (player_jet)
	{
		/*get bounding rectangle of the player jet and check intersection of the bounding rectangles*/
		player_jet->bounding_rectangle(pl_x, pl_y, pl_w, pl_h);
		if (pl_x <= (x + bullet_width) && (pl_x + pl_w) >= x  &&
			pl_y <= (y + bullet_height) && (pl_y + pl_h) >= y)
		{
			player_jet->hit();
			off_screen = true; /*flag this bullet to be deleted the next frame*/
		}
	}

}

void EnemyBullet::check_screen_bounds()
{
	if (y > 600)
		off_screen = true;
}

void EnemyBullet::set_player_jet(PlayerJet *jet)
{
	player_jet = jet;
}