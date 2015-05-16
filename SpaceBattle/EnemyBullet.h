#include "PlayerJet.h"

class EnemyBullet : public Bullet
{
private:
	PlayerJet *player_jet;

public:
	EnemyBullet(ALLEGRO_BITMAP *image, float speed, float x, float y, float w, float h);
	EnemyBullet(void);
	~EnemyBullet(void);

	void set_player_jet(PlayerJet *jet);

	virtual void travel();
	virtual void check_collision();
	virtual void check_screen_bounds();
};
