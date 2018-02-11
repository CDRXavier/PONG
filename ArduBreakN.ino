/*************************************
*                                    *
* #    #                             *
*  #  #                      *       *
*   ##                       *       *
*   ##       ***** ***** ***** ***** *
*  #  #      *     *   * *   * ***** *
* #    #     ***** ***** ***** ***** *
*                                    *
*************************************/
//things done:
//columns of bricks (line 18)
//void.intro (line 57)
//pad right limit (line 76)
//pad speed (line 73~82)
//ball speed (line 111~114)
//(line 122)
//Score()(334-337)
//hitbox(line 171)
//right limit(line 149)
//pause argument (line 703)
//"Hello World!"(line 663)
//framerate (line 664)
//draw score (line 369)
//initial health (line 47)
//improved failing(line 128)
//improved physics (line 180)
//ball on the pad(line 233)
//drawPaddle() (line 275,324)
//print live(line 289)
//pause() (line 306)
//redo print() (line 330)
//redo newLevel (line 340)
//sound boolean(line 61)
//toggle sound(line 756)
//added line to reset score, fix 5060 points_on_startup bug(line 703)
//High score interface (line 509)
//ONE_PRESS_ONCE if (arduboy.notPressed(A_BUTTON)) (line = finaLine - 1)
#include "Arduboy.h"

Arduboy arduboy;

const unsigned short COLUMNS = 10; //Columns of bricks
const unsigned short ROWS = 4;     //Rows of bricks
byte dx = -1;        //Initial movement of ball
byte dy = -1;        //Initial movement of ball
int xb;           //Balls starting possition
int yb;           //Balls starting possition
boolean released;     //If the ball has been released by the player
boolean paused = false;   //If the game has been paused
short xPaddle;       //X position of paddle
boolean isHit[ROWS][COLUMNS];   //Array of if bricks are hit or not
boolean bounced=false;  //Used to fix double bounce glitch
short lives = 2;       //Amount of lives
short level = 1;       //Current level
unsigned int score=0;   //Score for the game
unsigned int brickCount;  //Amount of bricks hit
char text[16];      //General string buffer
boolean start=false;    //If in menu or in game
boolean initialDraw=false;//If the inital draw has happened
char initials[3];     //Initials used in high score
boolean sound = true;//sound for the game
boolean Breleased = true;//stops repeating

//Ball Bounds used in collision detection
byte leftBall;
byte rightBall;
byte topBall;
byte bottomBall;

//Brick Bounds used in collision detection
byte leftBrick;
byte rightBrick;
byte topBrick;
byte bottomBrick;

void movePaddle() {
  //Move right
  if(xPaddle < WIDTH - 47) if (arduboy.pressed(RIGHT_BUTTON)) xPaddle+=3;
  //Move left
  if(xPaddle > 0) if (arduboy.pressed(LEFT_BUTTON)) xPaddle-=3;
  
  //prevent it from going over to the sides
    if (xPaddle < 1) xPaddle = 0;
   if (xPaddle > WIDTH-47) xPaddle = WIDTH - 47;
    
}

void moveBall() {
  if(released)
  {
    //Move ball
    
      xb += dx;
    
    yb=yb + dy;

    //Set bounds
    leftBall = xb;
    rightBall = xb + 2;
    topBall = yb;
    bottomBall = yb + 2;

    //Bounce off top edge
    if (yb <= 0)
    {
      yb = 2;
      dy = -dy;
      if(sound)arduboy.tunes.tone(523, 200);
    }

    //Lose a life if bottom edge hit
    if (yb >= 64)
    {
      arduboy.drawRect(xPaddle, 63, 16, 1, 0);
      if(sound)arduboy.tunes.tone(175, 200);
      delay(250);
      yb=60;
      released = false;
      lives--;
      drawLives();
      
      dx = (random(-2, 2));
    }

    //Bounce off left side
    if (xb <= 0)
    {
      xb = 2;
      dx = -dx;
      if(sound)arduboy.tunes.tone(523, 200);
    }

    //Bounce off right side
    if (xb >= WIDTH - 31)
    {
      xb = WIDTH - 32;
      dx = -dx;
      if(sound)arduboy.tunes.tone(523, 200);
    }

    //Bounce off paddle
    if (xb+1>=xPaddle && xb<=xPaddle+17 && yb+2>=63 && yb<=64)
    {
      dy = -dy;
      dx = ((xb-(xPaddle+7))/3); //Applies spin on the ball
      //"prevent" straight bounce
      if (dx == 0) {
        dx = (random(-1,1));
      }
      //limit horizontal speed
      if (dx < -2) dx = -2;
      if (dx > 2)  dx =  2;
      if(sound)arduboy.tunes.tone(200, 200);
    }

    //Bounce off Bricks
    for (byte row = 0; row < ROWS; row++)
    {
      for (byte column = 0; column < COLUMNS; column++)
      {
        if (!isHit[row][column])
        {
          //Sets Brick bounds
          leftBrick = 10 * column;
          rightBrick = 10 * column + 10;
          topBrick = 6 * row + 1;
          bottomBrick = 6 * row + 7;

          //If A collison has occured
          if (topBall <= bottomBrick && bottomBall >= topBrick &&
              leftBall <= rightBrick && rightBall >= leftBrick)
          {
            Score();
            brickCount++;
            isHit[row][column] = true;
            arduboy.drawRect(10*column, 2+6*row, 8, 4, 0);

            //Vertical collision
            if (bottomBall > bottomBrick || topBall < topBrick)
            {
              //Only bounce once each ball move
              if(!bounced)
              {
                dy =- dy;
                yb += dy;
                bounced = true;
                if(sound)arduboy.tunes.tone(261, 200);
              }
            }
            //Hoizontal collision
            if (leftBall < leftBrick || rightBall > rightBrick)
            {
              //Only bounce once brick each ball move
              if(!bounced)
              {
                dx =- dx;
                xb += dx;
                bounced = true;
                if(sound)arduboy.tunes.tone(261, 200);
              }
            }
          }
        }
      }
    }
    //Reset Bounce
    bounced = false;
  }
  else
  {
    //Ball follows paddle
    xb=xPaddle + 7;

    //Release ball if FIRE pressed
    if (arduboy.pressed(B_BUTTON) && Breleased) {
      Breleased = false;
      released=true;

      //Apply random direction to ball on release
      dx = random(-1, 1);
      //Makes sure the ball heads upwards
      dy = -1;
    }
  }
}

void drawBall()
{
  // arduboy.setCursor(0,0);
  // arduboy.print(arduboy.cpuLoad());
  // arduboy.print("  ");
  arduboy.drawPixel(xb,   yb,   0);
  arduboy.drawPixel(xb+1, yb,   0);
  arduboy.drawPixel(xb,   yb+1, 0);
  arduboy.drawPixel(xb+1, yb+1, 0);

  moveBall();

  arduboy.drawPixel(xb,   yb,   1);
  arduboy.drawPixel(xb+1, yb,   1);
  arduboy.drawPixel(xb,   yb+1, 1);
  arduboy.drawPixel(xb+1, yb+1, 1);
}

void drawPaddle()
{
  arduboy.drawRect(xPaddle, 63, 16, 1, 0);
  movePaddle();
  arduboy.drawRect(xPaddle, 63, 16, 1, 1);
}

void drawLives()
{
  arduboy.setCursor(100,25);
  arduboy.print("LIVES");
  arduboy.setCursor(100,35);
  arduboy.print(lives);
}

void drawGameOver()
{
  arduboy.drawPixel(xb,   yb,   0);
  arduboy.drawPixel(xb+1, yb,   0);
  arduboy.drawPixel(xb,   yb+1, 0);
  arduboy.drawPixel(xb+1, yb+1, 0);
  arduboy.setCursor(52, 42);
  arduboy.print("Game");
  arduboy.setCursor(52, 54);
  arduboy.print("Over");
  arduboy.display();
  delay(4000);
}

void pause()
{
  paused = true;
  //Draw pause to the screen
  arduboy.setCursor(52, 45);
  arduboy.print("PAUSE");
  arduboy.display();
  while (paused) {
    //Unpause if FIRE is pressed
    if (arduboy.pressed(A_BUTTON) && Breleased) {
        arduboy.fillRect(52, 45, 30, 16, 0);
        paused=false;
    }
  }
}
void Score()
{
  score += (level*10);
  arduboy.setCursor(100, 45);
  arduboy.print("SCORE");
  arduboy.setCursor(100,55);
  arduboy.print(score);
}

void newLevel(){
  //Undraw paddle
  arduboy.drawRect(xPaddle, 63, 16, 1, 0);

  //Undraw ball
  arduboy.drawPixel(xb,   yb,   0);
  arduboy.drawPixel(xb+1, yb,   0);
  arduboy.drawPixel(xb,   yb+1, 0);
  arduboy.drawPixel(xb+1, yb+1, 0);

  //Alter various variables to reset the game
  xPaddle = 39;
  yb = 60;
  brickCount = 0;
  lives = lives + 1;
  released = false;

  //Draws new bricks and resets their values
  for (byte row = 0; row < 4; row++) {
    for (byte column = 0; column < 10; column++)
    {
      isHit[row][column] = false;
      arduboy.drawRect(10*column, 2+6*row, 8, 4, 1);
    }
  }

  //Draws the initial lives
  drawLives();

  //Draws the initial score & level
  arduboy.setCursor(100,5);
  arduboy.print("LEVEL");
  arduboy.setCursor(100,15);
  arduboy.print(level);
}

//Used to delay images while reading button input
boolean pollFireButton(int n)
{
  
  for(int i = 0; i < n; i++)
  {
    delay(15);
    if(arduboy.pressed(B_BUTTON && Breleased)) return true;
    else return false;
  }
}
//Function by nootropic design to display highscores
boolean displayHighScores(byte file)
{
  byte y = 10;
  byte x = 24;
  // Each block of EEPROM has 10 high scores, and each high score entry
  // is 5 bytes long:  3 bytes for initials and two bytes for score.
  int address = file*10*5;
  byte hi, lo;
  arduboy.clear();
  arduboy.setCursor(32, 0);
  arduboy.print("HIGH SCORES");
  arduboy.display();

  for(int i = 0; i < 10; i++)
  {
    sprintf(text, "%2d", i+1);
    arduboy.setCursor(x,y+(i*8));
    arduboy.print( text);
    arduboy.display();
    hi = EEPROM.read(address + (5*i));
    lo = EEPROM.read(address + (5*i) + 1);

    if ((hi == 0xFF) && (lo == 0xFF)) score = 0;
    else score = (hi << 8) | lo;

    initials[0] = (char)EEPROM.read(address + (5*i) + 2);
    initials[1] = (char)EEPROM.read(address + (5*i) + 3);
    initials[2] = (char)EEPROM.read(address + (5*i) + 4);

    if (score > 0) {
      sprintf(text, "%c%c%c %u", initials[0], initials[1], initials[2], score);
      arduboy.setCursor(x + 24, y + (i*8));
      arduboy.print(text);
      arduboy.display();
    }
  }
  if (pollFireButton(300))
  {
    return true;
  }
  return false;
  arduboy.display();
}

boolean titleScreen()
{
  //Clears the screen
  arduboy.clear();
  arduboy.setCursor(10,22);
  arduboy.setTextSize(2);
  arduboy.print("ARDUBREAK");
  arduboy.setTextSize(1);
  arduboy.display();
  if (pollFireButton(25)) return true;
  

  //Flash "Press FIRE" 5 times
  for(byte i = 0; i < 5; i++)
  {
    //Draws "Press FIRE"
    //arduboy.bitmap(31, 53, fire);  arduboy.display();
    arduboy.setCursor(31, 53);
    arduboy.print("PRESS FIRE!");
    arduboy.display();

    if (pollFireButton(50)) return true;
    
    //Removes "Press FIRE"
    arduboy.clear();
    arduboy.setCursor(10,22);
    arduboy.setTextSize(2);
    arduboy.print("ARDUBREAK");
    arduboy.setTextSize(1);
    arduboy.display();

    arduboy.display();
    if (pollFireButton(25)) return true;
  }
  return false;
}

//Function by nootropic design to add high scores
void enterInitials()
{
  char index = 0;

  arduboy.clear();

  initials[0] = ' ';
  initials[1] = ' ';
  initials[2] = ' ';

  while (true)
  {
    arduboy.display();
    arduboy.clear();

    arduboy.setCursor(16,0);
    arduboy.print("HIGH SCORE");
    arduboy.setCursor(88, 0);
    arduboy.print(score);
    arduboy.setCursor(56, 20);
    arduboy.print(initials[0]);
    arduboy.setCursor(64, 20);
    arduboy.print(initials[1]);
    arduboy.setCursor(72, 20);
    arduboy.print(initials[2]);
    for(byte i = 0; i < 3; i++)
    {
      arduboy.drawLine(56 + (i*8), 27, 56 + (i*8) + 6, 27, 1);
    }
    arduboy.drawLine(56, 28, 88, 28, 0);
    arduboy.drawLine(56 + (index*8), 28, 56 + (index*8) + 6, 28, 1);
    delay(150);

    if (arduboy.pressed(LEFT_BUTTON) && Breleased) {
      if(sound)arduboy.tunes.tone(1046, 200);
      index--;
      if (index < 0) index = 0;
    }

    if (arduboy.pressed(RIGHT_BUTTON) && Breleased) {
      if(sound)arduboy.tunes.tone(1046, 200);
      index++;
      if (index > 2) index = 2;
    }

    if (arduboy.pressed(DOWN_BUTTON)) {
      initials[index]++;
      if(sound)arduboy.tunes.tone(523, 150);
      // A-Z 0-9 :-? !-/ ' '
      if (initials[index] == '0') initials[index] = ' ';
      if (initials[index] == '!') initials[index] = 'A';
      if (initials[index] == '[') initials[index] = '0';
      if (initials[index] == '@') initials[index] = '!';
    }

    if (arduboy.pressed(UP_BUTTON)) {
      initials[index]--;
      if (sound)arduboy.tunes.tone(523, 150);
      if (initials[index] == ' ') initials[index] = '?';
      if (initials[index] == '/') initials[index] = 'Z';
      if (initials[index] == 31) initials[index] = '/';
      if (initials[index] == '@') initials[index] = ' ';
    }
    if (arduboy.pressed(B_BUTTON) && Breleased)
      if (index >= 2)
      {
        if (sound)arduboy.tunes.tone(1046, 200);
        return;
      }
  }
}

void enterHighScore(byte file)
{
  // Each block of EEPROM has 10 high scores, and each high score entry
  // is 5 bytes long:  3 bytes for initials and two bytes for score.
  int address = file * 10 * 5;
  byte hi, lo;
  char tmpInitials[3];
  unsigned int tmpScore = 0;

  // High score processing
  for(byte i = 0; i < 10; i++) {
    hi = EEPROM.read(address + (5*i));
    lo = EEPROM.read(address + (5*i) + 1);
    if ((hi == 0xFF) && (lo == 0xFF)) 
      // The values are uninitialized, so treat this entry
      // as a score of 0.
      tmpScore = 0;
    else tmpScore = (hi << 8) | lo;
    if (score > tmpScore)
    {
      enterInitials();
      for(byte j=i;j<10;j++) {
        hi = EEPROM.read(address + (5*j));
        lo = EEPROM.read(address + (5*j) + 1);

        if ((hi == 0xFF) && (lo == 0xFF)) tmpScore = 0;
        else tmpScore = (hi << 8) | lo;

        tmpInitials[0] = (char)EEPROM.read(address + (5*j) + 2);
        tmpInitials[1] = (char)EEPROM.read(address + (5*j) + 3);
        tmpInitials[2] = (char)EEPROM.read(address + (5*j) + 4);

        // write score and initials to current slot
        EEPROM.write(address + (5*j), ((score >> 8) & 0xFF));
        EEPROM.write(address + (5*j) + 1, (score & 0xFF));
        EEPROM.write(address + (5*j) + 2, initials[0]);
        EEPROM.write(address + (5*j) + 3, initials[1]);
        EEPROM.write(address + (5*j) + 4, initials[2]);

        // tmpScore and tmpInitials now hold what we want to
        //write in the next slot.
        score = tmpScore;
        initials[0] = tmpInitials[0];
        initials[1] = tmpInitials[1];
        initials[2] = tmpInitials[2];
      }

      score = 0;
      initials[0] = ' ';
      initials[1] = ' ';
      initials[2] = ' ';

      return;
    }
  }
}


void setup()
{
  arduboy.begin();
  arduboy.setFrameRate(50);
  arduboy.display();
  //arduboy.tunes.tone(987, 160);
  delay(160);
  //arduboy.tunes.tone(1318, 400);
  delay(2000);
}


void loop()
{
  // pause render until it's time for the next frame
  if (!(arduboy.nextFrame()))
    return;

  //Title screen loop switches from title screen
  //and high scores until FIRE is pressed
  while (!start)
  {
    start = titleScreen();
    score = 0;
    start = displayHighScores(2);
  }

  //Initial level draw
  if (!initialDraw)
  {
    //Clears the screen
    arduboy.display();
    arduboy.clear();
    //Draws the new level
    newLevel();
    initialDraw=true;
  }

  if (lives>0)
  {
    drawPaddle();

    //Pause game if FIRE pressed
    if(arduboy.pressed(A_BUTTON) && Breleased) pause();
    drawBall();

    if(brickCount == ROWS * COLUMNS)
    {
      score = score + (level * 100);
      level++;
      newLevel();
    }
  }
  else
  {
    drawGameOver();
    if (score > 0) enterHighScore(2);

    arduboy.clear();
    initialDraw=false;
    start=false;
    lives=2;
    score=0;
    level=1;
  }
  if (arduboy.pressed(UP_BUTTON)) sound = true;
  if (arduboy.pressed(DOWN_BUTTON)) sound = false;
  if (arduboy.notPressed(LEFT_BUTTON && RIGHT_BUTTON && UP_BUTTON && DOWN_BUTTON && A_BUTTON && B_BUTTON))
    Breleased = true;
    //print a tone if sound is on
  if (sound) {
    arduboy.drawRect(WIDTH / 2, 3, 2, 2, 1);
    arduboy.drawLine(WIDTH / 2 + 1,0,WIDTH / 2 + 1,3,1);
    arduboy.drawPixel(WIDTH / 2 + 2, 1, 1);
  }
  arduboy.display();
}
