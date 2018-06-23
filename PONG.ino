/*************************************
*                                    *
  #    #                             *
   #  #                      *       *
    ##                       *       *
    ##       ***** ***** ***** ***** *
   #  #      *     *   * *   * ***** *
  #    #     ***** ***** ***** ***** *
*                                    *
*************************************/
#include <Arduboy.h>
//startup sound at line 260
Arduboy arduboy;
//Variables declared here
int8_t gamestate = 0;
bool released = 0;
short ballx = WIDTH / 2;
float bally = HEIGHT / 2;
int8_t ballsize = 3;
int8_t movex = 2;
float movey = 0;
int8_t paddlex = 3;
int8_t paddley = 15;
int8_t player1x = 0;
int8_t player1y = HEIGHT / 2 - (paddley / 2);
short player2x = WIDTH - paddlex;
int8_t player2y = HEIGHT / 2 - (paddley / 2);
unsigned short player1score = 0;
unsigned short player2score = 0;
bool twoplayer = false;
bool demo = false;
bool nonsense = true;
bool flash = true;
void resetgame() {
  ballx = WIDTH / 2;
  bally = HEIGHT / 2;
  player1score = 0;
  player2score = 0;
  player1y = player2y = HEIGHT / 2 - paddley / 2;
  if (random(0, 2) >= 1) movex = 2;
  else movex = -2;
  movey = 0;
}
void printGameMode() {
  if (demo) {
    arduboy.setCursor(50, 45);
    arduboy.setTextSize(1);
    arduboy.print("DEMO");
    arduboy.setTextSize(3);
  }
  //if not (it should not be)
  else {
    arduboy.setCursor(35, 45);
    arduboy.setTextSize(1);
    if (twoplayer) arduboy.print("TWO PLAYER");
    else arduboy.print("ONE PLAYER");
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
    if (demo) demo = false;
    else {
      twoplayer = false;
      demo = true;
    }
  }
  //toggle twoplayer
  if ((arduboy.pressed(UP_BUTTON) || arduboy.pressed(DOWN_BUTTON)) && released && !demo) {
    released = false;
    arduboy.tunes.tone(523, 200);
    if (twoplayer) twoplayer = false;
    else twoplayer = true;
  }
  //toggle sound
  if (arduboy.pressed(A_BUTTON) && released) {
    released = false;
    if (arduboy.audio.enabled())arduboy.audio.off();
    else arduboy.audio.on();
    arduboy.audio.saveOnOff();
  }
}
void player1AI() {
  if (ballx < WIDTH / 2) {
    //if ball is at field and is travelling opposite direction very fast
    if (ballx > WIDTH / 4 && abs(movey) > 1.5 && movex < 0) {
      //move against it because it is going to bonce
      if (bally > HEIGHT / 2) player1y -= 1;
      if (bally < HEIGHT / 2) player1y += 1;
    } else if (ballx < WIDTH / 2) {
      //If the top of the ball is higher than the center of player's paddle, move the player's paddle up
      if (bally < player1y + (paddley / 2)) player1y -= 1;
      //If the bottom of the ball is lower than the center of the player's paddle, move the player's paddle down
      if (bally  + ballsize > player1y + (paddley / 2)) player1y += 1;
    }
  }
}
void ball() {
  //Draw the ball
  arduboy.fillRect(ballx, bally, ballsize, ballsize, 1);
  //Move the ball
  ballx = ballx + movex;
  bally = bally + movey;
  //Reflect the ball off of the top of the screen
  if (bally < 0) {
    movey = -movey;
    bally = 0;
    arduboy.tunes.tone(523, 200);
  }
  //Reflect the ball off of the bottom of the screen
  if (bally + ballsize > HEIGHT) {
    movey = -movey;
    bally = HEIGHT - ballsize;
    arduboy.tunes.tone(523, 200);
  }
}
void player2AI() {
  if (ballx > WIDTH / 2) {
    //If the top of the ball is higher than the center of player2's paddle, move the player2's paddle up
    if (bally < player2y + (paddley / 2)) player2y -= 1;
    //If the bottom of the ball is lower than the center of the player2's paddle, move the comptuer's paddle down
    if (bally  + ballsize > player2y + (paddley / 2)) player2y += 1;
  }
  else if (5 < abs(player2y + paddley / 2 - HEIGHT / 2)) {
    if (player2y + (paddley / 2) > HEIGHT / 2) player2y -= 1;
    if (player2y + (paddley / 2) < HEIGHT / 2) player2y += 1;
  }
}
void score() {
  //If the ball moves off of the screen to the left...
  if (ballx < 1) {
    //Move the ball back to the middle of the screen
    ballx = WIDTH / 2;
    movey = 0;
    bally = HEIGHT / 2;
    arduboy.tunes.tone(175, 200);
    //Give the player2 a point
    player2score = player2score + 1;
  }
  //If the ball moves off of the screen to the right....
  if (ballx > WIDTH - 1) {
    //Move the ball back to the middle of the screen
    ballx = WIDTH / 2;
    movey = 0;
    bally = HEIGHT / 2;
    arduboy.tunes.tone(175, 200);
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
    if (arduboy.pressed(UP_BUTTON) && player1y > 0) player1y = player1y - 1;
    //If the player presses down and the paddle is not touching the bottom of the screen, move the paddle down
    if (arduboy.pressed(DOWN_BUTTON) && player1y + paddley < HEIGHT) player1y = player1y + 1;
  }
  //if in demo mode
  else
    player1AI();
  //keep the paddle inside the game
  if (player1y < 0) player1y = 0;
  if (player1y > HEIGHT - paddley) player1y = HEIGHT - paddley;
  //If the ball makes contact with the player's paddle, bounce it back to the right
  if (ballx <= player1x + paddlex && player1y < bally + ballsize && player1y + paddley > bally && ballx >= 0) {
    movey = movey + (bally - player1y - (paddley / 2)) / 3.5; //Applies spin on the ball
    //horizontal speed limit
    if (movey > 2.5) movey  = 2.5;
    if (movey < -2.5) movey = -2.5;
    //prevent straight bonce
    if (movey == 0) movey = random(-10, 10) * 0.1;
    //bonce it back
    movex = -movex;
    //sounds
    arduboy.tunes.tone(200, 200);
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
  if (player2y < 0) player2y = 0;
  if (player2y > HEIGHT - paddley) player2y = HEIGHT - paddley;
  //If the ball makes contact with the player2's paddle, bounce it back to the left
  if (ballx + ballsize >= player2x && player2y < bally + ballsize && player2y + paddley > bally && ballx + ballsize < WIDTH) {
    movey = movey + (bally - player2y - (paddley / 2)) / 3.5; //Applies spin on the ball
    //horizontal speed limit
    if (movey > 2.5) movey  = 2.5;
    if (movey < -2.5) movey = -2.5;
    //prevent straight bonce
    if (movey == 0) movey = random(-10, 10) * 0.1;
    //bonce it back
    movex = -movex;
    //sounds
    arduboy.tunes.tone(200, 200);
  }
}
void setup() {
  arduboy.begin();
  arduboy.setFrameRate(30);
  arduboy.initRandomSeed();
  if (arduboy.audio.enabled()) arduboy.audio.on();
  else arduboy.audio.off();
  arduboy.tunes.tone(987, 160);
  delay(160);
  arduboy.tunes.tone(1318, 400);
}
void loop() {
  //Prevent the Arduboy from running too fast
  if (!arduboy.nextFrame()) return;
  arduboy.clear();
  switch ( gamestate ) {
    case 0: { //Title screen
        arduboy.setCursor(30, 15);
        arduboy.setTextSize(3);
        arduboy.print("PONG");
        arduboy.setCursor(31, 53);
        arduboy.setTextSize(1);
        if (arduboy.everyXFrames(15)) flash = !flash;
        if (flash)printGameMode();
        //Change the gamestate
        if (arduboy.pressed(RIGHT_BUTTON) && released) {
          released = false;
          gamestate = 1;
        }
        toggleGameState();
      }
      break;
    case 1: {
        if (demo) arduboy.setFrameRate(45);
        else arduboy.setFrameRate(30);
        //Gameplay screen
        //draws the seperation line
        arduboy.drawRect(WIDTH / 2 - 1, 0, 2, 7, 1);
        arduboy.drawRect(WIDTH / 2 - 1, 14, 2, 7, 1);
        arduboy.drawRect(WIDTH / 2 - 1, 28, 2, 7, 1);
        arduboy.drawRect(WIDTH / 2 - 1, 42, 2, 7, 1);
        arduboy.drawRect(WIDTH / 2 - 1, 56, 2, 7, 1);
        //Display the players' score
        arduboy.setCursor(20, 0);
        arduboy.setTextSize(3);
        arduboy.print(player1score);
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
      break;
    case 2: {
        //Win screen
        arduboy.setCursor(20, 20);
        arduboy.setTextSize(2);
        arduboy.print("PLAYER 1");
        arduboy.setCursor(40, 40);
        arduboy.print("WINS!");
        arduboy.setTextSize(3);
        if (nonsense) {
          nonsense = false;
          arduboy.tunes.tone(987, 160);
          delay(160);
          arduboy.tunes.tone(1318, 400);
        }
        //Change the gamestate
        if (arduboy.pressed(LEFT_BUTTON) and released) {
          released = false;
          nonsense = true;
          resetgame();
          gamestate = 0;
        }
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
        if (nonsense) {
          nonsense = false;
          arduboy.tunes.tone(987, 160);
          delay(160);
          arduboy.tunes.tone(1318, 400);
        }
        //Change the gamestate
        if (arduboy.pressed(LEFT_BUTTON) and released) {
          released = false;
          resetgame();
          nonsense = true;
          gamestate = 0;
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
          resetgame();
          gamestate = 0;
        }
        if (arduboy.pressed(A_BUTTON) && released) {
          released = false;
          if (arduboy.audio.enabled()) arduboy.audio.off();
          else arduboy.audio.on();
          arduboy.audio.saveOnOff();
        }
      }
  }
  if (arduboy.audio.enabled()) {
    arduboy.drawRect(WIDTH / 2 + 3, 3, 2, 2, 1);
    arduboy.drawLine(WIDTH / 2 + 4, 0, WIDTH / 2 + 4, 4, 1);
    arduboy.drawPixel(WIDTH / 2 + 5, 1, 1);
  }
  //Check if the button is being held down
  if (arduboy.notPressed(LEFT_BUTTON) && arduboy.notPressed(RIGHT_BUTTON) && arduboy.notPressed(UP_BUTTON) && arduboy.notPressed(DOWN_BUTTON) && arduboy.notPressed(B_BUTTON) && arduboy.notPressed(A_BUTTON))
    released = true;
  arduboy.display();
}
