#ifndef NETWORK_MODULE_H
#define NETWORK_MODULE_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "../../include/app_types.h"
#include "../../include/config.h"
#include "../../include/secrets.h"

// Biến toàn cục
extern WiFiClient wifiClient;
extern PubSubClient mqttClient;

// Hàm công khai (chỉ những gì main.cpp cần dùng)
void networkInit();
void networkMaintain();

// Publish
bool networkPublishSensorData(const SensorData& data);
bool networkPublishAlert(AlertLevel level, const char* message);
bool networkPublishStatus(const SystemStatus& status);

// Getter
bool networkIsConnected();
void networkDisconnect();

#endif