
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "07701";
const uint8_t  channel = 104;


struct ultra_sonic{
  int ultra_sonic_num;

};
ultra_sonic ultra_sonic_data;


bool conn = false;



 
Servo myservo1;
Servo myservo2;

char inByte;
int speed_step;
int servo_id;

int pos_servo0;
  int pos_servo1 ;
void setup() {
  // Set Up Serial Comms - Debug
  Serial.begin(9600);

  // Pinmode Setup
 
  

 
  // Set up Radio
  radio.begin();
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_LOW,0);
  radio.setChannel(channel);
  Serial.println("Starting Radio"); 

  // Check for OLED Availability
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("LOADING...");
  display.setCursor(0, 20);
  display.display();
  delay(2500);

  //to recieve

  radio.startListening();




  myservo1.attach(10);
  myservo2.attach(9);

    pos_servo0 =45;
   pos_servo1 =90;


   
 
}
void loop() {
  int data[] = {0,0};
  if (radio.available()) {
    conn = true;
    
    //radio.startListening();

    radio.read(&data, sizeof(data));

    // Will delay code, only use if necessary
    debug(data);

    
   //  506    // 0
   //  517    // 0
    int temp0 = 0;
    int temp1 = 0  ;
    if((data[0])< 500){
      temp0 = map(data[0], 0, 495, 20, 0);
      if ( (pos_servo0 + temp0)  >40 ){
      pos_servo0 = pos_servo0 - temp0;
      }}
    if((data[0])> 500){
      temp0 = map(data[0], 520, 1023, 0, 20);
      if ( (pos_servo0 + temp0) < 150){
      pos_servo0 = pos_servo0 + temp0;
      }}
    if (pos_servo0 < 20 ) {pos_servo0 = 21;}
    if (pos_servo1 < 0 ) {pos_servo1 = 0;}
    
    if((data[1])< 550){
      temp1 = map(data[1], 0, 495, 15, 0);
      if ( (pos_servo1 + temp1) > 0){
      pos_servo1 = pos_servo1 - temp1;
      }}
    if((data[1])> 550){
      temp1 = map(data[1], 520, 1023, 0, 15);
      if ( (pos_servo1 + temp1) < 180){
      pos_servo1 = pos_servo1 + temp1;
      }}


    myservo1.write(pos_servo0);     // move servo  Y
    myservo2.write(pos_servo1);     // move servo   X
 
    // commands servos
    delay(4);
  //  radio.stopListening();
  //  ultra_sonic_data.ultra_sonic_num = 5;
  // radio.write(&ultra_sonic_data, sizeof(ultra_sonic_data));

  }
  else {
    Serial.println("NO CONNECTION");
    conn = false;
    
  }
  //   int y , int x ,int pos_Y,int pos_X
  OLED_display(data[0],data[1], pos_servo0 , pos_servo1);  
}


void OLED_display(int y , int x ,int pos_Y,int pos_X )
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  if (conn)
  {
    display.println("RF Status: PASS");
  }
  else
  {
    display.println("RF Status: FAIL");
  }
  display.setCursor(0,10);
 
  display.println(  "X= " );
  display.setCursor(18,10);
  display.println(  ( x) );
  display.setCursor(50,10);
  display.println(  "Y= " );
  display.setCursor(63,10);
  display.println(( y) );
  display.setCursor(0,20);
  display.println( "CAM POS: " );
  display.setCursor(50,20);
  display.println( pos_X );
  display.setCursor(68,20);
  display.println( "," );
  display.setCursor(76,20);
  display.println( pos_Y );

  display.setCursor(90,10);
  display.println( "Ultra:" );
  display.setCursor(98,20);
  display.println( "n/a" );

  
  display.display();
  
  
}

void debug(int data[]) {
      //THIS IS FOR DEBUG
      Serial.print("LY: ");
      Serial.println(data[0]);
      Serial.print(";   LX: ");
      Serial.println(data[1]);
  
      delay(5);
 
}
