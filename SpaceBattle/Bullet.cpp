#include "Bullet.h"


Bullet::Bullet(ALLEGRO_BITMAP *image, float speed, float x, float y, float w, float h) : travel_speed(speed)
{
	bullet_image = image;
	bullet_height = h; /*20 pixels*/
	bullet_width = w;  /*19 pixels*/

	this->x = x;
	this->y = y;
	off_screen = false;

	/*enable the drawing functions*/
	al_init_primitives_addon();
	al_init_image_addon();

}

Bullet::Bullet(void) :travel_speed(0)
{
	bullet_image = NULL;
	bullet_height = 0.0;
	bullet_width = 0.0;
	x = 0.0;
	y = 0.0;
	off_screen = false;

	al_init_primitives_addon();
	al_init_image_addon();
}

Bullet::~Bullet(void)
{
}

bool Bullet::is_off_screen()
{
	return off_screen;
}

void Bullet::draw()
{
	al_draw_bitmap(bullet_image, x, y, NULL);
}