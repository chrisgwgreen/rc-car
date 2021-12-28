#define ENA 13
#define IN_1 12
#define IN_2 14
#define IN_3 27
#define IN_4 26
#define ENB 25

#ifndef motor_c
#define motor_c
class MOTOR
{
public:
  MOTOR();

  void setup();
  void setMotor(int x, int y);
};
#endif
