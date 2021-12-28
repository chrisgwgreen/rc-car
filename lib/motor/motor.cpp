#include <motor.h>
#include <analogWrite.h>

int minSpeed = 175;

MOTOR::MOTOR() {}

/**
 * @brief setup initalises the pin outs for the l298n motor driver. 
 * 
 */
void MOTOR::setup()
{
  pinMode(ENA, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);

  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
};

/**
 * @brief setMotor takes the x/y position of the controller and determines the power to be sent each motor. 
 * 
 */
void MOTOR::setMotor(int x, int y)
{

  if (x == 0 && y == 0)
  {
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_2, LOW);
    digitalWrite(IN_3, LOW);
    digitalWrite(IN_4, LOW);
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
  }
  else
  {

    if (y > 0)
    {
      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
    }
    else
    {
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
    }

    double speedRatio = (255.0 - minSpeed) / 100;

    double left = speedRatio * abs(y);
    double right = speedRatio * abs(y);

    if (x < 0)
    {
      left = (left / 100) * (100 - abs(x));
    }
    else
    {
      right = (right / 100) * (100 - abs(x));
    }

    left += minSpeed;
    right += minSpeed;

    Serial.print(right);
    Serial.print(" - ");
    Serial.println(left);

    analogWrite(ENA, right);
    analogWrite(ENB, left);
  }
};
