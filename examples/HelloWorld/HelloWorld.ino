
#include <Arduino.h>
#include <Utf8.h>
#include <U8glib.h>
#include <U8GettextSimple.h>

void printHexString(const char *str) {
  while(*str) {
    Serial.print(" 0x");
    Serial.print((uint8_t)*str, HEX);
    ++ str;
  }
  Serial.println();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  u8gettextInitialize("en_US", 256);
}

void loop() {
  const char * oldLanguage = "";

  // put your main code here, to run repeatedly:
  delay(3000);
  Serial.println("[Test 1]");
  oldLanguage = u8gettextSetLanguage("en_US");
  Serial.print("Old language : ");
  Serial.println(oldLanguage);

  Serial.print("Set language to : ");
  Serial.println(u8gettextGetLanguage());

  Serial.println(_("Hello, world!"));

  delay(3000);
  Serial.println("[Test 2]");
  oldLanguage = u8gettextSetLanguage("zh_CN");
  Serial.print("Old language : ");
  Serial.println(oldLanguage);

  Serial.print("Set language to : ");
  Serial.println(u8gettextGetLanguage());

  Serial.println(_("Hello, world!"));

  delay(3000);
  Serial.println("[Test 3]");

  printHexString(U_("Hello, world!"));

  Serial.println("[Test END]");

  while(1) {
    delay(1000);
  }
}
