#include "SystemIO.h"

SerialTextCb SystemIO::_textCb = nullptr;
SerialFrameCb SystemIO::_frameCb = nullptr;
String SystemIO::_lineBuffer = "";
uint8_t SystemIO::_frameBuffer[HU_MAX_PAYLOAD_SIZE + 16];
size_t SystemIO::_frameIdx = 0;
bool SystemIO::_isReadingFrame = false;
uint8_t SystemIO::_expectedLen = 0;
bool SystemIO::_outputProduced = false;

void SystemIO::begin(uint32_t baud) {
    Serial.begin(baud);
    _lineBuffer.reserve(64);
}

void SystemIO::update() {
    while (Serial.available()) {
        handleRxByte(Serial.read());
    }
}

// === SMART LOGGING ===
void SystemIO::log(const char* level, const char* format, ...) {
    // Взводим флаг
    _outputProduced = true;

    // 1. VT100 Magick: \r (курсор в начало), \033[K (стереть все до конца строки)
    Serial.print("\r\033[K");

    // 2. Печатаем лог
    Serial.print("[");
    Serial.print(level);
    Serial.print("] ");

    char locBuf[128];
    va_list args;
    va_start(args, format);
    vsnprintf(locBuf, sizeof(locBuf), format, args);
    va_end(args);
    Serial.println(locBuf);

    // 3. Восстанавливаем приглашение и то, что юзер уже успел набрать
    Serial.print("> ");
    Serial.print(_lineBuffer);
}

// Чистый вывод с поддержкой форматирования и сохранением строки ввода
void SystemIO::println(const char* format, ...) {
    // Взводим флаг
    _outputProduced = true;

    // 1. Убираем текущий ввод пользователя с экрана
    // То же самое, только без тега [LEVEL]
    Serial.print("\r\033[K");

    // 2. Форматируем сообщение
    char locBuf[128];
    va_list args;
    va_start(args, format);
    vsnprintf(locBuf, sizeof(locBuf), format, args);
    va_end(args);

    // 3. Печатаем чистое сообщение
    Serial.println(locBuf);

    // 4. Восстанавливаем приглашение и буфер ввода
    Serial.print("> ");
    Serial.print(_lineBuffer);
}

void SystemIO::sendBinary(const uint8_t* data, size_t len) {
    Serial.write(data, len);
}

void SystemIO::showPrompt() {
    Serial.print("\n> ");
}

void SystemIO::resetOutputFlag() {
    _outputProduced = false;
}

bool SystemIO::wasOutputProduced() {
    return _outputProduced;
}

void SystemIO::onTextCommand(SerialTextCb cb) {
    _textCb = cb;
}
void SystemIO::onBinaryFrame(SerialFrameCb cb) {
    _frameCb = cb;
}

// === DISPATCHER ===
void SystemIO::handleRxByte(uint8_t b) {
    // 1. Логика распознавания бинарного пакета (Magic = 0xA5)
    if (!_isReadingFrame && b == HU_PROTOCOL_MAGIC && _frameCb) {
        _isReadingFrame = true;
        _frameIdx = 0;
        _frameBuffer[_frameIdx++] = b;
        return;
    }

    // 2. Если читаем фрейм
    if (_isReadingFrame) {
        _frameBuffer[_frameIdx++] = b;

        // Пытаемся понять длину (она в 9-м байте заголовка, индекс 8)
        // Header size = 9 bytes. Payload len is at offset 8.
        if (_frameIdx == 9) {
            // Структура header: magic(0), flags(1), src(2), dst(3), via(4), msg(5), seq(6,7), len(8)
            _expectedLen = _frameBuffer[8] + 9;  // Header + Payload
        }

        if (_frameIdx > 9 && _frameIdx == _expectedLen) {
            // Пакет собран
            if (_frameCb) _frameCb(_frameBuffer, _expectedLen);
            _isReadingFrame = false;
            _frameIdx = 0;
        }

        // Защита от переполнения
        if (_frameIdx >= sizeof(_frameBuffer)) {
            _isReadingFrame = false;  // Сброс (мусор)
        }
        return;
    }

    // 3. Если это не фрейм — значит текст для консоли
    if (b == '\n' || b == '\r') {
        if (_lineBuffer.length() > 0) {
            // ФИКС: Сначала сохраняем копию и чистим буфер
            String cmdToRun = _lineBuffer;
            _lineBuffer = "";

            // Потом запускаем выполнение.
            // Теперь любые логи внутри _textCb восстановят пустой промпт "> "
            if (_textCb) {
                _textCb(cmdToRun);
            }
        } else {
            // Если нажали Enter на пустой строке - просто переносим
            // Но чтобы восстановить промпт, можно просто ничего не делать
            // или перепечатать его. Обычно просто игнорируем или шлем новый промпт.
            // Для красоты можно сделать так:
            _lineBuffer = "";
            showPrompt();
        }
    } else if (b == 0x08 || b == 0x7F) {  // Backspace
        if (_lineBuffer.length() > 0) {
            _lineBuffer.remove(_lineBuffer.length() - 1);
            // Визуальное удаление (работает в PuTTY, в Monitor может глючить, но логику буфера правит)
            Serial.print("\b \b");
        }
    } else {
        _lineBuffer += (char)b;
    }
}
