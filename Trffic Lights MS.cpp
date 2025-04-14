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