// Traffic Light Pins Definitions
const int trafficLights[4][3] = {
    {13, 12, 11}, // Red, Yellow, Green for Traffic Light 1
    {10, 9, 8},   // Red, Yellow, Green for Traffic Light 2
    {7, 6, 5},    // Red, Yellow, Green for Traffic Light 3
    {4, 3, 2}     // Red, Yellow, Green for Traffic Light 4
  };
  
  // Ultrasonic sensor Trig and Echo pins (2 per section)
  const int trigPins[8] = {23, 27, 31, 35, 39, 43, 47, 51}; // Trigger
  const int echoPins[8] = {22, 26, 30, 34, 38, 42, 46, 50}; // Echo
  
  // Emergency buzzer pin (passive buzzer)
  const int buzzerPin = 52;
  
  // Distance threshold to detect a car
  const int vehicleThreshold = 6; // cm (adjusted for small demo lanes)
  
  // Green light timing
  const int maxGreenTime = 10000;
  const int minGreenTime = 5000;
  const int yellowTime = 2000;
  const int redBufferTime = 1000;
  
  // Emergency settings
  const int emergencyDuration = 6000; // 6 sec
  
  void setup() {
    Serial.begin(9600);
  
    // Traffic lights
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 3; j++) {
        pinMode(trafficLights[i][j], OUTPUT);
        digitalWrite(trafficLights[i][j], LOW);
      }
    }
  
    // Ultrasonic sensors
    for (int i = 0; i < 8; i++) {
      pinMode(trigPins[i], OUTPUT);
      pinMode(echoPins[i], INPUT);
    }
  
    // Buzzer
    pinMode(buzzerPin, OUTPUT);
    digitalWrite(buzzerPin, LOW);
  
    allRed();
    randomSeed(analogRead(A0)); // Random seed for emergency
  }
  
  // Distance sensor
  int getDistance(int trigPin, int echoPin) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
  
    long duration = pulseIn(echoPin, HIGH, 20000);
    if (duration == 0) return 50;
    return duration * 0.034 / 2;
  }
  
  // Traffic score
  int trafficScore(int laneIndex) {
    int s1 = laneIndex * 2;
    int s2 = s1 + 1;
  
    int d1 = getDistance(trigPins[s1], echoPins[s1]);
    int d2 = getDistance(trigPins[s2], echoPins[s2]);
  
    Serial.print("Lane "); Serial.print(laneIndex + 1);
    Serial.print(" Sensors: "); Serial.print(d1);
    Serial.print(" cm, "); Serial.println(d2);
  
    int score = 0;
    if (d1 < vehicleThreshold) score++;
    if (d2 < vehicleThreshold) score++;
  
    return score;
  }
  
  // Green time
  int computeGreenTime(int score) {
    if (score == 2) return maxGreenTime;
    else if (score == 1) return (maxGreenTime + minGreenTime) / 2;
    else return minGreenTime;
  }
  
  // All lights red
  void allRed() {
    for (int i = 0; i < 4; i++) {
      digitalWrite(trafficLights[i][0], HIGH);
      digitalWrite(trafficLights[i][1], LOW);
      digitalWrite(trafficLights[i][2], LOW);
    }
  }
  
  // Control lane
  void controlTrafficLight(int lane, int greenTime) {
    allRed();
    delay(redBufferTime);
  
    digitalWrite(trafficLights[lane][0], LOW);
    digitalWrite(trafficLights[lane][2], HIGH);
    delay(greenTime);
  
    digitalWrite(trafficLights[lane][2], LOW);
    digitalWrite(trafficLights[lane][1], HIGH);
    delay(yellowTime);
  
    digitalWrite(trafficLights[lane][1], LOW);
    digitalWrite(trafficLights[lane][0], HIGH);
  }
  
  // Emergency handler with siren effect (passive buzzer)
  bool checkEmergency() {
    int trigger = random(0, 1000);
    if (trigger < 10) { // ~1% chance
      Serial.println("🚨 EMERGENCY VEHICLE DETECTED!");
      allRed();
  
      unsigned long start = millis();
      while (millis() - start < emergencyDuration) {
        // WEE tone
        tone(buzzerPin, 880); // High-pitch
        delay(300);
        noTone(buzzerPin);
        delay(100);
  
        // OO tone
        tone(buzzerPin, 440); // Low-pitch
        delay(300);
        noTone(buzzerPin);
        delay(100);
      }
  
      noTone(buzzerPin);
      Serial.println("✅ Emergency cleared. Resuming traffic.");
      return true;
    }
    return false;
  }
  
  void loop() {
    static int laneIndex = 0;
  
    // Emergency override check
    if (checkEmergency()) {
      return; // Resume from same lane after emergency
    }
  
    int score = trafficScore(laneIndex);
  
    if (score == 0) {
      Serial.print("🔎 Lane "); Serial.print(laneIndex + 1);
      Serial.println(" has no vehicle. Checking for 3 seconds...");
  
      bool carDetected = false;
      unsigned long waitStart = millis();
  
      while (millis() - waitStart < 3000) {
        delay(500);
        if (checkEmergency()) return; // Emergency can interrupt wait
        score = trafficScore(laneIndex);
        if (score > 0) {
          carDetected = true;
          break;
        }
      }
  
      if (!carDetected) {
        Serial.print("⏩ Lane "); Serial.print(laneIndex + 1);
        Serial.println(" skipped (no vehicle in 3 seconds).");
        laneIndex = (laneIndex + 1) % 4;
        return;
      }
    }
  
    int greenTime = computeGreenTime(score);
    Serial.print("🚦 Lane "); Serial.print(laneIndex + 1);
    Serial.print(" Traffic Score: "); Serial.print(score);
    Serial.print(", Green Time: "); Serial.print(greenTime / 1000);
    Serial.println(" sec");
  
    controlTrafficLight(laneIndex, greenTime);
    laneIndex = (laneIndex + 1) % 4;
  
    delay(500);
  }