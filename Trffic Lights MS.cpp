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
    


}