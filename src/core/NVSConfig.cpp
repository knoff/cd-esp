#include "NVSConfig.h"

Preferences NVSConfig::prefs;

void NVSConfig::begin() {
  // Открываем namespace "cd-config".
  // false = read/write режим (нам нужно уметь писать ID)
  prefs.begin("cd-config", false);
  LOG_INF("NVS Initialized");
}

void NVSConfig::resetToFactory() {
  prefs.clear();
  LOG_WRN("NVS Cleared (Factory Reset)");
}

hu_device_type_t NVSConfig::getDeviceType() {
  // Читаем байт, приводим к enum.
  // Если ключа нет - возвращаем HU_TYPE_UNKNOWN (0)
  return (hu_device_type_t)prefs.getUChar("dev_type", HU_TYPE_UNKNOWN);
}

uint8_t NVSConfig::getLogicalID() {
  // Если ID не назначен, возвращаем UNASSIGNED (0xFE)
  return prefs.getUChar("log_id", HU_ADDR_UNASSIGNED);
}

uint8_t NVSConfig::getHardwareRevision() {
  // Версия железа, по умолчанию 1
  return prefs.getUChar("hw_rev", 1);
}

void NVSConfig::setDeviceType(hu_device_type_t type) {
  prefs.putUChar("dev_type", (uint8_t)type);
  LOG_INF("Device Type set to: 0x%02X", type);
}

void NVSConfig::setLogicalID(uint8_t id) {
  prefs.putUChar("log_id", id);
  LOG_INF("Logical ID set to: 0x%02X", id);
}
