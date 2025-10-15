#include <Arduino.h>
#include <cd_core/cd_core.hpp>
namespace cd {
namespace core {
void init() { /* логгер/шина/health заглушки */ }
void tick_bg() { /* обработка команд/телеметрии */ }
void tick_rt() { /* быстрый контур 1-5мс */ }
} // namespace core
} // namespace cd
