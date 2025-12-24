#include "Network.h"
#include "../hal/SystemIO.h"

QueueHandle_t Network::_rxQueue;
uint8_t Network::_myMac[6];

void Network::begin(uint8_t logicalID) {
    // 1. Инициализация WiFi в режиме Station
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    // Получаем MAC
    WiFi.macAddress(_myMac);
    LOG_INF("Network Init. MAC: %02X:%02X:%02X:%02X:%02X:%02X", _myMac[0], _myMac[1], _myMac[2], _myMac[3], _myMac[4],
            _myMac[5]);

    // 2. Инициализация ESP-NOW
    if (esp_now_init() != ESP_OK) {
        LOG_ERR("Error initializing ESP-NOW");
        return;
    }

    // 3. Создаем очередь (FreeRTOS)
    _rxQueue = xQueueCreate(NETWORK_RX_QUEUE_SIZE, sizeof(Packet));

    // 4. Регистрируем коллбеки
    esp_now_register_recv_cb(OnDataRecv);
    esp_now_register_send_cb(OnDataSent);

    // 5. Добавляем Broadcast пира (чтобы можно было слать всем)
    esp_now_peer_info_t peerInfo = {};
    memset(peerInfo.peer_addr, 0xFF, 6);  // Broadcast addr
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        LOG_ERR("Failed to add broadcast peer");
    }
}

void Network::update() {
    // Тут можно добавить логику перепосылки, если нужно
}

bool Network::sendBroadcast(const uint8_t *data, size_t len) {
    uint8_t broadcastAddr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    esp_err_t result = esp_now_send(broadcastAddr, (uint8_t *)data, len);
    return result == ESP_OK;
}

bool Network::readPacket(Packet &pkt) {
    if (!_rxQueue) return false;
    return xQueueReceive(_rxQueue, &pkt, 0) == pdTRUE;
}

// ISR Context! Не использовать тяжелые операции
void Network::OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
    if (len > sizeof(Packet::data)) return;  // Overflow protection

    Packet pkt;
    memcpy(pkt.mac, mac, 6);
    memcpy(pkt.data, incomingData, len);
    pkt.len = len;

    // Пушим в очередь (из ISR)
    xQueueSendFromISR(_rxQueue, &pkt, NULL);
}

void Network::OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    // Можно добавить дебаг, но будет спамить
    // LOG_INF("Last Packet Send Status: %s", status == ESP_NOW_SEND_SUCCESS ? "OK" : "FAIL");
}

uint8_t *Network::getMacAddress() {
    return _myMac;
}
