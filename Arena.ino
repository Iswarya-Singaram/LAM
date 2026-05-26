#include <Arduino.h>
#include <HX711.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

/* =========================================================================
   HARDWARE MAPPING (REVISED LAYOUT)
   ========================================================================= */
// --- SENSORS & ACTUATORS ---
#define PIN_GATE_1_IR    4   // Gate 1 Sensor
#define PIN_GATE_2_IR    13  // Gate 2 Sensor
#define PIN_GATE_3_IR    14  // Gate 3 Sensor
#define PIN_PUMP_RELAY   16  // Water Pump Relay
#define PIN_LED_RELAY    17  // LAM LED Relay

// --- LOAD CELL (HX711) ---
// Moved to 19 and 25 to free up Hardware SPI for the display
#define PIN_LOADCELL_DT  19  
#define PIN_LOADCELL_SCK 25

// --- TFT LCD DISPLAY (1.8" ST7735) ---
// Using Hardware SPI Pins: SCK=18, MOSI=23 automatically
#define TFT_CS     27  // Chip Select (Avoided Pin 5)
#define TFT_RST    22  // Reset
#define TFT_DC     21  // Data/Command

/* =========================================================================
   CONSTANTS & OBJECTS
   ========================================================================= */
// Initialize Display using Hardware SPI (Faster)
// Notice we only pass CS, DC, and RST. It automatically finds SCK(18) and MOSI(23).
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Initialize Load Cell
HX711 scale;

// --- CONFIGURATION (YOU MUST TUNE THESE) ---
const unsigned long PUMP_RUNTIME_MS = 5000;      // Time to dispense 125ml
const float LOADCELL_CALIBRATION_FACTOR = 420.0; // RUN CALIBRATION SKETCH FIRST!
const float TARGET_WEIGHT_MIN = 0.1;             // Min kg
const float TARGET_WEIGHT_MAX = 5.0;             // Max kg
const int SENSOR_TRIGGERED = LOW;                // IR Sensor State (LOW or HIGH)

// State Tracking
bool gate1Completed = false;
bool gate2Completed = false;
bool gate3Completed = false;

/* =========================================================================
   SETUP
   ========================================================================= */
void setup() {
  Serial.begin(115200);
  Serial.println("--- LAM ARENA CONTROLLER STARTING ---");

  // 1. Initialize Display
  // Try INITR_BLACKTAB, INITR_GREENTAB, or INITR_REDTAB if colors look wrong
  tft.initR(INITR_BLACKTAB); 
  tft.setRotation(1); // Landscape orientation
  tft.fillScreen(ST7735_BLACK);
  
  // Initial Welcome Screen
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("HARDWARE");
  tft.setCursor(10, 30);
  tft.println("HUSTLE");
  tft.setTextSize(1);
  tft.setCursor(10, 60);
  tft.println("Initializing...");
  delay(1000);

  // 2. Initialize Pins
  pinMode(PIN_PUMP_RELAY, OUTPUT);
  pinMode(PIN_LED_RELAY, OUTPUT);
  digitalWrite(PIN_PUMP_RELAY, LOW); 
  digitalWrite(PIN_LED_RELAY, LOW);

  pinMode(PIN_GATE_1_IR, INPUT);
  pinMode(PIN_GATE_2_IR, INPUT);
  pinMode(PIN_GATE_3_IR, INPUT);

  // 3. Initialize Load Cell
  Serial.println("Initializing Load Cell...");
  scale.begin(PIN_LOADCELL_DT, PIN_LOADCELL_SCK);
  scale.set_scale(LOADCELL_CALIBRATION_FACTOR);
  scale.tare(); // Reset scale to zero

  // Ready Status
  updateScreen("SYSTEM READY", "Waiting for Bot...", ST7735_GREEN);
  Serial.println("System Ready.");
}

/* =========================================================================
   MAIN LOOP
   ========================================================================= */
void loop() {
  // --- GATE 1: FLUID DISPENSING ---
  if (!gate1Completed && digitalRead(PIN_GATE_1_IR) == SENSOR_TRIGGERED) {
    Serial.println("GATE 1: Robot Detected");
    updateScreen("GATE 1", "Pumping Fluid...", ST7735_CYAN);
    
    digitalWrite(PIN_PUMP_RELAY, HIGH); // Pump ON
    delay(PUMP_RUNTIME_MS);             // Wait 
    digitalWrite(PIN_PUMP_RELAY, LOW);  // Pump OFF
    
    updateScreen("GATE 1", "Complete!", ST7735_GREEN);
    gate1Completed = true;
  }

  // --- GATE 2: LAM LEDs ---
  if (!gate2Completed && digitalRead(PIN_GATE_2_IR) == SENSOR_TRIGGERED) {
    Serial.println("GATE 2: Robot Detected");
    updateScreen("GATE 2", "LAM LEDs ON", ST7735_MAGENTA);
    
    digitalWrite(PIN_LED_RELAY, HIGH); // LEDs ON
    gate2Completed = true;
  }

  // --- GATE 3: PARKING & WEIGHING ---
  if (!gate3Completed && digitalRead(PIN_GATE_3_IR) == SENSOR_TRIGGERED) {
    Serial.println("GATE 3: Robot Detected");
    updateScreen("GATE 3", "Weighing...", ST7735_ORANGE);
    
    // Allow time for robot to stop and fluid to settle
    delay(3000); 

    // Take measurement
    float weight = scale.get_units(10); 
    Serial.print("Weight: "); Serial.println(weight);

    tft.fillScreen(ST7735_BLACK);
    
      // SUCCESS SCREEN
      tft.fillScreen(ST7735_GREEN);
      tft.setTextColor(ST7735_BLACK);
      tft.setTextSize(2);
      tft.setCursor(20, 30);
      tft.println("SUCCESS!");
      tft.setTextSize(1);
      tft.setCursor(20, 60);
      tft.print("Weight: ");       tft.print(weight); tft.print("kg");
      tft.setTextColor(ST7735_WHITE);
      
    
    gate3Completed = true;
  }
}

/* =========================================================================
   HELPER FUNCTION FOR LCD
   ========================================================================= */
void updateScreen(String title, String status, uint16_t color) {
  tft.fillScreen(ST7735_BLACK);
  tft.drawRect(0, 0, 160, 128, ST7735_WHITE); // Draw border
  
  tft.setCursor(10, 10);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
  tft.println("HARDWARE HUSTLE");
  
  tft.drawLine(0, 25, 160, 25, ST7735_WHITE); 

  tft.setCursor(10, 40);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(2);
  tft.println(title);

  tft.setCursor(10, 70);
  tft.setTextColor(color);
  tft.setTextSize(1);
  tft.println(status);
}
