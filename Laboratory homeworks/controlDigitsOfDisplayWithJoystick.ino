// joystick pins
const int pinSW = 4; // digital pin connected to switch output
const int pinX = A1; // A1 - analog pin connected to X output
const int pinY = A0; // A0 - analog pin connected to Y output

// segment pins
const int pinA = 12;
const int pinB = 8;
const int pinC = 5;
const int pinD = 3;
const int pinE = 2;
const int pinF = 11;
const int pinG = 6;
const int pinDP = A2;

// digit pins
const int pinD1 = 7;
const int pinD2 = 9;
const int pinD3 = 10;
const int pinD4 = 13;

const int noOfDisplays = 4;
const int noOfDigits = 10;
const int segSize = 8;

const int timeBetweenDpBlinks = 250;
const int timeBetweenSwitches = 50;

unsigned long lastDpTime = 0;
unsigned long lastSwitchTime = 0;
unsigned long currentTime = 0;

int currentDisplay = 0;
int currentDpState = LOW;
int digitIsLocked = 0;

int switchValue;
int xValue = 0;
int yValue = 0;

bool joyCanMove = false;
int minThreshold = 400;
int maxThreshold = 600;

// segments array
int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

// digits array
int digits[noOfDisplays] = {
  pinD1, pinD2, pinD3, pinD4
};

// number on display array
int currentNumber[noOfDisplays] {
  4, 3, 2, 1
};

// state of decimal point array
byte dpState[noOfDisplays] {
  LOW, LOW, LOW, LOW
};

// matrix to form the 0-9 numbers
byte digitMatrix[noOfDigits][segSize - 1] = {
  // a  b  c  d  e  f  g
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1} // 9
};

// displays the 'digit' number on the current display
void displayNumber(byte digit, byte decimalPointState) {
  for (int i = 0; i < segSize - 1; i++) {
    digitalWrite(segments[i], digitMatrix[digit][i]);
  }
  // write the decimalPointState
  digitalWrite(segments[segSize - 1], decimalPointState);
}

// activate the display number 'num' received as parameter
void showDigit(int num) {
  for (int i = 0; i < noOfDisplays; i++) {
    digitalWrite(digits[i], HIGH);
  }
  digitalWrite(digits[num], LOW);
}

void moveOneDigitLeft() {
  dpState[currentDisplay] = 0;
  if (currentDisplay < 3) {
    currentDisplay++;
  } else {
    currentDisplay = 0;
  }
  joyCanMove = false;
}

void moveOneDigitRight() {
  dpState[currentDisplay] = 0;
  if (currentDisplay > 0) {
    currentDisplay--;
  } else {
    currentDisplay = 3;
  }
  joyCanMove = false;
}

void checkDpState() {
  if (currentTime - lastDpTime  >= timeBetweenDpBlinks) {
    dpState[currentDisplay] = !dpState[currentDisplay];
    lastDpTime = currentTime;
  }
}

void increaseDigitValue() {
  if (currentNumber[currentDisplay] > 0) {
    currentNumber[currentDisplay]--;
  } else {
    currentNumber[currentDisplay] = 9;
  }
  joyCanMove = false;
}

void decreaseDigitValue() {
  if (currentNumber[currentDisplay] < 9) {
    currentNumber[currentDisplay]++;
  } else {
    currentNumber[currentDisplay] = 0;
  }
  joyCanMove = false;
}

void displayAllNumbers() {
  for (int digit = 0; digit < noOfDisplays; digit++) {
    showDigit(digit);
    displayNumber(currentNumber[digit], dpState[digit]);
    delay(5);
  }
}

// check if the current digit must be locked
void checkLockUnlock() {
  if (switchValue == 0 && currentTime - lastSwitchTime >= timeBetweenSwitches) {
    if (digitIsLocked == 0) {
      // lock the current digit and set the decimal point on
      digitIsLocked = 1;
      dpState[currentDisplay] = 1;
    }
    else {
      // unlock the current digit
      digitIsLocked = 0;
    }
    lastSwitchTime = currentTime;
  }
}

void setup() {
  pinMode(pinSW, INPUT_PULLUP); //activate pull-up resistor on the push-button pin

  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
  for (int i = 0; i < noOfDisplays; i++) {
    pinMode(digits[i], OUTPUT);
  }
  Serial.begin(9600);
}

void loop() {
  int digit;
  currentTime = millis();

  switchValue = digitalRead(pinSW);
  checkLockUnlock();

  if (digitIsLocked == 0) {
    // we are still moving between digits

    xValue = analogRead(pinX);

    // On Ox axis, if the value is lower than the min treshold and we can make a move, then we move one digit to the left
    if (xValue < minThreshold && joyCanMove == true)
      moveOneDigitLeft();

    // On Ox axis, if the value is bigger than the max treshold and we can make a move, then we move one digit to the right
    if (xValue > maxThreshold && joyCanMove == true)
      moveOneDigitRight();

    // the joystick is in the original position, so we can move
    if (xValue >= minThreshold && xValue <= maxThreshold) {
      joyCanMove = true;
    }

    // change digital point state for the current display after a certain amount of time
    checkDpState();
  }
  else {
    yValue = analogRead(pinY);
    // On OY axis, if the value is lower than the min treshold  and we can make a move, then we decrease the number displayed on the digit
    if (yValue < minThreshold && joyCanMove == true)
      increaseDigitValue();

    // On Oy axis, if the value is bigger than the max treshold and we can make a move, then we increase the number displayed on the digit
    if (yValue > maxThreshold && joyCanMove == true)
      decreaseDigitValue();

    // the joystick is in the original position, so we can move
    if (yValue >= minThreshold && yValue <= maxThreshold) {
      joyCanMove = true;
    }
  }

  // display the numbers and the decimal point
  displayAllNumbers();
}
