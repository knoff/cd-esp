#pragma once
#include "BaseManager.h"

class FactoryMgr : public BaseManager {
public:
  void begin() override;
  void update() override;

private:
  uint32_t _lastBlink = 0;
  bool _ledState = false;
};
