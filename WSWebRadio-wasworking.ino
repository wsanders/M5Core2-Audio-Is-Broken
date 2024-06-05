/* WAS working yesterday, now it's broken - no audio output - ???? Errors:
STATUS(srcbuf) '2' = 'Refilling buffer'
STATUS(generator) '257' = 'Decoding error 'lost synchronization' at byte offset 4308'
STATUS(generator) '257' = 'Decoding error 'lost synchronization' at byte offset 7081'
STATUS(generator) '565' = 'Decoding error 'bad main_data_begin pointer' at byte offset 708'
STATUS(generator) '568' = 'Decoding error 'Huffman data overrun' at byte offset 32720'
*/
/* Includes from:
https://gist.github.com/CelliesProjects/022235e5d126a3b8ce1b024c43ba6de5
DONT WORK
#include <M5Core2.h>
#include <WiFi.h>
#include "AudioFileSourceICYStream.h"
#include "AudioFileSourceBuffer.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
*/

/* includes from:
https://github.com/bwbguard/M5Stack-Core2-MediaPlayer/blob/main/Core2_Media_Player.ino
*/
#include <M5Core2.h>
#include <driver/i2s.h>
#include <WiFi.h>
#include <AudioFileSourceICYStream.h>
#include <AudioFileSource.h>
#include <AudioFileSourceBuffer.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>

AudioGeneratorMP3 *mp3;
AudioFileSourceICYStream *file;
AudioFileSourceBuffer *buff;
AudioOutputI2S *out;

const char * url = "http://stream1.early1900s.org:8080";
//const char * url = "http://streams.kut.org/4430_192.mp3?aw_0_1st.playerid=tmx-free";

const char* ssid = "changeme";
const char* password = "changeme";

void setup()
{
  // bool LCDEnable, bool SDEnable, bool SerialEnable
  // setting speaker to false does not fix
  M5.begin(true, true, true, true, kMBusModeOutput, false);
  M5.Lcd.setTextFont(2);
  WiFi.begin(ssid, password);
  M5.Lcd.printf("Connecting WiFi...\n");
  while ( !WiFi.isConnected() ) delay(100);
  
  delay(500);
  
  M5.Lcd.printf( "Streaming %s\n", url );
  
  file = new AudioFileSourceICYStream(url);
  // Create a buffer using that stream
  buff = new AudioFileSourceBuffer( file, 4096 );
  //out = new AudioOutputI2S(0, 2, -1); 
  out = new AudioOutputI2S(0,0); 
  out->SetPinout(12, 0, 2);
  out->SetOutputModeMono(true);
  //ineffective out->SetRate(44100);
  
  mp3 = new AudioGeneratorMP3();
  mp3->begin(buff, out);
}

void loop()
{
  if (mp3->isRunning()) {
    if (!mp3->loop()) mp3->stop();
  } else {
    Serial.printf("BARF\n");
    delay(20);
  }
}
