/*A program to control an Ironman Faceplate

   This program will activate a Servo with a X amount of degrees
   and then make a fading routine when a button press is register
   when another button press is register we will turno OFF the LED
   and move X amount of degrees the Servo

   Using an Arduino NANO
*/

#include <Servo.h> //include the Servo Library

Servo facePlateServo;//creating a Servo instance

const byte pushButton = 2;//button to activate (Interrupt)
const byte eyesLED = 3;//pin for the LED (PWM)

volatile boolean interruptFlag = false;//state of the button

void setup() {
  //Serial.begin(9600); //serial communication for debug

  facePlateServo.attach(11);//pin 11 for Servo (PWM)

  pinMode(eyesLED, OUTPUT);//pin of the LED as an Output
  pinMode(pushButton, INPUT);//pin of the Button as an Input

  //Attach an Interrupt Service Routine (ISR) to the button pin.
  attachInterrupt(digitalPinToInterrupt(pushButton), ISR_activation, FALLING);

  facePlateServo.write(60); //begin with closed Faceplate (Attention with this value, it may change from your set-up to mine)
  digitalWrite(eyesLED, HIGH);//begin with the LEDs ON
  facePlateServo.detach();


}//Close Setup

void loop() {

  static boolean prevInterruptFlag = false;//static to retain last value & boolean because the interruptFlag is boolean

  if (prevInterruptFlag == interruptFlag)//track interruptFlag changes
    return;

  prevInterruptFlag = interruptFlag; //when the if stmnt is accepted reset the prevInterruptFlag


  //If button has been pressed, execute this code
  if ( interruptFlag == true) {
    facePlateServo.attach(11);
    digitalWrite (eyesLED, LOW);//turn OFF the LEDs

      
    //for loop to ease the opening of the Faceplate
    for (int i = 60; i < 145; i++) {
      facePlateServo.write(i);
      delay (5);
    }//close For loop

  }//close If stmnt

  else {

    //for loop to ease the closing of the Faceplate
    for (int i = 145; i > 60; i--) {
      facePlateServo.write(i);
      delay (5);
    }//close For loop
  facePlateServo.detach();
  
    delay(1000);//Wait to launch the Fade sequence

    blinkLED(eyesLED);//Launch de User defined function to blink the LEDs

    //Fade the LED for loop
    for (int i = 0; i < 256; i++) {
      analogWrite(eyesLED, i);
      delay (10);
    }//close For loop


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
