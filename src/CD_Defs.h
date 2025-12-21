#pragma once
#include <Arduino.h>
#include <headunit_protocol.h> // Из lib/cd-protocol

// Глобальные пины (по умолчанию для DevKit V1)
#define PIN_LED_BUILTIN 2

// Логирование (обертка для удобства)
#define LOG_INF(fmt, ...) Serial.printf("[INF] " fmt "\n", ##__VA_ARGS__)
#define LOG_ERR(fmt, ...) Serial.printf("[ERR] " fmt "\n", ##__VA_ARGS__)
#define LOG_WRN(fmt, ...) Serial.printf("[WRN] " fmt "\n", ##__VA_ARGS__)
