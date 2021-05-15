
#include "snakeGame_Tasks.h"
#include <windows.h>

// Some Game Configs
#define GAME_DIFFICULTY_LEVEL       10
#define GAME_LIFE                   3
#define GAME_HEIGHT                 15//22
#define GAME_WIDTH                  50//100
// Static Parts
#define UP_KEY_STADE       11
#define DOWN_KEY_STADE     12
#define LEFT_KEY_STADE     13
#define RIGHT_KEY_STADE    14
#define SPACE_KEY_STADE    15
#define CRASH_ACTIVE       22
#define GAME_OVERX         12
#define CRASH_PASSIVE      11

void snakeGameClass::Initiation(void)
{
    snakeBody = (struct snakeBodyStr*)malloc(sizeof(struct snakeBodyStr)); 
    snakeBody = NULL;  
 
    Target_Point_Coordinate.x = 0;
    Target_Point_Coordinate.y = 0;
    
    Add_New_SnakeBody(GAME_WIDTH/2, GAME_HEIGHT/2);
    Add_New_SnakeBody(GAME_WIDTH / 2+1, GAME_HEIGHT / 2);
    Add_New_SnakeBody(GAME_WIDTH / 2+2, GAME_HEIGHT / 2);

    GAME_STADE = CRASH_PASSIVE;
    TIME_OF_DIFFICULTY = GAME_DIFFICULTY_LEVEL;
    GAME_TIME_MS = 0;
    SCORE = 0; 
    LIFE = GAME_LIFE;
    Diff_Counter = 0;
    Key_Stade = RIGHT_KEY_STADE;
    Last_Snake_Direction = RIGHT_KEY_STADE;

    Draw_Game_Border(); 
    Update_Score_Life(); 
    Update_Snake();
    Add_Random_Point();
}

void snakeGameClass::Game_Manager(void)
{
    while (1)
    {
        updateKeyStade();

        Snake_Body_Control();

        GAME_TIME_MS++;

        Sleep(1);  // 1 ms
    }
}

int snakeGameClass::isThe_Point_Suitable(int randX, int randY)
{
    // Check Point is in GameArea , this '6' value must be static now we can just test
    if (randX < 6 || randX > GAME_WIDTH - 1)
        return 0;
    if (randY < 6 || randY > GAME_HEIGHT - 1)
        return 0;

    // Check Point is in Last Point
    if (Target_Point_Coordinate.x == randX && Target_Point_Coordinate.y == randY)
        return 0;
    
    // Check Point is in Snakeb-Body
    snakeBodyStr* node = snakeBody;
    while (node->next != NULL)
    {
        if (node->x == randX && node->y == randY)
            return 0;
        
        node = node->next;
    }
    if (node->x == randX && node->y == randY)
        return 0;
    
    // true step
    return 1;
}

void snakeGameClass::Add_Random_Point()
{
    int rand_x, rand_y;

    // find true point coord
    do
    { // This Random value must be connect to Time this func 'rand()' is not good
        rand_x = (rand() % (GAME_WIDTH - 1)) + 1;
        rand_y = (rand() % (GAME_HEIGHT - 1)) + 1;
    }     
    while (isThe_Point_Suitable(rand_x, rand_y) == 0);

    Target_Point_Coordinate.x = rand_x;
    Target_Point_Coordinate.y = rand_y;

    //write point
    Goto_Console_Main_XY(rand_x, rand_y);
    printf("*");
}

void snakeGameClass::Draw_Game_Border()
{
    Goto_Console_Main_XY(0, 5);
    int x, y;
    for (x = 0; x < GAME_HEIGHT; x++)
    {
        for (y = 0; y < GAME_WIDTH; y++)
        {
            if (x == 0 || y == 0 || x == GAME_HEIGHT - 1 || y == GAME_WIDTH - 1)
                printf("X");
            else
                printf(" ");
        }
        printf("\n");
    }

    printf("\n\nGelistirici : Ali SAHBAZ ..  \n");
    printf("Tarih       : 15.05.2021 ..  \n");
    printf("ali_sahbaz@outlook.com \n\n\n");
}

void snakeGameClass::Update_Score_Life()
{
    Goto_Console_Main_XY(GAME_WIDTH/4, 4);
    printf("SKOR: %d " , SCORE);
    Goto_Console_Main_XY(GAME_WIDTH / 4 * 2.4, 4);
    printf("CAN: %d" , LIFE);
    Goto_Console_Main_XY(40, 30);
    printf("                                                     ");
}

void snakeGameClass::Move_Snake_Body()
{
    snakeBodyStr* node = snakeBody;
    int head_x, head_y;

    // clear first Pixell , because snake is moving
    Delete_Snake_Pixell(node->x, node->y);

    // move to Snake
    while (node->next != NULL)
    {
        node->x = node->next->x;
        node->y = node->next->y;

        node = node->next;
    }

    Update_Snake_Head(node); // to direction

    Update_Snake(); 

    head_x = node->x;
    head_y = node->y;

    isSnake_Arrive_toPoint(node);

    isSnake_Crash_theWall(node);
}

void snakeGameClass::isSnake_Crash_theWall(snakeBodyStr* node)
{
    // Snake can eat himself, you can add search in snakeBody coord here

    if (GAME_STADE != GAME_OVERX)
    {
        if (node->x < 1 || node->y<6 || node->x>GAME_WIDTH - 2 || node->y > GAME_HEIGHT + 3)
        {
            GAME_STADE = CRASH_ACTIVE;

            Error_Active();

            SCORE = 0;

            LIFE--;
            if (LIFE == 0)
                GAME_OVER();
        }
    }
}

void snakeGameClass::Update_Snake_Head(snakeBodyStr* node)
{
    if (Key_Stade == RIGHT_KEY_STADE)
    {
        node->x++;

        Last_Snake_Direction = RIGHT_KEY_STADE;
    }
    else if (Key_Stade == LEFT_KEY_STADE)
    {
        node->x--;

        Last_Snake_Direction = LEFT_KEY_STADE;
    }
    else if (Key_Stade == UP_KEY_STADE)
    {
        node->y--;

        Last_Snake_Direction = UP_KEY_STADE;
    }
    else if (Key_Stade == DOWN_KEY_STADE)
    {
        node->y++;

        Last_Snake_Direction = DOWN_KEY_STADE;
    }
}

void snakeGameClass::isSnake_Arrive_toPoint(snakeBodyStr* node)
{
    // eat the target
    if (Target_Point_Coordinate.x == node->x && Target_Point_Coordinate.y == node->y)
    {
        Add_New_SnakeBody(node->x, node->y);

        Add_Random_Point();

        SCORE++;

        Update_Score_Life();

        Diff_Counter++;
        if (Diff_Counter > GAME_DIFFICULTY_LEVEL / 2)
        {
            TIME_OF_DIFFICULTY -= 1;
            if (TIME_OF_DIFFICULTY < 2) TIME_OF_DIFFICULTY = 2;
            Diff_Counter = 0;
        }
    }
}

void snakeGameClass::Update_Snake()
{
    snakeBodyStr* node = snakeBody;

    while (node->next != NULL)
    {
        Add_Snake_Pixell(node->x, node->y);

        node = node->next;
    }

    Add_Snake_Pixell(node->x, node->y);
}

void snakeGameClass::Delete_Snake_Pixell(int x, int y)
{
    Goto_Console_Main_XY(x, y);
    printf(" ");
}

void snakeGameClass::Add_Snake_Pixell(int x, int y)
{
    Goto_Console_Main_XY(x, y);
    printf("*");
}

void snakeGameClass::Goto_Console_Main_XY(int x, int y)
{
    HANDLE a;
    COORD b;
    fflush(stdout);
    b.X = x;
    b.Y = y;
    a = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(a, b);
}

void snakeGameClass::updateKeyStade()
{
    if (GetKeyState('W') & 0x8000)
    {
        Key_Stade = UP_KEY_STADE;
    }
    else if (GetKeyState('S') & 0x8000)
    {
        Key_Stade = DOWN_KEY_STADE;
    }
    else if (GetKeyState('A') & 0x8000)
    {
        Key_Stade = LEFT_KEY_STADE;
    }
    else if (GetKeyState('D') & 0x8000)
    {
        Key_Stade = RIGHT_KEY_STADE;
    }
    else if (GetKeyState(' ') & 0x8000)
    {
        Key_Stade = SPACE_KEY_STADE;

        if (GAME_STADE == CRASH_ACTIVE)
        {
            // game starting again
            int holdLife = LIFE;
            Initiation();
            LIFE = holdLife;
            Update_Score_Life();
        }
    }
}

void snakeGameClass::Add_New_SnakeBody(int x , int y)
{
    snakeLength++;

    snakeBodyStr* new_node = (struct snakeBodyStr*)malloc(sizeof(struct snakeBodyStr));

    snakeBodyStr* last = snakeBody;

    new_node->x = x;
    new_node->y = y;
    new_node->next = NULL;

    if (snakeBody == NULL)
    {
        // First Item add
        snakeBody = new_node;
        return;
    }

    while (last->next != NULL)
    {
        //Goto end off LinkedList
        last = last->next;
    }

    last->next = new_node;

    return;
}

void snakeGameClass::Snake_Body_Control(void)
{
    if (GAME_TIME_MS > TIME_OF_DIFFICULTY && GAME_STADE != CRASH_ACTIVE && GAME_STADE != GAME_OVERX)
    {
        if (Last_Snake_Direction != Key_Stade)
        {
            // check key stade and game logic is suitable
            if (Key_Stade == LEFT_KEY_STADE && Last_Snake_Direction == RIGHT_KEY_STADE)
                Key_Stade = RIGHT_KEY_STADE;
            else if (Key_Stade == RIGHT_KEY_STADE && Last_Snake_Direction == LEFT_KEY_STADE)
                Key_Stade = LEFT_KEY_STADE;
            else if (Key_Stade == UP_KEY_STADE && Last_Snake_Direction == DOWN_KEY_STADE)
                Key_Stade = DOWN_KEY_STADE;
            else if (Key_Stade == DOWN_KEY_STADE && Last_Snake_Direction == UP_KEY_STADE)
                Key_Stade = UP_KEY_STADE;
        }

        Move_Snake_Body();

        GAME_TIME_MS = 0;
    }
}

void snakeGameClass::Error_Active()
{
    Goto_Console_Main_XY(40, 30);
    printf("YANDIN , Devam etmek istiyorsan 'space'e bas ");
}

void snakeGameClass::GAME_OVER()
{
    GAME_STADE = GAME_OVERX;
    Goto_Console_Main_XY(40, 30);
    printf("OYUN SONU                                            ");
}
