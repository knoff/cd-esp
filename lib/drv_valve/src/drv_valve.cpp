#include <Arduino.h>
#include <drv_valve/drv_valve.hpp>
namespace {
constexpr int VALVE_PIN = 5; // пример, поправишь под плату
}
namespace cd {
namespace drv {
void valve_open() { digitalWrite(VALVE_PIN, HIGH); }
void valve_close() { digitalWrite(VALVE_PIN, LOW); }
} // namespace drv
} // namespace cd
