
 

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <Wire.h> 

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);



RF24 radio(7, 8); // CE, CSN
const byte address[6] = "07701";
const uint8_t  channel = 104;



// Define the digital inputs
 

// Packet = [leftstickx, leftsticky]

/*
packet length = 17
packet[0] = leftstickxL
packet[1] = leftstickxH
packet[2] = leftstickyL
packet[3] = leftstickyH

*/

// Max size 32 bytes because of buffer limit
struct Data_Packet{
  int leftsticky;
  int leftstickx;

};
Data_Packet data;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW,0);
  radio.setChannel(channel);
  radio.stopListening();
  Serial.println("Sending");

  display.clearDisplay();


  // Check for OLED Availability
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
    
  }
  
   display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 20);
  display.println("LOADING...");
  display.setCursor(0, 20);
  display.display();
  delay(2500);
  }
  
   
  
  //Set Up Controls
 // pinMode(lBTN, INPUT_PULLUP);




void loop() {

 

  //Serial.print("up and down =" );
  //Serial.println(analogRead(A0));

 //Serial.print("left and right =" );
  //Serial.println(analogRead(A1));

 // delay(3000);
  // Read all analog inputs and map them to one Byte value
  //Gather Left Stick Values
  int temp0=analogRead(A0);
  int temp1=analogRead(A1);
  data.leftsticky = temp0;
  data.leftstickx = temp1;
 
  // Read potentiometer values and assign them to packet

  // Send the whole data from the structure to the receiver
  //radio.stopListening();
  //int ultra_sonic_data[] = {0};
   radio.write(&data, sizeof(data));
   delay(3);


 OLED_display( data.leftsticky ,  data.leftstickx ,  0);

  
}

 


void OLED_display(int y , int x, int ultra_data )
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(  "  Gimbal Transmitter" );
  display.setCursor(10,11);
  display.println(  "X = " );
  display.setCursor(32,11);
  display.println(  ( x) );
  display.setCursor(60,11);
  display.println(  "Y = " );
  display.setCursor(84,11);
  display.println(( y) );
  display.setCursor(1,30);
  display.println(  "Ultra Sonic Distance     = " );
  display.setCursor(55,40);
  display.println(( ultra_data) );
   
  display.display();
}




void debug() {
      //THIS IS FOR DEBUG
      delay(5);
}
