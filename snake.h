#ifndef SNAKE
#define SNAKE

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

//SCREEN SIZE
#define width 84
#define height 48
#define fixedWidth width/3
#define fixedHeight height/3
//PINS
#define joystickXPin 27
#define joystickYPin 26

//JOYSTICK VALUES
#define minJoystick 100
#define midJoystick 512
#define maxJoystick 923

enum SnakeDirections
{
    UP = 1,
    DOWN = 2,
    RIGHT = 3,
    LEFT = 4
};
typedef struct snake
{
    int x;
    int y;
    struct snake *next;
} S;

extern Adafruit_PCD8544 display;

void SnakeBegin();

void MoveHead();

void MoveBody();

void MoveHeadWithOldDirection();

void CheckForPoint();

void AddBody();

void CreateApple();

bool AppleInsideBody();

void CheckForCollision();

void GameOver();

void DisplaySnake();

void DisplayApple();

void DisplayBodyPart(int x, int y);
#endif
