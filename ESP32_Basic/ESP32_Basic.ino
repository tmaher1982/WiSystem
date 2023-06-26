// Include the libraries for the camera, server and wifi

#include <esp32cam.h>
#include <WebServer.h>
#include <WiFi.h>


// set the SSID and PASS for the ESP32 module , as a wifi accesspoint to use instead of using a router

#define AP_SSID "Wi_System_ESP"
#define AP_PASS "PSE_metsyS-iW"


// Create an instance of the webserver at port 80

WebServer server(80);


// This is the request handler to handle captured images
void handleCapture()
{
  // This captures an image
  auto img = esp32cam::capture();
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
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
