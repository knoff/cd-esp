#pragma once
#include "../CD_Defs.h"
#include <WiFi.h>
#include <esp_now.h>
#include <queue>

// Максимальный размер очереди входящих пакетов
#define NETWORK_RX_QUEUE_SIZE 10

struct Packet {
    uint8_t mac[6];
    uint8_t data[HU_MAX_PAYLOAD_SIZE + sizeof(hu_frame_header_t)];  // ~240 bytes
    uint8_t len;
};

class Network {
public:
    static void begin(uint8_t logicalID);
    static void update();  // Вызывать в loop()

    // Отправка (пока Broadcast для простоты)
    static bool sendBroadcast(const uint8_t *data, size_t len);

    // Получение (достать из очереди)
    static bool readPacket(Packet &pkt);

    static uint8_t *getMacAddress();

private:
    static void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);
    static void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

    static QueueHandle_t _rxQueue;
    static uint8_t _myMac[6];
};
