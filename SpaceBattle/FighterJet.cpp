#include "FighterJet.h"


FighterJet::FighterJet(ALLEGRO_BITMAP *image, ALLEGRO_BITMAP *xplode, float speed, float x_pos, float y_pos, float w, float h)
	:moving_speed(speed)
{
	x = x_pos;
	y = y_pos;
	width = w;
	height = h;

	jet_image = image;
	explode_image = xplode;

	got_hit = false;
	delete_me = false;
	explode_index = 0;
	exp_delay = 0;

	al_init_primitives_addon();
	al_init_image_addon();
	al_init_acodec_addon();

	al_install_audio();
}

FighterJet::FighterJet(void) : moving_speed(0)
{
	x = 0;
	y = 0;
	width = 0;
	height = 0;

	jet_image = NULL;
	explode_image = NULL;

	got_hit = false;
	delete_me = false;
	explode_index = 0;
	exp_delay = 0;

	al_init_primitives_addon();
	al_init_image_addon();
}


FighterJet::~FighterJet(void)
{
}

void FighterJet::draw()
{

	if (!got_hit)
		al_draw_bitmap(jet_image, x, y, NULL);
	else
		explode();
}


void FighterJet::explode()
{
	if (explode_index < 6)
	{
		al_draw_bitmap_region(explode_image, explode_index * 33, 0, 33, 30, x, y, NULL);
		exp_delay = (exp_delay + 1) % 5;
		if (exp_delay == 4)
			explode_index++;
	}
	else
		delete_me = true;
}

void FighterJet::hit()
{
	got_hit = true;
	al_play_sample(explode_sample, 1.0, 0.0, 5.0, ALLEGRO_PLAYMODE_ONCE, 0);
}

bool FighterJet::should_delete()
{
	return delete_me;
}
void FighterJet::bounding_rectangle(float &x, float &y, float &w, float &h)
{
	x = this->x;
	y = this->y;
	w = width;
	h = height;
}

void FighterJet::set_explosion_sample(ALLEGRO_SAMPLE *sample)
{
	explode_sample = sample;
}
