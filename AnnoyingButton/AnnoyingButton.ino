#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// Button
const int buttonPin = 13;
int buttonState = 0;
bool isButtonPressed = false;

// SD Card (9 = RX, 8 = TX)
SoftwareSerial mySoftwareSerial(9, 8);
DFRobotDFPlayerMini dfpPlayer;
void printDetail(uint8_t type, int value);

int fileCount;

void setup()
{
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  // Use softwareSerial to communicate with mp3.
  if (!dfpPlayer.begin(mySoftwareSerial)) 
  {  
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  
  // Set volume value. From 0 to 30
  dfpPlayer.volume(20);

  dfpPlayer.readFileCounts(); 
  fileCount = dfpPlayer.readFileCountsInFolder(0);
  Serial.println(fileCount);
}

void loop()
{
  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW && !isButtonPressed) 
  {
    PlayRandom();

    isButtonPressed = true;
  } 
  else if (buttonState == HIGH && isButtonPressed)
  {
    isButtonPressed = false;
  }

  // Print the detail message from DFPlayer to handle different errors and states.
  if (dfpPlayer.available()) 
  {
    printDetail(dfpPlayer.readType(), dfpPlayer.read());
  }
}

void PlayRandom()
{
  dfpPlayer.play(random(0, fileCount));
}

void Play()
{
  dfpPlayer.play(1);
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}