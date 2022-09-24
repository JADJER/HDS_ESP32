//
// Created by jadjer on 29.08.22.
//

#include "Controller.hpp"
#include "ServicesUUID.hpp"
#include <Arduino.h>

Controller::Controller() : m_indicator(2), m_button(0), m_protocol(16, 17), m_ecu(&m_protocol), m_bluetooth("HDS") {
  log_i("Start bluetooth server...");
  m_indicator.blink(100);

  m_bluetooth.createService(SERVICE_STATE_UUID, {
                                                    STATE_CONNECTED_UUID,
                                                });
  m_bluetooth.createService(SERVICE_VEHICLE_UUID, {
                                                      VEHICLE_BATTERY_UUID,
                                                      VEHICLE_SPEED_UUID,
                                                      VEHICLE_STATE_UUID,
                                                  });
  m_bluetooth.createService(SERVICE_ENGINE_UUID, {
                                                     ENGINE_FUEL_INJECT_UUID,
                                                     ENGINE_IGNITION_ADVANCE_UUID,
                                                     ENGINE_RPM_UUID,
                                                     ENGINE_UNKNOWN_1_UUID,
                                                     ENGINE_UNKNOWN_2_UUID,
                                                     ENGINE_UNKNOWN_3_UUID,
                                                 });
  m_bluetooth.createService(SERVICE_SENSORS_UUID, {
                                                      SENSORS_TPS_PERCENT_UUID,
                                                      SENSORS_TPS_VOLTAGE_UUID,
                                                      SENSORS_ECT_TEMP_UUID,
                                                      SENSORS_ECT_VOLTAGE_UUID,
                                                      SENSORS_IAT_TEMP_UUID,
                                                      SENSORS_IAT_VOLTAGE_UUID,
                                                      SENSORS_MAP_PRESSURE_UUID,
                                                      SENSORS_MAP_VOLTAGE_UUID,
                                                  });
  m_bluetooth.start();
  m_bluetooth.setValueString(STATE_CONNECTED_UUID, "Connecting...");
  m_bluetooth.advertising();

  log_i("Connect to ECU...");
  m_indicator.blink(250);

  esp_err_t errorConnection = m_ecu.connect();
  if (errorConnection) {
    log_e("Connect failed to Honda ECU (err %d)", errorConnection);

    if (errorConnection == ESP_ERR_INVALID_RESPONSE) { m_indicator.blinkErrorCode(1); }
    if (errorConnection == ESP_ERR_INVALID_SIZE) { m_indicator.blinkErrorCode(2); }
    if (errorConnection == ESP_ERR_INVALID_ARG) { m_indicator.blinkErrorCode(3); }
    if (errorConnection == ESP_ERR_INVALID_CRC) { m_indicator.blinkErrorCode(4); }

    m_bluetooth.setValueString(STATE_CONNECTED_UUID, "Disconnected");

    return;
  }

  m_bluetooth.setValueString(STATE_CONNECTED_UUID, "Connected");

  log_i("Detect active tables...");
  m_indicator.blink(500);

  m_ecu.detectActiveTables();

  log_i("Initialize done");
  m_indicator.enable();
}

Controller::~Controller() = default;

[[noreturn]] void Controller::spin() {
  while (true) {
    spinOnce();
    delay(10);
  }
}

void Controller::spinOnce() {
  if (Button::isPressed()) {
    log_i("Detect all tables...");

    m_indicator.blink(500);
    m_ecu.detectAllTables();
    m_indicator.enable();

    Button::resetState();
  }

  m_ecu.updateAllData();

  auto vehicleData = m_ecu.getVehicleData();
  m_bluetooth.setValueString(VEHICLE_BATTERY_UUID, std::to_string(vehicleData.batteryVolts));
  m_bluetooth.setValueString(VEHICLE_SPEED_UUID, std::to_string(vehicleData.speed));
  m_bluetooth.setValueString(VEHICLE_STATE_UUID, std::to_string(vehicleData.state));

  auto engineData = m_ecu.getEngineData();
  m_bluetooth.setValueString(ENGINE_RPM_UUID, std::to_string(engineData.rpm));
  m_bluetooth.setValueString(ENGINE_FUEL_INJECT_UUID, std::to_string(engineData.fuelInject));
  m_bluetooth.setValueString(ENGINE_IGNITION_ADVANCE_UUID, std::to_string(engineData.ignitionAdvance));
  m_bluetooth.setValueString(ENGINE_UNKNOWN_1_UUID, std::to_string(engineData.unkData1));
  m_bluetooth.setValueString(ENGINE_UNKNOWN_2_UUID, std::to_string(engineData.unkData2));
  m_bluetooth.setValueString(ENGINE_UNKNOWN_3_UUID, std::to_string(engineData.unkData3));

  auto sensorsData = m_ecu.getSensorsData();
  m_bluetooth.setValueString(SENSORS_TPS_PERCENT_UUID, std::to_string(sensorsData.tpsPercent));
  m_bluetooth.setValueString(SENSORS_TPS_VOLTAGE_UUID, std::to_string(sensorsData.tpsVolts));
  m_bluetooth.setValueString(SENSORS_IAT_TEMP_UUID, std::to_string(sensorsData.iatTemp));
  m_bluetooth.setValueString(SENSORS_IAT_VOLTAGE_UUID, std::to_string(sensorsData.iatVolts));
  m_bluetooth.setValueString(SENSORS_ECT_TEMP_UUID, std::to_string(sensorsData.ectTemp));
  m_bluetooth.setValueString(SENSORS_ECT_VOLTAGE_UUID, std::to_string(sensorsData.ectVolts));
  m_bluetooth.setValueString(SENSORS_MAP_PRESSURE_UUID, std::to_string(sensorsData.mapPressure));
  m_bluetooth.setValueString(SENSORS_MAP_VOLTAGE_UUID, std::to_string(sensorsData.mapVolts));
}
