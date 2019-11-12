const int speakerPin = A0;
const int buzzerPin = 11;
const int pushButton = 2;
const int interval = 1000;

// set the sensitivity
const int threshold = 5;

unsigned long int currentTime = 0;
unsigned long int previousTime = 0;

int speakerValue = 0;
int buzzerTone = 500;

int wasKnocked = 0;
int buttonState = 0;

int waitAfterPressing = 0;
int timeToWait = 500;

void setup() {
  pinMode(speakerPin, INPUT);
  pinMode(pushButton, INPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (waitAfterPressing == 0) {
    if (wasKnocked == 1) {
      currentTime = millis();
      if (currentTime - previousTime >= interval) {
        soundOn(buzzerPin, buzzerTone);
        wasKnocked = 0;
      }
    }
    else {
      speakerValue = analogRead(speakerPin);
      if (speakerValue > threshold) {
        Serial.println("Knocked");
        previousTime = millis();
        wasKnocked = 1;
      }

      buttonState = digitalRead(pushButton);
      if (buttonState == HIGH) {
        soundOff(buzzerPin);
        wasKnocked = 0;
        previousTime = millis();
        waitAfterPressing = 1;
      }
    }
  }
  else {
    currentTime = millis();
    if (currentTime - previousTime >= timeToWait) {
      waitAfterPressing = 0;
    }
  }

}

void soundOn(int buzzerPin, int buzzerTone) {
  Serial.println("Sound on");
  tone(buzzerPin, buzzerTone);
}

void soundOff(int buzzerPin) {
  Serial.println("Sound stopped");
  noTone(buzzerPin);
}
