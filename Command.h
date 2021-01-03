#define DEVICE_ID 0x3C
#define COMMAND 0x00
#define RAMCOMMAND 0x40

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


void RAMS_OP() {
  Wire.beginTransmission(DEVICE_ID);                //
  Wire.write(RAMCOMMAND);                           // Addresses RAM
  Wire.endTransmission();
}


byte read_OP() {                                    // uint8_t byteNumber
  byte reading;
  RAMS_OP();
  Wire.requestFrom(DEVICE_ID, 1);                   // DUMMY READ?
  byte dummy = Wire.read();
  RAMS_OP();
  Wire.requestFrom(DEVICE_ID, 1);                   // Request byteNumber amount of bytes
  reading = Wire.read();
  return reading;
}

void clrDisplay() {
  for (int y = 0; y < 8; y++) {                    // Page
    // RAM OPERATION
    //setPage(y);                                  // Just zeroes both column and page address
    singleC_OP(0xB0 + y);
    for (int i = 0; i < 132; i++) {                // Line. 128 in the future or just have a blanking time of 4px
      // at the end of 128, increment the page but also zero the column adress
      RAM_OP(0x00);                                // Clears the column
    }
  }
}

// This one sets it to the leftmost column position
void columnSet() {                                  // Resets column to the first one. Have arguments for a distance from the left
  //Set Start Column
  singleC_OP(B00010000);                            // 0000 - HIGHER  
  singleC_OP(B00000010);                            // 0010 - LOWER
}

// Sets the initial column Adress, takes a starting column as an argument
void incrementC(byte increment) {                   // Now takes one arg

  byte lower = increment & B00001111;               // Works out Lower 4 bits - just formatting
  byte higher = (increment & B11110000) >> 4;       // Works out Higher 4 bits - just formatting

  //Set Start Column
  singleC_OP(B00010000 + higher);                   // 0000
  singleC_OP(B00000000 + lower);                    // 0010
}

// Takes the page as an argument. Always calls column set so that whenever setPage is called it will also begin from the leftmost column
void setPage(byte page) {                           // Just sets the page and uses the first column
  singleC_OP(0xB0 + page);                          // , byte data? 0xB0 + page
  columnSet();                                      // REMOVE?? May cause issues
}

void writeSpace(uint8_t spaceSize) {                // Deals with writing spaces
  for (int y = 0; y < spaceSize; y++) {             // Iterates for the number of spaces
    RAM_OP(0x00);                                   // Writes spaces
  }
  //pageTable[page][0] = (text.length() + spaceSize);
}

void writeLetter(bool letterSelect[][8]) {          // In the future allow for custom width letters
  uint8_t space = 2;                                // Paramater for space width, in pixels. Default = 2. In future could have custom space width

  for (int i = 0; i < 8; i++) {                     // Line. Iterate for length of array
    byte column = B00000000;
    for (int yPos = 0; yPos < 8; yPos++) {          // Height
      column = column | ((letterSelect[yPos][i]) << yPos); // OLD = (7 - yPos). NEW yPos
    }
    RAM_OP(column);                                 // Writes one column to the display
  }
  //pageTable[page][0] = (text.length() + 8);
}

// ADD numbers mode?
void writeText(byte column, byte page, String text) {
  byte increment = 2 + column;                                            // 2 default to account for pixel difference
  setPage(page);                                                          // This one resets the column address, be aware of this
  incrementC(increment);                                                  // Just sets the start column address. Would it be better to only take one number as an argument?
  //pageTable[page][1] = (text.length() * 8);                             // This could probably be at the end of writing all the letters? Does it matter?

  for (uint8_t stringPos = 0; stringPos < text.length(); stringPos++) {   // Iterates for length of the string
    // THIS SHOULD BE TEMPORARY - TECH DEBT
    // THIS is to keep track of word length in each page
    if (text.charAt(stringPos) != ' ') {
      pageTable[page][0] = (pageTable[page][0] + 8);                      // SYNTAX - Ss it necessary to repeat pageTable[page][0]? Keep as it's safe to work. This wont be needed if we use RMW
    } else {
      pageTable[page][0] = (pageTable[page][0] + 2);
    }

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
      case '!':
        writeLetter(fontExl); //
        break;
      case '0':
        writeLetter(fontZero);// fontB (stringPos * 8)
        // return();
        break;
      case '1':
        writeLetter(fontOne); // fontB (stringPos * 8)
        // return();
        break;
      case '2':
        writeLetter(fontTwo); // fontB (stringPos * 8)
        // return();
        break;
      case '3':
        writeLetter(fontThree);// fontB (stringPos * 8)
        // return();
        break;
      case '4':
        writeLetter(fontFour); // fontB (stringPos * 8)
        // return();
        break;
      case '5':
        writeLetter(fontFive); // fontB (stringPos * 8)
        // return();
        break;
      case '6':
        writeLetter(fontSix);  // fontB (stringPos * 8)
        // return();
        break;
      case '7':
        writeLetter(fontSeven);// fontB (stringPos * 8)
        // return();
        break;
      case '8':
        writeLetter(fontEight);// fontB (stringPos * 8)
        // return();
        break;
      case '9':
        writeLetter(fontNine); // fontB (stringPos * 8)
        // return();
        break;
      case ':':
        writeLetter(fontCol); // fontB (stringPos * 8)
        // return();
        break;
      default:                 // If the letter does't exist draw the not found symbol
        writeLetter(fontDefault);
        break;
    }
    // pageTable[page][1] = text.length() // This could maybe be at the top?

    // Call here as one function call? writeLetter(font, ifSpace);
  }
}
