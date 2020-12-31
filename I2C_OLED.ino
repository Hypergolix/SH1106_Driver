#include <Wire.h>
#include "Fonts.h"
#include "Command.h"

#define BTNLEFT 0
#define BTNMIDDLE 1
#define BTNRIGHT 4

bool RAMArray[64][132];                   // Row by column. This array represents the total RAM

volatile char btnPressed;                 // Which button was pressed?

void clrDisplay() {
  for (int y = 0; y < 8; y++) {           // Page
    // RAM OPERATION
    //setPage(y);                         // Just zeroes both column and page address
    singleC_OP(0xB0 + y);
    for (int i = 0; i < 132; i++) {       // Line. 128 in the future or just have a blanking time of 4px
      // at the end of 128, increment the page but also zero the column adress
      RAM_OP(0x00);                       // Clears the column
    }
  }
}
/*
  void writeDisplay() {
  // START PAGE
  setPage(0xB0 + 0x00);                   // Just zeroes both column and page address

  for (int i = 0; i < 8; i++) {           // Line. Iterate for length of array
    byte column = B00000000;
    for (int yPos = 0; yPos < 8; yPos++) {// Height
      column = column | ((fontA[yPos][i]) << (7 - yPos));
    }
    // Writes to display
    RAM_OP(column);                       // Writes one column to the display
  }
  }
*/

void buttonPress(uint8_t buttonNr) {
  switch (buttonNr) {
    case BTNLEFT:

      break;
    case BTNMIDDLE:

      break;
    case BTNRIGHT:

      break;
  }
}

void btnLEFT() {
  buttonPress(BTNLEFT);
}

void btnMIDDLE() {
  buttonPress(BTNMIDDLE);
}

void btnRIGHT() {
  buttonPress(BTNRIGHT);
}


void setup() {
  // REDUCE DELAYS, WRITE FUNCTION
  Wire.begin();
  pinMode(BTNMIDDLE, INPUT);
  pinMode(BTNLEFT, INPUT);
  pinMode(BTNRIGHT, INPUT);
  attachInterrupt (digitalPinToInterrupt (BTNLEFT), btnLEFT, HIGH);
  attachInterrupt (digitalPinToInterrupt (BTNMIDDLE), btnMIDDLE, HIGH);
  attachInterrupt (digitalPinToInterrupt (BTNRIGHT), btnRIGHT, HIGH);
  delay(100);

  // INITIALIZE DISPLAY
  // Turn OFF Display
  Wire.beginTransmission(DEVICE_ID); // Transmit to device at 0x3C
  Wire.write(0x00);                  // Control Byte?
  Wire.write(0xae);                  // Command to turn display OFF
  Wire.endTransmission();
  delay(100);

  // Clock ratio - Double Byte
  Wire.beginTransmission(DEVICE_ID); // Transmit to device at 0x3C
  Wire.write(0x00);                  // Control Byte?
  Wire.write(0xd5);                  // Address a regsiter?
  Wire.write(0x80);                  // 0x0f
  Wire.endTransmission();
  delay(100);

  // Multiplex ratio? - Double Byte
  //Wire.beginTransmission(DEVICE_ID); // Transmit to device at 0x3C
  //Wire.write(0x00);                  // Control Byte?
  //Wire.write(0xa8);                  // Address a regsiter?
  //Wire.write(0x1f);                  // 0x3f
  //Wire.endTransmission();
  //delay(100);

  // Display Offset - Double Byte
  Wire.beginTransmission(DEVICE_ID); // Transmit to device at 0x3C
  Wire.write(0x00);                  // Control Byte?
  Wire.write(0xd3);                  // Address a register?
  Wire.write(0x00);                  // 0x00 = Top of screen? 0x10
  Wire.endTransmission();            // Think this is visual and not affected by RAM
  delay(100);

  // Set start line << CHECK THIS. I think 0x40 should be start
  Wire.beginTransmission(DEVICE_ID); // Transmit to device at 0x3C
  Wire.write(0x00);                  // Control Byte?
  Wire.write(0x40);                  // 0x40 should be top, explains why page is offset weirdly. 0x48
  Wire.endTransmission();
  delay(100);

  // Built-In DC-DC Used - Double Byte
  Wire.beginTransmission(DEVICE_ID); // Transmit to device at 0x3C
  Wire.write(0x00);                  // Control Byte?
  Wire.write(0xad);                  // 2-Byte Command
  Wire.write(0x8b);                  // 2nd Byte
  Wire.endTransmission();
  delay(100);

  // Set Pump Voltage
  Wire.beginTransmission(DEVICE_ID); // Transmit to device at 0x3C
  Wire.write(0x00);                  // Control Byte?
  Wire.write(0x33);                  // Set to 8 Volt. 0x30 - 0x33 for 9V
  Wire.endTransmission();
  delay(100);

  // Set scan direction
  Wire.beginTransmission(DEVICE_ID); // Transmit to device at 0x3C
  Wire.write(0x00);                  // Control Byte?
  Wire.write(0xc8);                  // C0 Normal. C8 Vertical Flip
  Wire.endTransmission();
  delay(100);

  // Segment remap
  Wire.beginTransmission(DEVICE_ID); // Transmit to device at 0x3C
  Wire.write(0x00);                  // Control Byte?
  Wire.write(0xa1);                  // a0 = left. a1 = right
  Wire.endTransmission();
  delay(100);

  //  Unsure below. POR MEANS DEFAULT?

  Wire.beginTransmission(DEVICE_ID); // Transmit to device at 0x3C
  Wire.write(0x00);                  // Control Byte?
  Wire.write(0xda);                  // COM HW CONFIG
  Wire.write(0x12);                  // Sequential, 0x12
  Wire.endTransmission();
  delay(100);
  Wire.beginTransmission(DEVICE_ID); // Transmit to device at 0x3C
  Wire.write(0x00);                  // Control Byte?
  Wire.write(0xd9);                  //
  Wire.write(0xf1);                  // 15 DCLK DisCharge. 1 DCLK PreCharge
  Wire.endTransmission();
  delay(100);
  Wire.beginTransmission(DEVICE_ID); // FIX THIS - VCOMH
  Wire.write(0x00);                  // Control Byte?
  Wire.write(0xdb);                  //
  Wire.write(0x40);                  // 64?
  Wire.endTransmission();
  delay(100);

  // CHECK ABOVE. Unsure of what this does

  // Contrast control
  Wire.beginTransmission(DEVICE_ID); // Transmit to device at 0x3C
  Wire.write(0x00);                  // Control Byte?
  Wire.write(0x81);                  // Contrast Register
  Wire.write(0xff);                  // Control Byte?
  Wire.endTransmission();
  delay(100);

  // Output RAM << CHECK THIS. Is this necessary? POR == 0xa4
  Wire.beginTransmission(DEVICE_ID); // Transmit to device at 0x3C
  Wire.write(0x00);                  // Control Byte?
  Wire.write(0xa4);                  // Command to turn display ON
  Wire.endTransmission();
  delay(100);

  // Set normal display mode
  Wire.beginTransmission(DEVICE_ID); // Transmit to device at 0x3C
  Wire.write(0x00);                  // Control Byte?
  Wire.write(0xa6);                  // Normal Mode. 0xa7 - inverse
  Wire.endTransmission();
  delay(100);

  clrDisplay();

  // Turn ON Display
  Wire.beginTransmission(DEVICE_ID); // Transmit to device at 0x3C
  Wire.write(0x00);                  // Control Byte?
  Wire.write(0xaf);                  // Command to turn display ON
  Wire.endTransmission();
  delay(100);

  //writeDisplay();

  //writeText(0, 0, "S E T TIN G S");         // Column, page, string. function should reflect its in page write mode
  //writeText(0, 1, "C L O C K");             // Add a graphics mode. Should use the big array
}

void loop() {

}
