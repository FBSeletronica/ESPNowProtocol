#include "ESPNowProtocol.h"

static ESPNowProtocol *instance = nullptr;

void ESPNowProtocol::begin()
{
    instance = this;

    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK)
    {
        Serial.println("[ENP] ESP-NOW init failed");
        return;
    }

    esp_now_register_recv_cb(ESPNowProtocol::onReceive);

    Serial.println("[ENP] started");
}

void ESPNowProtocol::loop()
{
    if (waitingAck)
    {
        if (millis() - ackStartTime > ACK_TIMEOUT)
        {
            if (retryCount < MAX_RETRIES)
            {
                retryCount++;

                esp_now_send(peerAddress, (uint8_t *)&lastMsg, sizeof(lastMsg));

                ackStartTime = millis();
            }
            else
            {
                waitingAck = false;
            }
        }
    }
}

void ESPNowProtocol::setPeer(const uint8_t *mac)
{
    memcpy(peerAddress, mac, 6);

    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, mac, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (!esp_now_is_peer_exist(mac))
    {
        if (esp_now_add_peer(&peerInfo) != ESP_OK)
        {
            Serial.println("[ENP] peer add failed");
            return;
        }
    }
}

void ESPNowProtocol::sendCommand(uint8_t cmd, int32_t value)
{
    enp_message_t msg;

    msg.type = ENP_MSG_CMD;
    msg.command = cmd;
    msg.value = value;
    msg.seq = seqCounter++;

    esp_now_send(peerAddress, (uint8_t *)&msg, sizeof(msg));
}

void ESPNowProtocol::sendReliable(uint8_t cmd, int32_t value)
{
    if (waitingAck)
        return;

    lastMsg.type = ENP_MSG_CMD;
    lastMsg.command = cmd;
    lastMsg.value = value;
    lastMsg.seq = seqCounter++;

    esp_now_send(peerAddress, (uint8_t *)&lastMsg, sizeof(lastMsg));

    pendingSeq = lastMsg.seq;
    waitingAck = true;
    retryCount = 0;
    ackStartTime = millis();
}

void ESPNowProtocol::sendStatus(int32_t value)
{
    enp_message_t msg;

    msg.type = ENP_MSG_STATUS;
    msg.command = 0;
    msg.value = value;
    msg.seq = 0;

    esp_now_send(peerAddress, (uint8_t *)&msg, sizeof(msg));
}

void ESPNowProtocol::onCommand(enp_command_cb_t cb)
{
    commandCallback = cb;
}

void ESPNowProtocol::onStatus(enp_status_cb_t cb)
{
    statusCallback = cb;
}


void ESPNowProtocol::onReceive(const esp_now_recv_info_t *info, const uint8_t *data, int len)
{
    if (len < sizeof(enp_message_t))
        return;

    if (instance)
    {
        memcpy(instance->peerAddress, info->src_addr, 6);

        if (!esp_now_is_peer_exist(info->src_addr))
        {
            esp_now_peer_info_t peerInfo = {};
            memcpy(peerInfo.peer_addr, info->src_addr, 6);
            peerInfo.channel = 0;
            peerInfo.encrypt = false;

            esp_now_add_peer(&peerInfo);
        }
    }

    enp_message_t msg;
    memcpy(&msg, data, sizeof(msg));

    if (msg.type == ENP_MSG_CMD)
    {
        enp_message_t ack;

        ack.type = ENP_MSG_ACK;
        ack.command = 0;
        ack.value = 0;
        ack.seq = msg.seq;

        esp_now_send(info->src_addr, (uint8_t *)&ack, sizeof(ack));
    }

    if (msg.type == ENP_MSG_ACK)
    {
        if (instance && instance->waitingAck)
        {
            if (msg.seq == instance->pendingSeq)
            {
                instance->waitingAck = false;
            }
        }
    }

    if (msg.type == ENP_MSG_STATUS)
    {
        if (instance && instance->statusCallback)
        {
            instance->statusCallback(msg.value);
        }
    }

    if (msg.type == ENP_MSG_CMD)
    {
        if (instance && instance->commandCallback)
        {
            instance->commandCallback(msg.command, msg.value);
        }
    }
}
