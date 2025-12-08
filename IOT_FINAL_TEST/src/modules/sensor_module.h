#ifndef SENSOR_MODULE_H
#define SENSOR_MODULE_H

#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <MPU6050.h>
#include "../../include/app_types.h"    // ← SỬA
#include "../../include/config.h"       // ← SỬA
extern DHT dhtSensor;
extern MPU6050 mpuSensor;
extern bool sensorInitialized;

// Khởi tạo cảm biến
void sensorInit();

// Đọc dữ liệu từ DHT22
DHT22Data sensorReadDHT();
MPU6050Data sensorReadMPU();
SensorData sensorReadAll();
bool sensorIsOnline();
void sensorTest();

#endif