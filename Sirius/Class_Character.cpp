#include <Novice.h>
#include <MathFunction.h>
#include <Class_Character.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void Player::Initalize() {
	pos.x = 100.0f;
	pos.y = 340.0f;
	width = 64.0f;
	height = 64.0f;
	hp = 100;
	hpMax = 100;
	shootCoolTime = 0;
	chance = 2;
	randPlayerPosX = 0;
	randPlayerPosY = 0;
	velocity.x = 10.0f;
	velocity.y = 10.0f;
	isAlive = true;
	isAttributeHoly = true;
}

void Player::AttributeChange() {
	isAttributeHoly = !isAttributeHoly;
}

Vector2 Player::Move(char keys[]) {
	Vector2 valVelocity = Normalize(velocity);
	if (keys[DIK_UP] && keys[DIK_LEFT]) {
		pos.x -= velocity.x * valVelocity.x;
		pos.y -= velocity.y * valVelocity.y;
	}
	else if (keys[DIK_UP] && keys[DIK_RIGHT]) {
		pos.x += velocity.x * valVelocity.x;
		pos.y -= velocity.y * valVelocity.y;
	}
	else if (keys[DIK_DOWN] && keys[DIK_LEFT]) {
		pos.x -= velocity.x * valVelocity.x;
		pos.y += velocity.y * valVelocity.y;
	}
	else if (keys[DIK_DOWN] && keys[DIK_RIGHT]) {
		pos.x += velocity.x * valVelocity.x;
		pos.y += velocity.y * valVelocity.y;
	}
	else if (keys[DIK_UP]) {
		pos.y -= velocity.y;
	}
	else if (keys[DIK_DOWN]) {
		pos.y += velocity.y;
	}
	else if (keys[DIK_LEFT]) {
		pos.x -= velocity.x;
	}
	else if (keys[DIK_RIGHT]) {
		pos.x += velocity.x;
	}

	//边界
	if (pos.x < 0) {
		pos.x = 0;
	}
	if (pos.x > 640 - width) {
		pos.x = 640 - width;
	}
	if (pos.y < 70) {
		pos.y = 70;
	}
	if (pos.y > 720 - 70 - height) {
		pos.y = 720 - 70 - height;
	}

	return pos;
}


void Player::Draw() {
	int Texture_Player_1 = Novice::LoadTexture("./Resources/Character/player_1.png");
	int Texture_Player_2 = Novice::LoadTexture("./Resources/Character/player_2.png");
	if (isAlive) {
		if (isAttributeHoly) {
			Novice::DrawSprite(
				(int)(pos.x + randPlayerPosX),
				(int)(pos.y + randPlayerPosY),
				Texture_Player_1,
				1, 1, 0.0f, WHITE
			);
		}
		else if (!isAttributeHoly) {
			Novice::DrawSprite(
				(int)(pos.x + randPlayerPosX),
				(int)(pos.y + randPlayerPosY),
				Texture_Player_2,
				1, 1, 0.0f, WHITE
			);
		}
	}
}

void Enemy_Zako::Initalize(float Magnification) {
	pos.x = (float)(rand() % 301 + 1300);
	pos.y = (float)(rand() % 517 + 70);
	width = 64.0f;
	height = 64.0f;
	velocity.x = 3.0f;
	velocity.y = 5.0f;
	shootCoolTime = 120;
	bulletNum = 1;
	hpMax = 1.0f * Magnification;
	hp = hpMax;
	respawnTimer = rand() % 121 + 60;
	isAlive = false;
}

void Enemy_Zako::Respawn(float Magnification) {
	if (isAlive == false) {
		if (respawnTimer > 0) {
			respawnTimer--;
		}
		else {
			Initalize(Magnification);
			isAlive = true;

		}
	}
}
void Enemy_Zako::Move() {

	if (isAlive) {
		pos.x -= velocity.x;
		pos.y -= velocity.y;
		if (pos.y <= 70 || pos.y >= 586) {
			velocity.y *= -1;
		}
		if (pos.x <= 700) {
			isAlive = false;
		}
	}

}

void Enemy_Zako::Draw() {
	int Texture_Enemy_Zako = Novice::LoadTexture("./Resources/Character/enemy.png");

	if (isAlive) {
		Novice::DrawSprite(
			(int)pos.x,
			(int)pos.y,
			Texture_Enemy_Zako,
			1, 1, 0.0f, WHITE
		);
	}

}


void Enemy_Zako2::Initalize(float Magnification) {
	pos.x = (float)(rand() % 901 + 1300);
	pos.y = -100;
	width = 64.0f;
	height = 64.0f;
	velocity.x = 25.0f;
	velocity.y = 5.0f;
	shootCoolTime = 0;
	hpMax = 1.0f * Magnification;
	hp = hpMax;
	respawnTimer = rand() % 721 + 240;
	isAlive = false;

}
void Enemy_Zako2::Respawn(Player* player, float Magnification) {
	if (isAlive == false) {
		if (respawnTimer > 0) {
			respawnTimer--;
		}
		else {
			Initalize(Magnification);
			pos.y = player->pos.y;
			isAlive = true;
		}
	}
}
void Enemy_Zako2::Move() {
	if (isAlive) {
		pos.x -= velocity.x;
		if (pos.x <= 0) {
			isAlive = false;
		}
	}

}

void Enemy_Zako2::Draw() {
	int Texture_Enemy_Zako2 = Novice::LoadTexture("./Resources/Character/enemy2.png");
	if (isAlive) {
		Novice::DrawSprite(
			(int)pos.x,
			(int)pos.y,
			Texture_Enemy_Zako2,
			1, 1, 0.0f, WHITE
		);
	}
}

void Enemy_Elite1::Initalize(float Magnification) {
	pos.x = (float)(rand() % (2000 - 1300 + 1) + 1300);
	pos.y = (float)(rand() % (586 - 70 + 1) + 70);
	width = 64.0f;
	height = 64.0f;
	velocity.x = 10.0f;
	velocity.y = 10.0f;
	shootCoolTime = 0;
	hpMax = 2.0f * Magnification;
	hp = hpMax;
	respawnTimer = rand() % 121 + 60;
	isAlive = false;
}
void Enemy_Elite1::Respawn(float Magnification) {
	if (isAlive == false) {
		if (respawnTimer > 0) {
			respawnTimer--;
		}
		else {
			Initalize(Magnification);
			isAlive = true;
		}
	}
}
void Enemy_Elite1::Move(Player* player) {
	Vector2 vector = {};
	Vector2 vectorNew = {};
	float length = GetLength(player->pos, pos);
	vector.x = player->pos.x - pos.x;
	vector.y = player->pos.y - pos.y;
	if (length != 0) {
		vectorNew.x = vector.x / length;
		vectorNew.y = vector.y / length;
	}
	if (isAlive == true) {
		if (length <= 400) {
			pos.x += vectorNew.x * velocity.x * 1.2f;
			pos.y += vectorNew.y * velocity.y * 1.2f;
		}
		else {
			pos.x -= velocity.x;
		}
		if (pos.x <= -64) {
			isAlive = false;
		}
	}
}
void Enemy_Elite1::Draw() {
	int Texture_Enemy_Elite1 = Novice::LoadTexture("./Resources/Character/enemy3.png");
	if (isAlive) {
		Novice::DrawSprite(
			(int)pos.x,
			(int)pos.y,
			Texture_Enemy_Elite1,
			1, 1, 0.0f, WHITE
		);
	}
}


void Enemy_Elite2::Initalize(float Magnification) {
	pos.x = 1300;
	pos.y = (float)(rand() % 517 + 70);
	velocity.x = 10.0f;
	velocity.y = 10.0f;
	width = 64.0f;
	height = 64.0f;
	shootCoolTime = 180;
	shootInterval = 0;
	respawnTimer = 180;
	isShoot = false;
	isAlive = false;
	hpMax = 25 * Magnification;
	hp = hpMax;
}
void Enemy_Elite2::Respawn(float Magnification) {
	if (isAlive == false) {
		if (respawnTimer > 0) {
			respawnTimer--;
		}
		else {
			Initalize(Magnification);
			isAlive = true;
		}
	}
}
void Enemy_Elite2::Move() {
	if (isAlive == true) {
		pos.x -= velocity.x;
		if (pos.x <= 1280 - 280) {
			pos.x = 1280 - 280;
		}
	}
}
void Enemy_Elite2::Draw() {
	int Texture_Enemy_Elite2 = Novice::LoadTexture("./Resources/Character/enemy4.png");
	if (isAlive) {
		Novice::DrawSprite(
			(int)pos.x,
			(int)pos.y,
			Texture_Enemy_Elite2,
			1, 1, 0.0f, WHITE
		);
	}
}


void Enemy_Boss1::Initalize() {
	pos.x = 1300;
	pos.y = 0;
	width = 128.0f;
	height = 720.0f;
	velocity.x = 5.0f;
	velocity.y = 5.0f;
	hpMax = 500;
	hp = hpMax;
	shootCoolTimeSG1 = 60;
	shootCoolTimeSG2 = 30;
	shootCoolTimeSG3 = 10;
	shootCoolTimeFL1 = 30;
	shootCoolTimeFL2 = 30;
	respawnTimer = 180;
	isAlive = false;
	isBossDefeated = false;
	bossPattern1 = false;
	bossPattern2 = false;
}
void Enemy_Boss1::Respawn() {
	if (isBossDefeated == false) {
		if (isAlive == false) {
			if (respawnTimer > 0) {
				respawnTimer--;
			}
			else {
				isAlive = true;
				respawnTimer = 180;
			}
		}
	}
}
void Enemy_Boss1::Move() {
	if (isBossDefeated == false) {
		if (isAlive == true) {
			pos.x -= velocity.x;
			if (pos.x <= 1280 - 400) {
				pos.x = 1280 - 400;
				bossPattern1 = true;
			}
		}
	}
}
void Enemy_Boss1::Draw() {
	int Texture_Enemy_Boss1 = Novice::LoadTexture("./Resources/Character/boss1.png");
	if (isBossDefeated == false) {
		if (isAlive) {
			Novice::DrawSprite(
				(int)pos.x,
				(int)pos.y,
				Texture_Enemy_Boss1,
				1, 1, 0.0f, WHITE
			);
		}
	}
}


void Enemy_Boss2::Initalize() {
	pos.x = 1300;
	pos.y = 0;
	width = 128.0f;
	height = 580.0f;
	hpMax=800;
	hp= hpMax;
	velocity.x = 5.0f;
	velocity.y = 5.0f;
	shootCoolTimeBH = 480;
	shootCoolTimeBA = 60;
	shootCoolTimeLS = 300;
	respawnTimer = 180;
	isAlive = false;
	isBossDefeated = false;
}

void Enemy_Boss2::Respawn() {
	if (isBossDefeated == false) {
		if (isAlive == false) {
			if (respawnTimer > 0) {
				respawnTimer--;
			}
			else {
				isAlive = true;
				respawnTimer = 180;
			}
		}
	}
}

void Enemy_Boss2::Move() {
	if (isBossDefeated == false) {
		if (isAlive == true) {
			pos.x -= velocity.x;
			if (pos.x <= 1280 - 400) {
				pos.x = 1280 - 400;
			}
		}
	}
}

void Enemy_Boss2::Draw() {
	int Texture_Enemy_Boss2 = Novice::LoadTexture("./Resources/Character/boss2.png");
	if (isBossDefeated == false) {
		if (isAlive) {
			Novice::DrawSprite(
				(int)pos.x,
				(int)pos.y,
				Texture_Enemy_Boss2,
				1, 1, 0.0f, WHITE
			);
		}
	}
}