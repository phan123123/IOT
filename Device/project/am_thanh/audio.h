#include "VS1053.h";

#define VS1053_CS     D1
#define VS1053_DCS    D0
#define VS1053_DREQ   D3


int VOLUME = 100;
VS1053 player(VS1053_CS, VS1053_DCS, VS1053_DREQ);
void setup_audio()
{
  SPI.begin();
  player.begin();
  player.switchToMp3Mode(); // optional, some boards require this
  player.setVolume(VOLUME);
  Serial.println("Hello VS1053!\n");
}
