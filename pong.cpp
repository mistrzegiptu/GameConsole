#include "pong.h"

PongDirections BallDir;

struct Point ball;

byte firstPlayer = height/2;
byte secondPlayer = height/2;

byte player1Score = 0;
byte player2Score = 0;

void PongBegin()
{
    pinMode(joystick1YPin, INPUT);
    pinMode(joystick2YPin, INPUT);

    SetBallDirection(random(1, 5));

    ball.x = width/2;
    ball.y = height/2;
}
void SetBallDirection(int direction)
{
    if(direction==1)
        BallDir=RIGHTUP;
    else if(direction==2)
        BallDir=RIGHTDOWN;
    else if(direction==3)
        BallDir=LEFTUP;
    else if(direction==4)
        BallDir=LEFTDOWN;
}
void ReadJoysticks()
{
    int player1 = analogRead(joystick1YPin);
    //int player2 = analogRead(joystick2YPin);

    if(player1>=minUpValue)
    {
        firstPlayer++;
    }
    else if(player1>=minDownValue)
    {
        firstPlayer--;
    }
    /*if(player2>=minUpValue)
    {
        secondPlayer++;
    }
    else if(player2>=minDownValue)
    {
        secondPlayer--;
    }*/
}
void MoveBall()
{
    if(BallDir == RIGHTUP)
    {
        ball.x++;
        ball.y++;
    }
    else if(BallDir == RIGHTDOWN)
    {
        ball.x++;
        ball.y--;
    }
    else if(BallDir == LEFTUP)
    {
        ball.x--;
        ball.y++;
    }
    else if(BallDir == LEFTDOWN)
    {
        ball.x--;
        ball.y--;
    }
    CheckForBallCollision();
}
void CheckForBallCollision()
{
    if((ball.x==0&&ball.y<=firstPlayer+2&&ball.y>=firstPlayer-2)||(ball.x==width&&ball.y<=secondPlayer+2&&ball.y>=secondPlayer-2))
    {
        PlayerChangeBallDirection();
        MoveBall();
    }
    else if(ball.x>0&&ball.x<width&&(ball.y==0||ball.y==height))
    {
        WallChangeBallDirection();
        //do wyjebania w zależności od działania odbicia
        MoveBall();
    }
    else if((ball.x==0&&(ball.y>=firstPlayer+2||ball.y<=firstPlayer-2))||(ball.x==width&&(ball.y>=secondPlayer+2||ball.y<=secondPlayer-2)))
    {
        if(ball.x==0)
            player1Score++;
        else 
            player2Score++;
        CheckForWin();
    }
}
void CheckForWin()
{
    if(player1Score>=5)
    {
        while(true)
        {
            display.clearDisplay();
            display.print("Player 1 won");
            display.display();
        }
    }
    else if(player2Score>=5)
    {
        while(true)
        {
            display.clearDisplay();
            display.print("Player 2 won");
            display.display();
        }   
    }
    else
        StartAfterScore();
}
void StartAfterScore()
{
    SetBallDirection(random(1, 5));

    ball.x = width/2;
    ball.y = height/2;
    firstPlayer = height/2;
    secondPlayer = height/2;
}
void WallChangeBallDirection()
{
    if(BallDir==RIGHTUP)
        BallDir = RIGHTDOWN;
    else if(BallDir==RIGHTDOWN)
        BallDir = RIGHTUP;
    else if(BallDir==LEFTUP)
        BallDir = LEFTDOWN;
    else if(BallDir==LEFTDOWN)
        BallDir = LEFTUP;
}
void PlayerChangeBallDirection()
{
    if(BallDir==RIGHTUP)
        BallDir = LEFTUP;
    else if(BallDir==RIGHTDOWN)
        BallDir = LEFTDOWN;
    else if(BallDir==LEFTUP)
        BallDir = RIGHTUP;
    else if(BallDir==LEFTDOWN)
        BallDir = RIGHTDOWN;
}
void Display()
{
    display.clearDisplay();

    display.drawPixel(ball.x, ball.y, 1);

    display.drawPixel(0, firstPlayer, 1);
    display.drawPixel(0, firstPlayer+1, 1);
    display.drawPixel(0, firstPlayer+2, 1);
    display.drawPixel(0, firstPlayer-1, 1);
    display.drawPixel(0, firstPlayer-2, 1);

    display.drawPixel(width, secondPlayer, 1);
    display.drawPixel(width, secondPlayer+1, 1);
    display.drawPixel(width, secondPlayer+2, 1);
    display.drawPixel(width, secondPlayer-1, 1);
    display.drawPixel(width, secondPlayer-2, 1);
    
    display.display();
}
