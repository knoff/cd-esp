#pragma once
#include "../CD_Defs.h"

class BaseManager {
public:
  virtual ~BaseManager() {}

  // Инициализация (аналог setup)
  virtual void begin() { LOG_INF("BaseManager started"); }

  // Главный цикл (аналог loop)
  virtual void update() = 0; // Чисто виртуальный метод

  // Обработка входящих пакетов (пока заглушка)
  virtual void handlePacket(const uint8_t *data, size_t len) {
    // По умолчанию игнорируем
  }
};
