const int inPinRed = A0;
const int inPinGreen = A1;
const int inPinBlue = A2;

const int outPinRed = 11;
const int outPinGreen = 10;
const int outPinBlue = 9;

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
  int potValueRed = analogRead(inPinRed);
  int potValueGreen = analogRead(inPinGreen);
  int potValueBlue = analogRead(inPinBlue);

  int ledValueRed = map(potValueRed, 0, 1023, 0, 255);
  int ledValueGreen = map(potValueGreen, 0, 1023, 0, 255);
  int ledValueBlue = map(potValueBlue, 0, 1023, 0, 255);

  Serial.print("Red: ");
  Serial.print(ledValueRed);
  Serial.print(" Green: ");
  Serial.print(ledValueGreen);
  Serial.print(" Blue: ");
  Serial.println(ledValueBlue);
  
  analogWrite(outPinRed, ledValueRed);
  analogWrite(outPinGreen, ledValueGreen);
  analogWrite(outPinBlue, ledValueBlue);
}
