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

const int noseUpSwitch = A5;
const int noseDownSwitch = A4;
const int rearUpSwitch = A3;
const int rearDownSwitch = A2;

String noseGearPosition = "unknown";
String rearGearPosition = "unknown";

String previousNoseGearPosition = "unknown";
String previousRearGearPosition = "unknown";

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
      previousNoseGearPosition = "up";
      setFrontGearLED(strip.Color(0, 0, 255)); // Blue is up followed by off
      Serial.println("The nose wheel is up");
    } else if(digitalRead(noseDownSwitch) == HIGH){
      noseGearPosition = "down";
      previousNoseGearPosition = "down";
      setFrontGearLED(strip.Color(0, 255, 0));
      Serial.println("The nose wheel is down");
    } else {
      noseGearPosition = "error";
      previousNoseGearPosition = "error";
      Serial.println("Error getting status of nose wheel");
    }

    Serial.println("Getting position of rear wheel(s)");
    // Get the position of the reargear
    if(digitalRead(rearUpSwitch) == HIGH){
      rearGearPosition = "up";
      previousRearGearPosition = "up";
      setRearGearLED(strip.Color(0, 0, 255));
      Serial.println("Rear gear is up");
    } else if(digitalRead(rearDownSwitch) == HIGH){
      rearGearPosition = "down";
      previousRearGearPosition = "down";
      setRearGearLED(strip.Color(0, 255, 0));
      Serial.println("Rear gear is down");
    } else {
      rearGearPosition = "error";
      previousRearGearPosition = "error";
      Serial.println("Error getting details of rear gear.");
    }

    // If the landing gear is up (both nose and rear)
    // delay 2 seconds so the pilot can see and then clear the blue 
    // and then clear the LEDs so that we can maintain good lights out behavior. 

    if(noseGearPosition == "up" && rearGearPosition == "up"){
      delay(2000);
      strip.clear();
    }

    Serial.println("Finished FirstBoot Sequence.");
    firstBoot = 0;
  } else {
    // Check the current state of the noseGear vs its known previous state.
    if(digitalRead(noseUpSwitch) == HIGH){
      noseGearPosition = "up";
    }

    if(digitalRead(noseDownSwitch) == HIGH){
      noseGearPosition = "down";
    }

    if(digitalRead(rearUpSwitch) == HIGH){
      rearGearPosition = "up";
    }

   if(digitalRead(rearDownSwitch) == HIGH){
      noseGearPosition = "down";
    }



    // If the noseGearPosition is up 
    // and the previousNoseGearPosition is up 
    // and the rearGearPosition is up
    // and the previousRearGearPosition is up
    // Then nothing has changed.
    if(noseGearPosition == "up" && previousNoseGearPosition == "up" && rearGearPosition == "up" && previousRearGearPosition == "up"){
      Serial.println("The gear switch is up on this cycle. Nothing to see here.");
      strip.clear();
      strip.show();
      delay(40);
    } 
    // If the current position is down and the old position is up then the pilot has asked for the gear to come down
    else if(noseGearPosition == "down" && previousNoseGearPosition == "up" && rearGearPosition == "down" && previousRearGearPosition == "up"){
      Serial.println("The pilot has asked for the landing gear to come down, flash the green LED");
      setFrontGearLED(strip.Color(0, 255, 0));
      setRearGearLED(strip.Color(0, 255, 0));
      delay(40);
      strip.clear();
      setFrontGearLED(strip.Color(0, 255, 0));
      setRearGearLED(strip.Color(0, 255, 0));
      delay(40);
      strip.clear();
    }
    // If the current position is up and the old position is down then the pilot has asked for the gear to come up
    else if(noseGearPosition == "up" && previousNoseGearPosition == "down" && rearGearPosition == "up" && previousRearGearPosition == "down"){
      Serial.println("The pilot has asked for the landing gear to come up, flash the blue LED");
      setFrontGearLED(strip.Color(0, 0, 255));
      setRearGearLED(strip.Color(0, 0, 255));
      delay(40);
      strip.clear();
      setFrontGearLED(strip.Color(0, 0, 255));
      setRearGearLED(strip.Color(0, 0, 255));
      delay(40);
      strip.clear();
    } 
    // If the current position is down and the old position is down, the landing gear is locked down. In this case permenantly keep the GREEN LED's lit.
    else if(noseGearPosition == "down" && previousNoseGearPosition == "down" && rearGearPosition == "down" && previousRearGearPosition == "down"){
      Serial.println("The the landing gear is down and locked.");
      strip.clear();
      setFrontGearLED(strip.Color(0, 255, 0));
      setRearGearLED(strip.Color(0, 255, 0));
    }

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
