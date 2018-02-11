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
#include <Arduboy.h>
//startup beep at 281
Arduboy arduboy;
//Variables declared here
short gamestate = 0;
boolean released = 0;
int ballx = WIDTH / 2;
int bally = HEIGHT / 2;
short ballsize = 3;
short i;
short movex = 2;
short movey = 0;
short paddlex = 3;
short paddley = 15;
short player1x = 0;
int player1y = HEIGHT / 2 - (paddley / 2);
short player2x = WIDTH - paddlex;
int player2y = HEIGHT / 2 - (paddley / 2);
short player1score = 0;
short player2score = 0;
boolean twoplayer = false;
boolean demo = false;
boolean sound = true;
boolean nonsense = true;

void resetgame() {
  ballx = WIDTH / 2 - (ballsize / 2);
  bally = HEIGHT / 2 - (ballsize / 2);
  player1score = 0;
  player2score = 0;
  player1y = player2y = HEIGHT / 2 - paddley / 2;
  if (random(0, 2) >= 1) movex = 2;
  else movex - -2;
  movey = 0;
}
void printgamemode() {
  if (demo) {
    arduboy.setCursor(50, 45);
      arduboy.setTextSize(1);
      arduboy.print("DEMO");
      arduboy.setTextSize(3);
      twoplayer = false;
  }
  //if not (it should not be)
  else
  //check if it is two player
  if (twoplayer) {
      arduboy.setCursor(35, 45);
      arduboy.setTextSize(1);
      arduboy.print("TWO PLAYER");
      arduboy.setTextSize(3);
  }
  //if not
  else {
      arduboy.setCursor(35, 45);
      arduboy.setTextSize(1);
      arduboy.print("ONE PLAYER");
      arduboy.setTextSize(3);
  }
}
void toggleGameState() {
  if (arduboy.pressed(RIGHT_BUTTON) && released) {
    released = false;
    resetgame();
    gamestate = 1;
  }
  //toggle demo
  if (((arduboy.pressed(UP_BUTTON) || arduboy.pressed(DOWN_BUTTON)) && released) && arduboy.pressed(A_BUTTON)) {
    released = false;
    if(sound)arduboy.tunes.tone(523, 200);
    if (demo) demo = false;
    else {
      twoplayer = false;
      demo = true;
    }
  }
  //toggle twoplayer
  if ((arduboy.pressed(UP_BUTTON) || arduboy.pressed(DOWN_BUTTON)) && released && !demo) {
    released = false;
    if(sound)arduboy.tunes.tone(523, 200);
    if (twoplayer) twoplayer = false;
    else twoplayer = true;
  }
  //toggle sound
  if (arduboy.pressed(B_BUTTON) && released) {
    released = false;
    if(sound) sound = false;
    else sound = true;
  }
}
void titleScreen(){
  //Title screen
  arduboy.setCursor(30, 15);
  arduboy.setTextSize(3);
  arduboy.print("PONG");
  //Change the gamestate
  if (arduboy.pressed(RIGHT_BUTTON) && released) {
    released = false;
    gamestate = 1;
  }
  printgamemode();
  toggleGameState();
}
void player1AI() {
  if (ballx < WIDTH / 2) {
  //if ball is at field and is travelling opposite direction very fast
  if (ballx > WIDTH / 4 && abs(movey) > 1) {
    //move against it because it is going to bonce
    if (bally > HEIGHT / 2) player1y -= 1;
    if (bally < HEIGHT / 2) player1y += 1;
  } else if (ballx < WIDTH / 2){
    //If the top of the ball is higher than the center of player's paddle, move the player's paddle up
    if (bally < player1y + (paddley / 2)) player1y -= 1;
    //If the bottom of the ball is lower than the center of the player's paddle, move the player's paddle down
    if (bally  + ballsize > player1y + (paddley / 2)) player1y += 1;
    //if ball is at middle and is going very fast diagnally
    }
  }else if (5 < abs(player1y + paddley / 2 - HEIGHT / 2)){
    if (player1y + (paddley / 2) > HEIGHT / 2) player1y -=1;
    if (player1y + (paddley / 2) < HEIGHT / 2) player1y +=1;
  }
}
void ball() {
  //Draw the ball
  arduboy.fillRect(ballx, bally, ballsize, ballsize, 1);
  //Move the ball
  ballx = ballx + movex;
  bally = bally + movey;
  //Reflect the ball off of the top of the screen
  if (bally <= 0) {
    movey = -movey;
    bally = 0;
    if(sound)arduboy.tunes.tone(523, 200);
  }
  //Reflect the ball off of the bottom of the screen
  if (bally + ballsize >= HEIGHT) {
    movey = -movey;
    bally = HEIGHT - ballsize;
    if(sound)arduboy.tunes.tone(523, 200);
  }
}
void player2AI() 
{
  if (ballx > WIDTH / 2) {
    //If the top of the ball is higher than the center of player2's paddle, move the player2's paddle up
    if (bally < player2y + (paddley / 2)) player2y -= 1;
    //If the bottom of the ball is lower than the center of the player2's paddle, move the comptuer's paddle down
    if (bally  + ballsize > player2y + (paddley / 2)) player2y += 1;
  }
  else if (5 < abs(player2y + paddley / 2 - HEIGHT / 2)){
    if (player2y + (paddley / 2) > HEIGHT / 2) player2y -=1;
    if (player2y + (paddley / 2) < HEIGHT / 2) player2y +=1;
  }
}
void score() {
  //If the ball moves off of the screen to the left...
  if (ballx < 0) {
    //Move the ball back to the middle of the screen
    ballx = WIDTH / 2;
    movey = 0;
    bally = HEIGHT / 2;
    if(sound)arduboy.tunes.tone(175, 200);
    //Give the player2 a point
    player2score = player2score + 1;
  }
  //If the ball moves off of the screen to the right....
  if (ballx > WIDTH) {
    //Move the ball back to the middle of the screen
    ballx = WIDTH / 2;
    movey = 0;
    bally = HEIGHT / 2;
    if(sound)arduboy.tunes.tone(175, 200);
    //Give the player1 a point
    player1score = player1score + 1;
  }
  //Check if the player1 wins
  if (player1score >= 5) gamestate = 2;
  //Check if the player2 wins
  if (player2score >= 5) gamestate = 3;
}
void player1() {
  //Draw the player1's paddle
  arduboy.fillRect(player1x, player1y, paddlex, paddley, WHITE);
  //if NOT in demo mode
  if (!demo) {
    //If the player presses Up and the paddle is not touching the top of the screen, move the paddle up
    if (arduboy.pressed(UP_BUTTON) and player1y > 0) player1y = player1y - 1;
    //If the player presses down and the paddle is not touching the bottom of the screen, move the paddle down
    if (arduboy.pressed(DOWN_BUTTON) and player1y + paddley < HEIGHT) player1y = player1y + 1;
  }
  //if in demo mode
  else 
    player1AI();
  //keep the paddle inside the game
  if (player1y <= 0) player1y = 0;
  if (player1y >= HEIGHT - paddley) player1y = HEIGHT - paddley;
  //If the ball makes contact with the player's paddle, bounce it back to the right
  if (ballx <= player1x + paddlex && player1y < bally + ballsize && player1y + paddley > bally && ballx >= 0) {
    movey = (bally - player1y - (paddley / 2)) / 2; //Applies spin on the ball
    //limit the horizontal speed
    if (movey > 2) movey = 2;
    if (movey < -2) movey = -2;
    //prevent straight bonce
    if (movey == 0) movey = random(-1, 1);
    //bonce it back
    movex = -movex;
    //sounds
    if(sound)arduboy.tunes.tone(200, 200);
  }
}
void player2() {
  //Draw the player2's paddle
  arduboy.fillRect(player2x, player2y, paddlex, paddley, WHITE);
  if (twoplayer) {
    //If the player presses Up and the paddle is not touching the top of the screen, move the paddle up
    if (arduboy.pressed(B_BUTTON) and player2y > 0) player2y = player2y - 1;
    //If the player presses down and the paddle is not touching the bottom of the screen, move the paddle down
    if (arduboy.pressed(A_BUTTON) and player2y + paddley < 63) player2y = player2y + 1;
  }
  //if not in two player mode
  else
    player2AI();
  //keep the paddle inside the game
  if (player2y <= 0) player2y = 0;
  if (player2y >= HEIGHT - paddley) player2y = HEIGHT - paddley;
  //If the ball makes contact with the player2's paddle, bounce it back to the left
  if (ballx + ballsize >= player2x && player2y < bally + ballsize && player2y + paddley > bally && ballx + ballsize < WIDTH) {
    movey = (bally - player1y - (paddley / 2)) / 2; //Applies spin on the ball
    //horizontal speed limit
    if (movey > 2) movey  = 2;
    if (movey < -2) movey = -2;
    //prevent straight bonce
    if (movey == 0) movey = random(-1, 1);
    //bonce it back
    movex = -movex;
    if(sound)arduboy.tunes.tone(200, 200);
  }
}
void gameScreen() {
  if(demo) arduboy.setFrameRate(45);
      else arduboy.setFrameRate(30);
      //Gameplay screen
      //draws the seperation line
      arduboy.drawRect(WIDTH / 2 -1,0,2,7,1);
      arduboy.drawRect(WIDTH / 2 -1,14,2,7,1);
      arduboy.drawRect(WIDTH / 2 -1,28,2,7,1);
      arduboy.drawRect(WIDTH / 2 -1,42,2,7,1);
      arduboy.drawRect(WIDTH / 2 -1,56,2,7,1);
      //Display the player's score
      arduboy.setCursor(20, 0);
      arduboy.print(player1score);
      //Display the player2's score
      arduboy.setCursor(WIDTH - 33, 0);
      arduboy.print(player2score);
      ball();
      player1();
      player2();
      score();
      //Pause
      if (arduboy.pressed(LEFT_BUTTON) && released) {
        released = false;
        gamestate = 4;
     }
}
void setup() {
  arduboy.begin();
  //Set the game to 30 frames per second
  arduboy.setFrameRate(30);
  arduboy.initRandomSeed();
  arduboy.clear();
  arduboy.tunes.tone(987, 160);
  delay(160);
  arduboy.tunes.tone(1318, 400);
}

void loop() {
  //Prevent the Arduboy from running too fast
  if (!arduboy.nextFrame()) {
    return;
  }
  arduboy.clear();
  switch ( gamestate ) {
    case 0:
      titleScreen();
      break;
    case 1:
      gameScreen();
      break;
    case 2:
      arduboy.setCursor(20, 20);
      arduboy.setTextSize(2);
      arduboy.print("PLAYER 1");
      arduboy.setCursor(40, 40);
      arduboy.print("WINS!");
      arduboy.setTextSize(3);
      if (sound && nonsense) {
        nonsense = false;
        delay(100);
        arduboy.tunes.tone(987, 160);
        delay(160);
        arduboy.tunes.tone(1318, 400);
      }
      //Change the gamestate
      if (arduboy.pressed(LEFT_BUTTON) and released) {
        released = false;
        resetgame();
        gamestate = 0;
      }
      break;
    case 3: {
        //Game over screen
        arduboy.setCursor(20, 20);
        arduboy.setTextSize(2);
        arduboy.print("PLAYER 2");
        arduboy.setCursor(40, 40);
        arduboy.print("WINS!");
        arduboy.setTextSize(3);
        if (sound && nonsense) { 
          nonsense = false;
          delay(100);
          arduboy.tunes.tone(987, 160);
          delay(160);
          arduboy.tunes.tone(1318, 400);
        }
        //Change the gamestate
        if (arduboy.pressed(LEFT_BUTTON) and released) {
          released = false;
          resetgame();
          gamestate = 0;
          nonsense = true;
        }
      }
      break;
    case 4: {
      //pause screen
      arduboy.clear();
      arduboy.setCursor(35, 25);
      arduboy.setTextSize(2);
      arduboy.print("PAUSE");
      arduboy.setTextSize(3);
      if (arduboy.pressed(RIGHT_BUTTON) && released) {
        released = false;
        gamestate = 1;
      }
      if (arduboy.pressed(LEFT_BUTTON) && released) {
        released = false;
        gamestate = 0;
        resetgame();
        nonsense = true;
      }
      //toggle sound
      if (arduboy.pressed(B_BUTTON) && released) {
        released = false;
        if(sound) sound = false;
        else sound = true;
      }
    }
  }
  //print a note if sound is on
  if (sound) {
    arduboy.drawRect(WIDTH / 2 + 2, 3, 2, 2, 1);
    arduboy.drawLine(WIDTH / 2 + 3,0,WIDTH / 2 + 3,3,1);
    arduboy.drawPixel(WIDTH / 2 + 4, 1, 1);
  }
  //Check if the button is being held down
  if (arduboy.notPressed(LEFT_BUTTON) && arduboy.notPressed(RIGHT_BUTTON) && arduboy.notPressed(UP_BUTTON) && arduboy.notPressed(DOWN_BUTTON) && arduboy.notPressed(B_BUTTON))
    released = true;
  arduboy.display();
}
