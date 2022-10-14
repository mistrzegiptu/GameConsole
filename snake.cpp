#include "snake.h"

Direction Dir = RIGHT;

S *head;
S *tail;

int score = 0;
int appleX;
int appleY;

void SnakeBegin()
{
    head = (S *)malloc(sizeof(S));
    head->x=fixedWidth/2;
    head->y=fixedHeight/2;
    head->next = NULL;
    
    appleX = random(fixedWidth);
    appleY = random(fixedHeight);
}
void MoveHead()
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
}
