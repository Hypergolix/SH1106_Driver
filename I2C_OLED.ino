#include <Wire.h>
#include "Fonts.h"
#include "Command.h"
#include "Graphics.h"
#include "appHandler.h"

#define BTNLEFT 1
#define BTNMIDDLE 0
#define BTNRIGHT 4

// RMW NOW HAS CUSTOM COLUMN OFFSET
void btnLEFT() {
  if (currentBtn != 0 && currentApp == 0) {    // As of now it unselects, changes the cursor position, then selects the updated position - May allow avoiding the need for an old state
    RMW(pageTable[currentBtn][0], currentBtn, 0); // To be unselected RMW(wordLength, tobeselected); - 1
    currentBtn--;
    RMW(pageTable[currentBtn][0], currentBtn, 0); // To be selected - 0. LAST number is column address offset
  }
}

void btnMIDDLE() {                             // ENTER SELECTED(currentBtn) IF VALID
  if (currentApp == 0) {                       // Only enter apps from the main menu
    appSelect(currentBtn);                     // Each has a number. Uses String right now instead of number, legibility.
  }
}

void btnRIGHT() {
  if (currentBtn != 7 && currentApp == 0) {    // Only use this kind of navigation for the home menu, could add others that follow the same nav method
    RMW(pageTable[currentBtn][0], currentBtn, 0); // Temporary , the LAST page + size should be saved somewhere and then put here so its unselected - 0
    currentBtn++;
    RMW(pageTable[currentBtn][0], currentBtn, 0); // Temporary , the NEW page + size should be saved somewhere and then put here so its unselected - 1
  }
  // Each page has its own nav system
}

// POR MEANS DEFAULT? - POWER ON RESET
void setup() {
  Wire.begin();
  pinMode(BTNMIDDLE, INPUT);
  pinMode(BTNLEFT, INPUT);
  pinMode(BTNRIGHT, INPUT);
  attachInterrupt (digitalPinToInterrupt (BTNLEFT), btnLEFT, HIGH);
  attachInterrupt (digitalPinToInterrupt (BTNMIDDLE), btnMIDDLE, HIGH);
  attachInterrupt (digitalPinToInterrupt (BTNRIGHT), btnRIGHT, HIGH);

  // INITIALIZE DISPLAY
  singleC_OP(0xae);                  // Command to turn display OFF

  dualCD_OP(0xd5, 0x80);             // Clock ratio 0x0f

  dualCD_OP(0xd3, 0x00);             // 0x00 = Top of screen? 0x10. Think this is visual and not affected by RAM. Display Offset - Double Byte

  singleC_OP(0x40);                  // 0x40 should be top, explains why page is offset weirdly. 0x48. Set start line << CHECK THIS. I think 0x40 should be start

  dualCD_OP(0xad, 0x8b);             // Built-In DC-DC Used - Double Byte

  singleC_OP(0x33);                  // Set Pump Voltage

  singleC_OP(0xc8);                  // Set scan direction

  singleC_OP(0xa1);                  // Segment remap
  // BELOW
  dualCD_OP(0xda, 0x12);             // Sequential, 0x12. COM HW CONFIG
  dualCD_OP(0xd9, 0xf1);             // 15 DCLK DisCharge. 1 DCLK PreCharge
  dualCD_OP(0xdb, 0x40);             // FIX THIS - VCOMH.64?
  // CHECK ABOVE. Unsure of what this does

  dualCD_OP(0x81, 0xff);             // Contrast control

  singleC_OP(0xa4);                  // Command to turn display ON? 0xa5. Output RAM << CHECK THIS. Is this necessary? POR == 0xa4

  singleC_OP(0xa6);                  // Normal Mode. 0xa7 - inverse. Set normal display mode

  // clrDisplay();                      // What would be useful to draw before the display is turned on?
  homePage();                        // Draws the home page. Could move this instead of clrDisplay to avoid repeating as it includes its own clrDisplay

  singleC_OP(0xaf);                  // Command to turn display ON

  // HAVING PAGE SPACES CAUSES ISSUES? MAY HAVE BEEN THAT 2 WERE OVERLAPPING ON THE SAME PAGE

}
// Function to work out the size of a written word as on screen - in pixels
// ADD SCROLLING MAX OPTIONS IS 8 RIGHT NOW
void loop() {


}
