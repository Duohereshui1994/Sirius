#define _USE_MATH_DEFINES
#include <Novice.h>
#include <stdlib.h>
#include <time.h>
#include <MathFunction.h>
#include <Class_Character.h>
#include <Class_Bullet.h>
#include <math.h>

#pragma region
#pragma endregion

const char kWindowTitle[] = "5204_Sirius";

typedef struct Box {
	Vector2 position;
	Vector2 size;
}Box;

typedef struct BulletEnemy {
	Vector2 pos;
	float width;
	float height;
	float speed;
	bool isShoot;
	bool isHoly;
} BulletEnemy;

float easeInSine(float x) {
	return 1.0f - cosf((x * (float)M_PI) / 2.0f);
}
float easeOutBack(float x) {
	const float c1 = 1.70158f;
	const float c3 = c1 + 1;
	return 1 + c3 * powf(x - 1, 3) + c1 * powf(x - 1, 2);
}
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	srand((unsigned)time(NULL));
	//全局难度变量
	float magnification = 1.0f;
#pragma region 音楽
	int SOUND_title = Novice::LoadAudio("./Resources/Sound/title.wav");
	int SOUND_titleLoading = Novice::LoadAudio("./Resources/Sound/titleLoading.wav");
	int SOUND_bossLaser = Novice::LoadAudio("./Resources/Sound/bossLaser.wav");
	int SOUND_stage = Novice::LoadAudio("./Resources/Sound/stage.wav");
	int SOUND_pause = Novice::LoadAudio("./Resources/Sound/pause.wav");
	int SOUND_blackHole = Novice::LoadAudio("./Resources/Sound/blackHole.wav");
	int SOUND_clear = Novice::LoadAudio("./Resources/Sound/win.wav");
	int SOUND_lose = Novice::LoadAudio("./Resources/Sound/lose.wav");
	int SOUND_select = Novice::LoadAudio("./Resources/Sound/select.wav");

	int voiceHandle_title = -1;
	int voiceHandle_titleLoading = -1;
	int voiceHandle_bossLaser = -1;
	int voiceHandle_stage = -1;
	int voiceHandle_pause = -1;
	int voiceHandle_blackHole = -1;
	int voiceHandle_clear = -1;
	int voiceHandle_lose = -1;
	int voiceHandle_select = -1;
#pragma endregion
#pragma region Shake
	bool isPlayerShake = false;
	float framePlayerShake = 0;

	bool isBackgroundShake = false;
	float frameBackgroundShake = 0;
	int randBackgroundPosX = 0;
	int randBackgroundPosY = 0;
#pragma endregion
#pragma region 暂停界面

	//暂停界面
	bool isPause = false;
	enum Menu {
		Continue,
		Back,
		Quit
	};
	Menu menu = Continue;
#pragma endregion
#pragma region scene 界面
	//scene 界面
	enum Test {
		TITLE,
		TUTORIAL,
		STAGE1,
		BOSS1,
		LOADING,
		STAGE2,
		BOSS2,
		CLEAR,
		OVER
	};
	Test scene = TITLE;
#pragma endregion
#pragma region 开局动画参数
	int titleTimer = 20;
	int clearTimer = 20;
	int overTimer = 20;
	bool isClear = false;
	bool isLose = false;

	float titleX = 0;
	float titleY = 0;
	int countTitle = 0;
	bool isTitleStart = false;
	float titleRatio = 1;

	float titlePlayerX = -320;
	float titlePlayerY = 128;
	float titlePlayerRatio = 5;
	bool titlePlayerMoving = false;
	bool titlePlayerBack = false;

	int titlePlayer = Novice::LoadTexture("./Resources/Character/player_1.png");

#pragma endregion

#pragma region loading 参数
	float loadingPlayerX = 100;
	float loadingPlayerY = 340;
	bool isBoss1DeathAnimeOn = false;
	bool isNextStage = false;
	int b1DAnimeStep = 0;
	int b1DAnimeCount = 0;
	int Texture_B1DAnime = Novice::LoadTexture("./Resources/Character/boss1Sheet.png");
#pragma endregion
	//输入射弹状态
	int inputArr[3] = { 0,0,0 };
	int state = 0;

	//元素
	int Texture_element = Novice::LoadTexture("./Resources/Bullet/elements.png");
	//pause
	int Texture_Pause = Novice::LoadTexture("./Resources/Background/pause.png");
#pragma region bg
	//bg
	int Texture_Bg0 = Novice::LoadTexture("./Resources/Background/background.png");
	int Texture_Bg1 = Novice::LoadTexture("./Resources/Background/stars1.png");
	int Texture_Bg2 = Novice::LoadTexture("./Resources/Background/stars2.png");
	int Texture_Bg3 = Novice::LoadTexture("./Resources/Background/UIborder.png");
	int Texture_Bg4 = Novice::LoadTexture("./Resources/Character/star.png");
	Vector2 bg0Pos = { 0,0 };
	Vector2 bg1Pos = { 0,0 };
	Vector2 bg2Pos = { 0,0 };
	float starPosX = 1100.0f;
	int starPosY = 75;
	float starMagnification = 0.01f;

	//danger
	int Texture_Danger1 = Novice::LoadTexture("./Resources/Background/danger1.png");
	int Texture_Danger2 = Novice::LoadTexture("./Resources/Background/danger2.png");

	//bgTitle
	int Texture_Title = Novice::LoadTexture("./Resources/Background/title.png");
	int Texture_Clear = Novice::LoadTexture("./Resources/Background/gameclear.png");
	int Texture_Over = Novice::LoadTexture("./Resources/Background/gameover.png");
	//tutorial
	int Texture_Tutorial = Novice::LoadTexture("./Resources/Background/tutorial.png");
	float tutoPosX = 0.0f;
	float endFrameTuto = 120.0f;
	float nowFrameTuto = 0.0f;
	float xFrameTuto = 0.0f;
	bool isTutoMove = false;
	//gameover
	float gameOverPosY = 0.0f;
	float endFrameOver = 30.0f;
	float nowFrameOver = 0.0f;
	float xFrameOver = 0.0f;
	bool isGameOverMove = false;
	//gameclear
	float gameClearPosY = 0.0f;
	float endFrameClear = 30.0f;
	float nowFrameClear = 0.0f;
	float xFrameClear = 0.0f;
	bool isGameClearMove = false;
	//chance
	int Texture_Chance = Novice::LoadTexture("./Resources/Character/player_chance.png");
	//warning
	int Texture_Warning = Novice::LoadTexture("./Resources/Background/warning.png");
	//attack alert
	int Texture_AlertYellow = Novice::LoadTexture("./Resources/Character/yellow.png");
	int Texture_AlertGreen = Novice::LoadTexture("./Resources/Character/green.png");
#pragma endregion
#pragma region Score

	//Score
	int Texture_ScoreNumber[10];
	Texture_ScoreNumber[0] = Novice::LoadTexture("./Resources/Background/number/0.png");
	Texture_ScoreNumber[1] = Novice::LoadTexture("./Resources/Background/number/1.png");
	Texture_ScoreNumber[2] = Novice::LoadTexture("./Resources/Background/number/2.png");
	Texture_ScoreNumber[3] = Novice::LoadTexture("./Resources/Background/number/3.png");
	Texture_ScoreNumber[4] = Novice::LoadTexture("./Resources/Background/number/4.png");
	Texture_ScoreNumber[5] = Novice::LoadTexture("./Resources/Background/number/5.png");
	Texture_ScoreNumber[6] = Novice::LoadTexture("./Resources/Background/number/6.png");
	Texture_ScoreNumber[7] = Novice::LoadTexture("./Resources/Background/number/7.png");
	Texture_ScoreNumber[8] = Novice::LoadTexture("./Resources/Background/number/8.png");
	Texture_ScoreNumber[9] = Novice::LoadTexture("./Resources/Background/number/9.png");
	int score = 0;
	int eachScoreNumber[4] = {};
#pragma endregion
#pragma region 子弹类型

	//子弹类型
	enum ShootStates {
		S0,
		S1,
		S2,
		S3,
		S4,
		S5,
		S6,
	};
	ShootStates shootStates = S0;
#pragma endregion
	//自分
	Player* player1 = new Player();//用 new 分配内存。不用时需要用delete语句释放掉。
	//自分初期化
	player1->Initalize();
#pragma region enemy变量

	//enemy example
	int Texture_Zako_eg = Novice::LoadTexture("./Resources/Character/enemy.png");
	int Texture_Zako2_eg = Novice::LoadTexture("./Resources/Character/enemy2.png");
	int Texture_Elite1_eg = Novice::LoadTexture("./Resources/Character/enemy3.png");
	int Texture_Elite2_eg = Novice::LoadTexture("./Resources/Character/enemy4.png");
	Enemy_Zako* enemyZako_eg = new Enemy_Zako;
	enemyZako_eg->Initalize(magnification);
	enemyZako_eg->pos.x = 1000.0f;
	enemyZako_eg->pos.y = 160.0f;
	enemyZako_eg->isAlive = true;
	Enemy_Zako2* enemyZako2_eg = new Enemy_Zako2();
	enemyZako2_eg->Initalize(magnification);
	enemyZako2_eg->pos.x = 1000.0f;
	enemyZako2_eg->pos.y = 240.0f;
	enemyZako2_eg->isAlive = true;
	Enemy_Elite1* enemyElite1_eg = new Enemy_Elite1;
	enemyElite1_eg->Initalize(magnification);
	enemyElite1_eg->pos.x = 1000.0f;
	enemyElite1_eg->pos.y = 336.0f;
	enemyElite1_eg->isAlive = true;
	Enemy_Elite2* enemyElite2_eg = new Enemy_Elite2;
	enemyElite2_eg->Initalize(magnification);
	enemyElite2_eg->pos.x = 1000.0f;
	enemyElite2_eg->pos.y = 432.0f;
	enemyElite2_eg->isAlive = true;

	//enemy
	Enemy_Zako* enemyZako[10];
	for (int i = 0; i < 10; i++) {
		enemyZako[i] = new Enemy_Zako();
		enemyZako[i]->Initalize(magnification);
	}

	Enemy_Zako2* enemyZako2[10];
	for (int i = 0; i < 10; i++) {
		enemyZako2[i] = new Enemy_Zako2();
		enemyZako2[i]->Initalize(magnification);
	}

	Enemy_Elite1* enemyElite1 = new Enemy_Elite1;
	enemyElite1->Initalize(magnification);

	Enemy_Elite2* enemyElite2 = new Enemy_Elite2;
	enemyElite2->Initalize(magnification);

	Enemy_Boss1* enemyBoss1 = new Enemy_Boss1;
	enemyBoss1->Initalize();

	Enemy_Boss2* enemyBoss2 = new Enemy_Boss2;
	enemyBoss2->Initalize();
#pragma endregion
#pragma region 子弹相关
	// 弾の変数
	float bulletTimer = 300.0f;
	bool isbulletTimerOn = false;
	float bulletBarlength = 0;
	int Texture_Bullet = Novice::LoadTexture("./Resources/Bullet/bullet.png");
	int Texture_BlackHole = Novice::LoadTexture("./Resources/Bullet/blackhole.png");

	//locked bullet
	int Texture_Lockedbullet = Novice::LoadTexture("./Resources/Bullet/lockedBullets.png");
	//普通连射弹
	Bullet0* bullet0[32];
	for (int i = 0; i < 32; i++) {
		bullet0[i] = new Bullet0();
		bullet0[i]->Initalize();
	}

	//五发散射
	Bullet1* bullet1[5];
	for (int i = 0; i < 5; i++) {
		bullet1[i] = new Bullet1();
		bullet1[i]->Initalize();
	}
	float theta1[5] = {};
	theta1[0] = (0.0f / 2 - 1.0f / 6) * float(M_PI);
	theta1[1] = (0.0f / 2 - 1.0f / 12) * float(M_PI);
	theta1[2] = 0.0f * float(M_PI);
	theta1[3] = (0.0f / 2 + 1.0f / 12) * float(M_PI);
	theta1[4] = (0.0f / 2 + 1.0f / 6) * float(M_PI);

	//12发散射
	Bullet2* bullet2[12];
	for (int i = 0; i < 12; i++) {
		bullet2[i] = new Bullet2();
		bullet2[i]->Initalize();
	}
	bool isBullet2On = false;

	int reloadTimer2 = 45;
	float theta2[12] = {};
	for (int i = 0; i < 12; i++) {
		theta2[i] = i / 6.0f * float(M_PI);
	}

	//32发正弦射击
	Bullet3* bullet3[32];
	for (int i = 0; i < 32; i++) {
		bullet3[i] = new Bullet3();
		bullet3[i]->Initalize();
	}
	float theta3[32] = { 0 };
	float amplitude = 30.0f;

	//2发追踪弹 仅拦截boss导弹用	
	Bullet4* bullet4[2];
	for (int i = 0; i < 2; i++) {
		bullet4[i] = new Bullet4();
		bullet4[i]->Initalize();
	}
	float lengthBullet4[2] = { 1300.0f,1300.0f };

	//32发旋转射弹
	Bullet5* bullet5[32];
	for (int i = 0; i < 32; i++) {
		bullet5[i] = new Bullet5();
		bullet5[i]->Initalize();
	}

	Vector2 rectCenter[32] = {};
	Vector2 leftTop0[32] = {};
	Vector2 rightTop0[32] = {};
	Vector2 leftBottom0[32] = {};
	Vector2 rightBottom0[32] = {};
	for (int i = 0; i < 32; i++) {
		rectCenter[i] = { bullet5[i]->width / 2,bullet5[i]->height / 2 };

		leftTop0[i] = { -bullet5[i]->width / 2, bullet5[i]->height / 2 };
		rightTop0[i] = { bullet5[i]->width / 2,bullet5[i]->height / 2 };
		leftBottom0[i] = { -bullet5[i]->width / 2,-bullet5[i]->height / 2 };
		rightBottom0[i] = { bullet5[i]->width / 2,-bullet5[i]->height / 2 };
	}

	float theta5 = 0;
	Vector2 scale = { 1.0f,1.0f };
	Vector2 scaleIncrement = { 0.02f,0.02f };
	Matrix3x3 affineMatrix[32] = {};

	Vector2 worldLeftTop[32] = {};
	Vector2 worldRightTop[32] = {};
	Vector2 worldLeftBottom[32] = {};
	Vector2 worldRightBottom[32] = {};
	//======================== enemy ========================
	//敌弹
	BulletEnemy bulletZako[10];
	for (int i = 0; i < 10; i++) {
		bulletZako[i].pos.x = -128.0f;
		bulletZako[i].pos.y = -128.0f;
		bulletZako[i].width = 64.0f;
		bulletZako[i].height = 64.0f;
		bulletZako[i].speed = 10.0f;
		bulletZako[i].isShoot = false;
		bulletZako[i].isHoly = true;
	}
	////elite2 子弹
	BulletElite2* bulletElite2[10];
	for (int i = 0; i < 10; i++) {
		bulletElite2[i] = new BulletElite2;
		bulletElite2[i]->Initalize();
	}
	int bulletElite2Num = 10;

	//========================Boss 1========================
	//shotgun
	BulletBoss1_1_1* bulletB111[5];
	BulletBoss1_1_2* bulletB112[5];
	BulletBoss1_1_3* bulletB113[5];
	for (int i = 0; i < 5; i++) {
		bulletB111[i] = new BulletBoss1_1_1();
		bulletB111[i]->Initalize();
		bulletB112[i] = new BulletBoss1_1_2();
		bulletB112[i]->Initalize();
		bulletB113[i] = new BulletBoss1_1_3();
		bulletB113[i]->Initalize();
	}
	float thetaB11[5] = { -6,-3,0,3,6 };

	//follow
	BulletBoss1_2_1* bulletB121[10];
	BulletBoss1_2_2* bulletB122[10];
	for (int i = 0; i < 10; i++) {
		bulletB121[i] = new BulletBoss1_2_1();
		bulletB121[i]->Initalize();
		bulletB122[i] = new BulletBoss1_2_2();
		bulletB122[i]->Initalize();
	}
	float bossSaveVectorX1[10] = { 0 };
	float bossSaveVectorY1[10] = { 0 };
	float bossSaveVectorX2[10] = { 0 };
	float bossSaveVectorY2[10] = { 0 };


	//========================Boss 2========================
	//黑洞
	BulletBoss2_1* bulletBlackHole = new BulletBoss2_1();
	bulletBlackHole->Initalize();
	Vector2 rectCenterBB2 = {};
	Vector2 leftTopBB2 = {};
	Vector2 rightTopBB2 = {};
	Vector2 leftBottomBB2 = {};
	Vector2 rightBottomBB2 = {};

	rectCenterBB2 = { bulletBlackHole->width / 2,bulletBlackHole->height / 2 };

	leftTopBB2 = { -bulletBlackHole->width / 2, bulletBlackHole->height / 2 };
	rightTopBB2 = { bulletBlackHole->width / 2,bulletBlackHole->height / 2 };
	leftBottomBB2 = { -bulletBlackHole->width / 2,-bulletBlackHole->height / 2 };
	rightBottomBB2 = { bulletBlackHole->width / 2,-bulletBlackHole->height / 2 };


	float thetaBB2 = 0;
	Vector2 scaleBB2 = { 1.0f,1.0f };
	Matrix3x3 affineMatrixBB2 = {};

	Vector2 worldLeftTopBB2 = {};
	Vector2 worldRightTopBB2 = {};
	Vector2 worldLeftBottomBB2 = {};
	Vector2 worldRightBottomBB2 = {};
	float lengthBulletBH = 0;

	//弹幕1
	BulletBoss2_2_1* bulletB221[5];
	BulletBoss2_2_2* bulletB222[5];
	BulletBoss2_2_3* bulletB223[5];
	for (int i = 0; i < 5; i++) {
		bulletB221[i] = new BulletBoss2_2_1();
		bulletB221[i]->Initalize();
		bulletB222[i] = new BulletBoss2_2_2();
		bulletB222[i]->Initalize();
		bulletB223[i] = new BulletBoss2_2_3();
		bulletB223[i]->Initalize();
	}
	float thetaB22[5] = {};
	thetaB22[0] = (0.0f / 2 - 1.0f / 6) * float(M_PI);
	thetaB22[1] = (0.0f / 2 - 1.0f / 12) * float(M_PI);
	thetaB22[2] = 0.0f * float(M_PI);
	thetaB22[3] = (0.0f / 2 + 1.0f / 12) * float(M_PI);
	thetaB22[4] = (0.0f / 2 + 1.0f / 6) * float(M_PI);
	//镭射
	int Texture_LaserPartical = Novice::LoadTexture("./Resources/bullet/paritical.png");
	Box box[10];
	for (int i = 0; i < 10; i++) {
		box[i].position.x = 0;
		box[i].position.y = 0;
		box[i].size.x = 20;
		box[i].size.y = 20;
	}
	BulletBoss2_3* bulletLaser = new BulletBoss2_3();
	bulletLaser->Initalize();
#pragma endregion
	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
#pragma region Score
		//Score
		//千位
		eachScoreNumber[0] = score % 10000 / 1000;
		//百位
		eachScoreNumber[1] = score % 1000 / 100;
		//十位
		eachScoreNumber[2] = score % 100 / 10;
		//个位
		eachScoreNumber[3] = score % 10;
#pragma endregion
#pragma region bg移动
		//bg移动
		bg0Pos.x -= 1;
		bg1Pos.x -= 5;
		bg2Pos.x -= 15;
		if (bg0Pos.x <= -1280) {
			bg0Pos.x = 0;
		}
		if (bg1Pos.x <= -1280) {
			bg1Pos.x = 0;
		}
		if (bg2Pos.x <= -1280) {
			bg2Pos.x = 0;
		}
		if (!isPause) {
			starPosX -= 0.1f;
			starPosY = 75;
			starMagnification += 0.0005f;
		}

#pragma endregion
#pragma region Pause
		if (scene == TUTORIAL || scene == STAGE1 || scene == STAGE2 || scene == BOSS1 || scene == BOSS2) {
			//暂停界面
			if (keys[DIK_ESCAPE] && preKeys[DIK_ESCAPE] == 0) {
				voiceHandle_pause = Novice::PlayAudio(SOUND_pause, false, 1.5f);
				menu = Continue;
				isPause = !isPause;
			}
			if (isPause) {
				if (menu == Continue) {
					if (keys[DIK_DOWN] && preKeys[DIK_DOWN] == 0) {
						voiceHandle_select = Novice::PlayAudio(SOUND_select, false, 1.5f);
						menu = Back;
					}
					if (keys[DIK_RETURN] && preKeys[DIK_RETURN] == 0) {
						isPause = false;
					}
				}
				else if (menu == Back) {
					if (keys[DIK_DOWN] && preKeys[DIK_DOWN] == 0) {
						voiceHandle_select = Novice::PlayAudio(SOUND_select, false, 1.5f);
						menu = Quit;
					}
				}
				if (menu == Back) {
					if (keys[DIK_UP] && preKeys[DIK_UP] == 0) {
						voiceHandle_select = Novice::PlayAudio(SOUND_select, false, 1.5f);
						menu = Continue;
					}
					if (keys[DIK_RETURN] && preKeys[DIK_RETURN] == 0) {
						//弹幕编辑初期化
						for (int i = 0; i < 3; i++) {
							inputArr[i] = 0;
						}
						bulletTimer = 300.0f;
						isbulletTimerOn = false;
						//背景星星位置初期化
						starPosX = 1100.0f;
						starMagnification = 0.01f;
						scene = TITLE;
						isPause = false;
					}
				}
				else if (menu == Quit) {
					if (keys[DIK_UP] && preKeys[DIK_UP] == 0) {
						voiceHandle_select = Novice::PlayAudio(SOUND_select, false, 1.5f);
						menu = Back;
					}
					if (keys[DIK_RETURN] && preKeys[DIK_RETURN] == 0) {
						break;
					}
				}
			}
		}
#pragma endregion
		//非暂停游戏中
		if (!isPause) {
			//===================↓↓↓↓↓↓↓↓↓↓↓↓测试用按键↓↓↓↓↓↓↓↓↓↓↓==================================
			//if (keys[DIK_K] && preKeys[DIK_K] == 0) {
			//	player1->chance = -1;
			//	player1->hp = -1;
			//}
			//if (keys[DIK_L] && preKeys[DIK_L] == 0) {
			//	isGameClearMove = true;
			//	scene = CLEAR;
			//}
			////画面切换test
			//if (keys[DIK_Q] && preKeys[DIK_Q] == 0) {
			//	scene = STAGE1;
			//	enemyBoss2->Initalize();
			//	enemyBoss1->Initalize();
			//}
			//if (keys[DIK_W] && preKeys[DIK_W] == 0) {
			//	for (int i = 0; i < 10; i++) {
			//		enemyZako[i]->Initalize(magnification);
			//		enemyZako2[i]->Initalize(magnification);
			//	}
			//	enemyElite1->Initalize(magnification);
			//	enemyElite2->Initalize(magnification);
			//	enemyBoss1->Initalize();
			//	scene = BOSS2;
			//}
			//if (keys[DIK_E] && preKeys[DIK_E] == 0) {
			//	for (int i = 0; i < 10; i++) {
			//		enemyZako[i]->Initalize(magnification);
			//		enemyZako2[i]->Initalize(magnification);
			//	}
			//	enemyElite1->Initalize(magnification);
			//	enemyElite2->Initalize(magnification);
			//	enemyBoss2->Initalize();
			//	scene = BOSS1;
			//}
			//===================↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑测试用按键↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑==================================
			//死亡
			if (player1->isAlive == false) {
				//射弹编辑重置
				for (int i = 0; i < 3; i++) {
					inputArr[i] = 0;
				}
				bulletTimer = 300.0f;
				isbulletTimerOn = false;
			}
			if (scene == STAGE1|| scene == STAGE2 || scene == BOSS1 || scene == BOSS2){
				//消耗一条命回复
				if (player1->hp <= 0 && player1->chance > 0) {
					player1->chance -= 1;
					player1->hp = player1->hpMax;
				}
				//彻底死亡
				if (player1->hp <= 0 && player1->chance <= 0) {
					player1->isAlive = false;
					isGameOverMove = true;
					isLose = true;
					scene = OVER;
				}
			}
			//胜利
			if(scene==BOSS2){
				if (enemyBoss2->isBossDefeated) {
					player1->isAlive = false;
					isGameClearMove = true;
					isClear = true;
					scene = CLEAR;
				}
			}
#pragma region shake
			if (isPlayerShake) {
				framePlayerShake++;

				if (framePlayerShake > 0 && framePlayerShake <= 10) {
					player1->randPlayerPosX = rand() % 21 - 10;
					player1->randPlayerPosY = rand() % 21 - 10;
				}
				if (framePlayerShake > 10 && framePlayerShake <= 20) {
					player1->randPlayerPosX = rand() % 11 - 5;
					player1->randPlayerPosY = rand() % 11 - 5;
				}
				if (framePlayerShake > 20 && framePlayerShake <= 30) {
					player1->randPlayerPosX = rand() % 5 - 2;
					player1->randPlayerPosY = rand() % 5 - 2;
				}
				if (framePlayerShake > 30) {
					isPlayerShake = false;
					framePlayerShake = 0;
					player1->randPlayerPosX = 0;
					player1->randPlayerPosY = 0;
				}
			}
			if (isBackgroundShake) {
				frameBackgroundShake++;

				if (frameBackgroundShake > 20 && frameBackgroundShake <= 30) {
					randBackgroundPosX = rand() % 41 - 20;
					randBackgroundPosY = rand() % 41 - 20;
				}
				if (frameBackgroundShake > 10 && frameBackgroundShake <= 20) {
					randBackgroundPosX = rand() % 21 - 10;
					randBackgroundPosY = rand() % 21 - 10;
				}
				if (frameBackgroundShake > 0 && frameBackgroundShake <= 10) {
					randBackgroundPosX = rand() % 11 - 5;
					randBackgroundPosY = rand() % 11 - 5;
				}
				if (frameBackgroundShake > 30) {
					isBackgroundShake = false;
					frameBackgroundShake = 0;
					randBackgroundPosX = 0;
					randBackgroundPosY = 0;
				}
			}
#pragma endregion
#pragma region 状态切换和移动
			if (scene == TUTORIAL || scene == STAGE1 || scene == STAGE2 || scene == BOSS1 || scene == BOSS2) {
				//状态切换
				if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0) {
					player1->AttributeChange();
				}

				//八方移动
				player1->Move(keys);
			}
#pragma endregion	
#pragma region 射弹编辑
			//射弹状态

			if (scene == TUTORIAL || scene == STAGE1 || scene == STAGE2 || scene == BOSS1 || scene == BOSS2) {
				if (keys[DIK_1] && preKeys[DIK_1] == 0) {
					for (int i = 0; i < 3; i++) {
						if (inputArr[i] == 1) {
							break;
						}
						else if (inputArr[i] == 0) {
							inputArr[i] = 1;
							break;
						}
					}
				}
				if (keys[DIK_2] && preKeys[DIK_2] == 0) {
					for (int i = 0; i < 3; i++) {
						if (inputArr[i] == 2) {
							break;
						}
						else if (inputArr[i] == 0) {
							inputArr[i] = 2;
							break;
						}
					}
				}
				if (keys[DIK_3] && preKeys[DIK_3] == 0) {
					for (int i = 0; i < 3; i++) {
						if (inputArr[i] == 3) {
							break;
						}
						else if (inputArr[i] == 0) {
							inputArr[i] = 3;
							break;
						}
					}
				}
				if (inputArr[2] != 0) {
					isbulletTimerOn = true;
				}
				if (keys[DIK_R] && preKeys[DIK_R] == 0) {
					for (int i = 0; i < 3; i++) {
						inputArr[i] = 0;
					}
					bulletTimer = 300.0f;
					isbulletTimerOn = false;
				}
			}

			state = inputArr[0] * 100 + inputArr[1] * 10 + inputArr[2];
			if (isbulletTimerOn) {
				if (bulletTimer > 0) {
					bulletTimer--;
				}
				else {
					bulletTimer = 300.0f;
					for (int i = 0; i < 3; i++) {
						inputArr[i] = 0;
					}
					isbulletTimerOn = false;
				}
			}
			//子弹计时条计算
			bulletBarlength = bulletTimer / 300.0f * 290;
			switch (state) {
				case 123:
					shootStates = S1;//散射弹
					break;
				case 132:
					shootStates = S5;//拦截飞弹
					break;
				case 213:
					shootStates = S4;//正弦射弹
					break;
				case 231:
					shootStates = S6;//旋转射弹
					break;
				case 312:
					shootStates = S3;//分裂弹
					break;
				case 321:
					shootStates = S2;//速射弹
					break;
				default:
					shootStates = S0;
					break;

			}
#pragma endregion
#pragma region 射弹发射
			// 弾の発射処理
			if (bulletTimer > 0) {
#pragma region S0空仓状态 其他弹幕重置

				if (shootStates == S0) {
					for (int i = 0; i < 32; i++) {
						bullet0[i]->Initalize();
					}
					for (int i = 0; i < 5; i++) {
						bullet1[i]->Initalize();
					}


					for (int i = 0; i < 12; i++) {
						bullet2[i]->Initalize();
					}
					isBullet2On = false;
					reloadTimer2 = 45;

					for (int i = 0; i < 32; i++) {
						bullet3[i]->Initalize();
					}

					amplitude = 25.0f;

					for (int i = 0; i < 2; i++) {
						bullet4[i]->Initalize();
					}
					lengthBullet4[0] = { 1300.0f };
					lengthBullet4[1] = { 1300.0f };

					for (int i = 0; i < 32; i++) {
						bullet5[i]->Initalize();
					}

				}
#pragma endregion
#pragma region S1射击状态 扩散

				if (shootStates == S1) {
					//连射初始化
					for (int i = 0; i < 32; i++) {
						bullet0[i]->Initalize();
					}
					for (int i = 0; i < 12; i++) {
						bullet2[i]->Initalize();
					}
					isBullet2On = false;
					reloadTimer2 = 45;
					for (int i = 0; i < 32; i++) {
						bullet3[i]->Initalize();
					}

					amplitude = 25.0f;


					for (int i = 0; i < 2; i++) {
						bullet4[i]->Initalize();
					}
					lengthBullet4[0] = { 1300.0f };
					lengthBullet4[1] = { 1300.0f };


					for (int i = 0; i < 32; i++) {
						bullet5[i]->Initalize();
					}

					// 発射間隔の調整用クールタイムの計算
					if (player1->shootCoolTime > 0) {
						player1->shootCoolTime--;
					}
					else {
						player1->shootCoolTime = 25;
					}

					// i番目の弾が撃たれていなかったら発射する
					if (player1->shootCoolTime <= 0) {
						if (!bullet1[0]->isShoot && !bullet1[1]->isShoot && !bullet1[2]->isShoot &&
							!bullet1[3]->isShoot && !bullet1[4]->isShoot) {
							for (int i = 0; i < 5; i++) {
								bullet1[i]->isShoot = true;
								bullet1[i]->pos.x = player1->pos.x;
								bullet1[i]->pos.y = player1->pos.y + player1->height / 4.0f;
							}
						}
					}
					// 弾道計算
					for (int i = 0; i < 5; i++) {
						bullet1[i]->Move(theta1[i]);
					}


					//あたり
					for (int i = 0; i < 10; i++) {
						if (enemyZako[i]->isAlive) {
							for (int j = 0; j < 5; j++) {
								if (bullet1[j]->isShoot) {
									if (IsSpriteCollision(bullet1[j]->pos.x, bullet1[j]->width, bullet1[j]->pos.y, bullet1[j]->height,
										enemyZako[i]->pos.x, enemyZako[i]->width, enemyZako[i]->pos.y, enemyZako[i]->height)) {
										bullet1[j]->isShoot = false;
										enemyZako[i]->hp -= 2.0f;
										if (enemyZako[i]->hp <= 0) {
											enemyZako[i]->isAlive = false;
											score += 1;
										}

									}
								}
							}
						}
					}
					for (int i = 0; i < 10; i++) {
						if (enemyZako2[i]->isAlive) {
							for (int j = 0; j < 5; j++) {
								if (bullet1[j]->isShoot) {
									if (IsSpriteCollision(bullet1[j]->pos.x, bullet1[j]->width, bullet1[j]->pos.y, bullet1[j]->height,
										enemyZako2[i]->pos.x, enemyZako2[i]->width, enemyZako2[i]->pos.y, enemyZako2[i]->height)) {
										bullet1[j]->isShoot = false;
										enemyZako2[i]->hp -= 2.0f;
										if (enemyZako2[i]->hp <= 0) {
											enemyZako2[i]->isAlive = false;
											score += 1;
										}

									}
								}
							}
						}
					}
					if (enemyElite1->isAlive) {
						for (int j = 0; j < 5; j++) {
							if (bullet1[j]->isShoot) {
								if (IsSpriteCollision(bullet1[j]->pos.x, bullet1[j]->width, bullet1[j]->pos.y, bullet1[j]->height,
									enemyElite1->pos.x, enemyElite1->width, enemyElite1->pos.y, enemyElite1->height)) {
									bullet1[j]->isShoot = false;
									enemyElite1->hp -= 2.0f;
									if (enemyElite1->hp <= 0) {
										enemyElite1->isAlive = false;
										score += 3;
									}
								}
							}
						}
					}
					if (enemyElite2->isAlive) {
						for (int j = 0; j < 5; j++) {
							if (bullet1[j]->isShoot) {
								if (IsSpriteCollision(bullet1[j]->pos.x, bullet1[j]->width, bullet1[j]->pos.y, bullet1[j]->height,
									enemyElite2->pos.x, enemyElite2->width, enemyElite2->pos.y, enemyElite2->height)) {
									bullet1[j]->isShoot = false;
									enemyElite2->hp -= 2.0f;
									if (enemyElite2->hp <= 0) {
										enemyElite2->isAlive = false;
										score += 5;
									}
								}
							}
						}
					}
					if (!enemyBoss1->isBossDefeated) {
						if (enemyBoss1->isAlive) {
							for (int j = 0; j < 5; j++) {
								if (bullet1[j]->isShoot) {
									if (IsSpriteCollision(bullet1[j]->pos.x, bullet1[j]->width, bullet1[j]->pos.y, bullet1[j]->height,
										enemyBoss1->pos.x, enemyBoss1->width, enemyBoss1->pos.y, enemyBoss1->height)) {
										bullet1[j]->isShoot = false;
										enemyBoss1->hp -= 2.0f;
										if (enemyBoss1->hp <= 0) {
											enemyBoss1->isAlive = false;
											enemyBoss1->isBossDefeated = true;
											score += 30;
										}
									}
								}
							}
						}
					}
					if (!enemyBoss2->isBossDefeated) {
						if (enemyBoss2->isAlive) {
							for (int j = 0; j < 5; j++) {
								if (bullet1[j]->isShoot) {
									if (IsSpriteCollision(bullet1[j]->pos.x, bullet1[j]->width, bullet1[j]->pos.y, bullet1[j]->height,
										enemyBoss2->pos.x, enemyBoss2->width, enemyBoss2->pos.y, enemyBoss2->height)) {
										bullet1[j]->isShoot = false;
										enemyBoss2->hp -= 2.0f;
										if (enemyBoss2->hp <= 0) {
											enemyBoss2->isAlive = false;
											enemyBoss2->isBossDefeated = true;
											score += 50;
										}
									}
								}
							}
						}
					}
				}
#pragma endregion
#pragma region S2射击状态 速射

				if (shootStates == S2) {
					//初始化
					for (int i = 0; i < 5; i++) {
						bullet1[i]->Initalize();
					}

					for (int i = 0; i < 12; i++) {
						bullet2[i]->Initalize();
					}
					isBullet2On = false;
					reloadTimer2 = 45;

					// 発射間隔の調整用クールタイムの計算
					if (player1->shootCoolTime > 0) {
						player1->shootCoolTime--;
					}
					else {
						player1->shootCoolTime = 5;
					}

					// i番目の弾が撃たれていなかったら発射する
					if (player1->shootCoolTime <= 0) {
						for (int i = 0; i < 32; i++) {
							if (!bullet0[i]->isShoot) {
								bullet0[i]->pos.x = player1->pos.x;
								bullet0[i]->pos.y = player1->pos.y + player1->height / 4.0f;
								bullet0[i]->isShoot = true;
								break;
							}
						}
					}
					// 弾道計算
					for (int i = 0; i < 32; i++) {
						bullet0[i]->Move();
					}
					//あたりexample
					if (enemyZako_eg->isAlive) {
						for (int j = 0; j < 32; j++) {
							if (bullet0[j]->isShoot) {
								if (IsSpriteCollision(bullet0[j]->pos.x, bullet0[j]->width, bullet0[j]->pos.y, bullet0[j]->height,
									enemyZako_eg->pos.x, enemyZako_eg->width, enemyZako_eg->pos.y, enemyZako_eg->height)) {
									bullet0[j]->isShoot = false;
									enemyZako_eg->isAlive = false;
								}
							}
						}
					}
					if (enemyZako2_eg->isAlive) {
						for (int j = 0; j < 32; j++) {
							if (bullet0[j]->isShoot) {
								if (IsSpriteCollision(bullet0[j]->pos.x, bullet0[j]->width, bullet0[j]->pos.y, bullet0[j]->height,
									enemyZako2_eg->pos.x, enemyZako2_eg->width, enemyZako2_eg->pos.y, enemyZako2_eg->height)) {
									bullet0[j]->isShoot = false;
									enemyZako2_eg->isAlive = false;
								}
							}
						}
					}
					if (enemyElite1_eg->isAlive) {
						for (int j = 0; j < 32; j++) {
							if (bullet0[j]->isShoot) {
								if (IsSpriteCollision(bullet0[j]->pos.x, bullet0[j]->width, bullet0[j]->pos.y, bullet0[j]->height,
									enemyElite1_eg->pos.x, enemyElite1_eg->width, enemyElite1_eg->pos.y, enemyElite1_eg->height)) {
									bullet0[j]->isShoot = false;
									enemyElite1_eg->isAlive = false;
								}
							}
						}
					}
					if (enemyElite2_eg->isAlive) {
						for (int j = 0; j < 32; j++) {
							if (bullet0[j]->isShoot) {
								if (IsSpriteCollision(bullet0[j]->pos.x, bullet0[j]->width, bullet0[j]->pos.y, bullet0[j]->height,
									enemyElite2_eg->pos.x, enemyElite2_eg->width, enemyElite2_eg->pos.y, enemyElite2_eg->height)) {
									bullet0[j]->isShoot = false;
									enemyElite2_eg->isAlive = false;
								}
							}
						}
					}
					//あたり
					for (int i = 0; i < 10; i++) {
						if (enemyZako[i]->isAlive && enemyZako[i]->pos.x <= 1280 - 64) {
							for (int j = 0; j < 32; j++) {
								if (bullet0[j]->isShoot) {
									if (IsSpriteCollision(bullet0[j]->pos.x, bullet0[j]->width, bullet0[j]->pos.y, bullet0[j]->height,
										enemyZako[i]->pos.x, enemyZako[i]->width, enemyZako[i]->pos.y, enemyZako[i]->height)) {
										bullet0[j]->isShoot = false;
										enemyZako[i]->hp -= 2.0f;
										if (enemyZako[i]->hp <= 0) {
											enemyZako[i]->isAlive = false;
											score += 1;
										}
									}
								}
							}
						}
					}
					for (int i = 0; i < 10; i++) {
						if (enemyZako2[i]->isAlive && enemyZako2[i]->pos.x <= 1280 - 64) {
							for (int j = 0; j < 32; j++) {
								if (bullet0[j]->isShoot) {
									if (IsSpriteCollision(bullet0[j]->pos.x, bullet0[j]->width, bullet0[j]->pos.y, bullet0[j]->height,
										enemyZako2[i]->pos.x, enemyZako2[i]->width, enemyZako2[i]->pos.y, enemyZako2[i]->height)) {
										bullet0[j]->isShoot = false;
										enemyZako2[i]->hp -= 2.0f;
										if (enemyZako2[i]->hp <= 0) {
											enemyZako2[i]->isAlive = false;
											score += 1;
										}
									}
								}
							}
						}
					}
					if (enemyElite1->isAlive && enemyElite1->pos.x <= 1280 - 64) {
						for (int j = 0; j < 32; j++) {
							if (bullet0[j]->isShoot) {
								if (IsSpriteCollision(bullet0[j]->pos.x, bullet0[j]->width, bullet0[j]->pos.y, bullet0[j]->height,
									enemyElite1->pos.x, enemyElite1->width, enemyElite1->pos.y, enemyElite1->height)) {
									bullet0[j]->isShoot = false;
									enemyElite1->hp -= 2.0f;
									if (enemyElite1->hp <= 0) {
										enemyElite1->isAlive = false;
										score += 3;
									}
								}
							}
						}
					}
					if (enemyElite2->isAlive) {
						for (int j = 0; j < 32; j++) {
							if (bullet0[j]->isShoot) {
								if (IsSpriteCollision(bullet0[j]->pos.x, bullet0[j]->width, bullet0[j]->pos.y, bullet0[j]->height,
									enemyElite2->pos.x, enemyElite2->width, enemyElite2->pos.y, enemyElite2->height)) {
									bullet0[j]->isShoot = false;
									enemyElite2->hp -= 2.0f;
									if (enemyElite2->hp <= 0) {
										enemyElite2->isAlive = false;
										score += 5;
									}
								}
							}
						}
					}
					if (!enemyBoss1->isBossDefeated) {
						if (enemyBoss1->isAlive) {
							for (int j = 0; j < 32; j++) {
								if (bullet0[j]->isShoot) {
									if (IsSpriteCollision(bullet0[j]->pos.x, bullet0[j]->width, bullet0[j]->pos.y, bullet0[j]->height,
										enemyBoss1->pos.x, enemyBoss1->width, enemyBoss1->pos.y, enemyBoss1->height)) {
										bullet0[j]->isShoot = false;
										enemyBoss1->hp -= 2.0f;
										if (enemyBoss1->hp <= 0) {
											enemyBoss1->isAlive = false;
											enemyBoss1->isBossDefeated = true;
											score += 30;
										}
									}
								}
							}
						}
					}
					if (!enemyBoss2->isBossDefeated) {
						if (enemyBoss2->isAlive) {
							for (int j = 0; j < 32; j++) {
								if (bullet0[j]->isShoot) {
									if (IsSpriteCollision(bullet0[j]->pos.x, bullet0[j]->width, bullet0[j]->pos.y, bullet0[j]->height,
										enemyBoss2->pos.x, enemyBoss2->width, enemyBoss2->pos.y, enemyBoss2->height)) {
										bullet0[j]->isShoot = false;
										enemyBoss2->hp -= 2.0f;
										if (enemyBoss2->hp <= 0) {
											enemyBoss2->isAlive = false;
											enemyBoss2->isBossDefeated = true;
											score += 50;
										}
									}
								}
							}
						}
					}

				}
#pragma endregion
#pragma region S3射击状态 分裂

				if (shootStates == S3) {

					//散射初始化
					for (int i = 0; i < 5; i++) {
						bullet1[i]->Initalize();
					}
					if (!isBullet2On) {
						for (int i = 0; i < 12; i++) {
							bullet2[i]->Initalize();
						}
					}
					// 発射間隔の調整用クールタイムの計算
					if (player1->shootCoolTime > 0) {
						player1->shootCoolTime--;
					}
					else {
						player1->shootCoolTime = 10;
					}

					// i番目の弾が撃たれていなかったら発射する
					if (player1->shootCoolTime <= 0) {
						for (int i = 0; i < 32; i++) {
							if (!bullet0[i]->isShoot) {
								bullet0[i]->pos.x = player1->pos.x;
								bullet0[i]->pos.y = player1->pos.y + player1->height / 4.0f;
								bullet0[i]->isShoot = true;
								break;
							}
						}
					}
					// 弾道計算
					for (int i = 0; i < 32; i++) {
						bullet0[i]->Move();
					}

					//あたり
					for (int i = 0; i < 10; i++) {
						if (enemyZako[i]->isAlive && enemyZako[i]->pos.x <= 1280 - 64) {
							for (int j = 0; j < 32; j++) {
								if (bullet0[j]->isShoot) {
									if (IsSpriteCollision(bullet0[j]->pos.x, bullet0[j]->width, bullet0[j]->pos.y, bullet0[j]->height,
										enemyZako[i]->pos.x, enemyZako[i]->width, enemyZako[i]->pos.y, enemyZako[i]->height)) {
										bullet0[j]->isShoot = false;
										if (scene == STAGE1) {
											isBullet2On = true;
											for (int k = 0; k < 12; k++) {
												if (!bullet2[k]->isShoot) {
													bullet2[k]->isShoot = true;
													bullet2[k]->pos.x = enemyZako[i]->pos.x;
													bullet2[k]->pos.y = enemyZako[i]->pos.y + enemyZako[i]->height / 4.0f;
												}
											}
										}
										enemyZako[i]->hp -= 1.0f;
										if (enemyZako[i]->hp <= 0) {
											enemyZako[i]->isAlive = false;
											score += 1;
										}
									}
								}
							}
						}
						/*if (isBullet2On) {
							for (int j = 0; j < 12; j++) {
								if (!bullet2[j]->isShoot) {
									bullet2[j]->isShoot = true;
									bullet2[j]->pos.x = enemyZako[i]->pos.x;
									bullet2[j]->pos.y = enemyZako[i]->pos.y + enemyZako[i]->height / 4.0f;
								}
							}
						}*/
					}
					//==========================================================================
					for (int i = 0; i < 10; i++) {
						if (enemyZako2[i]->isAlive && enemyZako2[i]->pos.x <= 1280 - 64) {
							for (int j = 0; j < 32; j++) {
								if (bullet0[j]->isShoot) {
									if (IsSpriteCollision(bullet0[j]->pos.x, bullet0[j]->width, bullet0[j]->pos.y, bullet0[j]->height,
										enemyZako2[i]->pos.x, enemyZako2[i]->width, enemyZako2[i]->pos.y, enemyZako2[i]->height)) {
										bullet0[j]->isShoot = false;
										if (scene == STAGE1) {
											isBullet2On = true;
											for (int k = 0; k < 12; k++) {
												if (!bullet2[k]->isShoot) {
													bullet2[k]->isShoot = true;
													bullet2[k]->pos.x = enemyZako2[i]->pos.x;
													bullet2[k]->pos.y = enemyZako2[i]->pos.y + enemyZako2[i]->height / 4.0f;
												}
											}
										}
										enemyZako2[i]->hp -= 1.0f;
										if (enemyZako2[i]->hp <= 0) {
											enemyZako2[i]->isAlive = false;
											score += 1;
										}

									}
								}
							}
						}
						/*if (isBullet2On) {
							for (int j = 0; j < 12; j++) {
								if (!bullet2[j]->isShoot) {
									bullet2[j]->isShoot = true;
									bullet2[j]->pos.x = enemyZako2[i]->pos.x;
									bullet2[j]->pos.y = enemyZako2[i]->pos.y + enemyZako2[i]->height / 4.0f;
								}
							}
						}*/
					}
					//==========================================================================
					if (enemyElite1->isAlive && enemyElite1->pos.x <= 1280 - 64) {
						for (int j = 0; j < 32; j++) {
							if (bullet0[j]->isShoot) {
								if (IsSpriteCollision(bullet0[j]->pos.x, bullet0[j]->width, bullet0[j]->pos.y, bullet0[j]->height,
									enemyElite1->pos.x, enemyElite1->width, enemyElite1->pos.y, enemyElite1->height)) {
									bullet0[j]->isShoot = false;
									if (scene == STAGE1) {
										isBullet2On = true;
										for (int k = 0; k < 12; k++) {
											if (!bullet2[k]->isShoot) {
												bullet2[k]->isShoot = true;
												bullet2[k]->pos.x = enemyElite1->pos.x;
												bullet2[k]->pos.y = enemyElite1->pos.y + enemyElite1->height / 4.0f;
											}
										}
									}
									enemyElite1->hp -= 1.0f;
									if (enemyElite1->hp <= 0) {
										enemyElite1->isAlive = false;
										score += 3;
									}
								}
							}
						}
					}
					/*	if (isBullet2On) {
							for (int j = 0; j < 12; j++) {
								if (!bullet2[j]->isShoot) {
									bullet2[j]->isShoot = true;
									bullet2[j]->pos.x = enemyElite1->pos.x;
									bullet2[j]->pos.y = enemyElite1->pos.y + enemyElite1->height / 4.0f;
								}
							}
						}*/
						//	//==========================================================================
						//if (enemyElite2->isAlive && enemyElite2->pos.x <= 1280 - 64) {
						//	for (int j = 0; j < 32; j++) {
						//		if (bullet0[j]->isShoot) {
						//			if (IsSpriteCollision(bullet0[j]->pos.x, bullet0[j]->width, bullet0[j]->pos.y, bullet0[j]->height,
						//				enemyElite2->pos.x, enemyElite2->width, enemyElite2->pos.y, enemyElite2->height)) {
						//				bullet0[j]->isShoot = false;
						//				if (scene == STAGE1) {
						//					isBullet2On = true;
						//					for (int k = 0; k < 12; k++) {
						//						if (!bullet2[k]->isShoot) {
						//							bullet2[k]->isShoot = true;
						//							bullet2[k]->pos.x = enemyElite2->pos.x;
						//							bullet2[k]->pos.y = enemyElite2->pos.y + enemyElite2->height / 4.0f;
						//						}
						//					}
						//				}
						//				enemyElite2->hp -= 1.0f;
						//				if (enemyElite2->hp <= 0) {
						//					enemyElite2->isAlive = false;
						//					score += 5;
						//				}
						//			}
						//		}
						//	}
						//}
						///*if (isBullet2On) {
						//	for (int j = 0; j < 12; j++) {
						//		if (!bullet2[j]->isShoot) {
						//			bullet2[j]->isShoot = true;
						//			bullet2[j]->pos.x = enemyElite2->pos.x;
						//			bullet2[j]->pos.y = enemyElite2->pos.y + enemyElite2->height / 4.0f;
						//		}
						//	}
						//}*/
						////==========================================================================
						//if (enemyBoss1->isAlive && enemyBoss1->isBossDefeated == false) {
						//	for (int j = 0; j < 32; j++) {
						//		if (bullet0[j]->isShoot) {
						//			if (IsSpriteCollision(bullet0[j]->pos.x, bullet0[j]->width, bullet0[j]->pos.y, bullet0[j]->height,
						//				enemyBoss1->pos.x, enemyBoss1->width, enemyBoss1->pos.y, enemyBoss1->height)) {
						//				bullet0[j]->isShoot = false;
						//				if (scene == STAGE1) {
						//					isBullet2On = true;
						//					enemyBoss1->hp -= 1.0f;
						//					if (enemyBoss1->hp <= 0) {
						//						enemyBoss1->isAlive = false;
						//						enemyBoss1->isBossDefeated = true;
						//						score += 30;
						//					}
						//				}
						//			}
						//		}
						//	}
						//}
						//if (isBullet2On) {
						//	for (int j = 0; j < 12; j++) {
						//		if (!bullet2[j]->isShoot) {
						//			bullet2[j]->isShoot = true;
						//			bullet2[j]->pos.x = enemyBoss1->pos.x;
						//			bullet2[j]->pos.y = enemyBoss1->pos.y + enemyBoss1->height / 2.0f + 16;
						//		}
						//	}
						//}
						////==========================================================================
						//if (enemyBoss2->isAlive && enemyBoss2->isBossDefeated == false) {
						//	for (int j = 0; j < 32; j++) {
						//		if (bullet0[j]->isShoot) {
						//			if (IsSpriteCollision(bullet0[j]->pos.x, bullet0[j]->width, bullet0[j]->pos.y, bullet0[j]->height,
						//				enemyBoss2->pos.x, enemyBoss2->width, enemyBoss2->pos.y, enemyBoss2->height)) {
						//				bullet0[j]->isShoot = false;
						//				if (scene == STAGE1) {
						//					isBullet2On = true;
						//					enemyBoss2->hp -= 1.0f;
						//					if (enemyBoss2->hp <= 0) {
						//						enemyBoss2->isAlive = false;
						//						enemyBoss2->isBossDefeated = true;
						//						score += 50;
						//					}
						//				}
						//			}
						//		}
						//	}
						//}
						//if (isBullet2On) {
						//	for (int j = 0; j < 12; j++) {
						//		if (!bullet2[j]->isShoot) {
						//			bullet2[j]->isShoot = true;
						//			bullet2[j]->pos.x = enemyBoss2->pos.x;
						//			bullet2[j]->pos.y = enemyBoss2->pos.y + enemyBoss2->height / 2.0f + 16;
						//		}
						//	}
						//}
						//==========================================================================
						// 分裂弾道計算
					for (int k = 0; k < 12; k++) {
						bullet2[k]->Move(theta2[k]);
						if (reloadTimer2 == 0) {
							for (int j = 0; j < 12; j++) {
								bullet2[j]->isShoot = false;
							}
							isBullet2On = false;
						}

					}
					if (reloadTimer2 > 0) {
						reloadTimer2--;
					}
					else {
						reloadTimer2 = 45;
					}
					//分裂atari
					if (isBullet2On) {
						for (int i = 0; i < 10; i++) {
							if (enemyZako[i]->isAlive && enemyZako[i]->pos.x <= 1280 - 64) {
								for (int j = 0; j < 12; j++) {
									if (bullet2[j]->isShoot) {
										if (IsSpriteCollision(bullet2[j]->pos.x, bullet2[j]->width, bullet2[j]->pos.y, bullet2[j]->height,
											enemyZako[i]->pos.x, enemyZako[i]->width, enemyZako[i]->pos.y, enemyZako[i]->height)) {
											bullet2[j]->isShoot = false;
											enemyZako[i]->hp -= 1.0f;
											if (enemyZako[i]->hp <= 0) {
												enemyZako[i]->isAlive = false;
												score += 1;
											}
										}
									}
								}
							}
						}
						for (int i = 0; i < 10; i++) {
							if (enemyZako2[i]->isAlive && enemyZako2[i]->pos.x <= 1280 - 64) {
								for (int j = 0; j < 12; j++) {
									if (bullet2[j]->isShoot) {
										if (IsSpriteCollision(bullet2[j]->pos.x, bullet2[j]->width, bullet2[j]->pos.y, bullet2[j]->height,
											enemyZako2[i]->pos.x, enemyZako2[i]->width, enemyZako2[i]->pos.y, enemyZako2[i]->height)) {
											bullet2[j]->isShoot = false;
											enemyZako2[i]->hp -= 1.0f;
											if (enemyZako2[i]->hp <= 0) {
												enemyZako2[i]->isAlive = false;
												score += 1;
											}
										}
									}
								}
							}
						}

						/*if (enemyElite2->isAlive && enemyElite2->pos.x <= 1280 - 64) {
							for (int j = 0; j < 12; j++) {
								if (bullet2[j]->isShoot) {
									if (IsSpriteCollision(bullet2[j]->pos.x, bullet2[j]->width, bullet2[j]->pos.y, bullet2[j]->height,
										enemyElite2->pos.x, enemyElite2->width, enemyElite2->pos.y, enemyElite2->height)) {
										bullet2[j]->isShoot = false;
										enemyElite2->hp -= 1.0f;
										if (enemyElite2->hp <= 0) {
											enemyElite2->isAlive = false;
											score += 5;
										}
									}
								}
							}
						}*/

						/*if (enemyBoss1->isAlive && enemyBoss1->isBossDefeated == false) {
							for (int j = 0; j < 12; j++) {
								if (bullet2[j]->isShoot) {
									if (IsSpriteCollision(bullet2[j]->pos.x, bullet2[j]->width, bullet2[j]->pos.y, bullet2[j]->height,
										enemyBoss1->pos.x, enemyBoss1->width, enemyBoss1->pos.y, enemyBoss1->height)) {
										bullet2[j]->isShoot = false;
										enemyBoss1->hp -= 1.0f;
										if (enemyBoss1->hp <= 0) {
											enemyBoss1->isAlive = false;
											enemyBoss1->isBossDefeated = true;
											score += 30;
										}
									}
								}
							}
						}

						if (enemyBoss2->isAlive && enemyBoss2->isBossDefeated == false) {
							for (int j = 0; j < 12; j++) {
								if (bullet2[j]->isShoot) {
									if (IsSpriteCollision(bullet2[j]->pos.x, bullet2[j]->width, bullet2[j]->pos.y, bullet2[j]->height,
										enemyBoss2->pos.x, enemyBoss2->width, enemyBoss2->pos.y, enemyBoss2->height)) {
										bullet2[j]->isShoot = false;
										enemyBoss2->hp -= 1.0f;
										if (enemyBoss2->hp <= 0) {
											enemyBoss2->isAlive = false;
											enemyBoss2->isBossDefeated = true;
											score += 50;
										}
									}
								}
							}
						}*/

					}

				}
#pragma endregion
#pragma region S4射击状态 正弦

				//正弦射弹
				if (shootStates == S4) {
					//散射初始化
					for (int i = 0; i < 32; i++) {
						bullet0[i]->Initalize();
					}
					for (int i = 0; i < 5; i++) {
						bullet1[i]->Initalize();
					}
					for (int i = 0; i < 12; i++) {
						bullet2[i]->Initalize();
					}
					isBullet2On = false;
					reloadTimer2 = 45;

					// 発射間隔の調整用クールタイムの計算
					if (player1->shootCoolTime > 0) {
						player1->shootCoolTime--;
					}
					else {
						player1->shootCoolTime = 25;
					}

					// i番目の弾が撃たれていなかったら発射する
					if (player1->shootCoolTime <= 0) {
						for (int i = 0; i < 32; i++) {
							if (!bullet3[i]->isShoot) {
								bullet3[i]->isShoot = true;
								bullet3[i]->pos.x = player1->pos.x;
								bullet3[i]->pos.y = player1->pos.y + player1->height / 4.0f;
								break;
							}
						}
					}
					// 弾道計算
					for (int i = 0; i < 32; i++) {
						bullet3[i]->Move(theta3[i], amplitude);
						if (bullet3[i]->isShoot) {
							theta3[i] += 1 / 15.0f * (float)M_PI;
							if (theta3[i] > 2 * (float)M_PI) {
								theta3[i] = 0;
							}
						}
					}

					//あたり
					for (int i = 0; i < 10; i++) {
						if (enemyZako[i]->isAlive && enemyZako[i]->pos.x <= 1280 - 64) {
							for (int j = 0; j < 32; j++) {
								if (bullet3[j]->isShoot) {
									if (IsSpriteCollision(bullet3[j]->pos.x, bullet3[j]->width, bullet3[j]->pos.y, bullet3[j]->height,
										enemyZako[i]->pos.x, enemyZako[i]->width, enemyZako[i]->pos.y, enemyZako[i]->height)) {
										bullet3[j]->isShoot = false;
										enemyZako[i]->hp -= 3.0f;
										if (enemyZako[i]->hp <= 0) {
											enemyZako[i]->isAlive = false;
											score += 1;
										}
									}
								}
							}
						}
					}
					for (int i = 0; i < 10; i++) {
						if (enemyZako2[i]->isAlive && enemyZako2[i]->pos.x <= 1280 - 64) {
							for (int j = 0; j < 32; j++) {
								if (bullet3[j]->isShoot) {
									if (IsSpriteCollision(bullet3[j]->pos.x, bullet3[j]->width, bullet3[j]->pos.y, bullet3[j]->height,
										enemyZako2[i]->pos.x, enemyZako2[i]->width, enemyZako2[i]->pos.y, enemyZako2[i]->height)) {
										bullet3[j]->isShoot = false;
										enemyZako2[i]->hp -= 3.0f;
										if (enemyZako2[i]->hp <= 0) {
											enemyZako2[i]->isAlive = false;
											score += 1;
										}
									}
								}
							}
						}
					}
					if (enemyElite1->isAlive && enemyElite1->pos.x <= 1280 - 64) {
						for (int j = 0; j < 32; j++) {
							if (bullet3[j]->isShoot) {
								if (IsSpriteCollision(bullet3[j]->pos.x, bullet3[j]->width, bullet3[j]->pos.y, bullet3[j]->height,
									enemyElite1->pos.x, enemyElite1->width, enemyElite1->pos.y, enemyElite1->height)) {
									bullet3[j]->isShoot = false;
									enemyElite1->hp -= 3.0f;
									if (enemyElite1->hp <= 0) {
										enemyElite1->isAlive = false;
										score += 3;
									}
								}
							}
						}
					}
					if (enemyElite2->isAlive && enemyElite2->pos.x <= 1280 - 64) {
						for (int j = 0; j < 32; j++) {
							if (bullet3[j]->isShoot) {
								if (IsSpriteCollision(bullet3[j]->pos.x, bullet3[j]->width, bullet3[j]->pos.y, bullet3[j]->height,
									enemyElite2->pos.x, enemyElite2->width, enemyElite2->pos.y, enemyElite2->height)) {
									bullet3[j]->isShoot = false;
									enemyElite2->hp -= 3.0f;
									if (enemyElite2->hp <= 0) {
										enemyElite2->isAlive = false;
										score += 5;
									}
								}
							}
						}
					}
					if (enemyBoss1->isAlive && enemyBoss1->isBossDefeated == false) {
						for (int j = 0; j < 32; j++) {
							if (bullet3[j]->isShoot) {
								if (IsSpriteCollision(bullet3[j]->pos.x, bullet3[j]->width, bullet3[j]->pos.y, bullet3[j]->height,
									enemyBoss1->pos.x, enemyBoss1->width, enemyBoss1->pos.y, enemyBoss1->height)) {
									bullet3[j]->isShoot = false;
									enemyBoss1->hp -= 3.0f;
									if (enemyBoss1->hp <= 0) {
										enemyBoss1->isAlive = false;
										enemyBoss1->isBossDefeated = true;
										score += 30;
									}
								}
							}
						}
					}
					if (enemyBoss2->isAlive && enemyBoss2->isBossDefeated == false) {
						for (int j = 0; j < 32; j++) {
							if (bullet3[j]->isShoot) {
								if (IsSpriteCollision(bullet3[j]->pos.x, bullet3[j]->width, bullet3[j]->pos.y, bullet3[j]->height,
									enemyBoss2->pos.x, enemyBoss2->width, enemyBoss2->pos.y, enemyBoss2->height)) {
									bullet3[j]->isShoot = false;
									enemyBoss2->hp -= 3.0f;
									if (enemyBoss2->hp <= 0) {
										enemyBoss2->isAlive = false;
										enemyBoss2->isBossDefeated = true;
										score += 50;
									}
								}
							}
						}
					}
				}
#pragma endregion
#pragma region S5射击状态 拦截

				//追踪射弹 仅拦截boss射击用
				if (shootStates == S5) {
					//散射初始化
					for (int i = 0; i < 5; i++) {
						bullet1[i]->Initalize();
					}
					for (int i = 0; i < 32; i++) {
						bullet0[i]->Initalize();
					}

					for (int i = 0; i < 12; i++) {
						bullet2[i]->Initalize();
					}
					isBullet2On = false;
					reloadTimer2 = 45;

					for (int i = 0; i < 32; i++) {
						bullet3[i]->Initalize();
					}
					amplitude = 25.0f;

					for (int i = 0; i < 32; i++) {
						bullet5[i]->Initalize();
					}
					for (int i = 0; i < 32; i++) {
						rectCenter[i] = { bullet5[i]->width / 2,bullet5[i]->height / 2 };

						leftTop0[i] = { -bullet5[i]->width / 2, bullet5[i]->height / 2 };
						rightTop0[i] = { bullet5[i]->width / 2,bullet5[i]->height / 2 };
						leftBottom0[i] = { -bullet5[i]->width / 2,-bullet5[i]->height / 2 };
						rightBottom0[i] = { bullet5[i]->width / 2,-bullet5[i]->height / 2 };
					}

					// 発射間隔の調整用クールタイムの計算
					if (player1->shootCoolTime > 0) {
						player1->shootCoolTime--;
					}
					else {
						player1->shootCoolTime = 15;
					}

					// i番目の弾が撃たれていなかったら発射する
					if (player1->shootCoolTime <= 0) {
						for (int i = 0; i < 2; i++) {
							if (!bullet4[i]->isShoot) {
								bullet4[i]->isShoot = true;
								bullet4[i]->pos.x = player1->pos.x;
								bullet4[i]->pos.y = player1->pos.y + player1->height / 4.0f;
							}
						}
					}
					// 弾道計算
					if (bullet4[0]->isShoot) {

						bullet4[0]->pos.y += bullet4[0]->speed;

						if (enemyElite1->isAlive) {
							Vector2 vector0 = {};
							Vector2 vector0New = {};
							vector0.x = enemyElite1->pos.x - bullet4[0]->pos.x;
							vector0.y = enemyElite1->pos.y - bullet4[0]->pos.y;
							lengthBullet4[0] = GetLength(enemyElite1->pos, bullet4[0]->pos);
							if (lengthBullet4[0] != 0) {
								vector0New.x = vector0.x / lengthBullet4[0];
								vector0New.y = vector0.y / lengthBullet4[0];
							}
							if (lengthBullet4[0] <= 300) {
								bullet4[0]->pos.x += vector0New.x * bullet4[0]->speed * 2;
								bullet4[0]->pos.y += vector0New.y * bullet4[0]->speed * 2;
							}
						}

						// 画面外に出たら発射フラグをFalseに変更する
						if (bullet4[0]->pos.x >= 1280 || bullet4[0]->pos.y > 720) {
							bullet4[0]->isShoot = false;
						}

					}
					if (bullet4[1]->isShoot) {
						bullet4[1]->pos.y -= bullet4[1]->speed;
						if (enemyElite1->isAlive) {
							Vector2 vector1 = {};
							Vector2 vector1New = {};
							vector1.x = enemyElite1->pos.x - bullet4[1]->pos.x;
							vector1.y = enemyElite1->pos.y - bullet4[1]->pos.y;
							lengthBullet4[1] = GetLength(enemyElite1->pos, bullet4[1]->pos);
							if (lengthBullet4[1] != 0) {
								vector1New.x = vector1.x / lengthBullet4[1];
								vector1New.y = vector1.y / lengthBullet4[1];
							}
							if (lengthBullet4[1] <= 300) {
								bullet4[1]->pos.x += vector1New.x * bullet4[1]->speed * 2;
								bullet4[1]->pos.y += vector1New.y * bullet4[1]->speed * 2;
							}
						}
						// 画面外に出たら発射フラグをFalseに変更する
						if (bullet4[1]->pos.x >= 1280 || bullet4[1]->pos.y < 0) {
							bullet4[1]->isShoot = false;
						}
					}
					//あたり
					if (enemyElite1->isAlive) {
						for (int j = 0; j < 2; j++) {
							if (bullet4[j]->isShoot) {
								if (IsSpriteCollision(bullet4[j]->pos.x, bullet4[j]->width, bullet4[j]->pos.y, bullet4[j]->height,
									enemyElite1->pos.x, enemyElite1->width, enemyElite1->pos.y, enemyElite1->height)) {
									enemyElite1->isAlive = false;
									bullet4[j]->isShoot = false;
									score += 3;
								}
							}
						}
					}
				}
#pragma endregion
#pragma region S6射击状态 旋转

				//旋转射弹思考
				if (shootStates == S6) {

					// 発射間隔の調整用クールタイムの計算
					if (player1->shootCoolTime > 0) {
						player1->shootCoolTime--;
					}
					else {
						player1->shootCoolTime = 18;
					}

					// i番目の弾が撃たれていなかったら発射する
					if (player1->shootCoolTime <= 0) {
						for (int i = 0; i < 32; i++) {
							if (!bullet5[i]->isShoot) {
								bullet5[i]->isShoot = true;
								rectCenter[i].x = player1->pos.x + player1->height / 2.0f;
								rectCenter[i].y = player1->pos.y + player1->height / 2.0f;
								break;
							}
						}
					}
					// 弾道計算
					for (int i = 0; i < 32; i++) {

						if (bullet5[i]->isShoot) {
							rectCenter[i].x += bullet5[i]->speed;


							// 画面外に出たら発射フラグをFalseに変更する
							if (rectCenter[i].x >= 1280) {
								bullet5[i]->isShoot = false;
							}
						}
					}


					theta5 -= 1.0f / 30 * (float)M_PI;
					if (theta5 < -2 * (float)M_PI) {
						theta5 = 0;
					}
					scale.x += scaleIncrement.x;
					scale.y += scaleIncrement.y;
					if (scale.x >= 2.0f && scale.y >= 2.0f) {
						scaleIncrement.x = -0.02f;
						scaleIncrement.y = -0.02f;
					}
					if (scale.x <= 1.0f && scale.y <= 1.0f) {
						scaleIncrement.x = 0.02f;
						scaleIncrement.y = 0.02f;
					}

					for (int i = 0; i < 32; i++) {
						affineMatrix[i] = MakeAffineMatrix(scale, theta5, rectCenter[i]);
						worldLeftTop[i] = Transform(leftTop0[i], affineMatrix[i]);
						worldRightTop[i] = Transform(rightTop0[i], affineMatrix[i]);
						worldLeftBottom[i] = Transform(leftBottom0[i], affineMatrix[i]);
						worldRightBottom[i] = Transform(rightBottom0[i], affineMatrix[i]);
					}

					//あたり
					for (int i = 0; i < 10; i++) {
						if (enemyZako[i]->isAlive && enemyZako[i]->pos.x <= 1280 - 64) {
							for (int j = 0; j < 32; j++) {
								if (bullet5[j]->isShoot) {
									if (rectCenter[j].x >= enemyZako[i]->pos.x && rectCenter[j].x <= enemyZako[i]->pos.x + enemyZako[i]->width &&
										rectCenter[j].y >= enemyZako[i]->pos.y && rectCenter[j].x <= enemyZako[i]->pos.x + enemyZako[i]->height) {
										bullet5[j]->isShoot = false;
										if (scale.x > 1.5) {
											enemyZako[i]->hp -= 3.0f;
										}
										else {
											enemyZako[i]->hp -= 2.0f;
										}
										if (enemyZako[i]->hp <= 0) {
											enemyZako[i]->isAlive = false;
											score += 1;
										}
									}
								}
							}
						}
					}
					for (int i = 0; i < 10; i++) {
						if (enemyZako2[i]->isAlive && enemyZako2[i]->pos.x <= 1280 - 64) {
							for (int j = 0; j < 32; j++) {
								if (bullet5[j]->isShoot) {
									if (rectCenter[j].x >= enemyZako2[i]->pos.x && rectCenter[j].x <= enemyZako2[i]->pos.x + enemyZako2[i]->width &&
										rectCenter[j].y >= enemyZako2[i]->pos.y && rectCenter[j].x <= enemyZako2[i]->pos.x + enemyZako2[i]->height) {
										bullet5[j]->isShoot = false;
										if (scale.x > 1.5) {
											enemyZako2[i]->hp -= 3.0f;
										}
										else {
											enemyZako2[i]->hp -= 2.0f;
										}
										if (enemyZako2[i]->hp <= 0) {
											enemyZako2[i]->isAlive = false;
											score += 1;
										}
									}
								}
							}
						}
					}
					if (enemyElite1->isAlive && enemyElite1->pos.x <= 1280 - 64) {
						for (int j = 0; j < 32; j++) {
							if (bullet5[j]->isShoot) {
								if (rectCenter[j].x >= enemyElite1->pos.x && rectCenter[j].x <= enemyElite1->pos.x + enemyElite1->width &&
									rectCenter[j].y >= enemyElite1->pos.y && rectCenter[j].x <= enemyElite1->pos.x + enemyElite1->height) {
									bullet5[j]->isShoot = false;
									if (scale.x > 1.5) {
										enemyElite1->hp -= 3.0f;
									}
									else {
										enemyElite1->hp -= 2.0f;
									}
									if (enemyElite1->hp <= 0) {
										enemyElite1->isAlive = false;
										score += 3;
									}
								}
							}
						}
					}
					if (enemyElite2->isAlive && enemyElite2->pos.x <= 1280 - 64) {
						for (int j = 0; j < 32; j++) {
							if (bullet5[j]->isShoot) {
								if (rectCenter[j].x >= enemyElite2->pos.x && rectCenter[j].x <= enemyElite2->pos.x + enemyElite2->width &&
									rectCenter[j].y >= enemyElite2->pos.y && rectCenter[j].x <= enemyElite2->pos.x + enemyElite2->height) {
									bullet5[j]->isShoot = false;
									if (scale.x > 1.5) {
										enemyElite2->hp -= 3.0f;
									}
									else {
										enemyElite2->hp -= 2.0f;
									}
									if (enemyElite2->hp <= 0) {
										enemyElite2->isAlive = false;
										score += 5;
									}
								}
							}
						}
					}
					if (enemyBoss1->isAlive && enemyBoss1->isBossDefeated == false) {
						for (int j = 0; j < 32; j++) {
							if (bullet5[j]->isShoot) {
								if (rectCenter[j].x >= enemyBoss1->pos.x && rectCenter[j].x <= enemyBoss1->pos.x + enemyBoss1->width &&
									rectCenter[j].y >= enemyBoss1->pos.y && rectCenter[j].x <= enemyBoss1->pos.x + enemyBoss1->height) {
									bullet5[j]->isShoot = false;
									if (scale.x > 1.5) {
										enemyBoss1->hp -= 3.0f;
									}
									else {
										enemyBoss1->hp -= 2.0f;
									}
									if (enemyBoss1->hp <= 0) {
										enemyBoss1->isAlive = false;
										enemyBoss1->isBossDefeated = true;
										score += 30;
									}
								}
							}
						}
					}
					if (enemyBoss2->isAlive && enemyBoss2->isBossDefeated == false) {
						for (int j = 0; j < 32; j++) {
							if (bullet5[j]->isShoot) {
								if (rectCenter[j].x >= enemyBoss2->pos.x && rectCenter[j].x <= enemyBoss2->pos.x + enemyBoss2->width &&
									rectCenter[j].y >= enemyBoss2->pos.y && rectCenter[j].x <= enemyBoss2->pos.x + enemyBoss2->height) {
									bullet5[j]->isShoot = false;
									if (scale.x > 1.5) {
										enemyBoss2->hp -= 3.0f;
									}
									else {
										enemyBoss2->hp -= 2.0f;
									}
									if (enemyBoss2->hp <= 0) {
										enemyBoss2->isAlive = false;
										enemyBoss2->isBossDefeated = true;
										score += 50;
									}
								}
							}
						}
					}
				}
#pragma endregion
			}
#pragma endregion
#pragma region TITLE界面
			if (scene == TITLE) {
				if (titleTimer <= 0) {
					titleTimer = 0;
				}
				else {
					titleTimer--;
				}
				
				Novice::StopAudio(voiceHandle_stage);
				Novice::StopAudio(voiceHandle_clear);
				Novice::StopAudio(voiceHandle_lose);
				if (Novice::IsPlayingAudio(voiceHandle_title) == 0 || voiceHandle_title == -1) {
					voiceHandle_title = Novice::PlayAudio(SOUND_title, false, 0.7f);
				}

				if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0 ) {
#pragma region 初期化
					//星星位置重置
					starPosX = 1100.0f;
					starMagnification = 0.01f;
					//得分重置
					score = 0;
					//player1重置
					player1->Initalize();
					//射弹编辑重置
					for (int i = 0; i < 3; i++) {
						inputArr[i] = 0;
					}
					bulletTimer = 300.0f;
					isbulletTimerOn = false;
					//自己子弹重置
					for (int i = 0; i < 32; i++) {
						bullet0[i]->Initalize();
					}
					for (int i = 0; i < 5; i++) {
						bullet1[i]->Initalize();
					}

					for (int i = 0; i < 12; i++) {
						bullet2[i]->Initalize();
					}
					isBullet2On = false;
					reloadTimer2 = 45;
					for (int i = 0; i < 32; i++) {
						bullet3[i]->Initalize();
						theta3[i] = 0;
					}

					for (int i = 0; i < 2; i++) {
						bullet4[i]->Initalize();
						lengthBullet4[i] = 1300.0f;
					}
					for (int i = 0; i < 32; i++) {
						bullet5[i]->Initalize();
					}
					//敌人杂鱼1例子重置
					enemyZako_eg->Initalize(magnification);
					enemyZako_eg->pos.x = 1000.0f;
					enemyZako_eg->pos.y = 160.0f;
					enemyZako_eg->isAlive = true;
					//敌人杂鱼2例子重置
					enemyZako2_eg->Initalize(magnification);
					enemyZako2_eg->pos.x = 1000.0f;
					enemyZako2_eg->pos.y = 240.0f;
					enemyZako2_eg->isAlive = true;
					//敌人精英1例子重置
					enemyElite1_eg->Initalize(magnification);
					enemyElite1_eg->pos.x = 1000.0f;
					enemyElite1_eg->pos.y = 336.0f;
					enemyElite1_eg->isAlive = true;
					//敌人精英2例子重置
					enemyElite2_eg->Initalize(magnification);
					enemyElite2_eg->pos.x = 1000.0f;
					enemyElite2_eg->pos.y = 432.0f;
					enemyElite2_eg->isAlive = true;
					//敌人杂鱼1正式重置
					for (int i = 0; i < 10; i++) {
						enemyZako[i]->Initalize(magnification);
					}
					//敌人杂鱼2正式重置
					for (int i = 0; i < 10; i++) {
						enemyZako2[i]->Initalize(magnification);
					}
					//敌人精英1正式重置
					enemyElite1->Initalize(magnification);
					//敌人精英2正式重置
					enemyElite2->Initalize(magnification);
					//敌人杂鱼1正式子弹重置
					for (int i = 0; i < 10; i++) {
						bulletZako[i].pos.x = -128.0f;
						bulletZako[i].pos.y = -128.0f;
						bulletZako[i].width = 64.0f;
						bulletZako[i].height = 64.0f;
						bulletZako[i].speed = 10.0f;
						bulletZako[i].isShoot = false;
						bulletZako[i].isHoly = true;
					}
					//敌人精英2正式子弹重置
					for (int i = 0; i < 10; i++) {
						bulletElite2[i]->Initalize();
					}
					bulletElite2Num = 10;
					//boss1重置
					enemyBoss1->Initalize();
					//boss2重置
					enemyBoss2->Initalize();
					//boss1子弹重置
					for (int i = 0; i < 5; i++) {
						bulletB111[i]->Initalize();
						bulletB112[i]->Initalize();
						bulletB113[i]->Initalize();
					}
					for (int i = 0; i < 10; i++) {
						bulletB121[i]->Initalize();
						bulletB122[i]->Initalize();
						bossSaveVectorX1[i] = 0;
						bossSaveVectorY1[i] = 0;
						bossSaveVectorX2[i] = 0;
						bossSaveVectorY1[i] = 0;
					}
					//boss2子弹重置
					bulletBlackHole->Initalize();
					thetaBB2 = 0;
					lengthBulletBH = 0;
					for (int i = 0; i < 5; i++) {
						bulletB221[i]->Initalize();
						bulletB222[i]->Initalize();
						bulletB223[i]->Initalize();
					}
					bulletLaser->Initalize();
					//教程重置
					isTutoMove = false;
					nowFrameTuto = 0.0f;
					tutoPosX = 0.0f;
#pragma endregion
					if(titleTimer == 0){
						isTitleStart = true;
					}
				}
				//anime
				if (isTitleStart == true) {
					if (Novice::IsPlayingAudio(voiceHandle_titleLoading) == 0 || voiceHandle_titleLoading == -1) {
						voiceHandle_titleLoading = Novice::PlayAudio(SOUND_titleLoading, false, 0.7f);
					}
					countTitle++;
					titleRatio += 0.02f;
					titleX -= 13;
					titleY -= 8;
				}
				if (keys[DIK_M]) {
					isTitleStart = false;
					titleRatio = 1;
					titleX = 0;
					titleY = 0;
				}
				if (countTitle >= 150) {
					isTitleStart = false;
					titlePlayerMoving = true;
				}
				if (titlePlayerMoving == true) {
					isTitleStart = false;
					titlePlayerRatio -= 0.04f;
					if (titlePlayerY < 340) {
						titlePlayerX += 13;
						titlePlayerY += 2;
					}
				}
				if (titlePlayerRatio <= 1) {
					titlePlayerRatio = 1;
					titlePlayerMoving = false;
					titlePlayerBack = true;
				}
				if (titlePlayerBack == true) {
					titlePlayerX -= 10;
				}
				if (titlePlayerX <= 100 && titlePlayerBack == true) {
					titleTimer = 20;
					scene = TUTORIAL;
				}
			}
#pragma endregion
#pragma region TUTORIAL界面
			if (scene == TUTORIAL) {
				Novice::StopAudio(voiceHandle_title);
				Novice::StopAudio(voiceHandle_titleLoading);
				if (Novice::IsPlayingAudio(voiceHandle_stage) == 0 || voiceHandle_stage == -1) {
					voiceHandle_stage = Novice::PlayAudio(SOUND_stage, true, 1.0f);
				}
#pragma region title anime reset
				titleX = 0;
				titleY = 0;
				countTitle = 0;
				isTitleStart = false;
				titleRatio = 1;

				titlePlayerX = -320;
				titlePlayerY = 128;
				titlePlayerRatio = 5;
				titlePlayerMoving = false;
				titlePlayerBack = false;
#pragma endregion
				if (!enemyZako_eg->isAlive && !enemyZako2_eg->isAlive && !enemyElite1_eg->isAlive && !enemyElite2_eg->isAlive) {
					for (int i = 0; i < 3; i++) {
						inputArr[i] = 0;
					}
					bulletTimer = 300.0f;
					isbulletTimerOn = false;
					isTutoMove = true;
				}
				if (isTutoMove == true) {
					if (nowFrameTuto >= endFrameTuto) {
						nowFrameTuto = endFrameTuto;
						isTutoMove = false;
						nowFrameTuto = 0.0f;
						tutoPosX = 0.0f;
						player1->Initalize();
						starPosX = 1100.0f;
						starMagnification = 0.01f;
						scene = STAGE1;
					}
					else {
						nowFrameTuto++;
						xFrameTuto = nowFrameTuto / endFrameTuto;
						tutoPosX = easeInSine(xFrameTuto) * 1300.f;
					}
				}
			}
#pragma endregion
#pragma region STAGE1界面

			if (scene == STAGE1) {
				Novice::StopAudio(voiceHandle_title);
				Novice::StopAudio(voiceHandle_titleLoading);
				if (Novice::IsPlayingAudio(voiceHandle_stage) == 0 || voiceHandle_stage == -1) {
					voiceHandle_stage = Novice::PlayAudio(SOUND_stage, true, 1.0f);
				}
				magnification = 1.0f;

				if (starPosX <= 600) {
					starPosX = 1100.0f;
					starMagnification = 0.01f;
					//射弹编辑重置
					for (int i = 0; i < 3; i++) {
						inputArr[i] = 0;
					}
					bulletTimer = 300.0f;
					isbulletTimerOn = false;
					//自己子弹重置
					for (int i = 0; i < 32; i++) {
						bullet0[i]->Initalize();
					}
					for (int i = 0; i < 5; i++) {
						bullet1[i]->Initalize();
					}

					for (int i = 0; i < 12; i++) {
						bullet2[i]->Initalize();
					}
					isBullet2On = false;
					reloadTimer2 = 45;
					for (int i = 0; i < 32; i++) {
						bullet3[i]->Initalize();
						theta3[i] = 0;
					}

					for (int i = 0; i < 2; i++) {
						bullet4[i]->Initalize();
						lengthBullet4[i] = 1300.0f;
					}
					for (int i = 0; i < 32; i++) {
						bullet5[i]->Initalize();
					}
					scene = BOSS1;
				}
				//敌人 zako1

				for (int i = 0; i < 10; i++) {
					enemyZako[i]->Respawn(magnification);//自机复活
					enemyZako[i]->Move();//自机移动
					//死亡后发射子弹
					if (enemyZako[i]->isAlive == false) {
						if (!bulletZako[i].isShoot && enemyZako[i]->bulletNum > 0 && enemyZako[i]->pos.x < 1216 && enemyZako[i]->pos.y>70 && enemyZako[i]->pos.y < 586) {
							bulletZako[i].isHoly = rand() % 2;
							bulletZako[i].pos.x = enemyZako[i]->pos.x;
							bulletZako[i].pos.y = enemyZako[i]->pos.y + enemyZako[i]->height / 2.0f;
							bulletZako[i].isShoot = true;
							enemyZako[i]->bulletNum = 0;
						}
					}
					if (bulletZako[i].isShoot) {
						bulletZako[i].pos.x -= bulletZako[i].speed;
						// 画面外に出たら発射フラグをFalseに変更する
						if (bulletZako[i].pos.x <= 0.0f - bulletZako[i].width && enemyZako[i]->isAlive == true) {
							bulletZako[i].isShoot = false;
						}
					}
					//あたり
					if (player1->isAlive) {
						if (bulletZako[i].isShoot) {
							if (bulletZako[i].isHoly == player1->isAttributeHoly) {
								if (IsSpriteCollision(bulletZako[i].pos.x, bulletZako[i].width, bulletZako[i].pos.y, bulletZako[i].height,
									player1->pos.x, player1->width, player1->pos.y, player1->height)) {
									bulletZako[i].isShoot = false;
									if (player1->hp < player1->hpMax && player1->hp > 0) {
										if (player1->hpMax - player1->hp < 0.5f) {
											player1->hp += (player1->hpMax - player1->hp);
										}
										else {
											player1->hp += 0.5;
										}
									}
									else {
										player1->hp += 0;
									}
								}
							}
							if (bulletZako[i].isHoly != player1->isAttributeHoly) {
								if (IsSpriteCollision(bulletZako[i].pos.x, bulletZako[i].width, bulletZako[i].pos.y, bulletZako[i].height,
									player1->pos.x, player1->width, player1->pos.y, player1->height)) {
									bulletZako[i].isShoot = false;
									if (player1->hp > 0) {
										player1->hp -= 2;
										isPlayerShake = true;
									}
									else {
										player1->hp = 0;
									}
								}
							}
						}
					}
				}
				//zako2

				for (int i = 0; i < 10; i++) {
					enemyZako2[i]->Respawn(player1, magnification);
					enemyZako2[i]->Move();

					if (enemyZako2[i]->isAlive) {
						if (IsSpriteCollision(enemyZako2[i]->pos.x, enemyZako2[i]->width, enemyZako2[i]->pos.y, enemyZako2[i]->height,
							player1->pos.x, player1->width, player1->pos.y, player1->height)) {
							enemyZako2[i]->isAlive = false;
							if (player1->hp > 0) {
								player1->hp -= 2;
								isPlayerShake = true;
							}
							else {
								player1->hp = 0;
							}
						}
					}
				}
				//elite 1
				enemyElite1->Respawn(magnification);
				enemyElite1->Move(player1);
				if (enemyElite1->isAlive) {
					if (IsSpriteCollision(enemyElite1->pos.x, enemyElite1->width, enemyElite1->pos.y, enemyElite1->height,
						player1->pos.x, player1->width, player1->pos.y, player1->height)) {
						enemyElite1->isAlive = false;
						if (player1->hp > 0) {
							player1->hp -= 5;
							isPlayerShake = true;
						}
						else {
							player1->hp = 0;
						}
					}
				}
			}
#pragma endregion
#pragma region BOSS1界面
			if (scene == BOSS1) {
				Novice::StopAudio(voiceHandle_title);
				Novice::StopAudio(voiceHandle_titleLoading);
				if (Novice::IsPlayingAudio(voiceHandle_stage) == 0 || voiceHandle_stage == -1) {
					voiceHandle_stage = Novice::PlayAudio(SOUND_stage, true, 1.0f);
				}
				enemyBoss1->Respawn();
				enemyBoss1->Move();
				if (enemyBoss1->isBossDefeated) {
					isBoss1DeathAnimeOn = true;
					scene = LOADING;
				}
				if (enemyBoss1->isBossDefeated == false) {
					if (enemyBoss1->isAlive == true) {
						if (enemyBoss1->hp <= enemyBoss1->hpMax * 1 / 2.0f) {
							enemyBoss1->bossPattern2 = true;
						}
#pragma region shotgun
						if (enemyBoss1->bossPattern1) {
							if (enemyBoss1->shootCoolTimeSG1 > 0) {
								enemyBoss1->shootCoolTimeSG1--;
							}
							else {
								enemyBoss1->shootCoolTimeSG1 = 60;
							}
							if (enemyBoss1->shootCoolTimeSG1 <= 0) {
								if (!bulletB111[0]->isShoot && !bulletB111[1]->isShoot && !bulletB111[2]->isShoot &&
									!bulletB111[3]->isShoot && !bulletB111[4]->isShoot) {
									for (int i = 0; i < 5; i++) {
										bulletB111[i]->isShoot = true;
										bulletB111[i]->isHoly = rand() % 2;
										bulletB111[i]->pos.x = enemyBoss1->pos.x + enemyBoss1->width / 2.0f;
										bulletB111[i]->pos.y = enemyBoss1->pos.y + enemyBoss1->height / 2.0f - 16;
									}
								}

							}
							if (enemyBoss1->shootCoolTimeSG2 > 0) {
								enemyBoss1->shootCoolTimeSG2--;
							}
							else {
								enemyBoss1->shootCoolTimeSG2 = 30;
							}
							if (enemyBoss1->shootCoolTimeSG2 <= 0) {
								if (!bulletB112[0]->isShoot && !bulletB112[1]->isShoot && !bulletB112[2]->isShoot &&
									!bulletB112[3]->isShoot && !bulletB112[4]->isShoot) {
									for (int i = 0; i < 5; i++) {
										bulletB112[i]->isShoot = true;
										bulletB112[i]->isHoly = rand() % 2;
										bulletB112[i]->pos.x = enemyBoss1->pos.x + enemyBoss1->width / 2.0f;
										bulletB112[i]->pos.y = enemyBoss1->pos.y + enemyBoss1->height / 2.0f - 16;
									}
								}

							}
							if (enemyBoss1->shootCoolTimeSG3 > 0) {
								enemyBoss1->shootCoolTimeSG3--;
							}
							else {
								enemyBoss1->shootCoolTimeSG3 = 10;
							}
							if (enemyBoss1->shootCoolTimeSG3 <= 0) {
								if (!bulletB113[0]->isShoot && !bulletB113[1]->isShoot && !bulletB113[2]->isShoot &&
									!bulletB113[3]->isShoot && !bulletB113[4]->isShoot) {
									for (int i = 0; i < 5; i++) {
										bulletB113[i]->isShoot = true;
										bulletB113[i]->isHoly = rand() % 2;
										bulletB113[i]->pos.x = enemyBoss1->pos.x + enemyBoss1->width / 2.0f;
										bulletB113[i]->pos.y = enemyBoss1->pos.y + enemyBoss1->height / 2.0f - 16;
									}
								}
							}


						}
#pragma endregion
#pragma region follow
						if (enemyBoss1->bossPattern2) {
							if (enemyBoss1->shootCoolTimeFL1 > 0) {
								enemyBoss1->shootCoolTimeFL1--;
							}
							else {
								enemyBoss1->shootCoolTimeFL1 = 30;
							}
							if (enemyBoss1->shootCoolTimeFL1 <= 0) {
								for (int i = 0; i < 10; i++) {
									if (!bulletB121[i]->isShoot) {
										bulletB121[i]->pos.x = enemyBoss1->pos.x + enemyBoss1->width / 2.0f - 16;
										bulletB121[i]->pos.y = enemyBoss1->pos.y + enemyBoss1->height / 4.0f - 16;
										bulletB121[i]->isHoly = rand() % 2;
										bulletB121[i]->isShoot = true;
										break;
									}
								}
							}
							for (int i = 0; i < 10; i++) {
								if (bulletB121[i]->isShoot) {
									bulletB121[i]->vector.x = player1->pos.x - bulletB121[i]->pos.x;
									bulletB121[i]->vector.y = player1->pos.y - bulletB121[i]->pos.y;
									bulletB121[i]->length = sqrtf(bulletB121[i]->vector.x * bulletB121[i]->vector.x + bulletB121[i]->vector.y * bulletB121[i]->vector.y);//距離をレンジに
									if (bulletB121[i]->length != 0 && bulletB121[i]->length > 500 && bulletB121[i]->pos.x > player1->pos.x) {//レンジがゼロ以外の時//追尾を短くしたければレンジを長く、長くしたかったら短く
										//条件で500より大きいときを足したら、500いかになった時止まった
										bulletB121[i]->vectorNew.x = bulletB121[i]->vector.x / bulletB121[i]->length;//ヴェロとレンジを使う
										bulletB121[i]->vectorNew.y = bulletB121[i]->vector.y / bulletB121[i]->length;

										bossSaveVectorX1[i] = bulletB121[i]->vectorNew.x;//コレでとりあえずある程度の範囲内になったらロック解除になった
										bossSaveVectorY1[i] = bulletB121[i]->vectorNew.y;

									}
									else {
										bulletB121[i]->vectorNew.x = 0;
										bulletB121[i]->vectorNew.y = 0;
									}
								}

							}
							if (enemyBoss1->shootCoolTimeFL2 > 0) {
								enemyBoss1->shootCoolTimeFL2--;
							}
							else {
								enemyBoss1->shootCoolTimeFL2 = 30;
							}
							if (enemyBoss1->shootCoolTimeFL2 <= 0) {
								for (int i = 0; i < 10; i++) {
									if (!bulletB122[i]->isShoot) {
										bulletB122[i]->pos.x = enemyBoss1->pos.x + enemyBoss1->width / 2.0f - 16;
										bulletB122[i]->pos.y = enemyBoss1->pos.y + enemyBoss1->height * 3 / 4.0f - 16;
										bulletB122[i]->isHoly = rand() % 2;
										bulletB122[i]->isShoot = true;
										break;
									}

								}
							}
							for (int i = 0; i < 10; i++) {
								if (bulletB122[i]->isShoot) {
									bulletB122[i]->vector.x = player1->pos.x - bulletB122[i]->pos.x;
									bulletB122[i]->vector.y = player1->pos.y - bulletB122[i]->pos.y;
									bulletB122[i]->length = sqrtf(bulletB122[i]->vector.x * bulletB122[i]->vector.x + bulletB122[i]->vector.y * bulletB122[i]->vector.y);//距離をレンジに 
									if (bulletB122[i]->length != 0 && bulletB122[i]->length > 500 && bulletB122[i]->pos.x > player1->pos.x) {//レンジがゼロ以外の時//追尾を短くしたければレンジを長く、長くしたかったら短く 
										//条件で500より大きいときを足したら、500いかになった時止まった 
										bulletB122[i]->vectorNew.x = bulletB122[i]->vector.x / bulletB122[i]->length;//ヴェロとレンジを使う 
										bulletB122[i]->vectorNew.y = bulletB122[i]->vector.y / bulletB122[i]->length;
										bossSaveVectorX2[i] = bulletB122[i]->vectorNew.x;//コレでとりあえずある程度の範囲内になったらロック解除になった 
										bossSaveVectorY2[i] = bulletB122[i]->vectorNew.y;
									}
									else {
										bulletB122[i]->vectorNew.x = 0;
										bulletB122[i]->vectorNew.y = 0;
									}
								}

							}
						}
#pragma endregion

					}
				}
				// shotgun弾道計算
				for (int i = 0; i < 5; i++) {
					bulletB111[i]->Move(thetaB22[i]);
					bulletB112[i]->Move(thetaB22[i]);
					bulletB113[i]->Move(thetaB22[i]);
				}
				//shotgunあたり
				for (int i = 0; i < 5; i++) {
					if (bulletB111[i]->isShoot) {
						if (bulletB111[i]->isHoly == player1->isAttributeHoly) {
							if (IsSpriteCollision(bulletB111[i]->pos.x, bulletB111[i]->width, bulletB111[i]->pos.y, bulletB111[i]->height,
								player1->pos.x, player1->width, player1->pos.y, player1->height)) {
								bulletB111[i]->isShoot = false;
								if (player1->hp < player1->hpMax && player1->hp > 0) {
									player1->hp += 0.5;
								}
								else {
									player1->hp += 0;
								}
							}
						}
						if (bulletB111[i]->isHoly != player1->isAttributeHoly) {
							if (IsSpriteCollision(bulletB111[i]->pos.x, bulletB111[i]->width, bulletB111[i]->pos.y, bulletB111[i]->height,
								player1->pos.x, player1->width, player1->pos.y, player1->height)) {
								bulletB111[i]->isShoot = false;
								if (player1->hp > 0) {
									player1->hp -= 2;
									isPlayerShake = true;
								}
								else {
									player1->hp = 0;
								}
							}
						}
					}
					if (bulletB112[i]->isShoot) {
						if (bulletB112[i]->isHoly == player1->isAttributeHoly) {
							if (IsSpriteCollision(bulletB112[i]->pos.x, bulletB112[i]->width, bulletB112[i]->pos.y, bulletB112[i]->height,
								player1->pos.x, player1->width, player1->pos.y, player1->height)) {
								bulletB112[i]->isShoot = false;
								if (player1->hp < player1->hpMax && player1->hp > 0) {
									player1->hp += 0.5;
								}
								else {
									player1->hp += 0;
								}
							}
						}
						if (bulletB112[i]->isHoly != player1->isAttributeHoly) {
							if (IsSpriteCollision(bulletB112[i]->pos.x, bulletB112[i]->width, bulletB112[i]->pos.y, bulletB112[i]->height,
								player1->pos.x, player1->width, player1->pos.y, player1->height)) {
								bulletB112[i]->isShoot = false;
								if (player1->hp > 0) {
									player1->hp -= 2;
									isPlayerShake = true;
								}
								else {
									player1->hp = 0;
								}
							}
						}
					}
					if (bulletB113[i]->isShoot) {
						if (bulletB113[i]->isHoly == player1->isAttributeHoly) {
							if (IsSpriteCollision(bulletB113[i]->pos.x, bulletB113[i]->width, bulletB113[i]->pos.y, bulletB113[i]->height,
								player1->pos.x, player1->width, player1->pos.y, player1->height)) {
								bulletB113[i]->isShoot = false;
								if (player1->hp < player1->hpMax && player1->hp > 0) {
									player1->hp += 0.5;
								}
								else {
									player1->hp += 0;
								}
							}
						}
						if (bulletB113[i]->isHoly != player1->isAttributeHoly) {
							if (IsSpriteCollision(bulletB113[i]->pos.x, bulletB113[i]->width, bulletB113[i]->pos.y, bulletB113[i]->height,
								player1->pos.x, player1->width, player1->pos.y, player1->height)) {
								bulletB113[i]->isShoot = false;
								if (player1->hp > 0) {
									player1->hp -= 2;
									isPlayerShake = true;
								}
								else {
									player1->hp = 0;
								}
							}
						}
					}
				}
				//follow弹道
				for (int i = 0; i < 10; i++) {
					if (bulletB121[i]->isShoot == true) {
						bulletB121[i]->pos.x += bossSaveVectorX1[i] * bulletB121[i]->speed;//動く条件　ヴェロニュウとムーブヴェロをかけたとき
						bulletB121[i]->pos.y += bossSaveVectorY1[i] * bulletB121[i]->speed;//ムーヴベロは多分動かしたいスピード
						//セーブヴェロに変えた
					}
					if (bulletB121[i]->pos.x < 0 - 32 || bulletB121[i]->pos.y < 70 - 32 || bulletB121[i]->pos.y > 720 - 70 - 32) {
						bulletB121[i]->isShoot = false;
					}
					if (bulletB122[i]->isShoot == true) {
						bulletB122[i]->pos.x += bossSaveVectorX2[i] * bulletB122[i]->speed;//動く条件　ヴェロニュウとムーブヴェロをかけたとき
						bulletB122[i]->pos.y += bossSaveVectorY2[i] * bulletB122[i]->speed;//ムーヴベロは多分動かしたいスピード
						//セーブヴェロに変えた
					}
					if (bulletB122[i]->pos.x < 0 - 32 || bulletB122[i]->pos.y < 70 - 32 || bulletB122[i]->pos.y > 720 - 70 - 32) {
						bulletB122[i]->isShoot = false;
					}
				}
				//follow atari
				for (int i = 0; i < 10; i++) {
					if (bulletB121[i]->isShoot) {
						if (bulletB121[i]->isHoly == player1->isAttributeHoly) {
							if (IsSpriteCollision(bulletB121[i]->pos.x, bulletB121[i]->width, bulletB121[i]->pos.y, bulletB121[i]->height,
								player1->pos.x, player1->width, player1->pos.y, player1->height)) {
								bulletB121[i]->isShoot = false;
								if (player1->hp < player1->hpMax && player1->hp > 0) {
									player1->hp += 0.5;
								}
								else {
									player1->hp += 0;
								}
							}
						}
						if (bulletB121[i]->isHoly != player1->isAttributeHoly) {
							if (IsSpriteCollision(bulletB121[i]->pos.x, bulletB121[i]->width, bulletB121[i]->pos.y, bulletB121[i]->height,
								player1->pos.x, player1->width, player1->pos.y, player1->height)) {
								bulletB121[i]->isShoot = false;
								if (player1->hp > 0) {
									player1->hp -= 2;
									isPlayerShake = true;
								}
								else {
									player1->hp = 0;
								}
							}
						}
					}
					if (bulletB122[i]->isShoot) {
						if (bulletB122[i]->isHoly == player1->isAttributeHoly) {
							if (IsSpriteCollision(bulletB122[i]->pos.x, bulletB122[i]->width, bulletB122[i]->pos.y, bulletB122[i]->height,
								player1->pos.x, player1->width, player1->pos.y, player1->height)) {
								bulletB122[i]->isShoot = false;
								if (player1->hp < player1->hpMax && player1->hp > 0) {
									player1->hp += 0.5;
								}
								else {
									player1->hp += 0;
								}
							}
						}
						if (bulletB122[i]->isHoly != player1->isAttributeHoly) {
							if (IsSpriteCollision(bulletB122[i]->pos.x, bulletB122[i]->width, bulletB122[i]->pos.y, bulletB122[i]->height,
								player1->pos.x, player1->width, player1->pos.y, player1->height)) {
								bulletB122[i]->isShoot = false;
								if (player1->hp > 0) {
									player1->hp -= 2;
									isPlayerShake = true;
								}
								else {
									player1->hp = 0;
								}
							}
						}
					}
				}
			}
#pragma endregion
#pragma region LOADING界面
			if (scene == LOADING) {
				Novice::StopAudio(voiceHandle_title);
				Novice::StopAudio(voiceHandle_stage);
				if (Novice::IsPlayingAudio(voiceHandle_titleLoading) == 0 || voiceHandle_titleLoading == -1) {
					voiceHandle_titleLoading = Novice::PlayAudio(SOUND_titleLoading, false, 0.7f);
				}
				starPosX = 1100.0f;
				starMagnification = 0.01f;
				//初期化
				player1->pos.x = 100.0f;
				player1->pos.y = 340.0f;
				//射弹编辑重置
				for (int i = 0; i < 3; i++) {
					inputArr[i] = 0;
				}
				bulletTimer = 300.0f;
				isbulletTimerOn = false;
				//自己子弹重置
				for (int i = 0; i < 32; i++) {
					bullet0[i]->Initalize();
				}
				for (int i = 0; i < 5; i++) {
					bullet1[i]->Initalize();
				}

				for (int i = 0; i < 12; i++) {
					bullet2[i]->Initalize();
				}
				isBullet2On = false;
				reloadTimer2 = 45;
				for (int i = 0; i < 32; i++) {
					bullet3[i]->Initalize();
					theta3[i] = 0;
				}

				for (int i = 0; i < 2; i++) {
					bullet4[i]->Initalize();
					lengthBullet4[i] = 1300.0f;
				}
				for (int i = 0; i < 32; i++) {
					bullet5[i]->Initalize();
				}
				//敌人杂鱼1正式重置
				for (int i = 0; i < 10; i++) {
					enemyZako[i]->Initalize(magnification);
				}
				//敌人杂鱼2正式重置
				for (int i = 0; i < 10; i++) {
					enemyZako2[i]->Initalize(magnification);
				}
				//敌人精英1正式重置
				enemyElite1->Initalize(magnification);
				//敌人精英2正式重置
				enemyElite2->Initalize(magnification);
				//敌人杂鱼1正式子弹重置
				for (int i = 0; i < 10; i++) {
					bulletZako[i].pos.x = -128.0f;
					bulletZako[i].pos.y = -128.0f;
					bulletZako[i].width = 64.0f;
					bulletZako[i].height = 64.0f;
					bulletZako[i].speed = 10.0f;
					bulletZako[i].isShoot = false;
					bulletZako[i].isHoly = true;
				}
				//敌人精英2正式子弹重置
				for (int i = 0; i < 10; i++) {
					bulletElite2[i]->Initalize();
				}
				bulletElite2Num = 10;
				//动画开始
				if (isBoss1DeathAnimeOn) {
					b1DAnimeCount++;
					if (b1DAnimeCount % 30 == 0) {
						b1DAnimeStep++;
					}

					if (b1DAnimeStep >= 4) {
						b1DAnimeStep = 4;
						isBoss1DeathAnimeOn = false;
					}
				}
				if (!isBoss1DeathAnimeOn) {
					loadingPlayerX += 10;
					if (loadingPlayerX >= 1280) {
						loadingPlayerX = 100;
						isNextStage = true;
					}
					if (isNextStage) {
						b1DAnimeCount = 0;
						b1DAnimeStep = 0;
						isNextStage = false;
						scene = STAGE2;
					}
				}


			}
#pragma endregion
#pragma region STAGE2界面
			if (scene == STAGE2) {
				Novice::StopAudio(voiceHandle_title);
				Novice::StopAudio(voiceHandle_titleLoading);
				if (Novice::IsPlayingAudio(voiceHandle_stage) == 0 || voiceHandle_stage == -1) {
					voiceHandle_stage = Novice::PlayAudio(SOUND_stage, true, 1.0f);
				}
				magnification = 2.0f;
				if (starPosX <= 600) {
					starPosX = 1100.0f;
					starMagnification = 0.01f;
					//射弹编辑重置
					for (int i = 0; i < 3; i++) {
						inputArr[i] = 0;
					}
					bulletTimer = 300.0f;
					isbulletTimerOn = false;
					//自己子弹重置
					for (int i = 0; i < 32; i++) {
						bullet0[i]->Initalize();
					}
					for (int i = 0; i < 5; i++) {
						bullet1[i]->Initalize();
					}

					for (int i = 0; i < 12; i++) {
						bullet2[i]->Initalize();
					}
					isBullet2On = false;
					reloadTimer2 = 45;
					for (int i = 0; i < 32; i++) {
						bullet3[i]->Initalize();
						theta3[i] = 0;
					}

					for (int i = 0; i < 2; i++) {
						bullet4[i]->Initalize();
						lengthBullet4[i] = 1300.0f;
					}
					for (int i = 0; i < 32; i++) {
						bullet5[i]->Initalize();
					}
					scene = BOSS2;
				}
				//敌人 zako1

				for (int i = 0; i < 10; i++) {
					enemyZako[i]->Respawn(magnification);//自机复活
					enemyZako[i]->Move();//自机移动
					//死亡后发射子弹
					if (enemyZako[i]->isAlive == false) {
						if (!bulletZako[i].isShoot && enemyZako[i]->bulletNum > 0 && enemyZako[i]->pos.x < 1216 && enemyZako[i]->pos.y>70 && enemyZako[i]->pos.y < 586) {
							bulletZako[i].isHoly = rand() % 2;
							bulletZako[i].pos.x = enemyZako[i]->pos.x;
							bulletZako[i].pos.y = enemyZako[i]->pos.y + enemyZako[i]->height / 2.0f;
							bulletZako[i].isShoot = true;
							enemyZako[i]->bulletNum = 0;
						}
					}
					if (bulletZako[i].isShoot) {
						bulletZako[i].pos.x -= bulletZako[i].speed;
						// 画面外に出たら発射フラグをFalseに変更する
						if (bulletZako[i].pos.x <= 0.0f - bulletZako[i].width && enemyZako[i]->isAlive == true) {
							bulletZako[i].isShoot = false;
						}
					}
					//あたり
					if (player1->isAlive) {
						if (bulletZako[i].isShoot) {
							if (bulletZako[i].isHoly == player1->isAttributeHoly) {
								if (IsSpriteCollision(bulletZako[i].pos.x, bulletZako[i].width, bulletZako[i].pos.y, bulletZako[i].height,
									player1->pos.x, player1->width, player1->pos.y, player1->height)) {
									bulletZako[i].isShoot = false;
									if (player1->hp < player1->hpMax && player1->hp > 0) {
										if (player1->hpMax - player1->hp < 0.5f) {
											player1->hp += (player1->hpMax - player1->hp);
										}
										else {
											player1->hp += 0.5;
										}
									}
									else {
										player1->hp += 0;
									}
								}
							}
							if (bulletZako[i].isHoly != player1->isAttributeHoly) {
								if (IsSpriteCollision(bulletZako[i].pos.x, bulletZako[i].width, bulletZako[i].pos.y, bulletZako[i].height,
									player1->pos.x, player1->width, player1->pos.y, player1->height)) {
									bulletZako[i].isShoot = false;
									if (player1->hp > 0) {
										player1->hp -= 2;
										isPlayerShake = true;
									}
									else {
										player1->hp = 0;
									}
								}
							}
						}
					}
				}
				//zako2

				for (int i = 0; i < 10; i++) {
					enemyZako2[i]->Respawn(player1, magnification);
					enemyZako2[i]->Move();

					if (enemyZako2[i]->isAlive) {
						if (IsSpriteCollision(enemyZako2[i]->pos.x, enemyZako2[i]->width, enemyZako2[i]->pos.y, enemyZako2[i]->height,
							player1->pos.x, player1->width, player1->pos.y, player1->height)) {
							enemyZako2[i]->isAlive = false;
							if (player1->hp > 0) {
								player1->hp -= 3;
								isPlayerShake = true;
							}
							else {
								player1->hp = 0;
							}
						}
					}
				}
				//elite 1
				enemyElite1->Respawn(magnification);
				enemyElite1->Move(player1);
				if (enemyElite1->isAlive) {
					if (IsSpriteCollision(enemyElite1->pos.x, enemyElite1->width, enemyElite1->pos.y, enemyElite1->height,
						player1->pos.x, player1->width, player1->pos.y, player1->height)) {
						enemyElite1->isAlive = false;
						if (player1->hp > 0) {
							player1->hp -= 5;
							isPlayerShake = true;
						}
						else {
							player1->hp = 0;
						}
					}
				}
				//elite 2
				enemyElite2->Respawn(magnification);
				enemyElite2->Move();
				if (enemyElite2->isAlive) {
					if (enemyElite2->shootCoolTime > 0) {
						enemyElite2->shootCoolTime--;
					}
					else {
						enemyElite2->shootCoolTime = 180;
						bulletElite2Num = 10;
						enemyElite2->isShoot = true;
					}
					if (enemyElite2->isShoot) {
						enemyElite2->shootInterval++;
					}
					for (int i = 0; i < 10; i++) {
						if (enemyElite2->isShoot && enemyElite2->shootInterval > 5 && !bulletElite2[i]->isShoot && bulletElite2Num >= 0) {
							if (enemyElite2->pos.y >= 586 || enemyElite2->pos.y < 70) {
								enemyElite2->velocity.y *= -1;
							}
							enemyElite2->pos.y += enemyElite2->velocity.y;
							bulletElite2[i]->pos.x = enemyElite2->pos.x;
							bulletElite2[i]->pos.y = enemyElite2->pos.y + enemyElite2->height / 4;
							bulletElite2[i]->isHoly = rand() % 2;
							bulletElite2[i]->isShoot = true;
							enemyElite2->shootInterval = 0;
							bulletElite2Num--;
							break;
						}
					}
				}
				if (bulletElite2Num <= 0) {
					enemyElite2->isShoot = false;
				}
				for (int i = 0; i < 10; i++) {
					if (bulletElite2[i]->isShoot) {
						bulletElite2[i]->pos.x -= +bulletElite2[i]->speed;
					}
					if (bulletElite2[i]->pos.x < -32) {
						bulletElite2[i]->isShoot = false;
					}
				}
				for (int i = 0; i < 10; i++) {
					if (bulletElite2[i]->isShoot) {
						if (IsSpriteCollision(bulletElite2[i]->pos.x, bulletElite2[i]->width, bulletElite2[i]->pos.y, bulletElite2[i]->height,
							player1->pos.x, player1->width, player1->pos.y, player1->height)) {
							if (bulletElite2[i]->isHoly == player1->isAttributeHoly) {
								bulletElite2[i]->isShoot = false;
								if (player1->hp < player1->hpMax && player1->hp > 0) {
									if (player1->hpMax - player1->hp < 0.5f) {
										player1->hp += (player1->hpMax - player1->hp);
									}
									else {
										player1->hp += 0.5;
									}
								}
								else {
									player1->hp += 0;
								}
							}
							if (bulletElite2[i]->isHoly != player1->isAttributeHoly) {
								bulletElite2[i]->isShoot = false;
								if (player1->hp > 0) {
									player1->hp -= 2;
									isPlayerShake = true;
								}
								else {
									player1->hp = 0;
								}
							}
						}
					}
				}
			}
#pragma endregion
#pragma region BOSS2 界面
			if (scene == BOSS2) {
				Novice::StopAudio(voiceHandle_title);
				Novice::StopAudio(voiceHandle_titleLoading);
				if (Novice::IsPlayingAudio(voiceHandle_stage) == 0 || voiceHandle_stage == -1) {
					voiceHandle_stage = Novice::PlayAudio(SOUND_stage, true, 1.0f);
				}
				enemyBoss2->Respawn();
				enemyBoss2->Move();

				if (enemyBoss2->isBossDefeated == false) {
					if (enemyBoss2->isAlive == true) {
#pragma region BlackHole Module
						//BlackHole Module
						if (enemyBoss2->shootCoolTimeBH > 0) {
							enemyBoss2->shootCoolTimeBH--;
						}
						else {
							enemyBoss2->shootCoolTimeBH = 480;
						}

						if (enemyBoss2->shootCoolTimeBH <= 0) {
							if (!bulletBlackHole->isShoot) {
								bulletBlackHole->isShoot = true;
								voiceHandle_blackHole = Novice::PlayAudio(SOUND_blackHole, false, 1.5f);
								rectCenterBB2.x = player1->pos.x + player1->width / 2.0f + rand() % 101 - 50;
								rectCenterBB2.y = player1->pos.y + player1->height / 2.0f + rand() % 101 - 50;
							}
						}
						if (rectCenterBB2.y <= 100 || rectCenterBB2.y >= 620) {
							Novice::StopAudio(voiceHandle_blackHole);
							bulletBlackHole->isShoot = false;
						}
						if (bulletBlackHole->isShoot) {
							if (bulletBlackHole->timer > 0) {
								bulletBlackHole->timer--;
							}
							else {
								bulletBlackHole->timer = 180;
								bulletBlackHole->isShoot = false;
							}
							thetaBB2 -= 1.0f / 30 * (float)M_PI;
							if (thetaBB2 < -2 * (float)M_PI) {
								thetaBB2 = 0;
							}
							affineMatrixBB2 = MakeAffineMatrix(scaleBB2, thetaBB2, rectCenterBB2);
							worldLeftTopBB2 = Transform(leftTopBB2, affineMatrixBB2);
							worldRightTopBB2 = Transform(rightTopBB2, affineMatrixBB2);
							worldLeftBottomBB2 = Transform(leftBottomBB2, affineMatrixBB2);
							worldRightBottomBB2 = Transform(rightBottomBB2, affineMatrixBB2);
						}
						Vector2 vectorBH = {};
						Vector2 vectorBHNew = {};
						Vector2 vectorPlayer = { player1->pos.x + player1->width * 1.0f / 2 ,player1->pos.y + player1->height * 1.0f / 2 };
						vectorBH.x = rectCenterBB2.x - vectorPlayer.x;
						vectorBH.y = rectCenterBB2.y - vectorPlayer.y;
						lengthBulletBH = GetLength(rectCenterBB2, vectorPlayer);
						if (lengthBulletBH != 0) {
							vectorBHNew.x = vectorBH.x / lengthBulletBH;
							vectorBHNew.y = vectorBH.y / lengthBulletBH;
						}
						if (bulletBlackHole->isShoot) {
							if ((player1->pos.x + player1->width) >= (rectCenterBB2.x - 100) &&
								(player1->pos.x) <= (rectCenterBB2.x + 100) &&
								(player1->pos.y + player1->height) >= (rectCenterBB2.y - 100) &&
								(player1->pos.y) <= (rectCenterBB2.y + 100)) {
								player1->pos.x += vectorBHNew.x * player1->velocity.x * 3.0f / 4;
								player1->pos.y += vectorBHNew.y * player1->velocity.y * 3.0f / 4;
							}
						}
#pragma endregion
#pragma region Barrage
						if (enemyBoss2->shootCoolTimeBA > 0) {
							enemyBoss2->shootCoolTimeBA--;
						}
						else {
							enemyBoss2->shootCoolTimeBA = 60;
						}
						if (enemyBoss2->shootCoolTimeBA <= 0) {
							if (!bulletB221[0]->isShoot && !bulletB221[1]->isShoot && !bulletB221[2]->isShoot &&
								!bulletB221[3]->isShoot && !bulletB221[4]->isShoot) {
								for (int i = 0; i < 5; i++) {
									bulletB221[i]->isShoot = true;
									bulletB221[i]->isHoly = rand() % 2;
									bulletB221[i]->pos.x = enemyBoss2->pos.x + enemyBoss2->width * 1.0f / 2;
									bulletB221[i]->pos.y = enemyBoss2->pos.y + enemyBoss2->height / 4.0f;
								}
							}
							if (!bulletB222[0]->isShoot && !bulletB222[1]->isShoot && !bulletB222[2]->isShoot &&
								!bulletB222[3]->isShoot && !bulletB222[4]->isShoot) {
								for (int i = 0; i < 5; i++) {
									bulletB222[i]->isShoot = true;
									bulletB222[i]->isHoly = rand() % 2;
									bulletB222[i]->pos.x = enemyBoss2->pos.x + enemyBoss2->width * 1.0f / 2;
									bulletB222[i]->pos.y = enemyBoss2->pos.y + enemyBoss2->height / 2.0f;
								}
							}
							if (!bulletB223[0]->isShoot && !bulletB223[1]->isShoot && !bulletB223[2]->isShoot &&
								!bulletB223[3]->isShoot && !bulletB223[4]->isShoot) {
								for (int i = 0; i < 5; i++) {
									bulletB223[i]->isShoot = true;
									bulletB223[i]->isHoly = rand() % 2;
									bulletB223[i]->pos.x = enemyBoss2->pos.x + enemyBoss2->width * 1.0f / 2;
									bulletB223[i]->pos.y = enemyBoss2->pos.y + enemyBoss2->height * 3.0f / 4;
								}
							}
						}
						// 弾道計算
						for (int i = 0; i < 5; i++) {
							bulletB221[i]->Move(thetaB22[i]);
							bulletB222[i]->Move(thetaB22[i]);
							bulletB223[i]->Move(thetaB22[i]);
						}

						//あたり
						for (int i = 0; i < 5; i++) {
							if (bulletB221[i]->isShoot) {
								if (bulletB221[i]->isHoly == player1->isAttributeHoly) {
									if (IsSpriteCollision(bulletB221[i]->pos.x, bulletB221[i]->width, bulletB221[i]->pos.y, bulletB221[i]->height,
										player1->pos.x, player1->width, player1->pos.y, player1->height)) {
										bulletB221[i]->isShoot = false;
										if (player1->hp < player1->hpMax && player1->hp > 0) {
											player1->hp += 0.5;
										}
										else {
											player1->hp += 0;
										}
									}
								}
								if (bulletB221[i]->isHoly != player1->isAttributeHoly) {
									if (IsSpriteCollision(bulletB221[i]->pos.x, bulletB221[i]->width, bulletB221[i]->pos.y, bulletB221[i]->height,
										player1->pos.x, player1->width, player1->pos.y, player1->height)) {
										bulletB221[i]->isShoot = false;
										if (player1->hp > 0) {
											player1->hp -= 2;
											isPlayerShake = true;
										}
										else {
											player1->hp = 0;
										}
									}
								}
							}
							if (bulletB222[i]->isShoot) {
								if (bulletB222[i]->isHoly == player1->isAttributeHoly) {
									if (IsSpriteCollision(bulletB222[i]->pos.x, bulletB222[i]->width, bulletB222[i]->pos.y, bulletB222[i]->height,
										player1->pos.x, player1->width, player1->pos.y, player1->height)) {
										bulletB222[i]->isShoot = false;
										if (player1->hp < player1->hpMax && player1->hp > 0) {
											player1->hp += 0.5;
										}
										else {
											player1->hp += 0;
										}
									}
								}
								if (bulletB222[i]->isHoly != player1->isAttributeHoly) {
									if (IsSpriteCollision(bulletB222[i]->pos.x, bulletB222[i]->width, bulletB222[i]->pos.y, bulletB222[i]->height,
										player1->pos.x, player1->width, player1->pos.y, player1->height)) {
										bulletB222[i]->isShoot = false;
										if (player1->hp > 0) {
											player1->hp -= 2;
											isPlayerShake = true;
										}
										else {
											player1->hp = 0;
										}
									}
								}
							}
							if (bulletB223[i]->isShoot) {
								if (bulletB223[i]->isHoly == player1->isAttributeHoly) {
									if (IsSpriteCollision(bulletB223[i]->pos.x, bulletB223[i]->width, bulletB223[i]->pos.y, bulletB223[i]->height,
										player1->pos.x, player1->width, player1->pos.y, player1->height)) {
										bulletB223[i]->isShoot = false;
										if (player1->hp < player1->hpMax && player1->hp > 0) {
											player1->hp += 0.5;
										}
										else {
											player1->hp += 0;
										}
									}
								}
								if (bulletB223[i]->isHoly != player1->isAttributeHoly) {
									if (IsSpriteCollision(bulletB223[i]->pos.x, bulletB223[i]->width, bulletB223[i]->pos.y, bulletB223[i]->height,
										player1->pos.x, player1->width, player1->pos.y, player1->height)) {
										bulletB223[i]->isShoot = false;
										if (player1->hp > 0) {
											player1->hp -= 2;
											isPlayerShake = true;
										}
										else {
											player1->hp = 0;
										}
									}
								}
							}
						}

#pragma endregion
#pragma region missile
						//elite 1
						enemyElite1->Respawn(magnification);
						enemyElite1->Move(player1);
						if (enemyElite1->isAlive) {
							if (IsSpriteCollision(enemyElite1->pos.x, enemyElite1->width, enemyElite1->pos.y, enemyElite1->height,
								player1->pos.x, player1->width, player1->pos.y, player1->height)) {
								enemyElite1->isAlive = false;
								if (player1->hp > 0) {
									player1->hp -= 5;
									isPlayerShake = true;
								}
								else {
									player1->hp = 0;
								}
							}
						}

#pragma endregion
#pragma region Laser
						if (enemyBoss2->shootCoolTimeLS > 0) {
							enemyBoss2->shootCoolTimeLS--;
						}
						else {
							enemyBoss2->shootCoolTimeLS = 300;
						}
						if (enemyBoss2->shootCoolTimeLS <= 0) {
							if (!bulletLaser->isShoot) {
								voiceHandle_bossLaser = Novice::PlayAudio(SOUND_bossLaser, false, 1.0f);
								bulletLaser->isShoot = true;
								bulletLaser->pos.x = enemyBoss2->pos.x + enemyBoss2->width / 2.0f;
								bulletLaser->pos.y = enemyBoss2->pos.y + 260.0f;
							}
						}

#pragma endregion
					}
				}
				//laser弹道及atari
				if (bulletLaser->isShoot) {
					for (int i = 0; i < 10; i++) {
						box[i].position.x = rand() % 801 + bulletLaser->pos.x;
						box[i].position.y = rand() % 201 + bulletLaser->pos.y;
					}
					bulletLaser->pos.x -= bulletLaser->speed;
					if (bulletLaser->pos.x <= 0) {
						bulletLaser->pos.x = 0;
					}
					if (bulletLaser->timer > 0) {
						bulletLaser->timer--;
					}
					else {
						bulletLaser->timer = 180;
						bulletLaser->isShoot = false;
					}
				}
				if (bulletLaser->isShoot) {
					if (IsSpriteCollision(bulletLaser->pos.x, bulletLaser->width, bulletLaser->pos.y, bulletLaser->height,
						player1->pos.x, player1->width, player1->pos.y, player1->height)) {
						if (player1->hp > 0) {
							player1->hp -= 0.2f;
							isPlayerShake = true;
						}
						else {
							player1->hp = 0;
						}
					}
				}
			}
#pragma endregion
#pragma region Clear 界面
			if (scene == CLEAR) {
				if (clearTimer <= 0) {
					clearTimer = 0;
				}
				else {
					clearTimer--;
				}
				Novice::StopAudio(voiceHandle_stage);
				if(isClear){
					if (Novice::IsPlayingAudio(voiceHandle_clear) == 0 || voiceHandle_clear == -1) {
						voiceHandle_clear = Novice::PlayAudio(SOUND_clear, false, 1.0f);
					}
					isClear = false;
				}
				if (isGameClearMove == true) {
					if (nowFrameClear >= endFrameClear) {
						nowFrameClear = endFrameClear;

					}
					else {
						nowFrameClear++;
						xFrameClear = nowFrameClear / endFrameClear;
						gameClearPosY = easeOutBack(xFrameClear) * 720.0f;
					}
				}

				if(clearTimer==0){
					if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0) {
						player1->Initalize();
						enemyBoss2->Initalize();
						/*for (int i = 0; i < 10; i++) {
							enemyZako[i]->Initalize(magnification);
							enemyZako2[i]->Initalize(magnification);
						}
						enemyElite1->Initalize(magnification);

						for (int i = 0; i < 32; i++) {
							bullet0[i]->Initalize();
						}
						for (int i = 0; i < 5; i++) {
							bullet1[i]->Initalize();
						}


						for (int i = 0; i < 12; i++) {
							bullet2[i]->Initalize();
						}
						isBullet2On = false;
						reloadTimer2 = 45;

						for (int i = 0; i < 32; i++) {
							bullet3[i]->Initalize();
						}

						amplitude = 25.0f;

						for (int i = 0; i < 2; i++) {
							bullet4[i]->Initalize();
						}
						lengthBullet4[0] = { 1300.0f };
						lengthBullet4[1] = { 1300.0f };

						for (int i = 0; i < 32; i++) {
							bullet5[i]->Initalize();
						}
						starPosX = 1100.0f;
						starMagnification = 0.01f;
						for (int i = 0; i < 10; i++) {
							bulletZako[i].pos.x = -128.0f;
							bulletZako[i].pos.y = -128.0f;
							bulletZako[i].width = 64.0f;
							bulletZako[i].height = 64.0f;
							bulletZako[i].speed = 10.0f;
							bulletZako[i].isShoot = false;
							bulletZako[i].isHoly = true;
						}*/
						gameClearPosY = 0.0f;
						nowFrameClear = 0.0f;
						xFrameClear = 0.0f;
						isGameClearMove = false;
						clearTimer = 20;
						scene = TITLE;
					}
				}
			}
#pragma endregion
#pragma region Over 界面
			if (scene == OVER) {
				if (overTimer <= 0) {
					overTimer = 0;
				}
				else {
					overTimer--;
				}
				Novice::StopAudio(voiceHandle_stage);
				if(isLose){
					if (Novice::IsPlayingAudio(voiceHandle_lose) == 0 || voiceHandle_lose == -1) {
						voiceHandle_lose = Novice::PlayAudio(SOUND_lose, false, 0.7f);
					}
					isLose = false;
				}
				if (isGameOverMove == true) {
					if (nowFrameOver >= endFrameOver) {
						nowFrameOver = endFrameOver;

					}
					else {
						nowFrameOver++;
						xFrameOver = nowFrameOver / endFrameOver;
						gameOverPosY = easeOutBack(xFrameOver) * 720.0f;
					}
				}
				if(overTimer==0){
					if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0) {
						player1->Initalize();
						/*for (int i = 0; i < 10; i++) {
							enemyZako[i]->Initalize(magnification);
							enemyZako2[i]->Initalize(magnification);
						}
						enemyElite1->Initalize(magnification);
						enemyBoss2->Initalize();
						for (int i = 0; i < 32; i++) {
							bullet0[i]->Initalize();
						}
						for (int i = 0; i < 5; i++) {
							bullet1[i]->Initalize();
						}


						for (int i = 0; i < 12; i++) {
							bullet2[i]->Initalize();
						}
						isBullet2On = false;
						reloadTimer2 = 45;

						for (int i = 0; i < 32; i++) {
							bullet3[i]->Initalize();
						}

						amplitude = 25.0f;

						for (int i = 0; i < 2; i++) {
							bullet4[i]->Initalize();
						}
						lengthBullet4[0] = { 1300.0f };
						lengthBullet4[1] = { 1300.0f };

						for (int i = 0; i < 32; i++) {
							bullet5[i]->Initalize();
						}
						starPosX = 1100.0f;
						starMagnification = 0.01f;
						for (int i = 0; i < 10; i++) {
							bulletZako[i].pos.x = -128.0f;
							bulletZako[i].pos.y = -128.0f;
							bulletZako[i].width = 64.0f;
							bulletZako[i].height = 64.0f;
							bulletZako[i].speed = 10.0f;
							bulletZako[i].isShoot = false;
							bulletZako[i].isHoly = true;
						}*/
						gameOverPosY = 0.0f;
						nowFrameOver = 0.0f;
						xFrameOver = 0.0f;
						isGameOverMove = false;
						overTimer = 20;
						scene = TITLE;
					}
				}
			}
#pragma endregion
		}
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		if (scene == TITLE) {
			//Novice::DrawSprite(0, 0, Texture_Title, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(int(titleX), int(titleY), Texture_Title, titleRatio, titleRatio, 0, WHITE);
			Novice::DrawSprite(int(titlePlayerX), int(titlePlayerY), titlePlayer, titlePlayerRatio, titlePlayerRatio, 0, WHITE);
			//Novice::ScreenPrintf(0, 0, "timer=%d", titleTimer);
		}
		if (scene == CLEAR) {
			Novice::DrawSprite(0, 0, Texture_Bg0, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(0, (int)gameClearPosY - 720, Texture_Clear, 1, 1, 0.0f, WHITE);
			for (int i = 0; i < 4; i++) {
				Novice::DrawSprite(636 + 48 * i, (int)gameClearPosY - 376, Texture_ScoreNumber[eachScoreNumber[i]], 1, 1, 0.0f, WHITE);
			}
		}
		if (scene == OVER) {
			Novice::DrawSprite(0, 0, Texture_Bg0, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(0, (int)gameOverPosY - 720, Texture_Over, 1, 1, 0.0f, WHITE);
			for (int i = 0; i < 4; i++) {
				Novice::DrawSprite(636 + 48 * i, (int)gameOverPosY - 376, Texture_ScoreNumber[eachScoreNumber[i]], 1, 1, 0.0f, WHITE);
			}
		}
#pragma region bg描画
		if (scene == TUTORIAL || scene == STAGE1 || scene == STAGE2 || scene == BOSS1 || scene == BOSS2) {
			//bg
			Novice::DrawSprite((int)bg0Pos.x + randBackgroundPosX, (int)bg0Pos.y + randBackgroundPosY, Texture_Bg0, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite((int)bg0Pos.x + randBackgroundPosX + 1280, (int)bg0Pos.y + randBackgroundPosY, Texture_Bg0, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite((int)starPosX, starPosY, Texture_Bg4, starMagnification, starMagnification, 0.0f, WHITE);
			Novice::DrawSprite((int)bg1Pos.x + randBackgroundPosX, (int)bg1Pos.y + randBackgroundPosY, Texture_Bg1, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite((int)bg1Pos.x + randBackgroundPosX + 1280, (int)bg1Pos.y + randBackgroundPosY, Texture_Bg1, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite((int)bg2Pos.x + randBackgroundPosX, (int)bg1Pos.y + randBackgroundPosY, Texture_Bg2, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite((int)bg2Pos.x + randBackgroundPosX + 1280, (int)bg1Pos.y + randBackgroundPosY, Texture_Bg2, 1, 1, 0.0f, WHITE);
		}


#pragma endregion
#pragma region 弹幕描画
		//弹幕描画
		if (shootStates == S1) {
			for (int i = 0; i < 5; i++) {
				bullet1[i]->Draw();
			}
		}
		if (shootStates == S2) {
			for (int i = 0; i < 32; i++) {
				bullet0[i]->Draw(0);
			}
		}
		if (shootStates == S3) {
			if (scene == TUTORIAL || scene == STAGE1) {
				for (int i = 0; i < 32; i++) {
					bullet0[i]->Draw(32);
				}
				for (int i = 0; i < 12; i++) {
					bullet2[i]->Draw();
				}
			}
			if (scene == BOSS1 || scene == STAGE2 || scene == BOSS2) {
				Novice::DrawSprite(0, 0, Texture_Lockedbullet, 1, 1, 0.0f, WHITE);
			}

		}
		if (shootStates == S4) {
			for (int i = 0; i < 32; i++) {
				bullet3[i]->Draw();
			}
		}
		if (shootStates == S5) {
			for (int i = 0; i < 2; i++) {
				bullet4[i]->Draw();
			}
		}
		if (shootStates == S6) {
			for (int i = 0; i < 32; i++) {
				if (bullet5[i]->isShoot) {
					Novice::DrawQuad(
						(int)worldLeftTop[i].x, (int)worldLeftTop[i].y,
						(int)worldRightTop[i].x, (int)worldRightTop[i].y,
						(int)worldLeftBottom[i].x, (int)worldLeftBottom[i].y,
						(int)worldRightBottom[i].x, (int)worldRightBottom[i].y,
						64, 0, 32, 32, Texture_Bullet, WHITE
					);
				}
			}
		}
#pragma endregion
#pragma region 敌人描画
		//敌人描画
		if (scene == STAGE1) {
			for (int i = 0; i < 10; i++) {
				enemyZako[i]->Draw();
				enemyZako2[i]->Draw();
			}
			enemyElite1->Draw();

			//敌人子弹
			for (int i = 0; i < 10; i++) {
				if (bulletZako[i].isShoot) {
					if (bulletZako[i].isHoly) {
						Novice::DrawSpriteRect(
							(int)bulletZako[i].pos.x,
							(int)bulletZako[i].pos.y,
							128, 0, 32, 32,
							Texture_Bullet,
							1.0f / 6, 1, 0.0f, WHITE
						);
					}
					if (!bulletZako[i].isHoly) {
						Novice::DrawSpriteRect(
							(int)bulletZako[i].pos.x,
							(int)bulletZako[i].pos.y,
							160, 0, 32, 32,
							Texture_Bullet,
							1.0f / 6, 1, 0.0f, WHITE
						);
					}
				}

			}
		}
		if (scene == STAGE2) {
			for (int i = 0; i < 10; i++) {
				enemyZako[i]->Draw();
				enemyZako2[i]->Draw();
			}
			enemyElite1->Draw();
			enemyElite2->Draw();
			//敌人子弹
			for (int i = 0; i < 10; i++) {
				if (bulletZako[i].isShoot) {
					if (bulletZako[i].isHoly) {
						Novice::DrawSpriteRect(
							(int)bulletZako[i].pos.x,
							(int)bulletZako[i].pos.y,
							128, 0, 32, 32,
							Texture_Bullet,
							1.0f / 6, 1, 0.0f, WHITE
						);
					}
					if (!bulletZako[i].isHoly) {
						Novice::DrawSpriteRect(
							(int)bulletZako[i].pos.x,
							(int)bulletZako[i].pos.y,
							160, 0, 32, 32,
							Texture_Bullet,
							1.0f / 6, 1, 0.0f, WHITE
						);
					}
				}
				if (bulletElite2[i]->isShoot) {
					if (bulletElite2[i]->isHoly) {
						Novice::DrawSpriteRect(
							(int)bulletElite2[i]->pos.x,
							(int)bulletElite2[i]->pos.y,
							128, 0, 32, 32,
							Texture_Bullet,
							1.0f / 6, 1, 0.0f, WHITE
						);
					}
					if (!bulletElite2[i]->isHoly) {
						Novice::DrawSpriteRect(
							(int)bulletElite2[i]->pos.x,
							(int)bulletElite2[i]->pos.y,
							160, 0, 32, 32,
							Texture_Bullet,
							1.0f / 6, 1, 0.0f, WHITE
						);
					}
				}
			}
		}
#pragma endregion
#pragma region Boss描画
		//boss1
		if (scene == BOSS1) {
			if (enemyBoss1->isBossDefeated == false) {
				if (enemyBoss1->isAlive == false) {
					Novice::DrawSprite(0, 0, Texture_Warning, 1, 1, 0.0f, WHITE);
					Novice::DrawSprite(0, 0, Texture_Lockedbullet, 1, 1, 0.0f, WHITE);
					isBackgroundShake = true;
				}
			}
			//boss弹幕
			for (int i = 0; i < 5; i++) {
				bulletB111[i]->Draw();
				bulletB112[i]->Draw();
				bulletB113[i]->Draw();
			}
			for (int i = 0; i < 10; i++) {
				bulletB121[i]->Draw();
				bulletB122[i]->Draw();
			}
			//本体
			enemyBoss1->Draw();
			//hpbar
			if (enemyBoss1->isBossDefeated == false) {
				if (enemyBoss1->isAlive == true) {
					Novice::DrawBox(1260, 360, 20, int(enemyBoss1->hp / enemyBoss1->hpMax * 160 * -1), 0.0f, GREEN, kFillModeSolid);
					Novice::DrawBox(1260, 360, 20, int(enemyBoss1->hp / enemyBoss1->hpMax * 160), 0.0f, GREEN, kFillModeSolid);
				}
			}

		}
		//boss2
		if (scene == BOSS2) {
			if (enemyBoss2->isBossDefeated == false) {
				if (enemyBoss2->isAlive == false) {
					Novice::DrawSprite(0, 0, Texture_Warning, 1, 1, 0.0f, WHITE);
					isBackgroundShake = true;
				}
			}
			//boss 镭射
			if (bulletLaser->isShoot) {
				bulletLaser->Draw();
				for (int i = 0; i < 10; i++) {

					Novice::DrawSprite(
						(int)box[i].position.x,
						(int)box[i].position.y,
						Texture_LaserPartical,
						i * 0.1f, i * 0.1f, 0.0f, WHITE
					);
				}
			}
			//boss弹幕
			for (int i = 0; i < 5; i++) {
				bulletB221[i]->Draw();
				bulletB222[i]->Draw();
				bulletB223[i]->Draw();
			}
			//boss黑洞
			if (bulletBlackHole->isShoot) {
				Novice::DrawQuad(
					(int)worldLeftTopBB2.x, (int)worldLeftTopBB2.y,
					(int)worldRightTopBB2.x, (int)worldRightTopBB2.y,
					(int)worldLeftBottomBB2.x, (int)worldLeftBottomBB2.y,
					(int)worldRightBottomBB2.x, (int)worldRightBottomBB2.y,
					0, 0, 200, 200, Texture_BlackHole, WHITE
				);
			}
			//missile
			enemyElite1->Draw();
			//本体
			enemyBoss2->Draw();
			//skill alert
			if (enemyBoss2->shootCoolTimeBH > 5 && enemyBoss2->shootCoolTimeBH < 65) {
				Novice::DrawSprite(1194, 327, Texture_AlertYellow, 1, 1, 0.0f, WHITE);
			}
			if (enemyBoss2->shootCoolTimeLS > 5 && enemyBoss2->shootCoolTimeLS < 65) {
				Novice::DrawSprite(961, 327, Texture_AlertGreen, 1, 1, 0.0f, WHITE);
			}
			//hpbar
			if (enemyBoss2->isBossDefeated == false) {
				if (enemyBoss2->isAlive == true) {
					Novice::DrawBox(1260, 360, 20, int(enemyBoss2->hp / enemyBoss2->hpMax * 160 * -1), 0.0f, GREEN, kFillModeSolid);
					Novice::DrawBox(1260, 360, 20, int(enemyBoss2->hp / enemyBoss2->hpMax * 160), 0.0f, GREEN, kFillModeSolid);
				}
			}

		}
#pragma endregion
#pragma region TUTORIAL描画

		if (scene == TUTORIAL) {
			Novice::DrawSprite((int)tutoPosX, 0, Texture_Tutorial, 1, 1, 0.0f, WHITE);
			if (enemyZako_eg->isAlive) {
				Novice::DrawSprite((int)enemyZako_eg->pos.x, (int)enemyZako_eg->pos.y, Texture_Zako_eg, 1, 1, 0.0f, WHITE);
			}
			if (enemyZako2_eg->isAlive) {
				Novice::DrawSprite((int)enemyZako2_eg->pos.x, (int)enemyZako2_eg->pos.y, Texture_Zako2_eg, 1, 1, 0.0f, WHITE);
			}
			if (enemyElite1_eg->isAlive) {
				Novice::DrawSprite((int)enemyElite1_eg->pos.x, (int)enemyElite1_eg->pos.y, Texture_Elite1_eg, 1, 1, 0.0f, WHITE);
			}
			if (enemyElite2_eg->isAlive) {
				Novice::DrawSprite((int)enemyElite2_eg->pos.x, (int)enemyElite2_eg->pos.y, Texture_Elite2_eg, 1, 1, 0.0f, WHITE);
			}
		}
#pragma endregion
		if (scene == LOADING) {
			Novice::DrawSprite(0, 0, Texture_Bg0, 1, 1, 0, WHITE);
			if (isBoss1DeathAnimeOn) {
				Novice::DrawSpriteRect(880, 0, 400 * b1DAnimeStep, 0, 400, 720, Texture_B1DAnime, 0.25, 1, 0.0f, WHITE);
			}
			Novice::DrawSprite(int(loadingPlayerX), int(loadingPlayerY), titlePlayer, 1, 1, 0, WHITE);
		}
		if (scene == TUTORIAL || scene == STAGE1 || scene == STAGE2 || scene == BOSS1 || scene == BOSS2) {
			//自分描画
			player1->Draw();
#pragma region 射弹计时条

			//射弹计时条
			if (isbulletTimerOn) {
				Novice::DrawBox(0, 360, 15, (int)bulletBarlength * -1, 0.0f, 0xFFFF00FF, kFillModeSolid);
				Novice::DrawBox(0, 360, 15, (int)bulletBarlength, 0.0f, 0xFFFF00FF, kFillModeSolid);
			}
#pragma endregion
#pragma region HUD
			//HUD
			Novice::DrawSprite(0, 0, Texture_Bg3, 1, 1, 0.0f, WHITE);
			//danger
			if (player1->hp <= player1->hpMax / 2) {
				Novice::DrawSprite(0, 70, Texture_Danger1, 1, 1, 0.0f, WHITE);
			}
			if (player1->hp <= player1->hpMax * 0.3f) {
				Novice::DrawSprite(0, 70, Texture_Danger2, 1, 1, 0.0f, WHITE);
			}
			//HP bar
			if (player1->hp >= player1->hpMax / 2) {
				Novice::DrawBox(440, 670, (int)(player1->hp / player1->hpMax * 400), 30, 0.0f, GREEN, kFillModeSolid);
			}
			if (player1->hp < player1->hpMax / 2 && player1->hp >= player1->hpMax * 0.3f) {
				Novice::DrawBox(440, 670, (int)(player1->hp / player1->hpMax * 400), 30, 0.0f, 0xFFFF00FF, kFillModeSolid);
			}
			if (player1->hp < player1->hpMax * 0.3f && player1->hp >= 0) {
				Novice::DrawBox(440, 670, (int)(player1->hp / player1->hpMax * 400), 30, 0.0f, RED, kFillModeSolid);
			}
			//chance
			for (int i = 0; i < player1->chance; i++) {
				Novice::DrawSprite(64 * i + 1000, 3, Texture_Chance, 1, 1, 0.0f, WHITE);
			}

#pragma endregion
#pragma region 元素灯
			//元素灯
			for (int i = 0; i < 3; i++) {
				if (inputArr[i] == 1) {
					Novice::DrawSpriteRect(50 + 64 * i, 720 - 64, 128, 0, 64, 64, Texture_element, 1.0f / 3, 1, 0.0f, WHITE);
				}
				if (inputArr[i] == 2) {
					Novice::DrawSpriteRect(50 + 64 * i, 720 - 64, 64, 0, 64, 64, Texture_element, 1.0f / 3, 1, 0.0f, WHITE);
				}
				if (inputArr[i] == 3) {
					Novice::DrawSpriteRect(50 + 64 * i, 720 - 64, 0, 0, 64, 64, Texture_element, 1.0f / 3, 1, 0.0f, WHITE);
				}
			}
#pragma endregion
#pragma region Score
			//Score
			for (int i = 0; i < 4; i++) {

				Novice::DrawSprite(400 + 48 * i, 3, Texture_ScoreNumber[eachScoreNumber[i]], 1, 1, 0.0f, WHITE);
			}
#pragma endregion
		}

#pragma region 暂停界面

		//暂停界面
		if (isPause) {


			switch (menu) {
				case Menu::Continue:
					/*Novice::DrawBox(150, 410, 200, 50, 0.0f, BLACK, kFillModeSolid);*/
					Novice::DrawSpriteRect(100, 100, 0, 0, 1080, 520, Texture_Pause, 1.0f / 3, 1, 0.0f, WHITE);
					break;
				case Menu::Back:
					/*Novice::DrawBox(150, 480, 200, 50, 0.0f, BLACK, kFillModeSolid);*/
					Novice::DrawSpriteRect(100, 100, 1080, 0, 1080, 520, Texture_Pause, 1.0f / 3, 1, 0.0f, WHITE);
					break;
				case Menu::Quit:
					/*Novice::DrawBox(150, 550, 200, 50, 0.0f, BLACK, kFillModeSolid);*/
					Novice::DrawSpriteRect(100, 100, 2160, 0, 1080, 520, Texture_Pause, 1.0f / 3, 1, 0.0f, WHITE);
					break;

			}
		}
#pragma endregion
		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_BACK] == 0 && keys[DIK_BACK] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
