/*
* Pololu Protocol Example
* For Pololu Micro Serial Servo Controller
* by Adam Borrell (http://www-personal.umich.edu/~aborrell/)
*
* Using the Arduino board to communicate
* with the Pololu Micro Serial Servo Controller 
* http://www.pololu.com/products/pololu/0207/
*
* This example focuses on the Set Position (absolute) command, but
* a wide variety of commands can be sent to the controller. 
* Documentation on the other commands can be found at:
* http://www.pololu.com/products/pololu/0207/ssc03a_guide.pdf
* 
*/

#define ROTATE_LEFT 500
#define ROTATE_RIGHT 5500

void servoPut(unsigned char servo, unsigned int angle){
   //servo is the servo number (typically 0-7)
   //angle is the absolute position from 500 to 5500

   //Send a Pololu Protocol command
   Serial.write(0x80); //start byte
   Serial.write(0x01); //device id
   Serial.write(0x04); //command number
   Serial.write(servo); //servo number
   //Convert the angle data into two 7-bit bytes
   Serial.write(((angle>>7)&0x3f)); //data1
   Serial.write((angle&0x7f)); //data2
}

void servoOff(unsigned char servo){//turns off a servo
   //(servo will go limp until next position command)
   //servo is the servo number (typically 0-7)

   //Send a Pololu Protocol command
   Serial.write(0x80);//start byte
   Serial.write(0x01);//device id
   Serial.write(0x00);//command number
   Serial.write(servo);//servo number
   Serial.write(0x0f);//data1 (turn servo off, keep full range) 
}

void servoSetSpeed(unsigned char servo, unsigned char speedcmd){
   //servo is the servo number (typically 0-7)
   //speed is servo speed (1=slowest, 127=fastest)
   //set speed to zero to turn off speed limiting
   
   speedcmd=speedcmd&0x7f;//take only lower 7 bits of the speed
   
   //Send a Pololu Protocol command
   Serial.write(0x80);//start byte
   Serial.write(0x01);//device id
   Serial.write(0x01);//command number
   Serial.write(servo);//servo number
   Serial.write(speedcmd);//data1
}

