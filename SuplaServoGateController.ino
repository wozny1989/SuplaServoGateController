#include <SuplaDevice.h>
#include <supla/control/relay.h>
#include <supla/control/virtual_relay.h>
#include <ESP32Servo.h>
#include "secrets.h"

#define STORAGE_OFFSET 100
#include <supla/storage/eeprom.h>
Supla::Eeprom eeprom(STORAGE_OFFSET);

// Choose proper network interface for your card:
#ifdef ARDUINO_ARCH_AVR
// Arduino Mega with EthernetShield W5100:
#include <supla/network/ethernet_shield.h>
// Ethernet MAC address
uint8_t mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
Supla::EthernetShield ethernet(mac);
#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)
// ESP8266 and ESP32 based board:
#include <supla/network/esp_wifi.h>
Supla::ESPWifi wifi(WIFI_SSID, WIFI_PASS);
#endif

const int SERVO_PIN = 17;

Servo servo;

class ServoRelay : public Supla::Control::VirtualRelay
{
public:
  void turnOn(_supla_int_t duration = 0) override
  {
    // Serial.println(String("Servo turned ON! Duration: ") + duration);
    servo.write(135);
    delay(duration);
    servo.write(0);
    // Serial.println("DONE!");
  }

  void turnOff(_supla_int_t duration = 0) override
  {
    // Serial.println(String("Servo turned OFF! Duration: ") + duration);
  }
};

void setup()
{
  Serial.begin(115200);

  servo.attach(SERVO_PIN);
  auto servoRelay = new ServoRelay();

  SuplaDevice.begin(
      GUID,                  // Global Unique Identifier
      "svr107.supla.org",    // SUPLA server address
      "wozny1989@gmail.com", // Email address used to login to Supla Cloud
      AUTHKEY);              // Authorization key
}

void loop()
{
  SuplaDevice.iterate();
}
