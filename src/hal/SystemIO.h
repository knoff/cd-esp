#pragma once
#include "../CD_Defs.h"
#include <functional>

// Типы коллбеков
using SerialTextCb = std::function<void(String)>;
using SerialFrameCb = std::function<void(const uint8_t*, size_t)>;

class SystemIO {
public:
    static void begin(uint32_t baud);
    static void update();

    // === OUTPUT ===
    // 1. Системный лог (добавляет [TAG] и Timestamp)
    static void log(const char* level, const char* format, ...);

    // 2. Чистый вывод (для меню и таблиц), поддерживает форматирование
    static void println(const char* format, ...);

    // 3. Бинарные данные
    static void sendBinary(const uint8_t* data, size_t len);

    // Управление приглашением
    static void showPrompt();

    // === TRACKING API ===
    // Сбросить флаг перед командой
    static void resetOutputFlag();
    // Узнать, был ли вывод
    static bool wasOutputProduced();

    // === INPUT ROUTING ===
    // Кто слушает текстовые команды (Console)
    static void onTextCommand(SerialTextCb cb);
    // Кто слушает бинарные пакеты (Gateway)
    static void onBinaryFrame(SerialFrameCb cb);

private:
    static void handleRxByte(uint8_t b);

    static SerialTextCb _textCb;
    static SerialFrameCb _frameCb;

    // Буферы
    static String _lineBuffer;
    static uint8_t _frameBuffer[HU_MAX_PAYLOAD_SIZE + 16];
    static size_t _frameIdx;
    static bool _isReadingFrame;
    static uint8_t _expectedLen;

    // Флаг отслеживания вывода
    static bool _outputProduced;
};
