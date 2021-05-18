/*A program to control an Ironman Faceplate with 2 MG90S servos

IMPORTANT! 
Please read the comments, you will find important information in case your program is not working correctly.

   This program will activate 2 servos with X amount of degrees
   and then make a fading routine (close faceplate) when a button press is register
   when another button press is register we will turn OFF the LED
   and move X amount of degrees the servo (open faceplate)

   Using an Arduino NANO
*/

#include <Servo.h> //include the Servo Library

Servo facePlateServo;//creating a Servo instance
Servo facePlateServo2;//creating a Servo instance

const byte pushButton = 2;//button to activate (Interrupt)
const byte eyesLED = 3;//pin for the LED (PWM)

volatile boolean interruptFlag = false;//state of the button

void setup() {
  //Serial.begin(9600); //serial communication for debug

  facePlateServo.attach(11);//pin 11 for Servo (PWM)
  facePlateServo2.attach(9);//pin 9 for Servo (PWM)

  pinMode(eyesLED, OUTPUT);//pin of the LED as an Output
  pinMode(pushButton, INPUT);//pin of the Button as an Input

  //Attach an Interrupt Service Routine (ISR) to the button pin.
  attachInterrupt(digitalPinToInterrupt(pushButton), ISR_activation, FALLING);

  facePlateServo.write(160); //begin with closed Faceplate (ATTENTION! This value may change, as your set-up will not be the exact same as mine)
  facePlateServo2.write(20); //begin with closed Faceplate (ATTENTION! This value may change, as your set-up will not be the exact same as mine)
  digitalWrite(eyesLED, HIGH);//begin with the LEDs ON
  facePlateServo.detach();//detach servo 1 to avoid mechanical problems
  facePlateServo2.detach();//detach servo 1 to avoid mechanical problems

}//Close Setup

void loop() {

  // In this first part we will check the state of the flag (button pressed register)
  static boolean prevInterruptFlag = false;//static to retain last value & boolean because the interruptFlag is boolean
  if (prevInterruptFlag == interruptFlag)//track interruptFlag changes
    return;
  prevInterruptFlag = interruptFlag; //when the if stmnt is accepted reset the prevInterruptFlag

  // CLOSE FACEPLATE

  //If button has been pressed, execute this code
  if ( interruptFlag == true) {

    facePlateServo.attach(11);//re-atach the servos
    facePlateServo2.attach(9);

    //for loop to close the Faceplate
    for (int i = 45; i < 160; i++) {//IMPORTANT! Perform some tests with these values (40 and 160) before installing the mechanics
      facePlateServo.write(i);
      facePlateServo2.write(180 - i); //mirrored servo
      delay (5);
    }//close For loop

    delay(1000);//Wait to launch the blink sequence

    //Launch de User defined function to blink the LEDs
    blinkLED(eyesLED);

    //Fade the LED for loop
    for (int i = 0; i < 256; i++) {
      analogWrite(eyesLED, i);
      delay (10);
    }//close For loop

    facePlateServo.detach();//detach servo 1 to avoid mechanical problems
    facePlateServo2.detach();//detach servo 2 to avoid mechanical problems

  }//close If stmnt
  
  // OPEN FACEPLATE
  else {
    digitalWrite (eyesLED, LOW);//turn OFF the LEDs

    facePlateServo.attach(11);//re-atach the servos
    facePlateServo2.attach(9);

    //for loop to ease the closing of the Faceplate
    for (int i = 160; i > 45; i--) {//IMPORTANT! Perform some tests with these values (40 and 160) before installing the mechanics
      facePlateServo.write(i);
      facePlateServo2.write(180 - i);//mirrored servo
      delay (5);
    }//close For loop

    facePlateServo.detach();//detach servo 1 to avoid mechanical problems
    facePlateServo2.detach();//detach servo 2 to avoid mechanical problems

  }//close Else stmnt

}//close loop


//*****Interrupt Service Routine*****
void ISR_activation() {
  interruptFlag = !interruptFlag;

}//close activation ISR


//*****User Defined Function*****
void blinkLED(int eyesLED) {

  digitalWrite(eyesLED, HIGH);
  delay (50);
  digitalWrite(eyesLED, LOW);
  delay (50);
  digitalWrite(eyesLED, HIGH);
  delay (50);
  digitalWrite(eyesLED, LOW);
  delay (500);

}//close blinkLED UDF
