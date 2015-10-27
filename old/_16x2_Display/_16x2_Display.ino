#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
unsigned long clock, time;
int hour, minute, secound;

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop() {
  UpdateClock();
  char buf[64];
  int cnt = sprintf(buf, 
                    "        %02d:%02d:%02d", 
                    hour, minute, secound);
  
  lcd.home();
  lcd.print(buf);
  
  while((millis() % 1000) > 200);
}

void UpdateClock() {
  clock     = millis();
  secound   = clock / 1000;
  minute    = secound / 60;
  hour      = minute / 60;
  secound  %= 60;
  minute   %= 60;
}

