const int interruptPin = 12;
const int recordPin = 13;
const int LEDPin = 10;

bool exitRecording = false;
bool onlyOnce = true;
bool recordVideo = LOW;
unsigned long interval = 0; // first time, reset below
unsigned long previousMillis = 0;

int brightness = 0;
int fadeAmount = 5;

void setup() {
  pinMode(LEDPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(recordPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), updateState, LOW);
}

void loop() {
  if (recordVideo) {
    digitalWrite(LEDPin, LOW);
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > interval) {
      interval = 1000 * 60 * 5; // ms
      previousMillis = currentMillis;
      pressRecord();
    }
    digitalWrite(LEDPin, HIGH);
    delay(200);
    digitalWrite(LEDPin, LOW);
    delay(200);
  } else {
    if (exitRecording) {
      exitRecording = false;
      onlyOnce = true;
      pressRecord();
    }
    analogWrite(LEDPin, brightness);
    brightness = brightness + fadeAmount;
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount;
    }
    delay(10);
  }
}

void pressRecord() {
  pinMode(recordPin, OUTPUT);
  digitalWrite(recordPin, HIGH); // finger down
  delay(500); // wait
  pinMode(recordPin, INPUT);

  delay(5000); // write video?
  if (onlyOnce == false) {
    pinMode(recordPin, OUTPUT);
    digitalWrite(recordPin, HIGH); // finger down
    delay(500); // wait
    pinMode(recordPin, INPUT);
  }
  onlyOnce = false; // create multiple videos
}

void updateState() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 200) {
    recordVideo = !recordVideo;
    if (recordVideo) {
      previousMillis = 0;
      onlyOnce = true;
    } else {
      exitRecording = true;
    }
  }
  last_interrupt_time = interrupt_time;
}
