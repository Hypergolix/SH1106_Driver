#include <RTCZero.h>

RTCZero rtc;                                          // Creates an object. Not sure how this works

volatile uint8_t currentBtn = 0;                      // Which button was pressed? start on 0. THIS probably should not be global but also makes sense that it is
volatile uint8_t currentApp = 0;                      // Keep track of app open? - Should this be a string? - may be inneficient

// HH:MM:SS - Clock app would be best if it worked on interrupts

// Keep track of app entered - they all need a back button too
void homePage() {                                     // Draws the home page
  rtc.begin();                                        // Starts the clock

  // This should be a function to set the time in the future - just for testing. Sets time.
  // May write this myself in future
  rtc.setHours(21);
  rtc.setMinutes(28);
  rtc.setSeconds(0);

  clrDisplay();                                       // This would run twice at the start which isn't ideal. Could move this function in its place? that is if we can write to ram like normal
  writeText(0, 0, "S E T TIN G S");                   // Column, page, string. function should reflect its in page write mode
  RMW(pageTable[currentBtn][0], currentBtn);          // 74 columns, 1st page, may need a start and end page actually. Could rename to INVERT. THIS one specifically highlights the top one
  writeText(0, 1, "C L O C K");                       // Add a graphics mode. Should use the big array
}

String underTen(uint8_t seconds, uint8_t minutes, uint8_t hours) {
  String writeTime;
  // What is this? again might actually be necessary or use a switch?
  // Weird bug - space after hours
  if (hours < 10) {
    writeTime = String("0" + hours );
    writeTime = String(writeTime + ":");
  }
  else {
    writeTime = String(writeTime + hours + ":");
  }
  if (minutes < 10) {
    writeTime = String(writeTime + "0" + minutes + ":");
  }
  else {
    writeTime = String(writeTime + minutes + ":");
  }
  if (seconds < 10) {
    writeTime = String(writeTime + "0" + seconds);
  }
  else {
    writeTime = String(writeTime + seconds);
  }

  return writeTime;
}

void clockApp() {
  clrDisplay();                                       // Clears the display
  writeText(35, 1, "C L O C K");                      // Renders this page
  // convert time to string

  while (currentApp == 1) {                           // OR something, needs to keep updating. This will be the app loop
    uint8_t seconds = rtc.getSeconds();               //
    uint8_t minutes = rtc.getMinutes();               //
    uint8_t hours = rtc.getHours();                   //

    String writeTime = underTen(seconds, minutes, hours);

    writeText(25, 4, writeTime);                      // writeTime, make colon character
  }
}

// CLOCK FUNCTION
void appSelect(uint8_t appNr) {
  switch (appNr) {
    case 1:
      currentApp = 1;                                // Sets the current app to the clock app
      currentBtn = 0;                                // Resets the current button position. Zero the currentBtn. Could also be inside the function
      clockApp();                                    // Enter/Render the actual app
      break;
    default:
      break;
  }
}
