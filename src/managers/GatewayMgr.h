#pragma once
#include "BaseManager.h"
#include "../core/Network.h"

class GatewayMgr : public BaseManager {
public:
    void begin() override;
    void update() override;

private:
    void processSerialInput();
    void processNetworkInput();
};
