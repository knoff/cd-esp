#include "CD_Defs.h"
#include "core/NVSConfig.h"
#include "managers/BaseManager.h"
#include "managers/FactoryMgr.h"

// Глобальный указатель на текущего менеджера
BaseManager *currentManager = nullptr;

void setup() {
  Serial.begin(115200);
  delay(1000); // Даем время на открытие монитора
  LOG_INF("=== Coffee Digital Universal Firmware v0.1 ===");

  // 1. Инициализация ядра
  NVSConfig::begin();

  // (Временный хак для тестов: раскомментируй, чтобы сбросить конфиг)
  // NVSConfig::resetToFactory();

  // 2. Определение личности
  hu_device_type_t type = NVSConfig::getDeviceType();
  uint8_t myID = NVSConfig::getLogicalID();

  LOG_INF("Detected Type: 0x%02X, ID: 0x%02X", type, myID);

  // 3. Фабрика менеджеров
  switch (type) {
  case HU_TYPE_COORDINATOR:
    LOG_INF("Starting as COORDINATOR");
    // currentManager = new GatewayMgr(); // Сделаем на след. шаге
    break;

  case HU_TYPE_PUMP_CTRL:
  case HU_TYPE_BOILER_PID:
    LOG_INF("Starting as ACTUATOR");
    // currentManager = new ActuatorMgr(); // Сделаем на след. шаге
    break;

  case HU_TYPE_UNKNOWN:
  default:
    LOG_WRN("Unknown device type. Falling back to Factory Mode.");
    currentManager = new FactoryMgr();
    break;
  }

  // Защита от null (если тип известен, но класс еще не написан)
  if (!currentManager) {
    LOG_ERR("Manager for this type not implemented yet!");
    currentManager = new FactoryMgr();
  }

  // 4. Запуск менеджера
  currentManager->begin();
}

void loop() {
  if (currentManager) {
    currentManager->update();
  }

  // В будущем здесь будет network.update()
}
