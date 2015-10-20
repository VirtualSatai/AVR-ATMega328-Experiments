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

void setup() {
  Serial.begin(9600);

  if (!rh.init())
    Serial.println("ERROR: RadioHead Init failed");

  pinMode(GREEN_PIN, OUTPUT);

}

bool GetPackage(const uint8_t * msg, uint32_t len, payload_type * res) {
  /*
   * 
   * if (len < MIN_PACKAGE_LEN)
    return false;

  if (len > MAX_PACKAGE_LEN)
    return false;
  
  res->sync = msg[0] << 7;
  res->sync |= (msg[1] & 0xFE) >> 1;

  res->okay = msg[1] & 0x1;

  res->addr = msg[2];

  res->mode = msg[3] & 0xE0 >> 5;

  res->cntd = msg[3] & 0x1F;

  uint8_t i;

  for (i = 0; i < len - 4; i++) {
    res->msga[i] = msg[4 + i];
  }

  res->msga[i + 1] = '\0';
  */

  memcpy(res, msg, len);
  return true;
}

void loop() {
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  if (rh.recv(buf, &buflen)) // Non-blocking
  {
    for(int i = 0; i < buflen; i++)
      Serial.println(buf[i], BIN);
    payload_type tmp;
    if (GetPackage(buf, buflen, &tmp)) {
      char printbuffer[256];
      makePayloadString(tmp, printbuffer);
      Serial.print(printbuffer);
    }

    Serial.println("_-------------------_");
  }

  t.update();
}
