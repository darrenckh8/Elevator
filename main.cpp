#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

const int floorButtons[] = {8, 9, 10, 11};
const int upDoorButton = 12;
const int downDoorButton = 13;
const int floorLEDs[] = {14, 15, 16, 17};
const int intOpenDoorButtons = 18;
const int intCloseDoorButtons = 19;

const char *floorLabels[] = {"2   3   4", "G   3   4", "G   2   4", "G   2   3"};

int currentFloor = 0; // 0-indexed, represents Ground floor
int targetFloor = -1; // -1 represents no target floor

bool clearLCD = true;

byte upArrow[8] = {
    B00100,
    B01110,
    B11111,
    B00100,
    B00100,
    B00100,
    B00100,
    B00000};

byte downArrow[8] = {
    B00000,
    B00100,
    B00100,
    B00100,
    B11111,
    B01110,
    B00100,
    B00000};

enum ElevatorState
{
  IDLE,
  OPENING_DOOR,
  CLOSING_DOOR,
  MOVING_UP,
  MOVING_DOWN,
  ARRIVED,
  ABORTED
};

ElevatorState elevatorState = IDLE;

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();

  lcd.createChar(0, upArrow);
  lcd.createChar(1, downArrow);

  for (int i = 0; i < 4; i++)
  {
    pinMode(floorButtons[i], INPUT);
    digitalWrite(floorButtons[i], LOW);
    pinMode(floorLEDs[i], OUTPUT);
    digitalWrite(floorLEDs[i], LOW);
  }

  pinMode(intOpenDoorButtons, INPUT);
  digitalWrite(intOpenDoorButtons, LOW);
  pinMode(intCloseDoorButtons, INPUT);
  digitalWrite(intCloseDoorButtons, LOW);

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
    if (clearLCD)
    {
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
    }

    delay(100);

    // Check for door open request
    if (digitalRead(upDoorButton) == HIGH || digitalRead(downDoorButton) == HIGH)
    {
      elevatorState = OPENING_DOOR;
    }
    if (digitalRead(intOpenDoorButtons) == HIGH)
    {
      elevatorState = ABORTED;
    }
    clearLCD = false;
    break;

  case OPENING_DOOR:
    lcd.clear();
    lcd.setCursor(4, 1);
    lcd.print("Door Opened");

    pinMode(intOpenDoorButtons, OUTPUT);
    for (int i = 0; i < 3; i++)
    {
      digitalWrite(intOpenDoorButtons, HIGH);
      delay(500);
      digitalWrite(intOpenDoorButtons, LOW);
      delay(500);
    }

    digitalWrite(intOpenDoorButtons, LOW);
    pinMode(intOpenDoorButtons, INPUT);

    lcd.setCursor(4, 1);
    lcd.print("Door Closed");

    pinMode(intCloseDoorButtons, OUTPUT);
    for (int i = 0; i < 3; i++)
    {
      digitalWrite(intCloseDoorButtons, HIGH);
      delay(500);
      digitalWrite(intCloseDoorButtons, LOW);
      delay(500);
    }

    digitalWrite(intCloseDoorButtons, LOW);
    pinMode(intCloseDoorButtons, INPUT);

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
    clearLCD = true;
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
        delay(100);
        pinMode(floorButtons[i], OUTPUT);
        digitalWrite(floorButtons[i], HIGH);
        break;
      }
      else if (digitalRead(intCloseDoorButtons) == HIGH)
      {
        elevatorState = CLOSING_DOOR;
        break;
      }
      else if (digitalRead(upDoorButton) == HIGH || digitalRead(downDoorButton) == HIGH || digitalRead(intOpenDoorButtons) == HIGH)
      {
        elevatorState = ABORTED;
        break;
      }
    }
    clearLCD = true;
    break;

  case MOVING_UP:
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.write(byte(0));
    lcd.setCursor(4, 1);
    lcd.print("UP [");
    if (targetFloor > 0)
    {
      lcd.print(targetFloor + 1);
    }
    else
    {
      lcd.print("G");
    }
    lcd.print("]...");
    lcd.setCursor(4, 0);
    lcd.print("Floor ");
    if (currentFloor > 0)
    {
      lcd.print(currentFloor + 1);
    }
    else
    {
      lcd.print("G");
    }
    delay(2000);
    currentFloor++;
    updateLEDs();
    elevatorState = (currentFloor == targetFloor) ? ARRIVED : MOVING_UP;
    clearLCD = true;
    break;

  case MOVING_DOWN:
    lcd.clear();
    lcd.setCursor(1, 1);
    lcd.write(byte(1));
    lcd.setCursor(4, 1);
    lcd.print("DOWN [");
    if (targetFloor > 0)
    {
      lcd.print(targetFloor + 1);
    }
    else
    {
      lcd.print("G");
    }
    lcd.print("]...");
    lcd.setCursor(4, 0);
    lcd.print("Floor ");
    if (currentFloor > 0)
    {
      lcd.print(currentFloor + 1);
    }
    else
    {
      lcd.print("G");
    }
    delay(2000);
    currentFloor--;
    updateLEDs();
    elevatorState = (currentFloor == targetFloor) ? ARRIVED : MOVING_DOWN;
    clearLCD = true;
    break;

  case ARRIVED:

    lcd.setCursor(4, 0);
    lcd.print("Floor ");
    if (currentFloor > 0)
    {
      lcd.print(currentFloor + 1);
    }
    else
    {
      lcd.print("G");
    }
    delay(2000);
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Arrived at");
    lcd.setCursor(4, 1);
    lcd.print("Floor ");
    if (currentFloor > 0)
    {
      lcd.print(currentFloor + 1);
    }
    else
    {
      lcd.print("G");
    }
    delay(2000);

    digitalWrite(floorButtons[currentFloor], LOW);
    pinMode(floorButtons[currentFloor], INPUT);

    lcd.clear();
    lcd.print("Doors Opened");

    pinMode(intOpenDoorButtons, OUTPUT);
    for (int i = 0; i < 3; i++)
    {
      digitalWrite(intOpenDoorButtons, HIGH);
      delay(500);
      digitalWrite(intOpenDoorButtons, LOW);
      delay(500);
    }

    digitalWrite(intOpenDoorButtons, LOW);
    pinMode(intOpenDoorButtons, INPUT);

    lcd.clear();
    lcd.print("Doors Closed");

    pinMode(intCloseDoorButtons, OUTPUT);
    for (int i = 0; i < 3; i++)
    {
      digitalWrite(intCloseDoorButtons, HIGH);
      delay(500);
      digitalWrite(intCloseDoorButtons, LOW);
      delay(500);
    }

    digitalWrite(intCloseDoorButtons, LOW);
    pinMode(intCloseDoorButtons, INPUT);

    targetFloor = -1;
    elevatorState = IDLE;
    clearLCD = true;
    break;

  case ABORTED:
    lcd.clear();
    lcd.print("Doors Opened");
    delay(3000);
    lcd.clear();
    lcd.print("Doors Closed");
    delay(3000);
    targetFloor = -1;
    elevatorState = IDLE;
    clearLCD = true;
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
