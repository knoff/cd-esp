# cd-esp

Универсальная прошивка Coffee Digital для ESP32.

## Структура
- `components/cd_core` — ядро, планировщик, health, watchdog
- `components/cd_rt` — жёсткие RT-модули (ISR/таймер)
- `components/cd_net` — Wi-Fi + MQTT (A/B OTA, mTLS)
- `components/cd_cmd` — шина команд (JSON ns)
- `components/cd_cfg` — журналируемый KV (версионирование)
- `components/cd_hal` — ресурс-менеджер GPIO/I2C/SPI/PWM
- `components/drv_*` — драйверы (клапан, помпа, термистор…)
- `components/rpk_*` — пользовательские пакеты (RPK)
- `apps/universal_firmware` — точка входа

## Политики
- ESP-IDF 5.x, C++17
- Secure Boot v2 + Flash Encryption
- A/B OTA + rollback
- Семантические версии core/drivers/rpks
- Тесты: unit (Unity) + HIL (pytest)
