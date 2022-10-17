#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#include "snake.h"
#include "pong.h"

//SCREEN SIZE
/*#define width 84
#define height 48
#define fixedWidth width/3
#define fixedHeight height/3*/

//PINS
#define joystickXPin 27
#define joystickYPin 26
#define randomSeedPin 28
#define menuButton 22

//JOYSTICK VALUES
#define minJoystick 100
#define midJoystick 512
#define maxJoystick 923

#define leftJoystick 0
#define rightJoystick 1

#define menuPositionsCount 2

int menuCounter = 1;
int menuState = 0;

unsigned long previousSnakeMove = 0;
unsigned long previousMenuMove = 0;
unsigned long previousPongMove = 0;

String menuPositions[menuPositionsCount+1] = {"Menu", "Snake", "Pong"};

Adafruit_PCD8544 display = Adafruit_PCD8544(6, 7, 4, 5, 8);

void setup()
{
    Serial.begin(9600);
    
    randomSeed(analogRead(randomSeedPin));
    
    display.begin();
    display.setContrast(40);
    display.clearDisplay();
    
    pinMode(joystickXPin, INPUT);
    pinMode(joystickYPin, INPUT);
    pinMode(menuButton, INPUT_PULLUP);

    SnakeBegin();
    PongBegin();
}
void loop()
{
    switch(menuState)
    {
        case 0:
        {
            if(millis()-previousMenuMove>=250)
            {
                ReadMenuJoystick();
                ReadMenuButton();
                DisplayMenu();
                previousMenuMove = millis();
            }
            break;
        }
        case 1:
        {
            if(millis()-previousSnakeMove>=250)
            {
                MoveBody();
                MoveHead();
                CheckForPoint();
                CheckForCollision();
                DisplaySnake();
                previousSnakeMove = millis();
            }
            break;
        }
        case 2:
        {
            if(millis()-previousPongMove>=0)
            {
                ReadJoysticks();
                MoveBall();
                Display();
                previousPongMove = millis();
            }
            break;
        }
    }
    
}
void ReadMenuJoystick()
{
    int joyY = analogRead(joystickXPin);

    if(joyY > maxJoystick)
    {
        if(menuCounter > 1)
        {
            menuCounter--;
        }
    }
    else if(joyY < minJoystick)
    {
        if(menuCounter < menuPositionsCount)
        {
            menuCounter++;
        }
    }
}
void ReadMenuButton()
{
    if(digitalRead(menuButton)==LOW)
    {
        menuState = menuCounter;
    }
}
void DisplayMenu()
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(3,5);
    display.print("1.");
    display.print(menuPositions[menuState]);

    display.setCursor(3,20);
    display.print("2.Pong");

    display.setCursor(3,30);
    display.print(menuCounter);
    
    display.display();
}