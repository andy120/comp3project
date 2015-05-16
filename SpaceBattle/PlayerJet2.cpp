#include "PlayerJet2.h"

PlayerJet::PlayerJet(ALLEGRO_BITMAP *image, ALLEGRO_BITMAP *xplode, float speed, float x, float y, float w, float h)
	: FighterJet(image, xplode, speed, x, y, w, h)
{

}

PlayerJet::PlayerJet(void)
{
}

PlayerJet::~PlayerJet(void)
{
}

void PlayerJet::move()
{
	/*left undefined*/
}

void PlayerJet::move_left()
{
	x -= moving_speed;
	x = x < 0 ? 0 : x; /*do not let it get off the left of the screen, reset it back to zero if it does*/
}

void PlayerJet::move_right()
{
	x += moving_speed;
	x = (x + width) > 600 ? (x - moving_speed) : x; /*restrict from going off the right of the screen by undiong the move*/
}

Bullet* PlayerJet::fire(ALLEGRO_BITMAP *bullet_image)
{
	/*create a bullet player and return it*/
	PlayerBullet *bullet = new PlayerBullet(bullet_image, 5.0, x + (0.3 * width), y, 19, 20);
	al_play_sample(gunfire_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	return bullet;
}

void PlayerJet::set_gunfire_sample(ALLEGRO_SAMPLE *gun_sample)
{
	gunfire_sound = gun_sample;
}

void PlayerJet::collision_detect(FighterJet * *const enemies, int total_enemies)
{
	float en_x = 0, en_y = 0, en_w = 0, en_h = 0.0;

	for (int i = 0; i < total_enemies; i++)
	{
		if (enemies[i])
		{
			enemies[i]->bounding_rectangle(en_x, en_y, en_w, en_h);
			if (x <= (en_x + en_w) && (x + width) >= en_x &&
				(y <= (en_y + en_h) && (y + height) >= en_y))
			{
				this->hit(); /*collision has been detected therefore the player jet has been hit*/
			}
		}
	}
}
