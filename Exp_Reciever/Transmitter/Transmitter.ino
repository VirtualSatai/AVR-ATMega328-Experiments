// Radiohead
#include <RH_ASK.h>
#include <SPI.h>

// Timer
#include <Event.h>
#include <Timer.h>

// Sass' payload format ?
#include "sass_ask.h"

#define TX_PIN 12
#define GREEN_PIN 2
#define MIN_PACKAGE_LEN 4
#define MAX_PACKAGE_LEN 16

Timer t;
RH_ASK rh;

payload_type plt;

void setup() {
  Serial.begin(9600);

  if (!rh.init())
    Serial.println("ERROR: RadioHead Init failed");

  pinMode(GREEN_PIN, OUTPUT);

  plt.sync = 0x33;
  plt.okay = 0x1;
  plt.addr = 0xDE;
  plt.mode = 2;
  plt.cntd = 4;
  //plt.msga = (uint8_t*) malloc(12);
  plt.msga = (uint8_t*) "abc";
}

void loop()
{
    Serial.println(sizeof(plt));

    uint8_t buf[16];

    
    memcpy(&buf, &plt, 16);
    
    rh.send((uint8_t *)buf, sizeof(buf));
    rh.waitPacketSent();
    delay(1000);
}
