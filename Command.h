// WriteDisplay(column add,Page add ,DATA); TEXT
//#include "Fonts.h"

#define DEVICE_ID 0x3C
#define COMMAND 0x00
#define RAMCOMMAND 0x40

// byte increment = 0x00;   // This may be ok to be global

// Rename some of these variables
void singleC_OP(byte firstParam) {                  // Single command OP
  Wire.beginTransmission(DEVICE_ID);                //
  Wire.write(COMMAND);                              //
  Wire.write(firstParam);                           // The only command
  Wire.endTransmission();
}

void dualCD_OP(byte firstParam, byte secondParam) { // Double command OP
  Wire.beginTransmission(DEVICE_ID);                //
  Wire.write(COMMAND);                              //
  Wire.write(firstParam);                           // Usually addresses the register
  Wire.write(secondParam);                          // DATA for the register
  Wire.endTransmission();
}

void RAM_OP(byte firstParam) {                      // RAM operation OP
  Wire.beginTransmission(DEVICE_ID);                //
  Wire.write(RAMCOMMAND);                           // Addresses RAM
  Wire.write(firstParam);                           // Usually addresses the register
  Wire.endTransmission();                           //
}

// This one sets it to the leftmost column position
void columnSet() {                                  // Resets column to the first one. Have arguments for a distance from the left
  //Set Start Column
  singleC_OP(B00000010);                            // 0010
  singleC_OP(B00010000);                            // 0000
}

// Manual increment, could change this so that only one arg is passed. Lower and Higher in the same byte
void incrementC(byte higher, byte lower) {          //
  //Set Start Column
  singleC_OP(B00010000 + higher);                   // 0000
  singleC_OP(B00000000 + lower);                    // 0010
}

// Takes the page as an argument. Always calls column set so that whenever setPage is called it will also begin from the leftmost column
void setPage(byte page) {                           // Just sets the page and uses the first column
  singleC_OP(0xB0 + page);                          // , byte data? 0xB0 + page
  columnSet();
}

void writeSpace(uint8_t spaceSize) {                // Deals with writing spaces
  for (int y = 0; y < spaceSize; y++) {             // Iterates for the number of spaces
    RAM_OP(0x00);                                   // Writes spaces
  }
}

void writeLetter(bool letterSelect[][8]) {
  uint8_t space = 2;                                // Paramater for space width, in pixels. Default = 2. In future could have custom space width

  for (int i = 0; i < 8; i++) {                     // Line. Iterate for length of array
    byte column = B00000000;
    for (int yPos = 0; yPos < 8; yPos++) {          // Height
      column = column | ((letterSelect[yPos][i]) << (7 - yPos));
    }
    // Writes column to display
    RAM_OP(column);                                 // Writes one column to the display
  }

}

void writeText(byte column, byte page, String text) {
  uint8_t increment = 2 + column;                                         // 2 default to account for pixel difference

  setPage(page);

  byte lower = increment & B00001111;                                     // Works out Lower 4 bits - just formatting
  byte higher = (increment & B11110000) >> 4;                             // Works out Higher 4 bits - just formatting

  incrementC(higher, lower);                                              // Just sets the start column address. Would it be better to only take one number as an argument?

  for (uint8_t stringPos = 0; stringPos < text.length(); stringPos++) {   // Iterates for length of the string
    switch (text.charAt(stringPos)) {                                     // Accesses specific character of string
      case 'A':
        writeLetter(fontA);   // In the future just return the array and then write the fucntion call once at the bottom. Deal with increment
        // return(fontA);
        break;
      case 'B':
        writeLetter(fontB);   // fontB (stringPos * 8)
        // return(fontB);
        break;
      case 'C':
        writeLetter(fontC);   // fontB (stringPos * 8)
        // return(fontC);
        break;
      case 'D':
        writeLetter(fontD);   // fontB (stringPos * 8)
        // return();
        break;
      case 'E':
        writeLetter(fontE);   // fontB (stringPos * 8)
        // return();
        break;
      case 'F':
        writeLetter(fontF);   // fontB (stringPos * 8)
        // return();
        break;
      case 'G':
        writeLetter(fontG);   // fontB (stringPos * 8)
        // return();
        break;
      case 'H':
        writeLetter(fontH);   // fontB (stringPos * 8)
        // return();
        break;
      case 'I':
        writeLetter(fontI);   // fontB (stringPos * 8)
        // return();
        break;
      case 'J':
        writeLetter(fontJ);   // fontB (stringPos * 8)
        // return();
        break;
      case 'K':
        writeLetter(fontK);   // fontB (stringPos * 8)
        // return();
        break;
      case 'L':
        writeLetter(fontL);   // fontB (stringPos * 8)
        // return();
        break;
      case 'M':
        writeLetter(fontM);   // fontB (stringPos * 8)
        // return();
        break;
      case 'N':
        writeLetter(fontN);   // fontB (stringPos * 8)
        // return();
        break;
      case 'O':
        writeLetter(fontO);   // fontB (stringPos * 8)
        // return();
        break;
      case 'P':
        writeLetter(fontP);   // fontB (stringPos * 8)
        // return();
        break;
      case 'Q':
        writeLetter(fontQ);   // fontB (stringPos * 8)
        // return();
        break;
      case 'R':
        writeLetter(fontR);   // fontB (stringPos * 8)
        // return();
        break;
      case 'S':
        writeLetter(fontS);   // fontB (stringPos * 8)
        // return();
        break;
      case 'T':
        writeLetter(fontT);   // fontB (stringPos * 8)
        // return();
        break;
      case 'U':
        writeLetter(fontU);   // fontB (stringPos * 8)
        // return();
        break;
      case 'V':
        writeLetter(fontV);   // fontB (stringPos * 8)
        // return();
        break;
      case 'W':
        writeLetter(fontW);   // fontB (stringPos * 8)
        // return();
        break;
      case 'X':
        writeLetter(fontX);   // fontB (stringPos * 8)
        // return();
        break;
      case 'Y':
        writeLetter(fontY);   // fontB (stringPos * 8)
        // return();
        break;
      case 'Z':
        writeLetter(fontZ);   // fontB (stringPos * 8)
        // return();
        break;
      case ' ':
        writeSpace(2);        // Argument is the width of the space
        // return(fontSpace);
        break;
      default:                // If the letter does't exist draw the not found symbol
        writeLetter(fontDefault);
        break;
    }

    // Call here as one function call? writeLetter(font, ifSpace);
  }
}
