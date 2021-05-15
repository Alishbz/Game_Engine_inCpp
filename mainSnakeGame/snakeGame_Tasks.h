#pragma once
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include<time.h>
#include<ctype.h>
#include <time.h>
#include <windows.h>
#include <process.h>

typedef struct snakeBodyStr {
	int x;
	int y;
	snakeBodyStr *next;
};

typedef struct pointCord {
	int x;
	int y;
};

class snakeGameClass {

public:

	void Initiation(void);

	void Game_Manager(void);

private:

	pointCord Target_Point_Coordinate;

	int SCORE;
	
	int LIFE;

	int GAME_STADE;

	int GAME_TIME_MS;

	int Diff_Counter;

	int TIME_OF_DIFFICULTY;

	int Key_Stade;

	int Last_Snake_Direction;

	snakeBodyStr *snakeBody;

	int snakeLength;

private:

	void GAME_OVER();

	void Error_Active();

	void isSnake_Crash_theWall(snakeBodyStr* node);

	void Update_Snake_Head(snakeBodyStr* node);

	void isSnake_Arrive_toPoint(snakeBodyStr* node);

	int isThe_Point_Suitable(int randX, int randY);

	void Add_Random_Point();

	void Snake_Body_Control(void);

	void Move_Snake_Body();

	void Delete_Snake_Pixell(int x, int y);

	void Add_Snake_Pixell(int x, int y);

	void updateKeyStade();

	void Goto_Console_Main_XY(int x, int y);
	
	void Draw_Game_Border();

	void Update_Score_Life();

	void Update_Snake();

	void Add_New_SnakeBody(int x, int y);

};