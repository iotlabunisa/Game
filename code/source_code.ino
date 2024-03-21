#include "LedControl.h"

#include "binary.h"

const int a_button = 2;
const int b_button = 3;
const int c_button = 4;
const int d_button = 5;
const int e_button = 6;
const int f_button = 7;

const int red_light_pin = 9;
const int green_light_pin = 10;
const int blue_light_pin = 8;

// center coordinates of the plane
int plane_x;
int plane_y;
int planeCurrentDisplay;

// direction of the plane
int planeDirection;

// direction constants
const int LEFT = 0;
const int RIGHT = 1;
const int UP = 2;
const int DOWN = 3;

// target coordinates
int target_x;
int target_y;
int targetCurrentDisplay;

// bullet coordinates
int bullet_x;
int bullet_y;
int bulletCurrentDisplay;

// number of the 8x8 led matrices
const int numDevices = 4;
LedControl lc = LedControl(12, 13, 11, numDevices);

void setup() {

  // sets up RGB led pins
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);

  // sets up joystick buttons
  pinMode(a_button, INPUT);
  pinMode(c_button, INPUT);
  pinMode(d_button, INPUT);
  pinMode(b_button, INPUT);
  pinMode(f_button, INPUT);
  pinMode(e_button, INPUT);

  // sets up led matrices
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);

  lc.shutdown(1, false);
  lc.setIntensity(1, 8);
  lc.clearDisplay(1);

  lc.shutdown(2, false);
  lc.setIntensity(2, 8);
  lc.clearDisplay(2);

  lc.shutdown(3, false);
  lc.setIntensity(3, 8);
  lc.clearDisplay(3);

  RGB_color(0, 0, 0);
  startNewGame();
}

void generateNewTarget() {
  targetCurrentDisplay = random(0, 4);
  if (targetCurrentDisplay == planeCurrentDisplay) {
    if (planeCurrentDisplay == 3) {
      --targetCurrentDisplay;
    } else {
      ++targetCurrentDisplay;
    }
  }
  target_x = random(1, 7);
  target_y = random(1, 7);
}

void drawDown() {
  lc.setLed(planeCurrentDisplay, plane_x, plane_y, true);
  lc.setLed(planeCurrentDisplay, plane_x - 1, plane_y, true);
  lc.setLed(planeCurrentDisplay, plane_x + 1, plane_y, true);

  if (plane_y == 0) {
    lc.setLed(planeCurrentDisplay + 1, plane_x + 1, 7, true);
    lc.setLed(planeCurrentDisplay + 1, plane_x - 1, 7, true);
  } else {
    lc.setLed(planeCurrentDisplay, plane_x + 1, plane_y - 1, true);
    lc.setLed(planeCurrentDisplay, plane_x - 1, plane_y - 1, true);
  }

  if (plane_y == 7) {
    lc.setLed(planeCurrentDisplay - 1, plane_x, 0, true);
  } else {
    lc.setLed(planeCurrentDisplay, plane_x, plane_y + 1, true);
  }

}

void drawUp() {
  lc.setLed(planeCurrentDisplay, plane_x, plane_y, true);
  lc.setLed(planeCurrentDisplay, plane_x - 1, plane_y, true);
  lc.setLed(planeCurrentDisplay, plane_x + 1, plane_y, true);

  if (plane_y == 7) {
    lc.setLed(planeCurrentDisplay - 1, plane_x + 1, 0, true);
    lc.setLed(planeCurrentDisplay - 1, plane_x - 1, 0, true);
  } else {
    lc.setLed(planeCurrentDisplay, plane_x + 1, plane_y + 1, true);
    lc.setLed(planeCurrentDisplay, plane_x - 1, plane_y + 1, true);
  }

  if (plane_y == 0) {
    lc.setLed(planeCurrentDisplay + 1, plane_x, 7, true);
  } else {
    lc.setLed(planeCurrentDisplay, plane_x, plane_y - 1, true);
  }
}

void drawLeft() {
  lc.setLed(planeCurrentDisplay, plane_x, plane_y, true);
  lc.setLed(planeCurrentDisplay, plane_x + 1, plane_y, true);

  if (plane_y == 0) {
    lc.setLed(planeCurrentDisplay + 1, plane_x, 7, true);
    lc.setLed(planeCurrentDisplay + 1, plane_x - 1, 7, true);
  } else {
    lc.setLed(planeCurrentDisplay, plane_x, plane_y - 1, true);
    lc.setLed(planeCurrentDisplay, plane_x - 1, plane_y - 1, true);
  }

  if (plane_y == 7) {
    lc.setLed(planeCurrentDisplay - 1, plane_x, 0, true);
    lc.setLed(planeCurrentDisplay - 1, plane_x - 1, 0, true);
  } else {
    lc.setLed(planeCurrentDisplay, plane_x, plane_y + 1, true);
    lc.setLed(planeCurrentDisplay, plane_x - 1, plane_y + 1, true);
  }

}

void drawRight() {
  lc.setLed(planeCurrentDisplay, plane_x, plane_y, true);
  lc.setLed(planeCurrentDisplay, plane_x - 1, plane_y, true);

  if (plane_y == 0) {
    lc.setLed(planeCurrentDisplay + 1, plane_x, 7, true);
    lc.setLed(planeCurrentDisplay + 1, plane_x + 1, 7, true);
  } else {
    lc.setLed(planeCurrentDisplay, plane_x, plane_y - 1, true);
    lc.setLed(planeCurrentDisplay, plane_x + 1, plane_y - 1, true);
  }

  if (plane_y == 7) {
    lc.setLed(planeCurrentDisplay - 1, plane_x, 0, true);
    lc.setLed(planeCurrentDisplay - 1, plane_x + 1, 0, true);
  } else {
    lc.setLed(planeCurrentDisplay, plane_x, plane_y + 1, true);
    lc.setLed(planeCurrentDisplay, plane_x + 1, plane_y + 1, true);
  }
}

void draw() {
  clearDisplays();

  // draws the bullet
  lc.setLed(targetCurrentDisplay, target_x, target_y, true);

  // draws the plane in on of the directions
  switch (planeDirection) {
  case UP:
    drawUp();
    break;
  case DOWN:
    drawDown();
    break;
  case LEFT:
    drawLeft();
    break;
  case RIGHT:
    drawRight();
    break;
  default:
    break;
  }
}

void waitWhilePressed(int button, bool maxDelay) {
  int delaySum = 0;
  while (digitalRead(button) == LOW && delaySum < 200) {
    delay(1);
    if (maxDelay) {
      ++delaySum;
    }
  }
}

void clearDisplays() {
  for (int i = 0; i < 4; ++i) {
    lc.clearDisplay(i);
  }
}

void buttonUpPressed() {
  if (planeDirection != UP) {
    planeDirection = UP;
  } else {
    if (plane_y > 0) {
      --plane_y;
    } else if (planeCurrentDisplay < 3) {
      ++planeCurrentDisplay;
      plane_y = 7;
    }
  }
  draw();
  waitWhilePressed(a_button, 200);
}

void buttonDownPressed() {
  if (planeDirection != DOWN) {
    planeDirection = DOWN;
  } else {
    if (plane_y < 7) {
      ++plane_y;
    } else if (planeCurrentDisplay > 0) {
      --planeCurrentDisplay;
      plane_y = 0;
    }
  }
  draw();
  waitWhilePressed(c_button, 200);
}

void buttonLeftPressed() {
  if (planeDirection != LEFT) {
    planeDirection = LEFT;
  } else if (plane_x < 6) {
    ++plane_x;
  }
  draw();
  waitWhilePressed(d_button, 200);
}

void buttonRightPressed() {
  if (planeDirection != RIGHT) {
    planeDirection = RIGHT;
  } else if (plane_x > 1) {
    --plane_x;
  }
  draw();
  waitWhilePressed(b_button, 200);
}

bool checkCrash() {
  if (targetCurrentDisplay == planeCurrentDisplay && (target_x + 1 == plane_x || target_x - 1 == plane_x || target_x == plane_x) && (target_y + 1 == plane_y || target_y - 1 == plane_y || target_y == plane_y)) {
    return true;
  }
  return false;
}

void drawTargetExplosion() {
  for (int i = 0; i < 4; ++i) {
    lc.setLed(bulletCurrentDisplay, bullet_x - 1, bullet_y - 1, true);
    lc.setLed(bulletCurrentDisplay, bullet_x + 1, bullet_y + 1, true);
    lc.setLed(bulletCurrentDisplay, bullet_x + 1, bullet_y - 1, true);
    lc.setLed(bulletCurrentDisplay, bullet_x - 1, bullet_y + 1, true);
    draw();
    delay(50);
  }
}

bool targetHit() {
  if (bullet_x == target_x && bullet_y == target_y && bulletCurrentDisplay == targetCurrentDisplay) {
    RGB_color(0, 255, 0);
    drawTargetExplosion();
    generateNewTarget();
    draw();
    RGB_color(0, 0, 0);
    return true;
  }
  return false;
}

void shootLeft() {
  while (bullet_x < 8) {
    ++bullet_x;
    lc.setLed(bulletCurrentDisplay, bullet_x, bullet_y, true);
    delay(20);
    if (targetHit()) {
      return;
    }
    draw();
  }
}

void shootRight() {
  while (bullet_x >= 0) {
    --bullet_x;
    lc.setLed(bulletCurrentDisplay, bullet_x, bullet_y, true);
    delay(20);
    if (targetHit()) {
      return;
    }
    draw();
  }
}

void shootDown() {
  while (bullet_y < 8 && bulletCurrentDisplay >= 0) {
    if (bullet_y == 7) {
      bullet_y = 0;
      --bulletCurrentDisplay;
    } else {
      ++bullet_y;
    }

    lc.setLed(bulletCurrentDisplay, bullet_x, bullet_y, true);
    delay(20);
    if (targetHit()) {
      return;
    }
    draw();
  }
}

void shootUp() {
  while (bullet_y >= 0 && bulletCurrentDisplay <= 3) {
    if (bullet_y == 0) {
      bullet_y = 7;
      ++bulletCurrentDisplay;
    } else {
      --bullet_y;
    }

    lc.setLed(bulletCurrentDisplay, bullet_x, bullet_y, true);
    delay(20);
    if (targetHit()) {
      return;
    }
    draw();
  }
}

void shootButtonPressed() {
  bullet_x = plane_x;
  bullet_y = plane_y;
  bulletCurrentDisplay = planeCurrentDisplay;

  switch (planeDirection) {
  case LEFT:
    shootLeft();
    break;
  case RIGHT:
    shootRight();
    break;
  case UP:
    shootUp();
    break;
  case DOWN:
    shootDown();
    break;
  default:
    break;
  }
}

void generateNewTargetButtonPressed() {
  generateNewTarget();
  draw();
  waitWhilePressed(e_button, false);
}

void generateNewPlane() {
  plane_x = random(1, 7);
  plane_y = random(1, 7);
  planeDirection = random(0, 4);
  planeCurrentDisplay = random(0, 4);
}

void startNewGame() {
  generateNewPlane();
  generateNewTarget();
  draw();
}

void drawGameOver() {
  for (int j = 0; j < 8; ++j) {
    for (int k = 0; k < 8; ++k) {
      lc.setLed(0, j, k, true);
      lc.setLed(1, k, j, true);
      lc.setLed(2, j, k, true);
      lc.setLed(3, k, j, true);
      delay(5);
    }
  }
  delay(250);
}

void RGB_color(int red_light_value, int green_light_value, int blue_light_value) {
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);
}

void loop() {
  if (digitalRead(a_button) == LOW) {
    buttonUpPressed();
  } else if (digitalRead(c_button) == LOW) {
    buttonDownPressed();
  } else if (digitalRead(d_button) == LOW) {
    buttonLeftPressed();
  } else if (digitalRead(b_button) == LOW) {
    buttonRightPressed();
  } else if (digitalRead(f_button) == LOW) {
    shootButtonPressed();
  } else if (digitalRead(e_button) == LOW) {
    generateNewTargetButtonPressed();
  }

  if (checkCrash()) {
    RGB_color(255, 0, 0);
    drawGameOver();
    startNewGame();
    RGB_color(0, 0, 0);
  }
}
