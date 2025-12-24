#include "FactoryMgr.h"
#include "../hal/SystemIO.h"
#include "../core/NVSConfig.h"

void FactoryMgr::begin() {
    pinMode(PIN_LED_BUILTIN, OUTPUT);
    LOG_INF("Entered FACTORY MODE. Waiting for configuration...");
}

void FactoryMgr::update() {
    // 1. Мигалка (оставляем)
    if (millis() - _lastBlink > 500) {
        _lastBlink = millis();
        _ledState = !_ledState;
        digitalWrite(PIN_LED_BUILTIN, _ledState);
    }
}
