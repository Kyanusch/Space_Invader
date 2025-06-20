#include "Player.h"

Player::Player(Vector3 position, int score, std::string name)
	: Entity(position, { 0,0,0 }, 100, 100, 0, PLAYER), lastShotTime(0.0), shootCooldown(0.2), score(score), name(name) {
	hitboxradius = 40;
}

void Player::Update() {
	if (IsKeyDown(KEY_LEFT) == IsKeyDown(KEY_RIGHT)) {  //reduces velocity x to 0
		if (getVelocity().x <= -0.1f) setVelocity_X(getVelocity().x + 0.1f);
		else if (getVelocity().x >= 0.1f) setVelocity_X(getVelocity().x - 0.1f);
		else setVelocity_X(0.0f);
	}
	else if (IsKeyDown(KEY_RIGHT)) {                    //increses velocity x
		setVelocity_X(getVelocity().x + 0.1f);
	}
	else if (IsKeyDown(KEY_LEFT)) {                     //decreses velocity x
		setVelocity_X(getVelocity().x - 0.1f);
	}
	if (IsKeyDown(KEY_UP) == IsKeyDown(KEY_DOWN)) {     //reduces velocity y to 0
		if (getVelocity().y <= -0.1f) setVelocity_Y(getVelocity().y + 0.1f);
		else if (getVelocity().y >= 0.1f) setVelocity_Y(getVelocity().y - 0.1f);
		else setVelocity_Y(0.0f);
	}
	else if (IsKeyDown(KEY_UP)) {                       //decreses velocity y
		setVelocity_Y(getVelocity().y - 0.1f);
	}
	else if (IsKeyDown(KEY_DOWN)) {                     //increses velocity y
		setVelocity_Y(getVelocity().y + 0.1f);
	}

	//move
	if (getVelocity().x > 20.0f) setVelocity_X(speedlimit);     //apply speedlimits
	if (getVelocity().x < -20.0f) setVelocity_X(-speedlimit);
	if (getVelocity().y > 20.0f) setVelocity_Y(speedlimit);
	if (getVelocity().y < -20.0f) setVelocity_Y(-speedlimit);

	if (getPosition().x + speedfactor * getVelocity().x < virtualCamera::worldWidth && getPosition().x + speedfactor * getVelocity().x > 0) {
		setPosition_X(getPosition().x + speedfactor * getVelocity().x);
	}
	else setVelocity_X(0.0f);
	if (getPosition().y + speedfactor * getVelocity().y < virtualCamera::worldHight && getPosition().y + speedfactor * getVelocity().y > 0) {
		setPosition_Y(getPosition().y + speedfactor * getVelocity().y);
	}
	else setVelocity_Y(0.0f);

	//move Camera
	virtualCamera::initialize({ getPosition().x,getPosition().y - 60.0f,-250.0f }, { virtualCamera::worldWidth / 2, virtualCamera::worldHight / 2, 10000.0f }, { 0,1,0 }, 250.0f);
}

bool Player::CanShoot() {
	double now = GetTime();
	if (now - lastShotTime >= shootCooldown) {
		lastShotTime = now;
		return true;
	}
	return false;
}

Bullet Player::Shoot() const {
	Sound sound = LoadSoundFromWave(Soundmanager::sounds.laserS);
	PlaySound(sound);	// Play shooting sound
	return Bullet(getPosition(), { 0.0f, 0.0f, 5.0f }, getThisEntity(), RED);
}

void Player::draw() const {
	virtualCamera::sVector3 Points3[7] = {	//design of the player ship
		virtualCamera::sVector3({-20,0,0}),
		virtualCamera::sVector3({0,5,0}),
		virtualCamera::sVector3({ 20,0,0 }),
		virtualCamera::sVector3({ 0,-12,0 }),
		virtualCamera::sVector3({ -53,0,0 }),
		virtualCamera::sVector3({ 53,0,0 }),
		virtualCamera::sVector3({ 0,0,40 })
	};
	//Rotation roll
	for (auto& p : Points3) {
		p = virtualCamera::rotatePointAroundAxis(p, { 0.0f,0.0f,1.0f }, getVelocity().x / (5 * PI));
	}
	//Rotation pitch
	for (auto& p : Points3) {
		p = virtualCamera::rotatePointAroundAxis(p, { -1.0f,0.0f,0.0f }, getVelocity().y / (2.5 * PI));
	}

	// Project points to 2D
	Vector2 Points2[7];
	auto pos = virtualCamera::sVector3(getPosition());
	for (int i = 0; i < 7; i++) {
		Points3[i] = Points3[i] + pos;
		Points2[i] = virtualCamera::projectPoint(Points3[i].vec).position2D;
	}
	// Draw lines between 2D points
	DrawLine(Points2[0].x, Points2[0].y, Points2[1].x, Points2[1].y, RAYWHITE);
	DrawLine(Points2[1].x, Points2[1].y, Points2[2].x, Points2[2].y, RAYWHITE);
	DrawLine(Points2[2].x, Points2[2].y, Points2[3].x, Points2[3].y, RAYWHITE);
	DrawLine(Points2[3].x, Points2[3].y, Points2[0].x, Points2[0].y, RAYWHITE);
	DrawLine(Points2[0].x, Points2[0].y, Points2[4].x, Points2[4].y, RAYWHITE);
	DrawLine(Points2[2].x, Points2[2].y, Points2[5].x, Points2[5].y, RAYWHITE);
	for (int i = 0; i < 6; i++) {
		DrawLine(Points2[i].x, Points2[i].y, Points2[6].x, Points2[6].y, RAYWHITE);
	}
}

void Player::setExperience(int exp) {
	score += exp;
}
int Player::getScore() {
	return score;
}
void Player::addScore(int s) {
	score += s;
}