#pragma once
#include<allegro5/allegro.h>
#include<allegro5/allegro_image.h>
#include<allegro5/allegro_primitives.h>

class Bullet
{
protected:

	/*==================bounding rectangle============================*/
	float bullet_height;
	float bullet_width;
	float x;
	float y;
	/*=================================================================*/

	bool off_screen;				  /*indicate whether the bullet has dissappeared off the screen or not*/
	const float travel_speed;		  /*the rate a which the y position is changing per frame*/
	ALLEGRO_BITMAP *bullet_image;	  /*sprite*/

	/*
	* these function dont need to be called from outside, the bullet calls them its self everytime it travels
	* therefore they are called in the travel function
	*/
	virtual void check_collision() = 0;
	virtual void check_screen_bounds() = 0;

public:
	Bullet(ALLEGRO_BITMAP *image, float speed, float x, float y, float w, float h);
	Bullet(void);
	~Bullet(void);

	bool is_off_screen();
	void draw();

	/*these functions will be implemented by congrete bullet classes*/
	virtual void travel() = 0;

};

