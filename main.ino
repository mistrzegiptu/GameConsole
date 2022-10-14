#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "snake.h"

/*//SCREEN SIZE
#define width 84
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

/*enum Direction
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

S *head;
S *tail;

int score = 0;
int appleX;
int appleY;*/
int menuCounter = 1;
int menuState = 0;

unsigned long previousSnakeMove = 0;
unsigned long previousMenuMove = 0;
unsigned long previousPongMove = 0;

String menuPositions[menuPositionsCount+1] = {"Menu", "Snake", "Pong"};

//Direction Dir = RIGHT;

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
    
    /*head = (S *)malloc(sizeof(S));
    head->x=fixedWidth/2;
    head->y=fixedHeight/2;
    head->next = NULL;
    
    appleX = random(fixedWidth);
    appleY = random(fixedHeight);*/
    SnakeBegin();
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
                //ReadJoystick(leftJoystick);
                //ReadJoystick(rightJoystick);
                previousPongMove = millis();
            }
            break;
        }
    }
    
}
/*void ReadJoystick(byte joystick)
{
    if(joystick == leftJoystick)
    {
        
    }
    else if(joystick == rightJoystick)
}*/
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
    /*switch (menuCounter)
    {
        case 0:
        {
            //display.drawLine(3, 13, 81, 13, BLACK);
            display.drawRect(3, 13, 81, 21, BLACK)
            break;
        }
    }*/
}
/*void MoveHead()
{
    int joyX = analogRead(joystickXPin);
    int joyY = analogRead(joystickYPin);
    if(joyX>maxJoystick||joyX<minJoystick||joyY>maxJoystick||joyY<minJoystick)
    {
        if(abs(joyX-midJoystick)>abs(joyY-midJoystick))
        {
            if(joyX>midJoystick)
            {
                if(Dir==UP||Dir==LEFT||Dir==RIGHT)
                {
                    Dir=UP;
                    head->y-=1;
                }
                else
                  MoveHeadWithOldDirection();
            }
            else
            {
                if(Dir==RIGHT||Dir==DOWN||Dir==LEFT)
                {
                    Dir=DOWN;
                    head->y+=1;
                }
                else
                  MoveHeadWithOldDirection();
            }
        }
        else
        {
            if(joyY>midJoystick)
            {
                if(Dir==RIGHT||Dir==UP||Dir==DOWN)
                {
                    Dir=RIGHT;
                    head->x+=1;
                }
                else
                  MoveHeadWithOldDirection();
            }
            else
            {
                if(Dir==UP||Dir==DOWN||Dir==LEFT)
                {
                    Dir=LEFT;
                    head->x-=1;
                }
                else
                  MoveHeadWithOldDirection();
            }
        }
    }
    else
      MoveHeadWithOldDirection();
}
void MoveHeadWithOldDirection()
{
    switch (Dir)
    {
        case UP:
            head->y-=1;
            break;
        case DOWN:
            head->y+=1;
            break;
        case RIGHT:
            head->x+=1;
            break;
        case LEFT:
            head->x-=1;
            break;
    }
}
void MoveBody()
{
    S* body;
    body = tail;
    
    if(body!=NULL)
    {
        while(body->next)
        {
            body->x=body->next->x;
            body->y=body->next->y;
            body = body->next;
        }
        if(tail->next==head)
        {
          body->x=head->x;
          body->y=head->y;
        }
    }
}
void CheckForPoint()
{
    if(head->x==appleX&&head->y==appleY)
    {
        score++;
        AddBody();
        DisplaySnake();
        CreateApple();
    }
}
void AddBody()
{
    S *body;
    body = (S *)malloc(sizeof(S));
    
    if(tail)
    {
        body->x=tail->x;
        body->y=tail->y;
        body->next=tail;
        tail=body;
    }
    else
    {
        body->x=head->x;
        body->y=head->y;
        tail = body;
        tail->next=head;
    }
}
void CreateApple()
{
    do
    {
        appleX = random(fixedWidth);
        appleY = random(fixedHeight); 
    } while(AppleInsideBody()); 
}
bool AppleInsideBody()
{
    S *body;
    body = tail;
    
    if(body)
    {
      while(body->next->next)
      {
          if(appleX==body->x&&appleY==body->y)
          {
              return true;
          }
          body = body->next;
      }
    }
    return false;
}
void CheckForCollision()
{
    if(head->x<0||head->x>=fixedWidth||head->y<0||head->y>=fixedHeight)
    {
        GameOver();
    }
    
    S *body;
    body = tail;
    
    if(tail)
    {
      while (body->next->next)
      {
          if((body->x==head->x)&&(body->y==head->y))
          {
              GameOver();
          }
          body = body->next;
      }
    }
}
void GameOver()
{
  while(true)
  {
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("Game Over");
    display.setCursor(0,10);
    display.print("Score: ");
    display.print(score);
    display.display();
  }
}
void DisplaySnake()
{
    S *body;
    body = tail;

    display.clearDisplay();
    DisplayApple();
    DisplayBodyPart(head->x, head->y);
    //display.drawPixel(appleX, appleY, 1);
    //display.drawPixel(head->x, head->y, 1);
    if(body)
    {
        while(body->next!=NULL)
        {
            DisplayBodyPart(body->x, body->y);
            body = body->next;
        }
    }
    display.display();
}
void DisplayApple()
{
    int fixedAppleX = appleX*3+1;
    int fixedAppleY = appleY*3+1;
    for(int i = fixedAppleX-1; i <= fixedAppleX + 1; i++)
    {
        for(int j = fixedAppleY - 1; j <= fixedAppleY + 1; j++)
        {
            display.drawPixel(i, j, 1);
        }
    }
    display.drawPixel(fixedAppleX, fixedAppleY, 0);
}
void DisplayBodyPart(int x, int y)
{
    int bodyX = x*3+1;
    int bodyY = y*3+1;

    display.drawPixel(bodyX-1, bodyY-1, 1);
    display.drawPixel(bodyX-1, bodyY, 1);
    display.drawPixel(bodyX-1, bodyY+1, 1);
    display.drawPixel(bodyX, bodyY-1, 1);
    display.drawPixel(bodyX, bodyY, 1);
    display.drawPixel(bodyX, bodyY+1, 1);
    display.drawPixel(bodyX+1, bodyY-1, 1);
    display.drawPixel(bodyX+1, bodyY, 1);
    display.drawPixel(bodyX+1, bodyY+1, 1);
}*/
