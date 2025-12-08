#ifndef CONFIG_H
#define CONFIG_H

// ============================================
// SYSTEM INFORMATION
// ============================================
#define DEVICE_NAME "IOT_BLACKBOX_001"
#define FIRMWARE_VERSION "1.0.0"

// ============================================
// MQTT BROKER CONFIGURATION
// ============================================
#define MQTT_BROKER "ke19046e.ala.asia-southeast1.emqxsl.com"
#define MQTT_PORT 8883  // MQTTS (TLS/SSL)

// MQTT TOPICS - PUBLISH (ESP32 → Server)

#define TOPIC_SENSOR_DATA "blackbox/sensor/data"           // Dữ liệu tổng hợp
#define TOPIC_VIBRATION "blackbox/vibration"               // Độ rung
#define TOPIC_TILT "blackbox/tilt"                         // Góc nghiêng
#define TOPIC_TEMPERATURE "blackbox/temperature"           // Nhiệt độ
#define TOPIC_HUMIDITY "blackbox/humidity"                 // Độ ẩm
#define TOPIC_ALERT "blackbox/alert"                       // Cảnh báo khẩn cấp
#define TOPIC_STATUS "blackbox/status"                     // Trạng thái thiết bị

// ============================================
// MQTT TOPICS - SUBSCRIBE (Server → ESP32)
// ============================================
#define TOPIC_OTA_UPDATE "blackbox/ota/update"            // Lệnh OTA
#define TOPIC_CONFIG_UPDATE "blackbox/config/update"      // Cập nhật cấu hình
#define TOPIC_ALARM_CONTROL "blackbox/alarm/control"      // Điều khiển còi

// ============================================
// SENSOR PINS
// ============================================
#define PIN_DHT 4               // DHT22 data pin
#define PIN_MPU_SDA 21          // MPU6050 SDA
#define PIN_MPU_SCL 22          // MPU6050 SCL
#define PIN_BUZZER 5            // Còi báo động

// ============================================
// SAFETY THRESHOLDS - Ngưỡng an toàn
// ============================================
// Concrete Curing (Bảo dưỡng bê tông)
#define TEMP_MAX_CONCRETE 35.0        // °C - Nhiệt độ tối đa
#define HUMIDITY_MIN_CONCRETE 50.0    // % - Độ ẩm tối thiểu

// Tilt Detection (Phát hiện nghiêng)
#define TILT_WARNING_ANGLE 10.0       // độ - Cảnh báo nghiêng
#define TILT_DANGER_ANGLE 15.0        // độ - Nguy hiểm nghiêng

// Vibration Detection (Phát hiện rung)
#define VIBRATION_NORMAL 0.5          // g - Rung bình thường
#define VIBRATION_WARNING 1.5         // g - Cảnh báo rung
#define VIBRATION_CRITICAL 3.0        // g - Rung nguy hiểm (crash)

// ============================================
// TIMING CONFIGURATION
// ============================================
#define SENSOR_READ_INTERVAL 1000     // ms - Đọc cảm biến mỗi 1 giây
#define MQTT_PUBLISH_INTERVAL 5000    // ms - Gửi MQTT mỗi 5 giây
#define WIFI_RECONNECT_INTERVAL 10000 // ms - Thử kết nối WiFi lại
#define HEARTBEAT_INTERVAL 30000      // ms - Gửi heartbeat mỗi 30 giây

// ============================================
// ALERT CONFIGURATION
// ============================================
#define BUZZER_BEEP_DURATION 200      // ms - Thời gian beep
#define BUZZER_WARNING_INTERVAL 2000  // ms - Khoảng cách giữa các beep (cảnh báo)
#define BUZZER_CRITICAL_INTERVAL 500  // ms - Khoảng cách giữa các beep (nguy hiểm)

// ============================================
// OTA CONFIGURATION
// ============================================
#define OTA_HOSTNAME "iot-blackbox"
#define OTA_PASSWORD "admin123"       // Mật khẩu OTA (nên lưu trong secrets.h)
#define OTA_PORT 3232

// ============================================
// DEBUG SETTINGS
// ============================================
#define ENABLE_SERIAL_DEBUG true
#define SERIAL_BAUD_RATE 115200

#endif