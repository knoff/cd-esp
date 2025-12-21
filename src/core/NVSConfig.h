#pragma once
#include "../CD_Defs.h"
#include <Preferences.h>

class NVSConfig {
public:
  static void begin();
  static void resetToFactory();

  // Геттеры
  static hu_device_type_t getDeviceType();
  static uint8_t getLogicalID();
  static uint8_t getHardwareRevision();

  // Сеттеры (вызываются при Provisioning)
  static void setDeviceType(hu_device_type_t type);
  static void setLogicalID(uint8_t id);

private:
  static Preferences prefs;
};
