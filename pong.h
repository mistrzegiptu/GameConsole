#ifndef PONG
#define PONG

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#define width 83
#define height 48

#define joystick1YPin 27
#define buttonUpPin 14
#define buttonDownPin 15

#define minUpValue 614
#define minDownValue 409

extern Adafruit_PCD8544 display;

enum PongDirections
{
    RIGHTUP = 1,
    RIGHTDOWN = 2,
    LEFTUP = 3,
    LEFTDOWN = 4
};

struct Point
{
    int x, y;
};

void PongBegin();

void ReadJoysticks();

void MoveBall();

void CheckForBallCollision();

void CheckForWin();

void StartAfterScore();

void WallChangeBallDirection();

void PlayerChangeBallDirection();

void SetBallDirection(int direction);

void Display();
#endif
