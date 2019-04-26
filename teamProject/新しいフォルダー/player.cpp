#include "DxLib.h"
#include <math.h>
#include "main.h"
#include "keycheck.h"
#include "player.h"


//定数
#define INIT_VELOCITY		50		//初速度
#define SECOND_PER_FRAM		0.18		//1フレーム当たり0.2秒

#define VELOCITY_X_MAX	10			//X方向の速度MAX


int playerStandImg[2];
int playerRunImg[4];
int playerJumpImg[2];
int playerDamageImg;
int playerFlightImg;
int playerAttackImg[4];

STATUS player;		




void PlayerSysInit(void)
{
	SetTransColor(0, 255, 0);
	LoadDivGraph("img/stand.png", 2, 2, 1, PLAYER_SIZE_X, PLAYER_SIZE_Y, playerStandImg);
	LoadDivGraph("img/run.png", 4, 4, 1, PLAYER_SIZE_X, PLAYER_SIZE_Y, playerRunImg);
	LoadDivGraph("img/jump.png", 2, 2, 1, PLAYER_SIZE_X, PLAYER_SIZE_Y, playerJumpImg);
	playerDamageImg = LoadGraph("img/damage.png");
	playerFlightImg = LoadGraph("img/flight.png");
	LoadDivGraph("img/attack.png", 4, 4, 1, PLAYER_SIZE_X, PLAYER_SIZE_Y, playerAttackImg);




}

void PlayerInit(void)
{
	player.pos = { 32,SCREEN_SIZE_Y / 6 * 5  - 32};
	player.size = { PLAYER_SIZE_X,PLAYER_SIZE_Y };
	player.offSet = { player.size.x / 2,player.size.y / 2 };
	player.speed = 5;
	player.aniCnt = 0;
	player.dir = DIR_RIGHT;
	player.jumpFlag = false;
	player.runFlag = false;
	player.jumpImgLockCnt = 50;
	player.attackImgLockCnt = 50;
	player.attackFlag = false;
}

void PlayerControl(void)
{

	player.jumpFlag = true;
	player.runFlag = false;
//	player.attackFlag = false;
	
	//移動制御---------------------------------------
	if (newKey[P1_RIGHT])
	{
		player.dir = DIR_RIGHT;
		player.runFlag = true;
		player.velocity.x += ACC_RUN;
		if (player.velocity.x > VELOCITY_X_MAX)			//滑らせる
		{
			player.velocity.x = VELOCITY_X_MAX;
		}
		if (player.velocity.x < 0)
		{
			player.velocity.x += ACC_RUN * 3;
		}
	}
	
	if (newKey[P1_LEFT])
	{
		player.dir = DIR_LEFT;
		player.runFlag = true;
		player.velocity.x -= ACC_RUN;
		if (player.velocity.x < -VELOCITY_X_MAX)		//滑らせる
		{
			player.velocity.x = -VELOCITY_X_MAX;
		}
		if (player.velocity.x > 0)
		{
			player.velocity.x -= ACC_RUN * 3;
		}
	}
	//-------------------------------------------------

	//速度制御-----------------------------------------
	if (!player.runFlag)
	{
		if (player.velocity.x > 0)
		{
			player.velocity.x -= ACC_STOP * 5;			//徐々にスピードを落としていく
		}

		if (player.velocity.x < 0)
		{
			player.velocity.x += ACC_STOP * 5;			//徐々にスピードを落としていく
		}

	}
	//------------------------------------------------
	player.pos.x += player.velocity.x * 1;				//プレイヤーポジションの移動


	//攻撃---------------------------------------------
	if (trgKey[P1_Z])
	{
		player.attackFlag = true;
		player.attackImgLockCnt = 0;
	}
	


	//プレイヤーを画面外に出させない----------------------
	if (player.pos.x + player.offSet.x > SCREEN_SIZE_X)
	{
		player.pos.x = SCREEN_SIZE_X - player.offSet.x;
	}
	if (player.pos.x - player.offSet.x < 0)
	{
		player.pos.x = 0 + player.offSet.x;
	}
	//-----------------------------------------------------

	//地面への当たり判定-----------------------------------
	if (player.pos.y + player.offSet.y < SCREEN_SIZE_Y / 6 * 5 - 1)
	{
		player.pos.y = player.pos.y;
	}
	else 
	{
		player.pos.y = SCREEN_SIZE_Y / 6 * 5 - player.offSet.y;
		player.velocity.y = 0;
		player.jumpFlag = false;
	}
	//------------------------------------------------------

	//ジャンプ----------------------------------------------
	if (player.jumpFlag)
	{
		player.pos.y -= player.velocity.y *(SECOND_PER_FRAM);		//加速度の計算
		player.velocity.y -= (ACC_G / 3) * (SECOND_PER_FRAM);		//高さの計算
	}
	else if (trgKey[P1_UP])
	{
		player.pos.y -= SCREEN_SIZE_Y / 6 * 5;
		player.jumpImgLockCnt = 0;
	}
	//-------------------------------------------------------

	//一定時間プレイヤーを座標Yを固定------------------------
	if (player.jumpImgLockCnt <= 15)
	{
		player.pos.y = SCREEN_SIZE_Y / 6;
	}
	//-------------------------------------------------------
	player.jumpImgLockCnt++;
	player.attackImgLockCnt++;

	
}

void PlayerDraw(void)
{
	int ranAniID = ((player.aniCnt / 8) % 4);			//走りアニメーションカウント
	int standAniID = ((player.aniCnt / 30) % 2);		//待機中アニメーションカウント
	int jumpAniID = ((player.aniCnt / 10) % 2);			//ジャンプ中アニメーションカウント
	int attackAniID = ((player.aniCnt / 8) % 4);		//攻撃アニメーションカウント

	
	//ジャンプ中の画像の表示---------------------------------
	if (player.jumpFlag)				
	{
		if (player.jumpImgLockCnt <= 10)
		{
			if (player.dir == DIR_RIGHT)
			{
				DrawGraph(player.pos.x - player.offSet.x, player.pos.y - player.offSet.y, playerFlightImg, true);
			}
			else
			{
				DrawTurnGraph(player.pos.x - player.offSet.x, player.pos.y - player.offSet.y, playerFlightImg, true);
			}
			
		}
		else
		{
			if (player.dir == DIR_RIGHT)
			{
				DrawGraph(player.pos.x - player.offSet.x, player.pos.y - player.offSet.y, playerJumpImg[jumpAniID], true);
			}
			else
			{
				DrawTurnGraph(player.pos.x - player.offSet.x, player.pos.y - player.offSet.y, playerJumpImg[jumpAniID], true);
			}
		}
		
	}
	//-------------------------------------------------
	//攻撃の画像の表時---------------------------------------

	else if (player.attackFlag)
	{
		{
			if (player.dir == DIR_RIGHT)
			{
				DrawGraph(player.pos.x - player.offSet.x, player.pos.y - player.offSet.y, playerAttackImg[attackAniID], true);
			}
			else
			{
				DrawTurnGraph(player.pos.x - player.offSet.x, player.pos.y - player.offSet.y, playerAttackImg[attackAniID], true);
			}
		}
	}

	//-------------------------------------------------

	//移動中の画像の表示-------------------------------
	else if (player.runFlag)		
	{
		if (player.dir == DIR_RIGHT)
		{
			DrawGraph(player.pos.x - player.offSet.x, player.pos.y - player.offSet.y, playerRunImg[ranAniID], true);
		}
		else
		{
			DrawTurnGraph(player.pos.x - player.offSet.x, player.pos.y - player.offSet.y, playerRunImg[ranAniID], true);
		}
	}
	//-------------------------------------------------------
	//待機中の画像の表示-------------------------------
	else								
	{
		if (player.dir == DIR_RIGHT)
		{
			DrawGraph(player.pos.x - player.offSet.x, player.pos.y - player.offSet.y, playerStandImg[standAniID], true);
		}
		else
		{
			DrawTurnGraph(player.pos.x - player.offSet.x, player.pos.y - player.offSet.y, playerStandImg[standAniID], true);
		}
	}
	//--------------------------------------------------

	player.aniCnt++;


	DrawFormatString(200, 0, 0xffffff, "velocity.y : %0.5f", player.velocity.y);
	DrawFormatString(200, 20, 0xffffff, "velocity.x : %0.5f", player.velocity.x);

}