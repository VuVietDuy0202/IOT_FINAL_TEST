#include <Arduino.h>

// Include các module đã có
#include "modules/sensor_module.h"
#include "modules/analytics_module.h"
#include "modules/alert_module.h"
#include "modules/network_module.h"
// BIẾN TOÀN CỤC
unsigned long lastSensorRead = 0;
unsigned long lastPublish = 0;
unsigned long lastHeartbeat = 0;

SensorData currentSensorData;
AnalyticsData currentAnalytics;

// SETUP - Chạy 1 lần khi khởi động

void setup() {
    // Khởi tạo Serial
    Serial.begin(SERIAL_BAUD_RATE);
    delay(2000);
    
    Serial.println("\n\n========================================");
    Serial.println("  IOT BLACKBOX - Starting..."); 
    
    Serial.println("  Firmware: " + String(FIRMWARE_VERSION));
    Serial.println("========================================\n");


    // 1. Khởi tạo cảm biến
    Serial.println("[MAIN] Initializing sensors...");
    sensorInit();
    delay(1000);

    // 2. Test cảm biến
    if (sensorIsOnline()) {
        sensorTest();
    } else {
        Serial.println("[MAIN] ERROR: Sensors initialization failed!");
    }
    
    // 3. Hiệu chuẩn (Calibration)
    Serial.println("\n[MAIN] Calibrating... Keep device STILL for 3 seconds!");
    delay(3000);
    
    MPU6050Data mpu = sensorReadMPU();
    analyticsCalibrate(mpu);
    
    // 4. Khởi tạo analytics
    analyticsInit();
    
    // 5. Khởi tạo alert (còi)
    alertInit();
    
    // 6. Khởi tạo network (WiFi + MQTT)
    Serial.println("\n[MAIN] Connecting to network...");
    networkInit();
    
    // 7. Phát tiếng beep báo sẵn sàng
    Serial.println("\n[MAIN] System ready!");
    alertBeep(200);
    delay(100);
    alertBeep(200);
    
    Serial.println("========================================\n");
}

// ============================================
// LOOP - Chạy liên tục
// ============================================
void loop() {
    unsigned long now = millis();
    
    // ============================================
    // 1. DUY TRÌ KẾT NỐI MẠNG
    // ============================================
    networkMaintain();
    
    // ============================================
    // 2. ĐỌC CẢM BIẾN (Mỗi 1 giây)
    // ============================================
    if (now - lastSensorRead >= SENSOR_READ_INTERVAL) {
        lastSensorRead = now;
        // Đọc tất cả cảm biến
        currentSensorData = sensorReadAll();
        
        // Phân tích dữ liệu
        currentAnalytics = analyticsProcess(currentSensorData);
        
        // Cập nhật analytics vào sensor data
        currentSensorData.analytics = currentAnalytics;
        
        // In dữ liệu ra Serial (debug)
        printSensorData();
        
        // ============================================
        // 3. KIỂM TRA CẢNH BÁO
        // ============================================
        AlertLevel alertLevel = currentAnalytics.alertLevel;
        
        if (alertLevel >= ALERT_WARNING) {
            // Có cảnh báo -> Kích hoạt
            const char* message = getAlertMessage(currentAnalytics);
            alertTrigger(alertLevel, message);
            
            // Xác định event type
            currentSensorData.eventType = getEventType(currentAnalytics);
            
        } else {
            // Không có cảnh báo -> Tắt còi
            if (alertGetCurrentLevel() != ALERT_NONE) {
                alertStop();
            }
            currentSensorData.eventType = EVENT_NORMAL;
        }
    }
    
    // ============================================
    // 4. CẬP NHẬT PATTERN CÒI
    // ============================================
    alertUpdate();
    
    // ============================================
    // 5. PUBLISH DỮ LIỆU LÊN MQTT (Mỗi 5 giây)
    // ============================================
    if (networkIsConnected() && (now - lastPublish >= MQTT_PUBLISH_INTERVAL)) {
        lastPublish = now;
        
        // Gửi dữ liệu cảm biến
        bool success = networkPublishSensorData(currentSensorData);
        
        if (success) {
            Serial.println("[MAIN] Data published to MQTT");
        }
        
        // Nếu có cảnh báo, gửi riêng alert message
        if (currentAnalytics.alertLevel >= ALERT_WARNING) {
            networkPublishAlert(
                currentAnalytics.alertLevel, 
                alertGetMessage()
            );
        }
    }
    
    // ============================================
    // 6. HEARTBEAT - Gửi trạng thái hệ thống (Mỗi 30s)
    // ============================================
    if (now - lastHeartbeat >= HEARTBEAT_INTERVAL) {
        lastHeartbeat = now;
        
        SystemStatus status = getSystemStatus();
        networkPublishStatus(status);
        
        Serial.println("[MAIN] Heartbeat sent");
    }
    
    // Delay nhỏ để tránh watchdog timeout
    delay(10);
}

// ============================================
// HÀM HỖ TRỢ - IN DỮ LIỆU CẢM BIẾN
// ============================================
void printSensorData() {
    Serial.println("\n----- SENSOR DATA -----");
    
    // DHT22
    Serial.print("Temp: ");
    Serial.print(currentSensorData.dht.temperature);
    Serial.print(" °C | Humidity: ");
    Serial.print(currentSensorData.dht.humidity);
    Serial.println(" %");
    
    // MPU6050 - Gia tốc
    Serial.print("Accel X: ");
    Serial.print(currentSensorData.mpu.accelX, 3);
    Serial.print(" | Y: ");
    Serial.print(currentSensorData.mpu.accelY, 3);
    Serial.print(" | Z: ");
    Serial.print(currentSensorData.mpu.accelZ, 3);
    Serial.println(" g");
    
    // Analytics
    Serial.print("Tilt Angle: ");
    Serial.print(currentAnalytics.tiltAngle, 2);
    Serial.print(" ° | Vibration: ");
    Serial.print(currentAnalytics.vibrationLevel, 3);
    Serial.println(" g");
    
    Serial.print("Concrete Safe: ");
    Serial.print(currentAnalytics.isConcreteSafe ? "YES" : "NO");
    Serial.print(" | Alert: ");
    Serial.println(alertLevelToString(currentAnalytics.alertLevel));
    
    Serial.println("-----------------------");
}

// ============================================
// HÀM HỖ TRỢ - LẤY MESSAGE CẢNH BÁO
// ============================================
const char* getAlertMessage(const AnalyticsData& analytics) {
    if (analytics.vibrationLevel >= VIBRATION_CRITICAL) {
        return "CRITICAL: High vibration detected!";
    }
    if (analytics.tiltAngle >= TILT_DANGER_ANGLE) {
        return "CRITICAL: Dangerous tilt angle!";
    }
    if (analytics.vibrationLevel >= VIBRATION_WARNING) {
        return "WARNING: Elevated vibration";
    }
    if (analytics.tiltAngle >= TILT_WARNING_ANGLE) {
        return "WARNING: Structure tilting";
    }
    if (!analytics.isConcreteSafe) {
        return "WARNING: Concrete curing at risk";
    }
    return "System OK";
}

// ============================================
// HÀM HỖ TRỢ - XÁC ĐỊNH EVENT TYPE
// ============================================
EventType getEventType(const AnalyticsData& analytics) {
    if (analytics.vibrationLevel >= VIBRATION_CRITICAL) {
        return EVENT_CRASH;
    }
    if (analytics.tiltAngle >= TILT_DANGER_ANGLE) {
        return EVENT_TILT_DANGER;
    }
    if (analytics.vibrationLevel >= VIBRATION_WARNING) {
        return EVENT_VIBRATION_HIGH;
    }
    if (analytics.tiltAngle >= TILT_WARNING_ANGLE) {
        return EVENT_TILT_WARNING;
    }
    if (!analytics.isConcreteSafe) {
        return EVENT_HUMIDITY_LOW;
    }
    return EVENT_NORMAL;
}

// ============================================
// HÀM HỖ TRỢ - LẤY TRẠNG THÁI HỆ THỐNG
// ============================================
SystemStatus getSystemStatus() {
    SystemStatus status;
    
    status.wifiStatus = networkGetWiFiStatus();
    status.mqttStatus = networkGetMQTTStatus();
    status.sensorsOnline = sensorIsOnline();
    status.uptime = millis();
    status.freeHeapKB = ESP.getFreeHeap() / 1024.0;
    status.otaStatus = OTA_IDLE; // Sẽ dùng khi có ota_module
    
    return status;
}