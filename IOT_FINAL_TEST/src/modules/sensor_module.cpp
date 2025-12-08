#include "sensor_module.h"


// BIẾN TOÀN CỤC

DHT dhtSensor(PIN_DHT, DHT22);
MPU6050 mpuSensor;
bool sensorInitialized = false;


// KHỞI TẠO CẢM BIẾN

void sensorInit() {
    Serial.println("[SENSOR] Initializing sensors...");
    // Khởi tạo DHT22
    dhtSensor.begin();
    Serial.println("[SENSOR] DHT22 initialized");
    // Khởi tạo I2C cho MPU6050
    Wire.begin(PIN_MPU_SDA, PIN_MPU_SCL);
    // Khởi tạo MPU6050
    mpuSensor.initialize();
    // Kiểm tra kết nối MPU6050
    if (mpuSensor.testConnection()) {
        Serial.println("[SENSOR] MPU6050 connected successfully");
        // Cấu hình MPU6050
        mpuSensor.setFullScaleAccelRange(MPU6050_ACCEL_FS_2); // ±2g
        mpuSensor.setFullScaleGyroRange(MPU6050_GYRO_FS_250); // ±250°/s
        sensorInitialized = true;
    } else {
        Serial.println("[SENSOR] ERROR: MPU6050 connection failed!");
        sensorInitialized = false;
    }
}
// ĐỌC DỮ LIỆU DHT22
DHT22Data sensorReadDHT() {
    DHT22Data data;
    data.temperature = dhtSensor.readTemperature();
    data.humidity = dhtSensor.readHumidity();
    // Kiểm tra dữ liệu hợp lệ
    if (isnan(data.temperature) || isnan(data.humidity)) {
        // ❌ Lỗi đọc cảm biến
        data.isValid = false;
        // Set giá trị mặc định an toàn
        data.temperature = 0;  // Giá trị không thể xảy ra
        data.humidity = 0;     // Dễ nhận biết lỗi
        Serial.println("[SENSOR] ERROR: DHT22 read failed!");
    } else {
        // ✅ Dữ liệu OK
        data.isValid = true;
        #ifdef ENABLE_SERIAL_DEBUG
        Serial.print("[SENSOR] DHT22 OK: T=");
        Serial.print(data.temperature);
        Serial.print("°C, H=");
        Serial.print(data.humidity);
        Serial.println("%");
        #endif
    }
    return data;
}
// ĐỌC DỮ LIỆU MPU6050
MPU6050Data sensorReadMPU() {
    MPU6050Data data;
    
    if (!sensorInitialized) {
        Serial.println("[SENSOR] ERROR: MPU6050 not initialized!");
        // Trả về dữ liệu 0
        data.accelX = data.accelY = data.accelZ = 0;
        data.gyroX = data.gyroY = data.gyroZ = 0;
        return data;
    }
    // Đọc gia tốc (raw values)
    int16_t ax, ay, az;
    mpuSensor.getAcceleration(&ax, &ay, &az);
    // Chuyển đổi sang đơn vị g (1g = 9.8m/s²)
    // MPU6050 range ±2g = ±16384 LSB
    data.accelX = ax / 16384.0;
    data.accelY = ay / 16384.0;
    data.accelZ = az / 16384.0;
    
    // Đọc tốc độ góc (raw values)
    int16_t gx, gy, gz;
    mpuSensor.getRotation(&gx, &gy, &gz);
    
    // Chuyển đổi sang đơn vị °/s
    // MPU6050 range ±250°/s = ±131 LSB
    data.gyroX = gx / 131.0;
    data.gyroY = gy / 131.0;
    data.gyroZ = gz / 131.0;
    return data;
}

// ĐỌC TẤT CẢ CẢM BIẾN

SensorData sensorReadAll() {
    SensorData data;
    data.timestamp = millis();
    data.dht = sensorReadDHT();
    data.mpu = sensorReadMPU();
    data.eventType = EVENT_NORMAL;
    return data;
}

// ============================================
// KIỂM TRA TRẠNG THÁI CẢM BIẾN
// ============================================
bool sensorIsOnline() {
    return sensorInitialized;
}
// ============================================
// TEST CẢM BIẾN (DEBUG)

void sensorTest() {
    Serial.println("\n========== SENSOR TEST ==========");
    
    // Test DHT22
    DHT22Data dht = sensorReadDHT();
    Serial.println("\n--- DHT22 ---");
    Serial.print("Temperature: ");
    Serial.print(dht.temperature);
    Serial.println(" °C");
    Serial.print("Humidity: ");
    Serial.print(dht.humidity);
    Serial.println(" %");
    Serial.print("Valid: ");
    Serial.println(dht.isValid ? "YES" : "NO");
    // Test MPU6050  
    MPU6050Data mpu = sensorReadMPU();
    Serial.println("\n--- MPU6050 ---");
    Serial.print("Accel X: ");
    Serial.print(mpu.accelX);
    Serial.println(" g");
    Serial.print("Accel Y: ");
    Serial.print(mpu.accelY);
    Serial.println(" g");
    Serial.print("Accel Z: ");
    Serial.print(mpu.accelZ);
    Serial.println(" g");
    Serial.print("Gyro X: ");
    Serial.print(mpu.gyroX);
    Serial.println(" °/s");
    Serial.print("Gyro Y: ");
    Serial.print(mpu.gyroY);
    Serial.println(" °/s");
    Serial.print("Gyro Z: ");
    Serial.print(mpu.gyroZ);
    Serial.println(" °/s");
    Serial.println("=================================\n");
}