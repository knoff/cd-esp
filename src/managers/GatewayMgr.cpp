#include "GatewayMgr.h"
#include "../hal/SystemIO.h"
#include "../console/Console.h"

void GatewayMgr::begin() {
    LOG_INF("Gateway Manager Started. Bridging Serial <-> ESP-NOW");
    // Инициализируем сеть с ID = COORDINATOR (0x01)
    Network::begin(HU_ADDR_COORDINATOR);

    // 1. Регистрация бинарного канала (Serial -> ESP-NOW)
    SystemIO::onBinaryFrame([this](const uint8_t *data, size_t len) {
        // Пришел пакет от RPi -> Отправляем в ESP-NOW
        // data уже содержит весь фрейм с заголовком
        Network::sendBroadcast(data, len);  // Тут надо бы слать конкретному, но пока Broadcast
    });
    // 2. Регистрация CLI команд (Text -> Logic)
    // Теперь 'ping' - это официальная команда системы, пока жив этот менеджер
    Console::registerCommand(
        "ping",
        [](String args) {
            const char *msg = "PING_PAYLOAD";
            // Для теста шлем raw байты. В реальности тут будет hu_frame_t
            Network::sendBroadcast((uint8_t *)msg, strlen(msg));
            LOG_INF("Ping sent to Broadcast!");
        },
        "Send broadcast ping frame");
}

void GatewayMgr::update() {
    Network::update();      // Обслуживание сети
    processNetworkInput();  // ESP-NOW -> Serial
}

void GatewayMgr::processNetworkInput() {
    Packet pkt;
    while (Network::readPacket(pkt)) {
        // Пришел пакет из Эфира -> Шлем в Serial (RPi)
        // Используем SystemIO для атомарной отправки
        SystemIO::sendBinary(pkt.data, pkt.len);
    }
}

void GatewayMgr::processSerialInput() {
    // В будущем тут будет чтение бинарного MeshFrame от Python скрипта.
    // Пока сделаем простую отправку для теста: если ввели "ping", шлем пакет.

    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();

        if (cmd == "ping") {
            const char *msg = "HELLO_FROM_GATEWAY";
            Network::sendBroadcast((uint8_t *)msg, strlen(msg));
            LOG_INF("Broadcast sent: %s", msg);
        } else if (cmd.startsWith("set_type")) {
            // Оставляем бэкдор, чтобы можно было "разжаловать" координатора обратно
            // (Копипаст логики FactoryMgr)
            int type = cmd.substring(9).toInt();
            // ... NVSConfig::setDeviceType ... (для краткости пропущено, но лучше вынести в утилиту)
        }
    }
}
