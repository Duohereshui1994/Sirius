#define _USE_MATH_DEFINES
#include <Novice.h>
#include <MathFunction.h>
#include <Class_Character.h>
#include <Class_Bullet.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void Bullet0::Initalize() {
	pos.x = -128.0f;
	pos.y = -128.0f;
	width = 32.0f;
	height = 32.0f;
	speed = 24.0f;
	isShoot = false;
}

void Bullet0::Move() {
	if (isShoot) {
		pos.x += speed;
		// 画面外に出たら発射フラグをFalseに変更する
		if (pos.x >= 1280) {
			isShoot = false;
		}

	}
}
void Bullet0::Draw(int x) {
	int Texture_Bullet = Novice::LoadTexture("./Resources/Bullet/bullet.png");
	if (isShoot) {
		Novice::DrawSpriteRect(
			(int)pos.x,
			(int)pos.y,
			x, 0, 32, 32,
			Texture_Bullet,
			1.0f / 6, 1, 0.0f, WHITE
		);
	}
}

void Bullet1::Initalize() {
	pos.x = -128.0f;
	pos.y = -128.0f;
	width = 32.0f;
	height = 32.0f;
	speed = 12.0f;
	isShoot = false;
	reloadTimer1 = 45;
}



void Bullet1::Move(float theta) {
	if (isShoot) {
		pos.x += speed * cosf(theta);
		pos.y += speed * sinf(theta);
		if (reloadTimer1 == 0) {
			isShoot = false;
		}
		if (reloadTimer1 > 0) {
			reloadTimer1--;
		}
		else {
			reloadTimer1 = 45;
		}
	}
}

void Bullet1::Draw() {
	int Texture_Bullet = Novice::LoadTexture("./Resources/Bullet/bullet.png");
	if (isShoot) {
		Novice::DrawSpriteRect(
			(int)pos.x,
			(int)pos.y,
			96, 0, 32, 32,
			Texture_Bullet,
			1.0f / 6, 1, 0.0f, WHITE
		);
	}
}

void Bullet2::Initalize() {
	pos.x = -128.0f;
	pos.y = -128.0f;
	width = 32.0f;
	height = 32.0f;
	speed = 28.0f;
	isShoot = false;
	reloadTimer2 = 45;
}



void Bullet2::Move(float theta) {
	if (isShoot) {
		pos.x += speed * cosf(theta);
		pos.y += speed * sinf(theta);
	}
}

void Bullet2::Draw() {
	int Texture_Bullet = Novice::LoadTexture("./Resources/Bullet/bullet.png");
	if (isShoot) {
		Novice::DrawSpriteRect(
			(int)pos.x,
			(int)pos.y,
			96, 0, 32, 32,
			Texture_Bullet,
			1.0f / 6, 1, 0.0f, WHITE
		);
	}
}

void Bullet3::Initalize() {
	pos.x = -128.0f;
	pos.y = -128.0f;
	width = 32.0f;
	height = 32.0f;
	speed = 6.0f;
	isShoot = false;
}



void Bullet3::Move(float theta, float amplitude) {
	if (isShoot) {
		pos.x += speed;
		pos.y += 1.0f / (float)M_PI * sinf(theta) * amplitude;
		if (pos.x >= 1280) {
			isShoot = false;
		}
	}
}

void Bullet3::Draw() {
	int Texture_Bullet = Novice::LoadTexture("./Resources/Bullet/bullet.png");
	if (isShoot) {
		Novice::DrawSpriteRect(
			(int)pos.x,
			(int)pos.y,
			96, 0, 32, 32,
			Texture_Bullet,
			1.0f / 6, 1, 0.0f, WHITE
		);
	}
}

void Bullet4::Initalize() {
	pos.x = -128.0f;
	pos.y = -128.0f;
	width = 32.0f;
	height = 32.0f;
	speed = 15.0f;
	isShoot = false;
}

void Bullet4::Draw() {
	int Texture_Bullet = Novice::LoadTexture("./Resources/Bullet/bullet.png");
	if (isShoot) {
		Novice::DrawSpriteRect(
			(int)pos.x,
			(int)pos.y,
			32, 0, 32, 32,
			Texture_Bullet,
			1.0f / 6, 1, 0.0f, WHITE
		);
	}
}

void Bullet5::Initalize() {
	pos.x = -128.0f;
	pos.y = -128.0f;
	width = 32.0f;
	height = 32.0f;
	speed = 12.0f;
	isShoot = false;
}

void BulletElite2::Initalize() {
	pos.x = -128.0f;
	pos.y = -128.0f;
	width = 32.0f;
	height = 32.0f;
	speed = 18.0f;
	isShoot = false;
	isHoly = true;
}

void BulletBoss1_1_1::Initalize() {
	pos.x = -128.0f;
	pos.y = -128.0f;
	width = 32.0f;
	height = 32.0f;
	speed = 10.0f;
	isShoot = false;
	isHoly = true;
	reloadTimer = 180;
}
void BulletBoss1_1_1::Move(float theta) {
	if (isShoot) {
		pos.x -= speed;
		pos.y -= theta * 7.5f;
		if (pos.y >= 618 || pos.y <= 70) {//反転
			theta *= -1;
		}
		if (reloadTimer == 0) {
			isShoot = false;
		}
		if (reloadTimer > 0) {
			reloadTimer--;
		}
		else {
			reloadTimer = 180;
		}
	}
}
void BulletBoss1_1_1::Draw() {
	int Texture_Bullet = Novice::LoadTexture("./Resources/Bullet/bullet.png");
	if (isShoot) {
		if (isHoly) {
			Novice::DrawSpriteRect(
				(int)pos.x,
				(int)pos.y,
				128, 0, 32, 32,
				Texture_Bullet,
				1.0f / 6, 1, 0.0f, WHITE
			);
		}
		if (!isHoly) {
			Novice::DrawSpriteRect(
				(int)pos.x,
				(int)pos.y,
				160, 0, 32, 32,
				Texture_Bullet,
				1.0f / 6, 1, 0.0f, WHITE
			);
		}
	}
}

void BulletBoss1_1_2::Initalize() {
	pos.x = -128.0f;
	pos.y = -128.0f;
	width = 32.0f;
	height = 32.0f;
	speed = 10.0f;
	isShoot = false;
	isHoly = true;
	reloadTimer = 180;
}
void BulletBoss1_1_2::Move(float theta) {
	if (isShoot) {
		pos.x -= speed;
		pos.y -= theta * 5.5f;
		if (pos.y >= 618 || pos.y <= 70) {//反転
			theta *= -1;
		}
		if (reloadTimer == 0) {
			isShoot = false;
		}
		if (reloadTimer > 0) {
			reloadTimer--;
		}
		else {
			reloadTimer = 180;
		}
	}
}
void BulletBoss1_1_2::Draw() {
	int Texture_Bullet = Novice::LoadTexture("./Resources/Bullet/bullet.png");
	if (isShoot) {
		if (isHoly) {
			Novice::DrawSpriteRect(
				(int)pos.x,
				(int)pos.y,
				128, 0, 32, 32,
				Texture_Bullet,
				1.0f / 6, 1, 0.0f, WHITE
			);
		}
		if (!isHoly) {
			Novice::DrawSpriteRect(
				(int)pos.x,
				(int)pos.y,
				160, 0, 32, 32,
				Texture_Bullet,
				1.0f / 6, 1, 0.0f, WHITE
			);
		}
	}
}

void BulletBoss1_1_3::Initalize() {
	pos.x = -128.0f;
	pos.y = -128.0f;
	width = 32.0f;
	height = 32.0f;
	speed = 10.0f;
	isShoot = false;
	isHoly = true;
	reloadTimer = 180;
}
void BulletBoss1_1_3::Move(float theta) {
	if (isShoot) {
		pos.x -= speed;
		pos.y -= theta * 3.5f;
		if (pos.y >= 618 || pos.y <= 70) {//反転
			theta *= -1;
		}
		if (reloadTimer == 0) {
			isShoot = false;
		}
		if (reloadTimer > 0) {
			reloadTimer--;
		}
		else {
			reloadTimer = 180;
		}
	}
}
void BulletBoss1_1_3::Draw() {
	int Texture_Bullet = Novice::LoadTexture("./Resources/Bullet/bullet.png");
	if (isShoot) {
		if (isHoly) {
			Novice::DrawSpriteRect(
				(int)pos.x,
				(int)pos.y,
				128, 0, 32, 32,
				Texture_Bullet,
				1.0f / 6, 1, 0.0f, WHITE
			);
		}
		if (!isHoly) {
			Novice::DrawSpriteRect(
				(int)pos.x,
				(int)pos.y,
				160, 0, 32, 32,
				Texture_Bullet,
				1.0f / 6, 1, 0.0f, WHITE
			);
		}
	}
}


void BulletBoss1_2_1::Initalize() {
	pos.x = -128.0f;
	pos.y = -128.0f;
	vector.x = 0.0f;
	vector.y = 0.0f;
	vectorNew.x = 0.0f;
	vectorNew.y = 0.0f;
	length = 0.0f;
	width = 32.0f;
	height = 32.0f;
	speed = 10.0f;
	isShoot = false;
	isHoly = true;
	reloadTimer = 0;
}
void BulletBoss1_2_1::Draw() {
	int Texture_Bullet = Novice::LoadTexture("./Resources/Bullet/bullet.png");
	if (isShoot) {
		if (isHoly) {
			Novice::DrawSpriteRect(
				(int)pos.x,
				(int)pos.y,
				128, 0, 32, 32,
				Texture_Bullet,
				1.0f / 6, 1, 0.0f, WHITE
			);
		}
		if (!isHoly) {
			Novice::DrawSpriteRect(
				(int)pos.x,
				(int)pos.y,
				160, 0, 32, 32,
				Texture_Bullet,
				1.0f / 6, 1, 0.0f, WHITE
			);
		}
	}
}

void BulletBoss1_2_2::Initalize() {
	pos.x = -128.0f;
	pos.y = -128.0f;
	vector.x = 0.0f;
	vector.y = 0.0f;
	vectorNew.x = 0.0f;
	vectorNew.y = 0.0f;
	length = 0.0f;
	width = 32.0f;
	height = 32.0f;
	speed = 10.0f;
	isShoot = false;
	isHoly = true;
	reloadTimer = 0;
}
void BulletBoss1_2_2::Draw() {
	int Texture_Bullet = Novice::LoadTexture("./Resources/Bullet/bullet.png");
	if (isShoot) {
		if (isHoly) {
			Novice::DrawSpriteRect(
				(int)pos.x,
				(int)pos.y,
				128, 0, 32, 32,
				Texture_Bullet,
				1.0f / 6, 1, 0.0f, WHITE
			);
		}
		if (!isHoly) {
			Novice::DrawSpriteRect(
				(int)pos.x,
				(int)pos.y,
				160, 0, 32, 32,
				Texture_Bullet,
				1.0f / 6, 1, 0.0f, WHITE
			);
		}
	}
}

void BulletBoss2_1::Initalize() {
	pos.x = -250.0f;
	pos.y = -250.0f;
	width = 200.0f;
	height = 200.0f;
	speed = 18.0f;
	timer = 180;
	isShoot = false;
}

void BulletBoss2_2_1::Initalize() {
	pos.x = -128.0f;
	pos.y = -128.0f;
	width = 32.0f;
	height = 32.0f;
	speed = 18.0f;
	isShoot = false;
	isHoly = true;
	reloadTimer = 180;
}
void BulletBoss2_2_1::Move(float theta) {
	if (isShoot) {
		pos.x -= speed * cosf(theta);
		pos.y -= speed * sinf(theta);
		if (reloadTimer == 0) {
			isShoot = false;
		}
		if (reloadTimer > 0) {
			reloadTimer--;
		}
		else {
			reloadTimer = 180;
		}
	}
}
void BulletBoss2_2_1::Draw() {
	int Texture_Bullet = Novice::LoadTexture("./Resources/Bullet/bullet.png");
	if (isShoot) {
		if (isHoly) {
			Novice::DrawSpriteRect(
				(int)pos.x,
				(int)pos.y,
				128, 0, 32, 32,
				Texture_Bullet,
				1.0f / 6, 1, 0.0f, WHITE
			);
		}
		if (!isHoly) {
			Novice::DrawSpriteRect(
				(int)pos.x,
				(int)pos.y,
				160, 0, 32, 32,
				Texture_Bullet,
				1.0f / 6, 1, 0.0f, WHITE
			);
		}
	}
}

void BulletBoss2_2_2::Initalize() {
	pos.x = -128.0f;
	pos.y = -128.0f;
	width = 32.0f;
	height = 32.0f;
	speed = 18.0f;
	isShoot = false;
	isHoly = true;
	reloadTimer = 180;
}
void BulletBoss2_2_2::Move(float theta) {
	if (isShoot) {
		pos.x -= speed * cosf(theta);
		pos.y -= speed * sinf(theta);
		if (reloadTimer == 0) {
			isShoot = false;
		}
		if (reloadTimer > 0) {
			reloadTimer--;
		}
		else {
			reloadTimer = 180;
		}
	}
}
void BulletBoss2_2_2::Draw() {
	int Texture_Bullet = Novice::LoadTexture("./Resources/Bullet/bullet.png");
	if (isShoot) {
		if (isHoly) {
			Novice::DrawSpriteRect(
				(int)pos.x,
				(int)pos.y,
				128, 0, 32, 32,
				Texture_Bullet,
				1.0f / 6, 1, 0.0f, WHITE
			);
		}
		if (!isHoly) {
			Novice::DrawSpriteRect(
				(int)pos.x,
				(int)pos.y,
				160, 0, 32, 32,
				Texture_Bullet,
				1.0f / 6, 1, 0.0f, WHITE
			);
		}
	}
}

void BulletBoss2_2_3::Initalize() {
	pos.x = -128.0f;
	pos.y = -128.0f;
	width = 32.0f;
	height = 32.0f;
	speed = 18.0f;
	isShoot = false;
	isHoly = true;
	reloadTimer = 180;
}
void BulletBoss2_2_3::Move(float theta) {
	if (isShoot) {
		pos.x -= speed * cosf(theta);
		pos.y -= speed * sinf(theta);
		if (reloadTimer == 0) {
			isShoot = false;
		}
		if (reloadTimer > 0) {
			reloadTimer--;
		}
		else {
			reloadTimer = 180;
		}
	}
}
void BulletBoss2_2_3::Draw() {
	int Texture_Bullet = Novice::LoadTexture("./Resources/Bullet/bullet.png");
	if (isShoot) {
		if (isHoly) {
			Novice::DrawSpriteRect(
				(int)pos.x,
				(int)pos.y,
				128, 0, 32, 32,
				Texture_Bullet,
				1.0f / 6, 1, 0.0f, WHITE
			);
		}
		if (!isHoly) {
			Novice::DrawSpriteRect(
				(int)pos.x,
				(int)pos.y,
				160, 0, 32, 32,
				Texture_Bullet,
				1.0f / 6, 1, 0.0f, WHITE
			);
		}
	}
}

void BulletBoss2_3::Initalize() {
	pos.x = -128.0f;
	pos.y = -128.0f;
	width = 1280.0f;
	height = 200.0f;
	speed = 50.0f;
	isShoot = false;
	reloadTimer = 300;
	timer = 180;
}
//void BulletBoss2_3::Move() {
//	if (isShoot) {
//		pos.x -= speed;
//		pos.y -= speed;
//		/*if (reloadTimer == 0) {
//			isShoot = false;
//		}
//		if (reloadTimer > 0) {
//			reloadTimer--;
//		}
//		else {
//			reloadTimer = 300;
//		}*/
//	}
//}
void BulletBoss2_3::Draw() {
	int Texture_bulletLaser = Novice::LoadTexture("./Resources/Bullet/laser.png");
	Novice::DrawSprite(
		(int)pos.x,
		(int)pos.y,
		Texture_bulletLaser,
		1, 1, 0.0f, WHITE
	);
}