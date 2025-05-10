// Joystick pins
const int xPin = A0;
const int yPin = A1;

// Password sequence (You can change this!)
String password[] = {"UP", "RIGHT", "DOWN", "LEFT"};
const int passwordLength = sizeof(password) / sizeof(password[0]);

// Track input sequence
String inputSequence[10]; // Max 10 steps
int inputIndex = 0;

String lastDirection = "CENTER";

void setup() {
  Serial.begin(9600);
}

void loop() {
  int xVal = analogRead(xPin);
  int yVal = analogRead(yPin);

  int center = 512;
  int deadzone = 100;

  int xDiff = xVal - center;
  int yDiff = yVal - center;

  String currentDirection = "CENTER";

  // Direction detection (strict 4-way)
  if (abs(xDiff) > deadzone || abs(yDiff) > deadzone) {
    if (abs(xDiff) > abs(yDiff) * 1.5) {
      if (xDiff < -deadzone) {
        currentDirection = "LEFT";
      } else if (xDiff > deadzone) {
        currentDirection = "RIGHT";
      }
    } else if (abs(yDiff) > abs(xDiff) * 1.5) {
      if (yDiff < -deadzone) {
        currentDirection = "UP";
      } else if (yDiff > deadzone) {
        currentDirection = "DOWN";
      }
    }
  }

  // Detect direction change (only on new press)
  if (currentDirection != lastDirection && currentDirection != "CENTER") {
    Serial.println(currentDirection);

    // Save the input
    if (inputIndex < 10) {
      inputSequence[inputIndex] = currentDirection;
      inputIndex++;
    }

    // Check if input matches password length
    if (inputIndex == passwordLength) {
      // Print underline row
      Serial.println("__________");

      // Check the sequence
      bool correct = true;
      for (int i = 0; i < passwordLength; i++) {
        if (inputSequence[i] != password[i]) {
          correct = false;
          break;
        }
      }

      if (correct) {
        Serial.println("CHECK");
      } else {
        Serial.println("Incorrect");
      }

      // Reset for next input
      inputIndex = 0;
    }
  }

  // Reset lastDirection when centered
  if (currentDirection == "CENTER") {
    lastDirection = "CENTER";
  } else {
    lastDirection = currentDirection;
  }

  delay(100); // Small delay for stability
}
