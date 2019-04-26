#include "DxLib.h"
#include <math.h>
#include "main.h"
#include "keycheck.h"
#include "player.h"


//�萔
#define INIT_VELOCITY		50		//�����x
#define SECOND_PER_FRAM		0.18		//1�t���[��������0.2�b

#define VELOCITY_X_MAX	10			//X�����̑��xMAX


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
	
	//�ړ�����---------------------------------------
	if (newKey[P1_RIGHT])
	{
		player.dir = DIR_RIGHT;
		player.runFlag = true;
		player.velocity.x += ACC_RUN;
		if (player.velocity.x > VELOCITY_X_MAX)			//���点��
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
		if (player.velocity.x < -VELOCITY_X_MAX)		//���点��
		{
			player.velocity.x = -VELOCITY_X_MAX;
		}
		if (player.velocity.x > 0)
		{
			player.velocity.x -= ACC_RUN * 3;
		}
	}
	//-------------------------------------------------

	//���x����-----------------------------------------
	if (!player.runFlag)
	{
		if (player.velocity.x > 0)
		{
			player.velocity.x -= ACC_STOP * 5;			//���X�ɃX�s�[�h�𗎂Ƃ��Ă���
		}

		if (player.velocity.x < 0)
		{
			player.velocity.x += ACC_STOP * 5;			//���X�ɃX�s�[�h�𗎂Ƃ��Ă���
		}

	}
	//------------------------------------------------
	player.pos.x += player.velocity.x * 1;				//�v���C���[�|�W�V�����̈ړ�


	//�U��---------------------------------------------
	if (trgKey[P1_Z])
	{
		player.attackFlag = true;
		player.attackImgLockCnt = 0;
	}
	


	//�v���C���[����ʊO�ɏo�����Ȃ�----------------------
	if (player.pos.x + player.offSet.x > SCREEN_SIZE_X)
	{
		player.pos.x = SCREEN_SIZE_X - player.offSet.x;
	}
	if (player.pos.x - player.offSet.x < 0)
	{
		player.pos.x = 0 + player.offSet.x;
	}
	//-----------------------------------------------------

	//�n�ʂւ̓����蔻��-----------------------------------
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

	//�W�����v----------------------------------------------
	if (player.jumpFlag)
	{
		player.pos.y -= player.velocity.y *(SECOND_PER_FRAM);		//�����x�̌v�Z
		player.velocity.y -= (ACC_G / 3) * (SECOND_PER_FRAM);		//�����̌v�Z
	}
	else if (trgKey[P1_UP])
	{
		player.pos.y -= SCREEN_SIZE_Y / 6 * 5;
		player.jumpImgLockCnt = 0;
	}
	//-------------------------------------------------------

	//��莞�ԃv���C���[�����WY���Œ�------------------------
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
	int ranAniID = ((player.aniCnt / 8) % 4);			//����A�j���[�V�����J�E���g
	int standAniID = ((player.aniCnt / 30) % 2);		//�ҋ@���A�j���[�V�����J�E���g
	int jumpAniID = ((player.aniCnt / 10) % 2);			//�W�����v���A�j���[�V�����J�E���g
	int attackAniID = ((player.aniCnt / 8) % 4);		//�U���A�j���[�V�����J�E���g

	
	//�W�����v���̉摜�̕\��---------------------------------
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
	//�U���̉摜�̕\��---------------------------------------

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

	//�ړ����̉摜�̕\��-------------------------------
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
	//�ҋ@���̉摜�̕\��-------------------------------
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