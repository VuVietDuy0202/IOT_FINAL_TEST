#include "network_module.h"

// ============================================
// BIẾN TOÀN CỤC
// ============================================
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// ============================================
// CALLBACK (NỘI BỘ)
// ============================================
void networkMqttCallback(char* topic, byte* payload, unsigned int length) {
    String message = "";
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    
    Serial.print("[MQTT] ");
    Serial.print(topic);
    Serial.print(" → ");
    Serial.println(message);
    
    // Xử lý lệnh
    if (strcmp(topic, TOPIC_ALARM_CONTROL) == 0) {
        if (message == "ON") {
            Serial.println("🔊 Còi BẬT từ xa");
            // TODO: Gọi alertTrigger() từ main.cpp
        } else if (message == "OFF") {
            Serial.println("🔇 Còi TẮT từ xa");
            // TODO: Gọi alertStop() từ main.cpp
        }
    }
    else if (strcmp(topic, TOPIC_OTA_UPDATE) == 0) {
        Serial.println("🔄 Chuẩn bị OTA...");
        // TODO: Gọi otaStart()
    }
}

// ============================================
// KHỞI TẠO NETWORK (WIFI + MQTT)
// ============================================
void networkInit() {
    Serial.println("[NETWORK] Khởi tạo...");
    
    // Kết nối WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    Serial.print("Kết nối WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" ✅ OK");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    
    // Cấu hình MQTT
    mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
    mqttClient.setCallback(networkMqttCallback);
    
    // Kết nối MQTT
    Serial.println("Kết nối MQTT...");
    while (!mqttClient.connected()) {
        if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
            Serial.println("✅ MQTT OK!");
            
            // Subscribe topics
            mqttClient.subscribe(TOPIC_OTA_UPDATE);
            mqttClient.subscribe(TOPIC_CONFIG_UPDATE);
            mqttClient.subscribe(TOPIC_ALARM_CONTROL);
            
            // Publish online
            mqttClient.publish(TOPIC_STATUS, "ONLINE", true);
        } else {
            Serial.print("❌ Lỗi MQTT rc=");
            Serial.println(mqttClient.state());
            delay(2000);
        }
    }
}

// ============================================
// DUY TRÌ KẾT NỐI
// ============================================
void networkMaintain() {
    // Reconnect MQTT nếu mất kết nối
    if (!mqttClient.connected()) {
        Serial.println("Kết nối lại MQTT...");
        if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
            mqttClient.subscribe(TOPIC_OTA_UPDATE);
            mqttClient.subscribe(TOPIC_CONFIG_UPDATE);
            mqttClient.subscribe(TOPIC_ALARM_CONTROL);
        }
    }
    mqttClient.loop();
}

// ============================================
// PUBLISH DỮ LIỆU CẢM BIẾN
// ============================================
bool networkPublishSensorData(const SensorData& data) {
    if (!mqttClient.connected()) return false;
    
    mqttClient.publish("blackbox/sensor/temperature", String(data.dht.temperature).c_str());
    mqttClient.publish("blackbox/sensor/humidity", String(data.dht.humidity).c_str());
    mqttClient.publish("blackbox/sensor/accel_x", String(data.mpu.accelX).c_str());
    mqttClient.publish("blackbox/sensor/accel_y", String(data.mpu.accelY).c_str());
    mqttClient.publish("blackbox/sensor/accel_z", String(data.mpu.accelZ).c_str());
    mqttClient.publish("blackbox/sensor/event_type", eventTypeToString(data.eventType));
    mqttClient.publish("blackbox/sensor/timestamp", String(data.timestamp).c_str());
    
    Serial.println("[MQTT] ✅ Gửi sensor data");
    return true;
}

// ============================================
// PUBLISH CẢNH BÁO
// ============================================
bool networkPublishAlert(AlertLevel level, const char* message) {
    if (!mqttClient.connected()) return false;
    
    mqttClient.publish("blackbox/alert/level", alertLevelToString(level));
    mqttClient.publish("blackbox/alert/message", message);
    mqttClient.publish("blackbox/alert/timestamp", String(millis()).c_str());
    
    Serial.println("[MQTT] 🚨 Gửi alert");
    return true;
}

// ============================================
// PUBLISH TRẠNG THÁI HỆ THỐNG
// ============================================
bool networkPublishStatus(const SystemStatus& status) {
    if (!mqttClient.connected()) return false;
    
    mqttClient.publish("blackbox/status/wifi", connectionStatusToString(status.wifiStatus));
    mqttClient.publish("blackbox/status/mqtt", connectionStatusToString(status.mqttStatus));
    mqttClient.publish("blackbox/status/sensors", status.sensorsOnline ? "true" : "false");
    mqttClient.publish("blackbox/status/uptime", String(status.uptime).c_str());
    mqttClient.publish("blackbox/status/free_heap", String(status.freeHeapKB).c_str());
    
    Serial.println("[MQTT] 📊 Gửi status");
    return true;
}

// ============================================
// GETTER
// ============================================
bool networkIsConnected() {
    return mqttClient.connected();
}

// ============================================
// DISCONNECT
// ============================================
void networkDisconnect() {
    mqttClient.publish(TOPIC_STATUS, "OFFLINE", true);
    mqttClient.disconnect();
    WiFi.disconnect();
    Serial.println("[NETWORK] 🔌 Ngắt kết nối");
}