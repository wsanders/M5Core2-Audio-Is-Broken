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

AudioGeneratorMP3 *mp3;
AudioFileSourceICYStream *file;
AudioFileSourceBuffer *buff;
AudioOutputI2S *out;

const char * url = "http://vprbbc.streamguys1.com/vprbbc24.mp3";
//const char * url = "http://stream1.early1900s.org:8080";
//const char * url = "http://streams.kut.org/4430_192.mp3?aw_0_1st.playerid=tmx-free";

#include "WSWebRadio.h"
// const char* ssid = "changeme";
// const char* password = "changeme";

const int preallocateBufferSize = 16*1024;
const int preallocateCodecSize = 85332; // AAC+SBR codec max mem needed
void *preallocateBuffer = NULL;
void *preallocateCodec = NULL;

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
  buff = new AudioFileSourceBuffer(file, 8192);
  //buff = new AudioFileSourceBuffer(file, preallocateBuffer, preallocateBufferSize);
  buff->RegisterStatusCB(StatusCallback, (void*)"buffer");
  out = new AudioOutputI2S(0,0); 
  out->SetPinout(12, 0, 2);
  out->SetOutputModeMono(true);
  out->SetGain(0.1);
  
  mp3 = new AudioGeneratorMP3();
  //mp3 = new AudioGeneratorMP3(preallocateCodec, preallocateCodecSize);
  mp3->RegisterStatusCB(StatusCallback, (void*)"mp3");
  Serial.printf("BEGIN\n");
  mp3->begin(buff, out);
}

void loop()
{
  if (mp3->isRunning()) {
    if (!mp3->loop()) mp3->stop();
  } else {
    Serial.printf("BARF\n");
    delay(20000);
  }
}


