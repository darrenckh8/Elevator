#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

const int floorButtons[] = {8, 9, 10, 11};
const int upDoorButton = 12;
const int downDoorButton = 13;
const int floorLEDs[] = {14, 15, 16, 17};

const char *floorLabels[] = {"2   3   4", "G   3   4", "G   2   4", "G   2   3"};

int currentFloor = 0; // 0-indexed, represents Ground floor
int targetFloor = -1; // -1 represents no target floor

enum ElevatorState
{
  IDLE,
  OPENING_DOOR,
  CLOSING_DOOR,
  MOVING_UP,
  MOVING_DOWN,
  ARRIVED
};

ElevatorState elevatorState = IDLE;

void setup()
{
  lcd.begin(16, 2);
  lcd.clear();

  for (int i = 0; i < 4; i++)
  {
    pinMode(floorButtons[i], INPUT);
    digitalWrite(floorButtons[i], LOW);
    pinMode(floorLEDs[i], OUTPUT);
    digitalWrite(floorLEDs[i], LOW);
  }

  pinMode(upDoorButton, INPUT);
  digitalWrite(upDoorButton, LOW);
  pinMode(downDoorButton, INPUT);
  digitalWrite(downDoorButton, LOW);

  updateLEDs();
}

void loop()
{
  switch (elevatorState)
  {
  case IDLE:
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Current Floor");
    lcd.setCursor(4, 1);
    lcd.print("Floor ");

    if (currentFloor > 0)
    {
      lcd.print(currentFloor + 1); // Display floor number
    }
    else
    {
      lcd.print("G"); // Display G floor
    }

    delay(100);

    // Check for door open request
    if (digitalRead(upDoorButton) == HIGH || digitalRead(downDoorButton) == HIGH)
    {
      elevatorState = OPENING_DOOR;
    }
    break;

  case OPENING_DOOR:
    lcd.clear();
    lcd.setCursor(4, 1);
    lcd.print("Door Opened");
    delay(3000);
    lcd.setCursor(4, 1);
    lcd.print("Door Closed");
    delay(1500);
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Select Floor");
    lcd.setCursor(1, 1);

    if (currentFloor >= 0 && currentFloor < 4)
    {
      lcd.print(floorLabels[currentFloor]);
    }

    delay(100);
    elevatorState = CLOSING_DOOR;
    break;

  case CLOSING_DOOR:
    // Check for floor selection request
    for (int i = 0; i < 4; i++)
    {
      if (digitalRead(floorButtons[i]) == HIGH)
      {
        targetFloor = i;
        if (targetFloor == currentFloor)
        {
          elevatorState = CLOSING_DOOR;
          break;
        }
        elevatorState = (targetFloor > currentFloor) ? MOVING_UP : MOVING_DOWN;
        break;
      }
    }
    break;

  case MOVING_UP:
    lcd.clear();
    lcd.setCursor(4, 1);
    lcd.print("UP [");
    lcd.print(targetFloor + 1);
    lcd.print("]...");
    lcd.setCursor(4, 0);
    lcd.print("Floor ");
    lcd.print(currentFloor + 1); // Display current floor
    delay(2000);
    currentFloor++;
    updateLEDs();
    elevatorState = (currentFloor == targetFloor) ? ARRIVED : MOVING_UP;
    break;

  case MOVING_DOWN:
    lcd.clear();
    lcd.setCursor(4, 1);
    lcd.print("DOWN [");
    lcd.print(targetFloor + 1);
    lcd.print("]...");
    lcd.setCursor(4, 0);
    lcd.print("Floor ");
    lcd.print(currentFloor + 1); // Display current floor
    delay(2000);
    currentFloor--;
    updateLEDs();
    elevatorState = (currentFloor == targetFloor) ? ARRIVED : MOVING_DOWN;
    break;

  case ARRIVED:
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Arrived at");
    lcd.setCursor(4, 1);
    lcd.print("Floor ");
    lcd.print(currentFloor + 1);
    delay(2000);
    lcd.clear();
    lcd.print("Doors Opened");
    delay(3000);
    lcd.clear();
    lcd.print("Doors Closed");
    delay(3000);
    targetFloor = -1;
    elevatorState = IDLE;
    break;
  }
}

void updateLEDs()
{
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(floorLEDs[i], (currentFloor == i) ? HIGH : LOW);
  }
}
