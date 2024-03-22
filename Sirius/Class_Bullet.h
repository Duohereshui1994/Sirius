#pragma once
#include <MathFunction.h>

//普通弹
class Bullet0 {
public:
	Vector2 pos;
	float width;
	float height;
	float speed;
	bool isShoot;
	void Initalize();
	void Move();
	void Draw(int x);
};

//5发散射弹
class Bullet1 {
public:
	Vector2 pos;
	float width;
	float height;
	float speed;
	bool isShoot;
	int reloadTimer1;
	void Initalize();
	void Move(float theta);
	void Draw();
};

//12发圆形散射
class Bullet2 {
public:
	Vector2 pos;
	float width;
	float height;
	float speed;
	bool isShoot;
	int reloadTimer2;
	void Initalize();
	void Move(float theta);
	void Draw();
};

//32发正弦射击
class Bullet3 {
public:
	Vector2 pos;
	float width;
	float height;
	float speed;
	bool isShoot;
	void Initalize();
	void Move(float theta, float amplitude);
	void Draw();
};

//2发追踪弹
class Bullet4 {
public:
	Vector2 pos;
	float width;
	float height;
	float speed;
	bool isShoot;
	void Initalize();
	void Draw();
};

//32发旋转射弹
class Bullet5 {
public:
	Vector2 pos;
	float width;
	float height;
	float speed;
	bool isShoot;
	void Initalize();
};

//精英子弹
class BulletElite2 {
public:
	Vector2 pos;
	float width;
	float height;
	float speed;
	bool isShoot;
	bool isHoly;
	void Initalize();
};

//boss1子弹
class BulletBoss1_1_1 {
public:
	Vector2 pos;
	float width;
	float height;
	float speed;
	int timer;
	bool isShoot;
	bool isHoly;
	int reloadTimer;
	void Initalize();
	void Move(float theta);
	void Draw();
};

class BulletBoss1_1_2 {
public:
	Vector2 pos;
	float width;
	float height;
	float speed;
	int timer;
	bool isShoot;
	bool isHoly;
	int reloadTimer;
	void Initalize();
	void Move(float theta);
	void Draw();
};

class BulletBoss1_1_3 {
public:
	Vector2 pos;
	float width;
	float height;
	float speed;
	int timer;
	bool isShoot;
	bool isHoly;
	int reloadTimer;
	void Initalize();
	void Move(float theta);
	void Draw();
};

class BulletBoss1_2_1 {
public:
	Vector2 pos;
	Vector2 vector;
	Vector2 vectorNew;
	float length;
	float width;
	float height;
	float speed;
	int timer;
	bool isShoot;
	bool isHoly;
	int reloadTimer;
	void Initalize();
	void Draw();
};

class BulletBoss1_2_2 {
public:
	Vector2 pos;
	Vector2 vector;
	Vector2 vectorNew;
	float length;
	float width;
	float height;
	float speed;
	int timer;
	bool isShoot;
	bool isHoly;
	int reloadTimer;
	void Initalize();
	void Draw();
};
//boss2子弹
class BulletBoss2_1 {
public:
	Vector2 pos;
	float width;
	float height;
	float speed;
	int timer;
	bool isShoot;
	void Initalize();
};

class BulletBoss2_2_1 {
public:
	Vector2 pos;
	float width;
	float height;
	float speed;
	int timer;
	bool isShoot;
	bool isHoly;
	int reloadTimer;
	void Initalize();
	void Move(float theta);
	void Draw();
};
class BulletBoss2_2_2 {
public:
	Vector2 pos;
	float width;
	float height;
	float speed;
	int timer;
	bool isShoot;
	bool isHoly;
	int reloadTimer;
	void Initalize();
	void Move(float theta);
	void Draw();
};
class BulletBoss2_2_3 {
public:
	Vector2 pos;
	float width;
	float height;
	float speed;
	int timer;
	bool isShoot;
	bool isHoly;
	int reloadTimer;
	void Initalize();
	void Move(float theta);
	void Draw();
};

class BulletBoss2_3{
public:
	Vector2 pos;
	float width;
	float height;
	float speed;
	int timer;
	bool isShoot;
	int reloadTimer;
	void Initalize();
	//void Move();
	void Draw();
};