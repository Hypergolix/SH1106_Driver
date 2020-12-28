// WriteDisplay(column add,Page add ,DATA); TEXT
//#include "Fonts.h"

#define DEVICE_ID 0x3C
#define COMMAND 0x00
#define RAMCOMMAND 0x40

// byte increment = 0x00;   // This may be ok to be global

bool fontSpc [8][2] =
{ {0, 0},
  {0, 0},
  {0, 0},
  {0, 0},
  {0, 0},
  {0, 0},
  {0, 0},
  {0, 0}
};

bool fontA [8][8] =
{ {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 1, 1, 0, 0, 1, 1, 0},
  {1, 1, 0, 0, 0, 0, 1, 1},
  {1, 1, 0, 0, 0, 0, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 0, 0, 0, 0, 1, 1},
  {1, 1, 0, 0, 0, 0, 1, 1}
};          // Row by column. Byte array may be more efficient
// One big array or multiple letters?

bool fontB [8][8] =
{ {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 0, 0, 0, 0, 1, 1},
  {1, 1, 0, 0, 0, 0, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 0, 0, 0, 0, 1, 1},
  {1, 1, 0, 0, 0, 0, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 0}
};


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
void incrementC(byte higher, byte lower) {         //
  //Set Start Column
  singleC_OP(B00010000 + higher);                   // 0000
  singleC_OP(B00000010 + lower);                    // 0010
}
// Takes the page as an argument. Always calls column set so that whenever setPage is called it will also begin from the leftmost column
void setPage(byte page) {                           // Just sets the page and uses the first column
  singleC_OP(0xB0 + page);                          // , byte data? 0xB0 + page
  columnSet();
}

void writeLetter(bool letterSelect[][8], byte increment) {

  // increment = increment + 2;                        // Account for spacing, replace 2 with var for variable spacing

  byte lower = increment & B00001111;               // Works out Lower 4 bits - just formatting
  byte higher = (increment & B11110000) >> 4;              // Works out Higher 4 bits - just formatting

  incrementC(higher, lower);                        // Just zeroes both column and page address, adds increment (0xB0 + increment);

  for (int i = 0; i < 8; i++) {                     // Line. Iterate for length of array
    byte column = B00000000;
    for (int yPos = 0; yPos < 8; yPos++) {          // Height
      column = column | ((letterSelect[yPos][i]) << (7 - yPos));
    }
    // Writes to display
    RAM_OP(column);                                 // Writes one column to the display
  }

}

uint8_t space = 1; // MAX 1px space at the moment, must be something odd going on further up when going above this. Masks/Shifts etc
// Again, could use a global variable that keeps track of the column where the next letter is allowed to be placed and this counter
// Is incremented with each letter
// Alternatively string together the entire "sentence" into one array and then just print that entire array in one go to the display
// Remember to decide between using individual arrays for each letter or one big one for the entire font face which contains 
// The entire alphabet. Could Optimize by makeing byte arrays - however less legible. Advantages of individual array for each
// Letter means you can mix and match fonts and styles 

// Sort out the reason for why the display stops incrementing by itself after each letter, This would solve a lot of issues related 
// To spacing and accidental overwrites

void writeText(byte column, byte page, String text) {
  setPage(page);
  
  for (uint8_t stringPos = 0; stringPos < text.length(); stringPos++) {   // Iterates for length of the string
    byte increment = (stringPos * (8 + space)) + column;                            // Works out the amount to increment by so letters don't overwrite. Now adds column offset
    //increment = increment + 8; // + spacing?
    switch (text.charAt(stringPos)) {                                     // Accesses specific character of string
      case 'A':
        writeLetter(fontA, increment);   // fontA (stringPos * 8) In the future just return the array and then write the fucntion call once at the bottom. Deal with increment
        // return(fontA);
        break;
      case 'B':
        writeLetter(fontB, increment);   // fontB (stringPos * 8)
        // return(fontB);
        break;
    }
  }
}
