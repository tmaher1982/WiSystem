// Include the libraries for the camera, server and wifi

#include <esp32cam.h>
#include <WebServer.h>
#include <WiFi.h>


// set the SSID and PASS for the ESP32 module , as a wifi accesspoint to use instead of using a router

// #define AP_SSID "Wi_System_ESP"
// #define AP_PASS "PSE_metsyS-iW"

// Connecting to the router
const char* WIFI_SSID = "WI_Router";
const char* WIFI_PASS = "retouR_IW";
 
// Create an instance of the webserver at port 80

WebServer server(80);


// This is the request handler to handle captured images
void handleCapture()
{
  // For flash 
  digitalWrite(4, HIGH);
  delay(250);

  
  // This captures an image
  auto img = esp32cam::capture();
  
  // Turn off the flash after capturing the image
  digitalWrite(4, LOW);
  
  if (img == nullptr)
  {
    server.send(500, "", ""); // 500 is a generic internal server code fif an error happens 
    return;
    }
    server.setContentLength(img->size()); // sets the content length to the image size
    server.send(200, "image/jpeg");  // 200 if everything is ok
    
    // write the image to the client
    WiFiClient client = server.client();
    img->writeTo(client);
  }

void setup() {

  // Connect to the router
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  
  // For Flash
  pinMode(4, OUTPUT);
  
  // put your setup code here, to run once:
  auto res=esp32cam::Resolution::find(1024,768);
  esp32cam::Config cfg;
  cfg.setPins(esp32cam::pins::AiThinker); // This sets the pin configuration to AiThinker profile
  cfg.setResolution(res); // for resolution
  cfg.setJpeg(80); // for image quality
  esp32cam::Camera.begin(cfg); // Starts the camera module

  // Cancelling the access point at the moment
  // WiFi.softAP(AP_SSID, AP_PASS); // starts the wifi access point
  
  server.on("/capture.jpg", handleCapture); // This sets the camera capture to that handler "/capture.jog" and sends it as a response when requested from a browser
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  // This reaads the http requests and sends them to the handler function
  server.handleClient();

}
