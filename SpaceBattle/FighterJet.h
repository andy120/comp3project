#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "Bullet.h"


class FighterJet
{
protected:

	/*variable defining the bounding rectangle*/
	float x;
	float y;
	float width;
	float height;

	/*Flags*/
	bool got_hit;   /*Signals whether or not the jet got hit by a missile*/
	bool delete_me; /*Signals whether or not the this Jet object should be deleted out of RAM memory, if it is not needed anymore in the game*/

	const float moving_speed;

	/*jet sprites*/
	ALLEGRO_BITMAP *jet_image;
	ALLEGRO_BITMAP *explode_image;

	ALLEGRO_SAMPLE *explode_sample;

	/*These variables are used in the explode() function to animate explosion*/

	int explode_index;/*used to index the picture to be displayed in the explosion animation*/
	int exp_delay;  /*used to delay the explosion animation to make it visible on the screen*/

public:
	FighterJet(ALLEGRO_BITMAP *image, ALLEGRO_BITMAP *xplode, float speed, float x, float y, float w, float h);
	FighterJet(void);
	~FighterJet(void);


	void explode();
	void hit();
	void draw();
	bool should_delete();
	void set_explosion_sample(ALLEGRO_SAMPLE *sample);
	void bounding_rectangle(float &x, float &y, float &w, float &h);

	virtual void move() = 0;
	virtual Bullet* fire(ALLEGRO_BITMAP *bullet_image) = 0;

};

