//
// Created by jadjer on 29.08.22.
//

#include "Controller.hpp"
#include "BlinkIndicator.hpp"
#include "ErrorCodeIndicator.hpp"
#include "ServicesUUID.hpp"
#include <Arduino.h>

Controller::Controller() : m_indicator(new BlinkIndicator(2)), m_button(0), m_protocol(16, 17), m_ecu(&m_protocol) {
  log_i("Start bluetooth server...");
  m_indicator->blink(100);

  auto bluetooth = Bluetooth("HDS");

  m_bluetoothServer = bluetooth.createServer();
  m_bluetoothServer->createService(SERVICE_STATE_UUID, {
                                                           STATE_CONNECTED_UUID,
                                                       });
  m_bluetoothServer->createService(SERVICE_VEHICLE_UUID, {
                                                             VEHICLE_BATTERY_UUID,
                                                             VEHICLE_SPEED_UUID,
                                                             VEHICLE_STATE_UUID,
                                                         });
  m_bluetoothServer->createService(SERVICE_ENGINE_UUID, {
                                                            ENGINE_FUEL_INJECT_UUID,
                                                            ENGINE_IGNITION_ADVANCE_UUID,
                                                            ENGINE_RPM_UUID,
                                                            ENGINE_UNKNOWN_1_UUID,
                                                            ENGINE_UNKNOWN_2_UUID,
                                                            ENGINE_UNKNOWN_3_UUID,
                                                        });
  m_bluetoothServer->createService(SERVICE_SENSORS_UUID, {
                                                             SENSORS_TPS_PERCENT_UUID,
                                                             SENSORS_TPS_VOLTAGE_UUID,
                                                             SENSORS_ECT_TEMP_UUID,
                                                             SENSORS_ECT_VOLTAGE_UUID,
                                                             SENSORS_IAT_TEMP_UUID,
                                                             SENSORS_IAT_VOLTAGE_UUID,
                                                             SENSORS_MAP_PRESSURE_UUID,
                                                             SENSORS_MAP_VOLTAGE_UUID,
                                                         });
  m_bluetoothServer->start();
  m_bluetoothServer->setValue(STATE_CONNECTED_UUID, "Connecting...");
  m_bluetoothServer->advertising();

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

    m_bluetoothServer->setValue(STATE_CONNECTED_UUID, "Disconnected");

    return;
  }

  m_bluetoothServer->setValue(STATE_CONNECTED_UUID, "Connected");

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
  m_bluetoothServer->setValue(VEHICLE_BATTERY_UUID, vehicleData.batteryVolts);
  m_bluetoothServer->setValue(VEHICLE_SPEED_UUID, vehicleData.speed);
  m_bluetoothServer->setValue(VEHICLE_STATE_UUID, vehicleData.state);

  auto engineData = m_ecu.getEngineData();
  m_bluetoothServer->setValue(ENGINE_RPM_UUID, engineData.rpm);
  m_bluetoothServer->setValue(ENGINE_FUEL_INJECT_UUID, engineData.fuelInject);
  m_bluetoothServer->setValue(ENGINE_IGNITION_ADVANCE_UUID, engineData.ignitionAdvance);
  m_bluetoothServer->setValue(ENGINE_UNKNOWN_1_UUID, engineData.unkData1);
  m_bluetoothServer->setValue(ENGINE_UNKNOWN_2_UUID, engineData.unkData2);
  m_bluetoothServer->setValue(ENGINE_UNKNOWN_3_UUID, engineData.unkData3);

  auto sensorsData = m_ecu.getSensorsData();
  m_bluetoothServer->setValue(SENSORS_TPS_PERCENT_UUID, sensorsData.tpsPercent);
  m_bluetoothServer->setValue(SENSORS_TPS_VOLTAGE_UUID, sensorsData.tpsVolts);
  m_bluetoothServer->setValue(SENSORS_IAT_TEMP_UUID, sensorsData.iatTemp);
  m_bluetoothServer->setValue(SENSORS_IAT_VOLTAGE_UUID, sensorsData.iatVolts);
  m_bluetoothServer->setValue(SENSORS_ECT_TEMP_UUID, sensorsData.ectTemp);
  m_bluetoothServer->setValue(SENSORS_ECT_VOLTAGE_UUID, sensorsData.ectVolts);
  m_bluetoothServer->setValue(SENSORS_MAP_PRESSURE_UUID, sensorsData.mapPressure);
  m_bluetoothServer->setValue(SENSORS_MAP_VOLTAGE_UUID, sensorsData.mapVolts);
}
