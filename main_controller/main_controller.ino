#include <Servo.h>
#include <Wire.h>

int thruster_speed[8] = {0, 0, 0, 0, 0, 0};

#define cam_switch_adr 0x03//if pulled high use 0x86 | if pulled low use 0x06

//thrusters
#define thruster_1_pin 55 //pin 96 | analog A1
#define thruster_2_pin 56 //pin 95 | analog A2
#define thruster_3_pin 57 //pin 94 | analog A3
#define thruster_4_pin 58 //pin 93 | analog A4
#define thruster_5_pin 59 //pin 92 | analog A5
#define thruster_6_pin 60 //pin 91 | analog A6
#define thruster_7_pin 61 //pin 90 | analog A7
#define thruster_8_pin 62 //pin 89 | analog A8

#define red 46 //pin 38 | PWM
#define green 45 //pin 39 | PWM
#define blue 44 //pin 40 | PWM

#define gpio_1 64 //pin 87 | analog A10
#define gpio_2 65 //pin 86 | analog A11
#define gpio_3 66 //pin 85 | analog A12
#define gpio_4 67 //pin 84 | analog A13
#define gpio_5 68 //pin 83 | analog A14
#define gpio_6 22 //pin 78
#define gpio_7 23 //pin 77
#define gpio_8 24 //pin 76
#define gpio_9 25 //pin 75
#define gpio_10 26 //pin 74
#define gpio_11 27 //pin 73
#define gpio_12 28 //pin 72

Servo thruster_1;
Servo thruster_2;
Servo thruster_3;
Servo thruster_4;
Servo thruster_5;
Servo thruster_6;
//Servo thruster_7;
Servo james; //just ignore
Servo thruster_8;

Servo servos[8] = {thruster_1,
                   thruster_2,
                   thruster_3,
                   thruster_4,
                   thruster_5,
                   thruster_6,
                   james,
                   thruster_8
                  };

void set_camera(int camera_1 = 1, int camera_2 = 2) {
  Wire.beginTransmission(cam_switch_adr);
  Wire.write(0b01); //output 1 register
  Wire.write(224 + camera_1); //add 64 to alter gain1 and 32 to alter gain2
  Wire.endTransmission();
  delay(1);
  Wire.beginTransmission(cam_switch_adr);
  Wire.write(0b11); //output 1 register
  Wire.write(224 + camera_2); //add 64 to alter gain1 and 32 to alter gain2
  Wire.endTransmission();
}

void setup() {
  // put your setup code here, to run once:
  Serial3.begin(115200);
  thruster_1.attach(55);
  thruster_2.attach(56);
  thruster_3.attach(57);
  thruster_4.attach(58);
  thruster_5.attach(59);
  thruster_6.attach(60);
  //  thruster_7.attach(61);
  thruster_8.attach(62);
  pinMode(gpio_1, OUTPUT);
  pinMode(gpio_2, OUTPUT);
  pinMode(gpio_3, OUTPUT);
  pinMode(gpio_4, OUTPUT);
  pinMode(gpio_5, OUTPUT);
  pinMode(gpio_6, OUTPUT);
  pinMode(gpio_7, OUTPUT);
  pinMode(gpio_8, OUTPUT);
  pinMode(gpio_9, OUTPUT);
  pinMode(gpio_10, OUTPUT);
  pinMode(gpio_11, OUTPUT);
  pinMode(gpio_12, OUTPUT);

  digitalWrite(gpio_1, LOW);
  digitalWrite(gpio_2, LOW);
  digitalWrite(gpio_3, LOW);
  digitalWrite(gpio_4, LOW);
  digitalWrite(gpio_5, LOW);
  digitalWrite(gpio_6, LOW);
  digitalWrite(gpio_7, LOW);
  digitalWrite(gpio_8, LOW);
  digitalWrite(gpio_9, LOW);
  digitalWrite(gpio_10, LOW);
  digitalWrite(gpio_11, LOW);
  digitalWrite(gpio_12, LOW);
  digitalWrite(red, HIGH);
  digitalWrite(green, HIGH);
  digitalWrite(blue, HIGH);
  delay(1000);
  int i = 1500;
  i = 1500;
  //for (int i = 1500; i < 1500; i++) {
  for (int j = 0; j < 7; j++) {
    servos[j].write(i);
  }
  //  thruster_1.writeMicroseconds(i);
  //  thruster_2.writeMicroseconds(i);
  //  thruster_3.writeMicroseconds(i);
  //  thruster_4.writeMicroseconds(i);
  //  thruster_5.writeMicroseconds(i);
  //  thruster_6.writeMicroseconds(i);
  //  //  thruster_7.writeMicroseconds(i);
  //  thruster_8.writeMicroseconds(i);
  delay(7000);
}

char temp;

void loop() {
  if (Serial3.available()) 
  {
    delay(5);
    if (Serial3.read() == 'S')
    {
      temp = Serial3.read();
      if (temp == 'A')
      {
        Serial3.println("in");
        for (int i = 0; i < 6; i++)
        {
          thruster_speed[i] = 100 * Serial3.read() + 10 * Serial3.read() + Serial3.read() - 4328;
          Serial3.print(thruster_speed[i]);
          Serial3.println(" inside the for loop");
        }
        if (Serial3.read() == '!') 
        {
          for (int i = 0; i < 6; i++) {
            //set the speed of each thruster using thruster_speed[i]
            if (thruster_speed[i] >= 1100 && thruster_speed[i] <= 1900)
            {
              servos[i].writeMicroseconds(thruster_speed[i]);
            }
            else {
              Serial3.print("Servo: ");
              Serial3.print(i);
              Serial3.println(" has failed");
            }
          }
          Serial3.print("\n");
          Serial3.read();
        }
      }
      else if (temp == 'F')  //main gripper
      {
        if (Serial3.read() - 48) 
        {
          //open gripper
        } 
        else 
        {
          //close gripper
        }
        Serial3.read();
      }
      else if (temp == 'G')  //trap door
      { 
        if (Serial3.read() - 48) 
        {
          //open trap door
        } 
        else {
          //close trap door
        }
        Serial3.read();
      }
      else if (temp == 'H') 
      {
        set_camera(Serial3.read() - 48, Serial3.read() - 48);
        Serial3.read();
      }
      else
        Serial3.println("Not A");
    }
    else
    {
      Serial3.print(temp);
      Serial3.print(" * ");
      Serial3.println("Not S");
    }
  }
}
