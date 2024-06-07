#include <M5Core2.h>
#include <driver/i2s.h>
#include <WiFi.h>
#include <AudioFileSourceICYStream.h>
#include <AudioFileSource.h>
#include <AudioFileSourceBuffer.h>
#include <AudioFileSourceSPIRAMBuffer.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>
#include <spiram-fast.h>

AudioGeneratorMP3 *gen;
AudioFileSourceICYStream *file;
AudioFileSourceBuffer *srcbuf;
AudioOutputI2S *out;

//const char * url = "http://vprbbc.streamguys1.com/vprbbc24.mp3";
//const char * url = "http://stream1.early1900s.org:8080";
const char * url = "http://streams.kut.org/4430_192.mp3?aw_0_1st.playerid=tmx-free";

// uncomment and enter your params here or use a .h file:
#include "WSWebRadio.h"
// const char* ssid = "changeme";
// const char* password = "changeme";

const int preallocateBufferSize = 128*1024;
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
  M5.begin(true, false, true, true, kMBusModeOutput, false);
  M5.Lcd.setTextFont(2);
  WiFi.begin(ssid, password);
  M5.Lcd.printf("Connecting WiFi...\n");
  while ( !WiFi.isConnected() ) delay(100);
  delay(1000);
  M5.Lcd.printf( "Streaming %s\n", url );
 
  preallocateBuffer = malloc(preallocateBufferSize);
  preallocateCodec = malloc(preallocateCodecSize);  
  file = new AudioFileSourceICYStream(url);
  //srcbuf = new AudioFileSourceBuffer(file, 128*1024);
  srcbuf = new AudioFileSourceBuffer(file, preallocateBuffer, preallocateBufferSize);
  srcbuf->RegisterStatusCB(StatusCallback, (void*)"srcbuf");
  out = new AudioOutputI2S(0,0); 
  out->SetPinout(12, 0, 2);
  out->SetOutputModeMono(true);
  out->SetGain(0.3);
  
  gen = new AudioGeneratorMP3();
  //gen = new AudioGeneratorMP3(preallocateCodec, preallocateCodecSize);
  gen->RegisterStatusCB(StatusCallback, (void*)"generator");
  Serial.printf("BEGIN\n");
  gen->begin(srcbuf, out);
}

void loop()
{
    if (!gen->loop()) {
        gen->stop();
        Serial.printf("BARF\n");
    }
    // won't play if this is here sleep(1;
}


