// Radiohead
#include <RH_ASK.h>
#include <SPI.h>

// Timer
#include <Event.h>
#include <Timer.h>

// Must be designed for Addr.cpp/h to compile
#include <EEPROM.h>
#include "Addr.h"

// Sass' payload format ?
#include "sass_ask.h"

// General headers
#include <inttypes.h>

// if debug is defined then debug info is printed.
#define DEBUG
#define TIME_SLOT_LEN_MS 1000
#define MAX_WAIT_FOR_NETWORK_TIME_MS 10000

#define GREEN_PIN 2
#define RED_PIN 3
#define BUTTON_PIN 4
#define TX_PIN 12
#define RX_PIN 11

// Global variables
uint8_t addr = 0;
Timer t;
RH_ASK rh;

uint8_t RED_PIN_ID = 0;
uint8_t GREEN_PIN_ID = 0;

int listenForNetwork() {
#ifdef DEBUG
  Serial.print("Started listening for network. Max time: ");
  Serial.print(MAX_WAIT_FOR_NETWORK_TIME_MS);
  Serial.println(" [ms]");
#endif

  long startTime = millis();
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  int recivedPackages = 0;
  GREEN_PIN_ID = t.oscillate(GREEN_PIN, 100, LOW, MAX_WAIT_FOR_NETWORK_TIME_MS / 100);

  while (millis() - startTime < MAX_WAIT_FOR_NETWORK_TIME_MS) {
    // Call recv as often as posible
    if (rh.recv(buf, &buflen))
    {
      recivedPackages++;
      ProcessPackage(buf, buflen);
    }

    // Update Timer
    t.update();
  }

  t.stop(GREEN_PIN_ID);

  return recivedPackages;
}

void ProcessPackage(const uint8_t* buf, uint8_t len) {
  if(len < 4) {
    return;
  }
  payload_type * tmp = (payload_type *) malloc(sizeof(payload_type) + 1);
  memcpy(&tmp, &buf, len);
  tmp->msga[len - 4] = '\0';
  // msg isn't null terminated? 
  
#ifdef DEBUG
  char stringbuffer[64];
  makePayloadString(*tmp, stringbuffer);
  Serial.println(stringbuffer);
  //rh.printBuffer("Got: ", buf, len);
#endif
}

void setup() {
  Serial.begin(9600);

  if (!rh.init())
    Serial.println("ERROR: RadioHead Init failed");

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  RED_PIN_ID = t.oscillate(RED_PIN, 1000, LOW);


  Addr a;
  addr = a.get();
#ifdef DEBUG
  if (addr != 0x0) {
    Serial.print("Address of device is: 0x");
    Serial.println(addr, HEX);
  } else {
    Serial.print("Failed to get address.");
  }
#endif

  t.stop(RED_PIN_ID);
  t.stop(GREEN_PIN_ID);
  if (listenForNetwork() > 0) {
    // Network found!
#ifdef DEBUG
    Serial.println("Network found, waiting for input on button before attempting to join");
#endif
    GREEN_PIN_ID = t.oscillate(GREEN_PIN, 250, HIGH);
    while (digitalRead(BUTTON_PIN) == HIGH) {
      // TODO LISTEN TO NETWORK WHILE WAITING ?
      t.update();
    }
  } else {
    // No network found. :(
#ifdef DEBUG
    Serial.println("No network found. Waiting for input on button before attempting to start one.");
#endif
    RED_PIN_ID = t.oscillate(RED_PIN, 250, HIGH);
    while (digitalRead(BUTTON_PIN) == HIGH) {
      t.update();
    }
    
  }

  Serial.println("----- END OF SETUP -----");
}

void loop() {

  // Update "Timer" dependant sutff.
  t.update();
}
