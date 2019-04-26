//#pragma once

//定数
//#define SCREEN_SIZE_X	800
//#define SCREEN_SIZE_Y	600
#define SCREEN_SIZE_X	1024
#define SCREEN_SIZE_Y	576

#define PI	3.141592

#define INIT_VELOCITY							20
#define VELOCITY_X_MAX							6

#define ACC_G									10
#define SECOND_PER_FRAME						0.1

#define ACC_RUN									0.15
#define ACC_STOP								0.10

enum {
	DIR_RIGHT,
	DIR_LEFT,
	DIR_MAX
};

//定義
typedef struct {
	float x;
	float y;
} XY_F;

typedef struct {
	int x;
	int y;
} XY;

typedef struct
{
	XY pos;
	XY size;
	XY shotPos;
	XY offSet;
	XY_F velocity;
	bool visible;
	int aniCnt;
	int imgLockCnt;
	int moved;
	int speed;
	int dir;
	int runFlag;
	int jumpCnt;
	int jumpFlag;
	int shotFlag;
	int shotMove;
	int shotMoveCnt;
	int shotSpeed;
	int shotCnt;
	int knockBackFlag;
}STATUS;

//プロトタイプ宣言
