#ifndef APP_TYPES_H
#define APP_TYPES_H

#include <Arduino.h>

// ============================================
// ENUMS - Các trạng thái hệ thống
// ============================================

// Mức độ cảnh báo
enum AlertLevel {
    ALERT_NONE = 0,      // Không có cảnh báo
    ALERT_INFO = 1,      // Thông tin
    ALERT_WARNING = 2,   // Cảnh báo
    ALERT_CRITICAL = 3   // Nguy hiểm
};

// Trạng thái kết nối
enum ConnectionStatus {
    CONN_DISCONNECTED = 0,
    CONN_CONNECTING = 1,
    CONN_CONNECTED = 2,
    CONN_ERROR = 3
};

// Loại sự kiện
enum EventType {
    EVENT_NORMAL = 0,           // Hoạt động bình thường
    EVENT_TILT_WARNING = 1,     // Cảnh báo nghiêng
    EVENT_TILT_DANGER = 2,      // Nghiêng nguy hiểm
    EVENT_VIBRATION_HIGH = 3,   // Rung mạnh
    EVENT_CRASH = 4,            // Sự cố nghiêm trọng
    EVENT_TEMP_ABNORMAL = 5,    // Nhiệt độ bất thường
    EVENT_HUMIDITY_LOW = 6      // Độ ẩm thấp (ảnh hưởng bê tông)
};

// Trạng thái OTA
enum OTAStatus {
    OTA_IDLE = 0,
    OTA_DOWNLOADING = 1,
    OTA_INSTALLING = 2,
    OTA_SUCCESS = 3,
    OTA_FAILED = 4
};

// ============================================
// STRUCT - Cấu trúc dữ liệu cảm biến
// ============================================

// Dữ liệu từ MPU6050
struct MPU6050Data {
    float accelX;      // Gia tốc trục X (g)
    float accelY;      // Gia tốc trục Y (g) 
    float accelZ;      // Gia tốc trục Z (g)
    float gyroX;       // Tốc độ góc X (°/s)
    float gyroY;       // Tốc độ góc Y (°/s)
    float gyroZ;       // Tốc độ góc Z (°/s)
    float pitch;
    float roll;
    bool isValid;     // Dữ liệu hợp lệ?
};

// Dữ liệu từ DHT22
struct DHT22Data {
    float temperature; // Nhiệt độ (°C)
    float humidity;    // Độ ẩm (%)
    bool isValid;      // Dữ liệu hợp lệ?
};
// Dữ liệu tổng hợp (gửi lên MQTT)
struct SensorData {
    unsigned long timestamp;    // Thời gian (ms từ khi boot)
    MPU6050Data mpu;           // Dữ liệu MPU6050
    DHT22Data dht;             // Dữ liệu DHT22
    EventType eventType;       // Loại sự kiện
};
// ============================================
// STRUCT - Trạng thái hệ thống
// ============================================
struct SystemStatus {
    ConnectionStatus wifiStatus;     // Trạng thái WiFi
    ConnectionStatus mqttStatus;     // Trạng thái MQTT 
    bool sensorsOnline;             // Cảm biến hoạt động?
    unsigned long uptime;           // Thời gian hoạt động (ms)
    float freeHeapKB;               // RAM còn trống (KB)
};

// HELPER FUNCTIONS - Hàm tiện ích

// Chuyển AlertLevel thành chuỗi
inline const char* alertLevelToString(AlertLevel level) {
    switch(level) {
        case ALERT_NONE:     return "NONE";
        case ALERT_INFO:     return "INFO";
        case ALERT_WARNING:  return "WARNING";
        case ALERT_CRITICAL: return "CRITICAL";
        default:             return "UNKNOWN";
    }
}

// Chuyển EventType thành chuỗi
inline const char* eventTypeToString(EventType type) {
    switch(type) {
        case EVENT_NORMAL:          return "NORMAL";
        case EVENT_TILT_WARNING:    return "TILT_WARNING";
        case EVENT_TILT_DANGER:     return "TILT_DANGER";
        case EVENT_VIBRATION_HIGH:  return "VIBRATION_HIGH";
        case EVENT_CRASH:           return "CRASH";
        case EVENT_TEMP_ABNORMAL:   return "TEMP_ABNORMAL";
        case EVENT_HUMIDITY_LOW:    return "HUMIDITY_LOW";
        default:                    return "UNKNOWN";
    }
}
// Chuyển ConnectionStatus thành chuỗi
inline const char* connectionStatusToString(ConnectionStatus status) {
    switch(status) {
        case CONN_DISCONNECTED: return "DISCONNECTED";
        case CONN_CONNECTING:   return "CONNECTING";
        case CONN_CONNECTED:    return "CONNECTED";
        case CONN_ERROR:        return "ERROR";
        default:                return "UNKNOWN";
    }
}

#endif