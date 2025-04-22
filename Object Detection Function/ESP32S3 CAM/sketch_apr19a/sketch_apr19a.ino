#include <WebServer.h>      // Include WebServer library for handling HTTP requests over WiFi
#include <WiFi.h>           // Include WiFi library to connect to wireless network
#include <esp32cam.h>       // Include ESP32-CAM helper library for camera control

// THIS PROGRAM SENDS IMAGE IF IT IS PLACED IN WEB IP,
// BUT IF IT IS PLACED IN PYTHON IT SENDS VIDEO THROUGH THE ITERATIONS... (IF IT WORKS IN PYTHON)
const char* WIFI_SSID = "朕御赐天恩给你";  // Define the SSID of the WiFi network to connect to
const char* WIFI_PASS = "CHIU20001031";   // Define the password for the WiFi network

WebServer server(80); // Create a web server object listening on port 80 (HTTP)

static auto loRes = esp32cam::Resolution::find(320, 240); // Find and store a low resolution (320×240)
static auto hiRes = esp32cam::Resolution::find(800, 600); // Find and store a high resolution (800×600)
// static auto hiRes = esp32cam::Resolution::find(640, 480); // Alternative high resolution for higher FPS

// Function to capture and serve a JPEG image
void serveJpg() {
  auto frame = esp32cam::capture();                // Capture a frame from the camera
  if (frame == nullptr) {                          // Check if capture failed
    Serial.println("Capture Fail");              // Log failure message
    server.send(503, "", "");                  // Respond with HTTP 503 Service Unavailable
    return;                                        // Exit the function
  }
  Serial.printf("CAPTURE OK %dx%d %db\n",      // Log success message with image dimensions and size
                frame->getWidth(),
                frame->getHeight(),
                static_cast<int>(frame->size()));

  server.setContentLength(frame->size());          // Set HTTP response content length
  server.send(200, "image/jpeg");                // Send HTTP 200 OK header with MIME type
  WiFiClient client = server.client();             // Get the client that connected
  frame->writeTo(client);                          // Write JPEG image data to the client
}

// Handler to set low resolution then serve image
void handleJpgLo() {
  if (!esp32cam::Camera.changeResolution(loRes)) { // Change camera to low resolution
    Serial.println("SET-LO-RES FAIL");           // Log if resolution change fails
  }
  serveJpg();                                      // Serve the captured image
}

// Handler to set high resolution then serve image
void handleJpgHi() {
  if (!esp32cam::Camera.changeResolution(hiRes)) { // Change camera to high resolution
    Serial.println("SET-HI-RES FAIL");           // Log if resolution change fails
  }
  serveJpg();                                      // Serve the captured image
}

void setup() {
  Serial.begin(115200);                            // Initialize serial for debugging at 115200 baud
  Serial.println();                                // Print a blank line for readability

  {  // Camera initialization block
    using namespace esp32cam;                       // Use esp32cam namespace for Camera and Config
    Config cfg;                                    // Create camera configuration object
    cfg.setPins(pins::AiThinker);                  // Set the pin mapping for the AI-Thinker module
    cfg.setResolution(hiRes);                      // Set initial resolution to high
    cfg.setBufferCount(2);                         // Allocate 2 frame buffers for smoother capture
    cfg.setJpeg(80);                               // Set JPEG quality to 80 (0-100 scale)

    bool ok = Camera.begin(cfg);                   // Initialize the camera with specified config
    Serial.println(ok ? "CAMERA OK" : "CAMERA FAIL"); // Log success or failure
  }

  WiFi.persistent(false);                          // Disable WiFi persistent storage for faster boot
  WiFi.mode(WIFI_STA);                             // Set WiFi mode to station (client)
  WiFi.begin(WIFI_SSID, WIFI_PASS);                // Connect to the specified WiFi network
  while (WiFi.status() != WL_CONNECTED) {          // Wait for connection
    delay(500);                                    // Delay 500 ms between checks
  }

  // Print URLs for low and high resolution JPEG endpoints
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/cam-lo.jpg");

  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/cam-hi.jpg");

  server.on("/cam-lo.jpg", handleJpgLo);         // Register low-res handler for /cam-lo.jpg
  server.on("/cam-hi.jpg", handleJpgHi);         // Register high-res handler for /cam-hi.jpg

  server.begin();                                  // Start the web server
}

void loop() {
  server.handleClient();                           // Handle incoming HTTP requests
}
