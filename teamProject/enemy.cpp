#include "DxLib.h"
#include "main.h"
#include "enemy.h"
#include "player.h"
#include "keycheck.h"

STATUS enemy1[ZAKO_ENEMYS];

STATUS enemy2[FLY_ENEMYS];

void EnemySysInit(void)
{

}

void EnemyInit(void)
{
	for (int i = 0; i < ZAKO_ENEMYS; i++)
	{
		//ZAKO_ENEMY‚Ì‰Šú‰»
		enemy1[i].pos = { GetRand(SCREEN_SIZE_X) + SCREEN_SIZE_X ,SCREEN_SIZE_Y / 6 * 5};
		enemy1[i].speed = ZAKO_SPEED;
		enemy1[i].visible = true;
		enemy1[i].dir = DIR_LEFT;
		enemy1[i].velocity.y = INIT_VELOCITY;
		enemy1[i].jumpCnt = GetRand(60000000);
		enemy1[i].jumpFlag = false;
		enemy1[i].size = { ENEMY_SIZE_X, ENEMY_SIZE_Y };
		enemy1[i].offSet = { enemy1[i].size.x / 2, enemy1[i].size.y / 2 };
		enemy1[i].shotFlag = false;
		enemy1[i].shotMove = false;
		enemy1[i].shotMoveCnt = 0;
		enemy1[i].shotPos = { enemy1[i].pos.x,enemy1[i].pos.y };
		enemy1[i].shotSpeed = SHOT_SPEED;
		enemy1[i].shotCnt = 1;
		enemy1[i].knockBackFlag = false;
	}

	for (int y = 0; y < FLY_ENEMYS; y++)
	{
		enemy2[y].pos = { GetRand(SCREEN_SIZE_X),GetRand(SCREEN_SIZE_Y / 6) + SCREEN_SIZE_Y / 6 };
		enemy2[y].speed = FLY_SPEED;
		enemy2[y].visible = true;
		enemy2[y].dir = DIR_LEFT;
		enemy2[y].size = { ENEMY_SIZE_X / 2, ENEMY_SIZE_Y / 2 };
		enemy2[y].moved = true;
	}
}

void EnemyControl(void)
{
	//ZAKO_ENEMY‚Ìˆ—
	for (int i = 0; i < ZAKO_ENEMYS; i++)
	{
		enemy1[i].moved = false;
		enemy1[i].jumpFlag = false;

		//ƒWƒƒƒ“ƒv§Œä

		enemy1[i].pos.y -= enemy1[i].velocity.y * SECOND_PER_FRAME;

		enemy1[i].velocity.y -= ACC_G * SECOND_PER_FRAME * 6;

		enemy1[i].jumpCnt++;
		if (enemy1[i].jumpCnt % 60 == 0)
		{
			enemy1[i].jumpFlag = true;
			enemy1[i].jumpCnt = 0;
		}

		if (enemy1[i].jumpFlag == true)
		{
			enemy1[i].velocity.y = GetRand(INIT_VELOCITY) * 6;
		}

		if (enemy1[i].pos.y >= SCREEN_SIZE_Y / 6 * 5)
		{
			enemy1[i].pos.y = SCREEN_SIZE_Y / 6 * 5;
		}

		//ˆÚ“®‘€ì

		if (enemy1[i].pos.x > player.pos.x - 64 * 4
			&& enemy1[i].pos.x < player.pos.x + 64 * 4
			&& enemy1[i].pos.y >= SCREEN_SIZE_Y / 6 * 5
			)
		{
			enemy1[i].jumpCnt = 0;
			enemy1[i].shotCnt++;
		}
		else if (enemy1[i].pos.y < SCREEN_SIZE_Y / 6 * 5)
		{
			enemy1[i].moved = true;
		}

		if (enemy1[i].jumpFlag == false)
		{
			if (player.pos.x > enemy1[i].pos.x)
			{
				enemy1[i].dir = DIR_RIGHT;
			}

			if (player.pos.x <= enemy1[i].pos.x)
			{
				enemy1[i].dir = DIR_LEFT;
			}
		}

		if (enemy1[i].moved == true && enemy1[i].shotFlag == false)
		{
			switch (enemy1[i].dir)
			{
			case DIR_RIGHT:
				enemy1[i].pos.x += enemy1[i].speed;
				break;
			case DIR_LEFT:
				enemy1[i].pos.x -= enemy1[i].speed;
				break;
			}
		}
	}


	//FLY_ENEMY‚Ìˆ—
	
	for (int y = 0; y < FLY_ENEMYS; y++)
	{
		if (enemy2[y].moved == true)
		{
			switch (enemy2[y].dir)
			{
			case DIR_RIGHT:
				enemy2[y].pos.x += enemy2[y].speed;
				break;
			case DIR_LEFT:
				enemy2[y].pos.x -= enemy2[y].speed;
				break;
			}
		}

		if (player.pos.x - player.size.x * 6 < enemy2[y].pos.x
			&& player.pos.x + player.size.x * 6 > enemy2[y].pos.x)
		{
			if (enemy2[y].velocity.x >= 0)
			{
				enemy2[y].velocity.x -= ACC_STOP;
			}

			if (enemy2[y].velocity.x <= 0)
			{
				enemy2[y].velocity.x += ACC_STOP;
			}

			enemy2[y].moved = false;
		}
		else
		{	
			enemy2[y].moved = true;

			if (player.pos.x > enemy2[y].pos.x)
			{
				enemy2[y].dir = DIR_RIGHT;
				enemy2[y].velocity.x += ACC_RUN;
			}

			if (player.pos.x < enemy2[y].pos.x)
			{
				enemy2[y].dir = DIR_LEFT;
				enemy2[y].velocity.x -= ACC_RUN;
			}
		}

		switch (enemy2[y].dir)
		{
		case DIR_RIGHT:
			enemy2[y].pos.x += enemy2[y].velocity.x;
			break;
		case DIR_LEFT:
			enemy2[y].pos.x += enemy2[y].velocity.x;
			break;
		}

		if (enemy2[y].velocity.x > VELOCITY_X_MAX)
		{
			enemy2[y].velocity.x = VELOCITY_X_MAX;
		}

		if (enemy2[y].velocity.x < -VELOCITY_X_MAX)
		{
			enemy2[y].velocity.x = -VELOCITY_X_MAX;
		}
	}

	ShotControl();
}

void EnemyDraw(void) 
{
	for (int i = 0; i < ZAKO_ENEMYS; i++)
	{
		if (enemy1[i].visible == true)
		{
			DrawCircle(enemy1[i].pos.x, enemy1[i].pos.y - enemy1[i].offSet.y,
				16, 0xFFFFFF, false);

			DrawLine(enemy1[i].shotPos.x - 8, enemy1[i].shotPos.y - 16,
				enemy1[i].shotPos.x + 8, enemy1[i].shotPos.y - 16, 0xFF0000);
		}
	}

	for (int y = 0; y < FLY_ENEMYS; y++)
	{
		if (enemy2[y].visible == true)
		{
			DrawCircle(enemy2[y].pos.x, enemy2[y].pos.y,
				16, 0xFFFFFF, false);
		}
	}

	DrawLine(0, SCREEN_SIZE_Y / 6 * 5, SCREEN_SIZE_X, SCREEN_SIZE_Y / 6 * 5, 0xFFFFFF);

}

void ShotControl(void)
{
	for (int i = 0; i < ZAKO_ENEMYS; i++)
	{
		enemy1[i].shotFlag = false;

		if (enemy1[i].shotCnt % 30 == 0)
		{
			enemy1[i].shotFlag = true;
			enemy1[i].shotMove = true;
			enemy1[i].knockBackFlag = true;
			enemy1[i].shotCnt = 1;
			enemy1[i].velocity.y = INIT_VELOCITY * 6;
		}

		if (enemy1[i].knockBackFlag == true)
		{
			if (enemy1[i].pos.y >= SCREEN_SIZE_Y / 6 * 5 && enemy1[i].shotMove == false)
			{
				enemy1[i].knockBackFlag = false;
			}
		}

		if (enemy1[i].shotMove == true)
		{
			enemy1[i].shotMoveCnt++;
			if (enemy1[i].shotMoveCnt >= 60)
			{
				enemy1[i].shotMove = false;
			}

			switch (enemy1[i].dir)
			{
			case DIR_RIGHT:
				enemy1[i].shotPos.x += enemy1[i].shotSpeed;
				break;
			case DIR_LEFT:
				enemy1[i].shotPos.x -= enemy1[i].shotSpeed;
				break;
			}
		}

		if (enemy1[i].knockBackFlag == true && enemy1[i].pos.y < SCREEN_SIZE_Y / 6 * 5)
		{
			switch (enemy1[i].dir)
			{
			case DIR_RIGHT:
				enemy1[i].pos.x -= enemy1[i].speed * 2;
				break;
			case DIR_LEFT:
				enemy1[i].pos.x += enemy1[i].speed * 2;
				break;
			}
		}

		if (enemy1[i].shotPos.x > player.pos.x - 32
			&& enemy1[i].shotPos.x < player.pos.x + 32)
		{
			enemy1[i].shotMove = false;
			enemy1[i].shotCnt = 1;
		}

		if (enemy1[i].shotMove == false)
		{
			enemy1[i].shotPos = { enemy1[i].pos.x,enemy1[i].pos.y };
			enemy1[i].shotMoveCnt = 0;
		}
	}
}