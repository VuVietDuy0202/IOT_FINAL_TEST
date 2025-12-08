#ifndef SECRETS_H
#define SECRETS_H

// ============================================
// WIFI CREDENTIALS
// ============================================
#define WIFI_SSID "SkyNet"           // Tên WiFi
#define WIFI_PASSWORD "12345678123"   // Mật khẩu WiFi
// ============================================
// MQTT CREDENTIALS
// ============================================
#define MQTT_USERNAME "DUY2"
#define MQTT_PASSWORD "DUY"
#define MQTT_CLIENT_ID "IOT_BLACKBOX_001"  // ID duy nhất cho client

// ============================================
// OTA UPDATE PASSWORD
// ============================================
#define OTA_PASSWORD "admin123"            // Mật khẩu để bảo vệ OTA

// ============================================
// API KEYS (Nếu cần)
// ============================================
// #define OPENWEATHER_API_KEY "your_api_key_here"
// #define FIREBASE_API_KEY "your_firebase_key"

// ============================================
// TLS/SSL CERTIFICATES (Cho MQTTS)
// ============================================
// Root CA Certificate cho EMQX
const char* MQTT_ROOT_CA = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3
d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD
QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT
... (Copy toàn bộ certificate của bạn vào đây)
-----END CERTIFICATE-----
)EOF";

// ============================================
// SECURITY NOTES
// ============================================
// ⚠️ QUAN TRỌNG:
// 1. KHÔNG commit file này lên GitHub/GitLab
// 2. Thêm "secrets.h" vào file .gitignore
// 3. Tạo file "secrets_template.h" để chia sẻ với team (không chứa thông tin thật)

#endif