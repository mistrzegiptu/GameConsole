#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

//SCREEN SIZE
#define width 84
#define height 48

//PINS
#define joystickXPin 27
#define joystickYPin 26
#define randomSeedPin 28

//JOYSTICK VALUES
#define minJoystick 100
#define midJoystick 512
#define maxJoystick 923

enum Direction
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
int appleY;
unsigned long moveTime = 250;
unsigned long previousMove = 0;

Direction Dir = RIGHT;

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
    
    head = (S *)malloc(sizeof(S));
    head->x=width/2;
    head->y=height/2;
    head->next = NULL;
    
    appleX = random(width);
    appleY = random(height);
}
void loop()
{
    if(millis()-previousMove>=moveTime)
    {
        MoveBody();
        MoveHead();
        CheckForPoint();
        CheckForCollision();
        Display();
        previousMove = millis();
    }
}
void MoveHead()
{
    float joyX = analogRead(joystickXPin);
    float joyY = analogRead(joystickYPin);
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
        Display();
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
        appleX = random(width);
        appleY = random(height); 
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
    if(head->x<0||head->x>=width||head->y<0||head->y>=height)
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
void Display()
{
    S *body;
    body = tail;

    display.clearDisplay();
    display.drawPixel(appleX, appleY, 1);
    display.drawPixel(head->x, head->y, 1);
    if(body)
    {
        while(body->next!=NULL)
        {
            display.drawPixel(body->x, body->y, 1);
            body = body->next;
        }
    }
    display.display();
}
