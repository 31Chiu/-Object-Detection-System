/* Blynk Template Configuration */
#define BLYNK_TEMPLATE_ID   "TMPL69tmqUUef"   // Define the Blynk template ID for this project
#define BLYNK_TEMPLATE_NAME "Group Assignment"  // Define the Blynk project name displayed in the app
#define BLYNK_AUTH_TOKEN    "K1Z91KsR3o2KIi64o-oQx_PnTkl7IF6_"  // Define the Blynk authentication token for device login

/* Hardware and Library Configuration */
#include <Wire.h>                     // Include Wire library for I2C communication
#include <LiquidCrystal_I2C.h>       // Include I2C LCD library for display
#include <WiFi.h>                     // Include WiFi library for network connectivity
#include <BlynkSimpleEsp32.h>         // Include Blynk library for ESP32 integration
#define BLYNK_PRINT Serial            // Enable debug prints to Serial

/* Network Credentials */
char auth[] = BLYNK_AUTH_TOKEN;       // Store Blynk auth token in a char array
char ssid[] = "朕御赐天恩给你";       // Store WiFi SSID in a char array
char pass[] = "CHIU20001031";        // Store WiFi password in a char array

/* GPIO Pin Definitions */
#define RED_PIN   15                // Assign GPIO 15 to the red LED pin
#define GREEN_PIN 2                 // Assign GPIO 2 to the green LED pin

/* I2C LCD Configuration */
#define LCD_I2C_ADDR 0x27           // Define the I2C address of the LCD module (change if needed)
#define LCD_COLS     16             // Define number of columns on the LCD display
#define LCD_ROWS     2              // Define number of rows on the LCD display
LiquidCrystal_I2C lcd(LCD_I2C_ADDR, LCD_COLS, LCD_ROWS);  // Create an LCD object with defined parameters

void setup() {
  Serial.begin(115200);             // Initialize serial communication at 115200 baud rate
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);  // Connect to Blynk cloud using provided credentials and port 80

  /* LED Pin Initialization */
  pinMode(RED_PIN, OUTPUT);         // Configure RED_PIN as an output
  pinMode(GREEN_PIN, OUTPUT);       // Configure GREEN_PIN as an output
  digitalWrite(RED_PIN, LOW);       // Ensure red LED is off at startup
  digitalWrite(GREEN_PIN, LOW);     // Ensure green LED is off at startup

  /* LCD Initialization */
  Wire.begin();                     // Initialize I2C communication on default SDA and SCL pins
  lcd.init();                       // Initialize the LCD object
  lcd.backlight();                  // Turn on the LCD backlight
  lcd.clear();                      // Clear any existing text on the LCD
}

void loop() {
  Blynk.run();  // Keep the Blynk connection alive and process incoming commands
}

/* Manual Red Light Control — Binding to Blynk Controls V0 */
BLYNK_WRITE(V0) {                    // Define a Blynk event handler for virtual pin V0
  bool state = param.asInt();        // Read the integer value sent from the Blynk app (0 or 1)
  if (state) {                       // If the button is pressed (state == 1)
    digitalWrite(RED_PIN, HIGH);     // Turn on the red LED
    digitalWrite(GREEN_PIN, LOW);    // Ensure the green LED is turned off (mutually exclusive)
    lcd.clear();                     // Clear the LCD screen before printing
    lcd.setCursor(0, 0);             // Set cursor to column 0, row 0
    lcd.print("Crocodile");          // Print 'Crocodile' when red LED is on
  } else {                           // If the button is released (state == 0)
    digitalWrite(RED_PIN, LOW);      // Turn off the red LED
    lcd.clear();                     // Clear the LCD screen
    if (digitalRead(GREEN_PIN)) {    // If the green LED is active
      lcd.setCursor(0, 0);           // Set cursor to column 0, row 0
      lcd.print("No Crocodile");          // Print 'No Crocodile' if green LED is on
    }
  }
}

/* Manual Green Light Control - Binds to Blynk Control V1 */
BLYNK_WRITE(V1) {                    // Define a Blynk event handler for virtual pin V1
  bool state = param.asInt();        // Read the integer value sent from the Blynk app (0 or 1)
  if (state) {                       // If the button is pressed (state == 1)
    digitalWrite(GREEN_PIN, HIGH);   // Turn on the green LED
    digitalWrite(RED_PIN, LOW);      // Ensure the red LED is turned off (mutually exclusive)
    lcd.clear();                     // Clear the LCD screen before printing
    lcd.setCursor(0, 0);             // Set cursor to column 0, row 0
    lcd.print("No Crocodile");            // Print 'No Crocodile' when green LED is on
  } else {                           // If the button is released (state == 0)
    digitalWrite(GREEN_PIN, LOW);    // Turn off the green LED
    lcd.clear();                     // Clear the LCD screen
    if (digitalRead(RED_PIN)) {      // If the red LED is active
      lcd.setCursor(0, 0);           // Set cursor to column 0, row 0
      lcd.print("Crocodile");        // Print 'Crocodile' if red LED is on
    }
  }
}
