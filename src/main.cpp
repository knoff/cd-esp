#include <Arduino.h>
#include <cd_core/cd_core.hpp>
#include <cd_hal/cd_hal.hpp>
#include <rpk_pump/rpk_pump.hpp>

TaskHandle_t rtTaskHandle;

void rtTask(void *) {
  TickType_t last = xTaskGetTickCount();
  const TickType_t period = pdMS_TO_TICKS(5); // 5мс контур
  for (;;) {
    cd::core::tick_rt();
    vTaskDelayUntil(&last, period);
  }
}

void setup() {
  cd::hal::init_pins();
  cd::core::init();
  cd::rpk::pump::init();

  xTaskCreatePinnedToCore(rtTask, "rt", 4096, nullptr, configMAX_PRIORITIES - 1,
                          &rtTaskHandle, APP_CPU_NUM);
}

void loop() {
  cd::core::tick_bg();
  delay(10);
}
