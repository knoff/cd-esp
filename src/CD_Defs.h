#pragma once
#include <Arduino.h>
#include <headunit_protocol.h>  // Из lib/cd-protocol

// Глобальные пины (по умолчанию для DevKit V1)
#define PIN_LED_BUILTIN 2

// Forward declaration, чтобы не инклудить хедер сюда
class SystemIO;

// Логирование (обертка для удобства)
#define LOG_INF(fmt, ...) SystemIO::log("INF", fmt, ##__VA_ARGS__)
#define LOG_ERR(fmt, ...) SystemIO::log("ERR", fmt, ##__VA_ARGS__)
#define LOG_WRN(fmt, ...) SystemIO::log("WRN", fmt, ##__VA_ARGS__)
