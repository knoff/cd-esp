#include "FactoryMgr.h"

void FactoryMgr::begin() {
  pinMode(PIN_LED_BUILTIN, OUTPUT);
  LOG_INF("Entered FACTORY MODE. Waiting for configuration...");
}

void FactoryMgr::update() {
  // Просто мигаем SOS или медленно, показывая, что нет конфига
  if (millis() - _lastBlink > 500) {
    _lastBlink = millis();
    _ledState = !_ledState;
    digitalWrite(PIN_LED_BUILTIN, _ledState);

    // В будущем тут будет проверка Serial на команду "SET_TYPE"
  }
}
