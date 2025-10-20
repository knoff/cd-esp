<!-- markdownlint-configure-file {"MD041": false} -->

## [2025-10-15]

**Начало:**    2025-10-15 14:00 GMT+3
**Окончание:** 2025-10-20 15:30 GMT+3

### Выполнено

- Инициализирован проект под PlatformIO (Arduino Framework).
- Принята структура кода как набор библиотек в `lib/` (`cd_core`, `cd_hal`, `drv_*`, `rpk_*`) с `library.json`.
- Настроен pre-commit (clang-format, end-of-file-fixer, trailing-whitespace, check-json, check-yaml) и исправлены конфигурации с экранированием regex.
- Сформированы `.repo_instructions/` и база проектной памяти: `WORKFLOW.md` (V3), нормативный `repo_notes.md`, `CD_AI_NOTES.md`.
- Подготовлены шаблоны и заготовки документации; добавлен `COMMITS.md`.
- Согласованы принципы телеметрии: узлы → MQTT только в Headunit; Headunit → агрегированная телеметрия → SaaS по HTTPS/mTLS.

### Коммиты

- `chore(bootstrap): init PlatformIO (Arduino) + libs skeleton + formatting`
- `docs: Определение структуры документации`
- `docs(repo_instructions): repo instructions for AI`
- `docs(repo_instructions): AI workflow and repo_notes`

### Незавершённые задачи

- Минимальная реализация `cd_core` (инициализация, конфиг, логирование, messaging bus).
- Минимальная реализация `cd_hal` (GPIO/PWM/I2C/SPI интерфейсы).
- Базовый `rpk_pump` с smoke-тестом.
- GitHub Actions: `lint` + `build` для базовой платы.
- Согласование и фиксация схем обмена в `cd-schemas` (манифесты RPK, кадры телеметрии).
- Финализация `partitions_ota.csv` под A/B и проверка совместимости.

### Следующие шаги

- Реализовать каркас `cd_core` и `cd_hal`, собрать на целевой плате.
- Добавить `rpk_pump` и тестовый сценарий работы.
- Включить CI и артефакты сборки.
- Обновить `repo_notes.md` итоговыми решениями по API между модулями.
