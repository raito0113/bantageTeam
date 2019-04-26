//#pragma once

#define ZAKO_ENEMYS									10000
#define FLY_ENEMYS									10

#define ZAKO_SPEED									5
#define FLY_SPEED									2

#define ENEMY_SIZE_X								32
#define ENEMY_SIZE_Y								32

#define SHOT_SPEED									4

extern STATUS enemy1[ZAKO_ENEMYS];

void EnemySysInit(void);

void EnemyInit(void);

void EnemyControl(void);

void EnemyDraw(void);

void ShotControl(void);