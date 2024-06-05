/* includes from:
https://github.com/bwbguard/M5Stack-Core2-MediaPlayer/blob/main/Core2_Media_Player.ino
*/
#include <M5Core2.h>
#include <driver/i2s.h>   // required
#include <WiFi.h>
#include <AudioFileSourceICYStream.h>
//#include <AudioFileSource.h>
#include <AudioFileSourceBuffer.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>

AudioGeneratorMP3 *gen;
AudioFileSourceICYStream *file;
AudioFileSourceBuffer *srcbuf;
AudioOutputI2S *out;

//const char * url = "http://vprbbc.streamguys1.com/vprbbc24.mp3";
const char * url = "http://stream1.early1900s.org:8080";
//const char * url = "http://streams.kut.org/4430_192.mp3?aw_0_1st.playerid=tmx-free";

// uncomment and enter your params here or use a .5 file with these stmts:
#include "WSWebRadio.h"
// const char* ssid = "changeme";
// const char* password = "changeme";

/* 
const int preallocateBufferSize = 16*1024;
const int preallocateCodecSize = 85332; // AAC+SBR codec max mem needed
void *preallocateBuffer = NULL;
void *preallocateCodec = NULL;
*/

void StatusCallback(void *cbData, int code, const char *string) {
  const char *ptr = reinterpret_cast<const char *>(cbData);
  // Note that the string may be in PROGMEM, so copy it to RAM for printf
  char s1[64];
  strncpy_P(s1, string, sizeof(s1));
  s1[sizeof(s1) - 1] = 0;
  Serial.printf("STATUS(%s) '%d' = '%s'\n", ptr, code, s1);
  Serial.flush();
}


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
  
  //preallocateBuffer = malloc(preallocateBufferSize);
  //preallocateCodec = malloc(preallocateCodecSize);  
  file = new AudioFileSourceICYStream(url);
  file->RegisterStatusCB(StatusCallback, (void*)"ICY");
  srcbuf = new AudioFileSourceBuffer(file, 8192);
  //srcbuf = new AudioFileSourceBuffer(file, preallocateBuffer, preallocateBufferSize);
  srcbuf->RegisterStatusCB(StatusCallback, (void*)"srcbuf");
  out = new AudioOutputI2S(0,2); 
  out->SetPinout(12, 0, 2);
  out->SetOutputModeMono(true);
  
  gen = new AudioGeneratorMP3();
  //gen = new AudioGeneratorMP3(preallocateCodec, preallocateCodecSize);
  gen->RegisterStatusCB(StatusCallback, (void*)"generator");
  Serial.printf("BEGIN\n");
  gen->begin(srcbuf, out);
}

void loop()
{
  if (gen->isRunning()) {
    if (!gen->loop()) gen->stop();
  } else {
    Serial.printf("BARF\n");
    delay(20000);
  }
}


