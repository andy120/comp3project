
#include "EnemyJet.h"
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <ctime>

#include <string>

#define SCREEN_HEIGHT		600
#define SCREEN_WIDTH		600

using namespace std;

void pause(ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_FONT *font, ALLEGRO_EVENT *event);
void enemy_fire(PlayerJet * player, EnemyJet ** enemies, Bullet **bullets, ALLEGRO_BITMAP *enemy_bullet_image, int &fire_index);
void display_score(ALLEGRO_FONT *f, int score, bool gameover, int enemies_left);

int main()
{
	/***********************************************INITIALIZATION******************************************************/
	/*initialize allegro*/
	if (!al_init())
	{
		return -1;
	}

	/*=============================set up the window=====================*/
	al_set_new_display_flags(ALLEGRO_WINDOWED);
	ALLEGRO_DISPLAY *display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!display)
		return -1;
	al_set_window_position(display, 200, 100);
	/*===================================================================*/

	/*============================INITIALIZE ADD-ONS======================*/
	al_init_primitives_addon();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	al_init_image_addon();
	al_init_acodec_addon();
	al_install_audio();
	/*====================================================================*/

	/*=================GAME SPRITES====================================*/
	ALLEGRO_BITMAP *player_bullet_image = al_load_bitmap("player_bullet_image.png");
	ALLEGRO_BITMAP *enemy_bullet_image = al_load_bitmap("enemy_bullet.png");
	ALLEGRO_BITMAP *player_image = al_load_bitmap("player_image.png");
	ALLEGRO_BITMAP *enemy_image = al_load_bitmap("enemy_image.png");
	ALLEGRO_BITMAP *enemy_explode_image = al_load_bitmap("enemy_explode_sprite_sheet.png");
	/*==================================================================*/

	/*create font*/
	ALLEGRO_FONT *font = al_load_font("Pristina.ttf", 30, NULL);

	/*Load sound effects*/
	ALLEGRO_SAMPLE *player_exlode_sample = al_load_sample("explode_1.wav");
	ALLEGRO_SAMPLE *enemy_explode_sample = al_load_sample("explode_2.wav");
	ALLEGRO_SAMPLE *gunshot_sample = al_load_sample("gunshot_2.wav");

	al_reserve_samples(50);

	if (!al_install_keyboard())
	{
		al_show_native_message_box(NULL, NULL, NULL, "could not install keyboard", NULL, NULL);
		return -1;
	}

	/*=======================INITIALIZE TIMERS AND EVENT SYSTEM=======================*/
	ALLEGRO_TIMER *timer = al_create_timer(1 / 150.0); /*150 Frames per Second*/
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	ALLEGRO_KEYBOARD_STATE keyState;
	ALLEGRO_EVENT ret_event;
	/*================================================================================*/

	/*=====================================INITIALIZE GAME CONTROL VARIABLES AND GAME OBJECTS==============================*/

	bool gameover = false;
	int enemies_left = 100; /*in total we want to create 100 enemies, if they are all finished then the game is over*/

	Bullet *bullets[20] = { NULL }; /*to contain player and enemy bullets*/
	int bullet_count = 0;    /*number of bullets in the array*/

	/*create the player and enemy jets*/
	PlayerJet *player = new PlayerJet(player_image, enemy_explode_image, 1.0, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 42, 48, 41);
	player->set_explosion_sample(player_exlode_sample);
	player->set_gunfire_sample(gunshot_sample);
	EnemyJet *enemies[10] = { NULL };

	/*enemy fire control variables*/
	int fire_delay = 100; /*fire after every 100 frames*/
	int fire_index = 0;	  /*starting at index 0 in the enemies array*/

	int score = 0;

	/*======================================================================================================================*/



	/**********************************************END INITIALIZATION*****************************************************/

	al_start_timer(timer);

	while (!gameover)
	{
		al_wait_for_event(event_queue, &ret_event);

		/*check if the player still exists, if he is not, then the game is over*/
		if (player)
		{
			if (player->should_delete())
			{
				/*will be deleted outside the game loop*/
				gameover = true;
			}
		}

		/*
		*  check which enemies are still in the game, which ones should be deleted and create new ones if
		*  space is available in the array and we havent created more than 100 enemies
		*/

		for (int i = 0; i < 10; i++)
		{
			if (enemies[i])
			{
				/*
				* delete enemies that are no longer in the game, these are eneny jets that have gone off the bottom
				* of the screen or got hit by player bullets
				*/
				if (enemies[i]->should_delete())
				{
					/*we only increase the score if the enemy jet was hit by a bullet*/
					if (enemies[i]->bullet_hit())
					{
						score += 10; /*increase the score if the enemy jet was hit*/
					}

					delete enemies[i];
					enemies[i] = NULL;
				}
			}

			/*create new enemies in the spaces available*/
			if (!enemies[i])
			{
				if (enemies_left > 0)
				{
					enemies[i] = new EnemyJet(enemy_image, enemy_explode_image, 0.4, rand() % (600 - 30), -50, 28, 31);
					enemies[i]->set_explosion_sample(enemy_explode_sample);
					enemies_left--;
				}
			}
		}

		/*delete bullets that have gone off the screen the screen and the ones that have collided with the jets*/
		for (int i = 0; i < 20; i++)
		{
			if (bullets[i])
			{
				if (bullets[i]->is_off_screen())
				{
					delete bullets[i];

					bullets[i] = NULL;
					bullet_count--;
				}
			}
		}

		/*update the player jet by responding to keyboard events*/
		al_get_keyboard_state(&keyState);

		if (al_key_down(&keyState, ALLEGRO_KEY_LEFT))
		{
			player->move_left();
		}

		else if (al_key_down(&keyState, ALLEGRO_KEY_RIGHT))
		{
			player->move_right();
		}

		/*the space bar event is handled differently, we want to fire one bullet per press*/
		if (ret_event.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (ret_event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{
				pause(timer, event_queue, font, &ret_event);
			}

			if (ret_event.keyboard.keycode == ALLEGRO_KEY_SPACE)
			{
				/*we want to keep a maximum of two player bullets on the screen*/
				if (PlayerBullet::bullet_count < 2)
				{

					/*put a bullet in the bullets array in an empty location*/
					for (int i = 12; i < 20; i++)
					{
						if (!bullets[i])
						{
							bullets[i] = player->fire(player_bullet_image);
							((PlayerBullet*)bullets[i])->setEnemies((FighterJet**)enemies);
						}
						break;
					}
				}

			}
		}


		/*re draw all the game objects*/

		for (int i = 0; i < 10; i++)
		{
			if (enemies[i])
			{
				enemies[i]->move();
				enemies[i]->draw();

				if (fire_delay == 0)
				{
					enemy_fire(player, enemies, bullets, enemy_bullet_image, fire_index);
				}
			}
		}

		if (player)
		{
			/*detect collision with the enemy jets*/
			player->collision_detect((FighterJet**)enemies, 10);
			player->draw();
		}

		for (int i = 0; i < 20; i++)
		{
			if (bullets[i])
			{
				bullets[i]->travel();
				bullets[i]->draw();
			}
		}

		display_score(font, score, gameover, enemies_left);
		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));

		fire_delay = ((--fire_delay) < 0) ? 100 : fire_delay;


	} //end game loop

	al_rest(5);

	/*free the memory that was used*/
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	al_destroy_bitmap(player_image);
	al_destroy_bitmap(player_bullet_image);
	al_destroy_bitmap(enemy_bullet_image);
	al_destroy_bitmap(enemy_explode_image);
	al_destroy_bitmap(enemy_image);

	if (player)
		delete player;
}

void pause(ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_FONT *font, ALLEGRO_EVENT *event)
{

	al_draw_text(font, al_map_rgb(233, 241, 10), 200, 300, NULL, "PAUSE");
	al_flip_display();

	bool pause = true;
	do
	{
		al_wait_for_event(queue, event);

		if (event->type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (event->keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{
				pause = false;
				return;
			}
		}
	} while (pause);
}

void enemy_fire(PlayerJet * player, EnemyJet ** enemies, Bullet **bullets, ALLEGRO_BITMAP *enemy_bullet_image, int &fire_index)
{
	for (int count = 0; count < 2; count++) /*allow 2 enemy jets to fire at the same time*/
		for (int j = 0; j < 11; j++)
		{
			if (!bullets[j])/*check if there is space available in the bullets array to accomodate more bullets*/
			{
				if (enemies[fire_index]) /*check if the enemy at the fire_index exists before requesting fire from it*/
				{
					bullets[j] = enemies[fire_index]->fire(enemy_bullet_image);
					((EnemyBullet*)bullets[j])->set_player_jet(player);
					fire_index = (fire_index + 1) % 10; /*change the fire index so that enemies take turns in firing, but we
														*restrict the firing index to 9
														*/
					break;
				}
			}
		}
}

void display_score(ALLEGRO_FONT *f, int score, bool gameover, int enemies_left)
{
	string scr = "Score : ";
	scr.append(std::to_string(score));

	if (!gameover)
	{
		al_draw_text(f, al_map_rgb(255, 255, 0), 30, 10, NULL, scr.c_str());
	}
	else if (enemies_left <= 0)
	{
		string game_over = "CONGRADULATIONS!!! YOU SURVIVED!!!  ";
		al_draw_text(f, al_map_rgb(255, 255, 0), 250, 300, NULL, game_over.c_str());
		al_draw_text(f, al_map_rgb(255, 255, 0), 250, 340, NULL, scr.c_str());
		al_flip_display();
	}
	else
	{
		string game_over = "GAME OVER  ";
		game_over.append(scr);
		al_draw_text(f, al_map_rgb(255, 255, 0), 250, 300, NULL, game_over.c_str());
		al_flip_display();
	}
}