const int inPinRed = A0;
const int inPinGreen = A1;
const int inPinBlue = A2;

const int outPinRed = 11;
const int outPinGreen = 10;
const int outPinBlue = 9;

int potValueRed = 0;
int potValueGreen = 0;
int potValueBlue = 0;

int ledValueRed = 0;
int ledValueGreen = 0;
int ledValueBlue = 0;

void setup() {
  pinMode(inPinRed, INPUT);
  pinMode(inPinGreen, INPUT);
  pinMode(inPinBlue, INPUT);

  pinMode(outPinRed, OUTPUT);
  pinMode(outPinGreen, OUTPUT);
  pinMode(outPinBlue, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  potValueRed = analogRead(inPinRed);
  potValueGreen = analogRead(inPinGreen);
  potValueBlue = analogRead(inPinBlue);

  ledValueRed = map(potValueRed, 0, 1023, 0, 255);
  ledValueGreen = map(potValueGreen, 0, 1023, 0, 255);
  ledValueBlue = map(potValueBlue, 0, 1023, 0, 255);

  printLedValues(ledValueRed, ledValueGreen, ledValueBlue);
  setColor(ledValueRed, ledValueGreen, ledValueBlue);
}

void printLedValues(int red, int green, int blue) {
  Serial.print("Red: ");
  Serial.print(red);
  Serial.print(" Green: ");
  Serial.print(green);
  Serial.print(" Blue: ");
  Serial.println(blue);
}

void setColor(int red, int green, int blue) {
  analogWrite(outPinRed, red);
  analogWrite(outPinGreen, green);
  analogWrite(outPinBlue, blue);
}
