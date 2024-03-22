#pragma once
#include <MathFunction.h>

class Player {
public:
	Vector2 pos;
	Vector2 velocity;
	float width;
	float height;
	float hp;
	float hpMax;
	int shootCoolTime;
	bool isAlive;
	bool isAttributeHoly;
	int chance;
	int randPlayerPosX;
	int randPlayerPosY;
	void Initalize();
	void AttributeChange();
	Vector2 Move(char keys[]);
	void Draw();
};

class Enemy_Zako {
public:
	Vector2 pos;
	Vector2 velocity;
	float width;
	float height;
	int shootCoolTime;
	int respawnTimer;
	int bulletNum;
	bool isAlive;
	float hp;
	float hpMax;
	void Initalize(float Magnification);
	void Respawn(float Magnification);
	void Move();
	void Draw();
};

class Enemy_Zako2 {
public:
	Vector2 pos;
	Vector2 velocity;
	float width;
	float height;
	int shootCoolTime;
	int respawnTimer;
	bool isAlive;
	float hp;
	float hpMax;
	void Initalize(float Magnification);
	void Respawn(Player* player, float Magnification);
	void Move();
	void Draw();
};

class Enemy_Elite1 {
public:
	Vector2 pos;
	Vector2 velocity;
	float width;
	float height;
	int shootCoolTime;
	int respawnTimer;
	bool isAlive;
	float hp;
	float hpMax;
	void Initalize(float Magnification);
	void Respawn(float Magnification);
	void Move(Player* player);
	void Draw();
};

class Enemy_Elite2 {
public:
	Vector2 pos;
	Vector2 velocity;
	float width;
	float height;
	int shootCoolTime;
	int shootInterval;
	int respawnTimer;
	bool isShoot;
	bool isAlive;
	float hp;
	float hpMax;
	void Initalize(float Magnification);
	void Respawn(float Magnification);
	void Move();
	void Draw();
};

class Enemy_Boss1 {
public:
	Vector2 pos;
	Vector2 velocity;
	float width;
	float height;
	float hpMax;
	float hp;
	int shootCoolTimeSG1;
	int shootCoolTimeSG2;
	int shootCoolTimeSG3;
	int shootCoolTimeFL1;
	int shootCoolTimeFL2;
	int respawnTimer;
	bool isAlive;
	bool isBossDefeated;
	bool bossPattern1 ;
	bool bossPattern2 ;
	void Initalize();
	void Respawn();
	void Move();
	void Draw();
};

class Enemy_Boss2 {
public:
	Vector2 pos;
	Vector2 velocity;
	float width;
	float height;
	float hpMax;
	float hp;
	int shootCoolTimeBH;
	int shootCoolTimeBA;
	int shootCoolTimeLS;
	int respawnTimer;
	bool isAlive;
	bool isBossDefeated;
	void Initalize();
	void Respawn();
	void Move();
	void Draw();
};