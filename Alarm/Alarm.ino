#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


// Set the PORT for the web server
ESP8266WebServer server(80);

// The WiFi details
const char* ssid = "WI_Router";
const char* password = "retouR_IW";


// Initialise the RGB puns
const int red_led_pin = D2;
const int green_led_pin = D3;
const int blue_led_pin = D4;


 // Buzzer
 const int buzzer_pin = D5;
 int signaled = false;

 
bool wild = false;

void setup() {
  // put your setup code here, to run once:

 // Connect tot he wifi network
  WiFi.begin(ssid, password);

  // put your setup code here, to run once:
  pinMode(red_led_pin, OUTPUT);
  pinMode(green_led_pin, OUTPUT);
  pinMode(blue_led_pin, OUTPUT);


  // Buzzer
  // Set the buzzer to OUTPUT
  pinMode(buzzer_pin, OUTPUT);
  //No sound to start with 
  digitalWrite(buzzer_pin, LOW);

  
  // Start the serial to debug the values
  Serial.begin(9600);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Waiting to connect...");
  }

  // Print the board IP address
  Serial.print("IP address:" );
  Serial.println(WiFi.localIP());

  // add here something for the LED detection / index page or so 
  
  server.begin(); // Start the server
  Serial.println("Server listening");

   


}

void loop() {
  // put your main code here, to run repeatedly:
  // This will keeo the server and serial monitor available
  Serial.println("server is running");  

  // Handling of incoming client requests
  server.handleClient();

  // put your main code here, to run repeatedly:
  if (isWild()) {
    AlertSystemOn();
  } 
  else 
  {
    SafeOn();
  }
}

bool isWild() 
{

  if (wild == true)
  {
    return true; // wild animal
  }
  else return false;
  
}

void AlertSystemOn()
{
  // turn on buzzer
  turnBuzzerOn();


 // turn red light on 
 rgbLed(255,0,0);
    delay(500);
}


void SafeOn()
{

  // turn off buzzer
  turnBuzzerOff();

  // turn green light on 
  rgbLed(0,255,0);
    delay(500);
}


// add an extra standby mode one 
// and turn the light to blue 


void rgbLed(int red_led_amount, int green_led_amount , int blue_led_amount)
{
  analogWrite(red_led_pin, red_led_amount);
  analogWrite(green_led_pin, green_led_amount);
  analogWrite(blue_led_pin, blue_led_amount);
}


// trun buzzer on when wild
void turnBuzzerOn()
{
  tone(buzzer_pin, 1000);
  delay(2000);
  noTone(buzzer_pin);
  signaled = true; 
}


// turn buzzer off when safe
void turnBuzzerOff()
{
  noTone(buzzer_pin);
  signaled = false;
  
}

// add utility function here to read the status of the wild animal
