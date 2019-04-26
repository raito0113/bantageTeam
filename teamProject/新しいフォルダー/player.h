#pragma once

#define PLAYER_SIZE_X 64
#define PLAYER_SIZE_Y 64


extern STATUS player;

void PlayerSysInit(void);

void PlayerInit(void);

void PlayerControl(void);

void PlayerDraw(void);

