//
// Created by jadjer on 29.08.22.
//

#include "Controller.hpp"
#include "BLE2901.hpp"
#include "BlinkIndicator.hpp"
#include "ErrorCodeIndicator.hpp"
#include "ServicesUUID.hpp"
#include <Arduino.h>
#include <BLE2902.h>
#include <BLEDevice.h>

Controller::Controller() : m_indicator(new BlinkIndicator(2)), m_button(0), m_protocol(16, 17), m_ecu(&m_protocol) {
  BLEDevice::init("HDS");

  log_i("Start bluetooth server...");
  m_indicator->blink(100);

  m_server = BLEDevice::createServer();

  {
    auto service = m_server->createService(serviceVehicleUUID, 16);

    m_vehicleBattery = service->createCharacteristic(vehicleBatteryUUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    m_vehicleBattery->addDescriptor(new BLE2901("Battery"));
    m_vehicleBattery->addDescriptor(new BLE2902());

    m_vehicleSpeed = service->createCharacteristic(vehicleSpeedUUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    m_vehicleSpeed->addDescriptor(new BLE2901("Speed"));
    m_vehicleSpeed->addDescriptor(new BLE2902());

    m_vehicleState = service->createCharacteristic(vehicleStateUUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    m_vehicleState->addDescriptor(new BLE2901("State"));
    m_vehicleState->addDescriptor(new BLE2902());

    service->start();
  }

  {
    auto service = m_server->createService(serviceEngineUUID, 21);

    m_engineRpm = service->createCharacteristic(engineRpmUUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    m_engineRpm->addDescriptor(new BLE2901("RPM"));
    m_engineRpm->addDescriptor(new BLE2902());

    m_engineFuelInject = service->createCharacteristic(engineFuelInjectUUID, BLECharacteristic::PROPERTY_READ);
    m_engineFuelInject->addDescriptor(new BLE2901("Fuel Inject"));

    m_engineIgnitionAdvance = service->createCharacteristic(engineIgnitionAdvanceUUID, BLECharacteristic::PROPERTY_READ);
    m_engineIgnitionAdvance->addDescriptor(new BLE2901("Ignition Advance"));

    m_engineUnk1 = service->createCharacteristic(engineUnknown1UUID, BLECharacteristic::PROPERTY_READ);
    m_engineUnk1->addDescriptor(new BLE2901("Unknown 1"));

    m_engineUnk2 = service->createCharacteristic(engineUnknown2UUID, BLECharacteristic::PROPERTY_READ);
    m_engineUnk2->addDescriptor(new BLE2901("Unknown 2"));

    m_engineUnk3 = service->createCharacteristic(engineUnknown3UUID, BLECharacteristic::PROPERTY_READ);
    m_engineUnk3->addDescriptor(new BLE2901("Unknown 2"));

    service->start();
  }

  {
    auto service = m_server->createService(serviceSensorsUUID, 33);

    m_sensorTpsPercent = service->createCharacteristic(sensorTpsPercentUUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    m_sensorTpsPercent->addDescriptor(new BLE2901("TPS Percent"));
    m_sensorTpsPercent->addDescriptor(new BLE2902());

    m_sensorTpsVoltage = service->createCharacteristic(sensorTpsVoltageUUID, BLECharacteristic::PROPERTY_READ);
    m_sensorTpsVoltage->addDescriptor(new BLE2901("TPS Voltage"));

    m_sensorEctTemp = service->createCharacteristic(sensorEctTempUUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    m_sensorEctTemp->addDescriptor(new BLE2901("ECT Temp"));
    m_sensorEctTemp->addDescriptor(new BLE2902());

    m_sensorEctVoltage = service->createCharacteristic(sensorEctVoltageUUID, BLECharacteristic::PROPERTY_READ);
    m_sensorEctVoltage->addDescriptor(new BLE2901("ECT Voltage"));

    m_sensorIatTemp = service->createCharacteristic(sensorIatTempUUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    m_sensorIatTemp->addDescriptor(new BLE2901("IAT Temp"));
    m_sensorIatTemp->addDescriptor(new BLE2902());

    m_sensorIatVoltage = service->createCharacteristic(sensorIatVoltageUUID, BLECharacteristic::PROPERTY_READ);
    m_sensorIatVoltage->addDescriptor(new BLE2901("IAT Voltage"));

    m_sensorMapPressure = service->createCharacteristic(sensorMapPressureUUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    m_sensorMapPressure->addDescriptor(new BLE2901("MAP Pressure"));
    m_sensorMapPressure->addDescriptor(new BLE2902());

    m_sensorMapVoltage = service->createCharacteristic(sensorMapVoltageUUID, BLECharacteristic::PROPERTY_READ);
    m_sensorMapVoltage->addDescriptor(new BLE2901("MAP Voltage"));

    service->start();
  }

  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(serviceVehicleUUID);
  pAdvertising->addServiceUUID(serviceEngineUUID);
  pAdvertising->addServiceUUID(serviceSensorsUUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);

  BLEDevice::startAdvertising();

  log_i("Connect to ECU...");
  m_indicator->blink(250);

  esp_err_t errorConnection = m_ecu.connect();
  if (errorConnection) {
    log_e("Connect failed to Honda ECU (err %d)", errorConnection);

    delete m_indicator;

    m_indicator = new ErrorCodeIndicator(2);

    if (errorConnection == ESP_ERR_INVALID_RESPONSE) { m_indicator->blink(1); }
    if (errorConnection == ESP_ERR_INVALID_SIZE) { m_indicator->blink(2); }
    if (errorConnection == ESP_ERR_INVALID_ARG) { m_indicator->blink(3); }
    if (errorConnection == ESP_ERR_INVALID_CRC) { m_indicator->blink(4); }

    return;
  }

  log_i("Detect active tables...");
  m_indicator->blink(500);

  m_ecu.detectActiveTables();

  log_i("Initialize done");
  m_indicator->enable();
}

Controller::~Controller() = default;

[[noreturn]] void Controller::spin() {
  while (true) {
    spinOnce();
    delay(10);
  }
}

void Controller::spinOnce() {
  if (m_button.isPressed()) {
    log_i("Detect all tables...");

    m_indicator->blink(500);
    m_ecu.detectAllTables();
    m_indicator->enable();

    m_button.resetState();
  }

  m_ecu.updateAllData();

  auto vehicleData = m_ecu.getVehicleData();

  m_vehicleBattery->setValue(vehicleData.batteryVolts);
  m_vehicleBattery->notify();

  m_vehicleSpeed->setValue(vehicleData.speed);
  m_vehicleSpeed->notify();

  m_vehicleState->setValue(vehicleData.state);
  m_vehicleState->notify();

  auto engineData = m_ecu.getEngineData();

  m_engineRpm->setValue(engineData.rpm);
  m_engineRpm->notify();

  m_engineFuelInject->setValue(engineData.fuelInject);
  m_engineIgnitionAdvance->setValue(engineData.ignitionAdvance);
  m_engineUnk1->setValue(engineData.unkData1);
  m_engineUnk2->setValue(engineData.unkData2);
  m_engineUnk3->setValue(engineData.unkData3);

  auto sensorsData = m_ecu.getSensorsData();

  m_sensorTpsPercent->setValue(sensorsData.tpsPercent);
  m_sensorTpsPercent->notify();

  m_sensorTpsVoltage->setValue(sensorsData.tpsVolts);

  m_sensorIatTemp->setValue(sensorsData.iatTemp);
  m_sensorIatTemp->notify();

  m_sensorIatVoltage->setValue(sensorsData.iatVolts);

  m_sensorEctTemp->setValue(sensorsData.ectTemp);
  m_sensorEctTemp->notify();

  m_sensorEctVoltage->setValue(sensorsData.ectVolts);

  m_sensorMapPressure->setValue(sensorsData.mapPressure);
  m_sensorMapPressure->notify();

  m_sensorMapVoltage->setValue(sensorsData.mapVolts);
}
