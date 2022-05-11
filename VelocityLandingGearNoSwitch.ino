// Velocity Landing Gear No Switch

#include <Adafruit_NeoPixel.h>

// This landing gear setup does not include the pilot switch from Arduino Control.
// The setup is Landing Gear Switch -> High Voltage/Current direct to relays -> Hydraulic Pump
// The Arduino here is used to feedback to the user based on changes to the landing gear switches.

// Four Switches are used to determine the position and state of the landing gear.
// 1. Nose Wheel Up Switch
// 2. Nose Wheel Down Switch
// 3. Rear Gear Up Switch
// 4. Rear Gear Down Switch

// The Landing gear state is fedback to the Pilot(s) via a 12 ring NeoPixel LED Ring.
// The 12oclock LED is the nose
// The 4oclock is Right Wheel
// the 8oclock is Left Wheel

// Both rear wheels are tied to the same micro switches as its not possible for them to move interdependently. 

// **********
// WARNING! ALL CODE HERE IS EXPERIMENTAL AND NOT TO BE USED IN AN AIRCRAFT DESIGN.
// PLEASE BE SENSIBLE WITH YOUR FLYING AND YOUR CODING. 
// *********

const int noseUpSwitch = A5;
const int noseDownSwitch = A4;
const int rearUpSwitch = A3;
const int rearDownSwitch = A2;

String noseGearPosition = "unknown";
String rearGearPosition = "unknown";

int firstBoot = 1;
int pixelBrightnessLevel = 255;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, 6, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  Serial.println("Landing Gear Manager Booting");

  
  // Setup Inputs for position Switches
  pinMode(noseUpSwitch, INPUT_PULLUP);
  pinMode(noseDownSwitch, INPUT_PULLUP);
  pinMode(rearUpSwitch, INPUT_PULLUP);
  pinMode(rearDownSwitch, INPUT_PULLUP);

  // Setup the NeoPixel Ring
  strip.begin();
  strip.show();
  //strip.setBrightness(pixelBrightnessLevel);

  Serial.println("Cycling Neo Pixel Ring");
  colorWipe(strip.Color(0,0,0), 25); // Black
  colorWipe(strip.Color(255, 0, 0), 100); // Red
  colorWipe(strip.Color(0, 255, 0), 100); // Green
  colorWipe(strip.Color(0, 0, 255), 100); // Blue
  strip.clear();

  Serial.println("Landing Gear Manager Finished Booting");
}

void loop() {
  if(firstBoot == 1){
    // If this is the first loop sequence, then we are going to just get values for where and what the sensors are doing.
    // Get the position of the nosegear
    Serial.println("Getting position of nose wheel");
    if(digitalRead(noseUpSwitch) == HIGH){
      noseGearPosition = "up";
      setFrontGearLED(strip.Color(0, 0, 255)); // Blue is up followed by off
      Serial.println("The nose wheel is up");
    } else if(digitalRead(noseDownSwitch) == HIGH){
      noseGearPosition = "down";
      setFrontGearLED(strip.Color(0, 255, 0));
      Serial.println("The nose wheel is down");
    } else {
      noseGearPosition = "error";
      Serial.println("Error getting status of nose wheel");
    }

    Serial.println("Getting position of rear wheel(s)");
    // Get the position of the reargear
    if(digitalRead(rearUpSwitch) == HIGH){
      rearGearPosition = "up";
      setRearGearLED(strip.Color(0, 0, 255));
      Serial.println("Rear gear is up");
    } else if(digitalRead(rearDownSwitch) == HIGH){
      rearGearPosition = "down";
      setRearGearLED(strip.Color(0, 255, 0));
      Serial.println("Rear gear is down");
    } else {
      rearGearPosition = "error";
      Serial.println("Error getting details of rear gear.");
    }

    Serial.println("Finished FirstBoot Sequence.");
    firstBoot = 0;
  } else {
    // Check the current state of the noseGear
  }
}

// Delete after testing

// Fill the dots one after the other with a color
void colorWipe(uint32_t color, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, color);
      strip.show();
      delay(wait);
  }
}

// Set the Rear gear position LEDs
void setRearGearLED(uint32_t color){
  strip.setPixelColor(4, color);
  strip.setPixelColor(8, color);
  strip.show();
}

// Set the front gear position LED
void setFrontGearLED(uint32_t color){
  strip.setPixelColor(0, color);
  strip.show();
}

// Cycle Lights Clockwise
void cyclePixelRingClockwise(uint32_t color){
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, color);
      strip.show();
      delay(40);
  }
}

// Cycle lights counterclockwise
void cyclePixelRingCounterClockwise(uint32_t color){

}
