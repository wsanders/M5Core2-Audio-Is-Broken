'm about ready to give up after three days of trial and error. Has anyone gotten MP3 streaming to work on Core2?

Yesterday, I was able to stream a low-bandwidth station but today even that doesn't work. I posted some examples of what I have tried, in https://github.com/wsanders/M5Core2-Audio-Is-Broken. The sketch WSWebRadio-broken.ino was working yesterday for a low-bandwith (32k) stream, but produced broken distorted audio from a 192k stream. Today none of the programs in my repo produce any audio.

I haven't seen any posts about working MP3 streaming sketches less than a couple years old, which seems to hint that it just doesn't work. Maybe everyone has switched to MicroPython?

I am using Arduino IDE, M5Core2 library 0.1.9, ESP8266Audio 1.9.7.
