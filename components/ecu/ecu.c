//
// Created by jadjer on 01.09.22.
//

#include "ecu.h"
#include "command_result.h"
#include "utils.h"

esp_err_t connect() {
  return m_protocol.connect();
}

void test() {
  uint8_t address = 0x0;

  for (size_t i = 0; i <= 255; i++) {
    uint8_t message[5] = {0x72, 0x05, 0x71, address, 0x00};
    message[4] = calcChecksum(message, 4);

    address++;

    m_protocol.writeData(message, 5);
    m_protocol.readData();
  }
}

char* get_id() {
  return m_id;
}

engine_data_t get_engine_data() {
  return m_engineData;
}

sensors_data_t get_sensors_data() {
  return m_sensorsData;
}

vehicle_data_t get_vehicle_data() {
  return m_vehicleData;
}

error_data_t get_error_data() {
  return m_errorData;
}

unknown_data_t get_unknown_data() {
  return m_unknownData;
}

command_result_t* update_data_from_table(uint8_t table) {
  uint8_t message[5] = {0x72, 0x05, 0x71, table, 0x00};
  message[4] = calcChecksum(message, 4);

  m_protocol.writeData(message, 5);

  return m_protocol.readData();
}

esp_err_t update_data_from_table_0() {
  command_result_t* response = update_data_from_table(0x0);
  if (response == NULL) { return ESP_FAIL; }
  if (response->len != 0xf) { return ESP_FAIL; }

  for (size_t i = 4; i < response->len - 1; i++) {
    m_id += std::to_string(static_cast<int>(response->data[i]));
  }

  return ESP_OK;
}

esp_err_t update_data_from_table_10() {
  command_result_t* response = update_data_from_table(0x10);
  if (response == NULL) { return ESP_FAIL; }
  if (response->len != 0x16) { return ESP_FAIL; }

  m_engineData.rpm = (response->data[4] << 8) + response->data[5];
  m_sensorsData.tpsVolts = calcValueDivide256(response->data[6]);
  m_sensorsData.tpsPercent = calcValueDivide16(response->data[7]);
  m_sensorsData.ectVolts = calcValueDivide256(response->data[8]);
  m_sensorsData.ectTemp = calcValueMinus40(response->data[9]);
  m_sensorsData.iatVolts = calcValueDivide256(response->data[10]);
  m_sensorsData.iatTemp = calcValueMinus40(response->data[11]);
  m_sensorsData.mapVolts = calcValueDivide256(response->data[12]);
  m_sensorsData.mapPressure = response->data[13];
  m_vehicleData.batteryVolts = calcValueDivide10(response->data[16]);
  m_vehicleData.speed = response->data[17];
  m_engineData.fuelInject = (response->data[18] << 8) + response->data[19];
  m_engineData.ignitionAngle = calcValueDivide10(response->data[20]);

  return ESP_OK;
}

esp_err_t update_data_from_table_11() {
  command_result_t* response = update_data_from_table(0x11);
  if (response == NULL) { return ESP_FAIL; }
  if (response->len != 0x19) { return ESP_FAIL; }

  m_engineData.rpm = (response->data[4] << 8) + response->data[5];
  m_sensorsData.tpsVolts = calcValueDivide256(response->data[6]);
  m_sensorsData.tpsPercent = calcValueDivide16(response->data[7]);
  m_sensorsData.ectVolts = calcValueDivide256(response->data[8]);
  m_sensorsData.ectTemp = calcValueMinus40(response->data[9]);
  m_sensorsData.iatVolts = calcValueDivide256(response->data[10]);
  m_sensorsData.iatTemp = calcValueMinus40(response->data[11]);
  m_sensorsData.mapVolts = calcValueDivide256(response->data[12]);
  m_sensorsData.mapPressure = response->data[13];
  m_vehicleData.batteryVolts = calcValueDivide10(response->data[16]);
  m_vehicleData.speed = response->data[17];
  m_engineData.fuelInject = (response->data[18] << 8) + response->data[19];
  m_engineData.ignitionAngle = calcValueDivide10(response->data[20]);

  m_unknownData.unkData1 = response->data[21];
  m_unknownData.unkData2 = response->data[22];
  m_unknownData.unkData3 = response->data[23];

  return ESP_OK;
}

esp_err_t update_data_from_table_20() {
  command_result_t* response = update_data_from_table(0x20);
  if (response == NULL) { return ESP_FAIL; }
  if (response->len != 0x8) { return ESP_FAIL; }

  m_unknownData.unkData4 = response->data[4];
  m_unknownData.unkData5 = response->data[5];
  m_unknownData.unkData6 = response->data[6];

  return ESP_OK;
}

esp_err_t update_data_from_table_21() {
  command_result_t* response = update_data_from_table(0x21);
  if (response == NULL) { return ESP_FAIL; }
  if (response->len != 0xb) { return ESP_FAIL; }

  m_unknownData.unkData1 = response->data[4];
  m_unknownData.unkData2 = response->data[5];
  m_unknownData.unkData3 = response->data[6];
  m_unknownData.unkData4 = response->data[7];
  m_unknownData.unkData5 = response->data[8];
  m_unknownData.unkData6 = response->data[9];

  return ESP_OK;
}

esp_err_t update_data_from_table_D0() {
  command_result_t* response = update_data_from_table(0xD0);
  if (response == NULL) { return ESP_FAIL; }
  if (response->len != 0x13) { return ESP_FAIL; }

  m_unknownData.unkData7 = response->data[4];
  m_unknownData.unkData8 = response->data[5];
  m_unknownData.unkData9 = response->data[6];
  m_unknownData.unkData10 = response->data[7];
  m_unknownData.unkData11 = response->data[8];
  m_unknownData.unkData12 = response->data[9];
  m_unknownData.unkData13 = response->data[10];
  m_unknownData.unkData14 = response->data[11];
  m_unknownData.unkData15 = response->data[12];
  m_unknownData.unkData16 = response->data[13];
  m_unknownData.unkData17 = response->data[14];
  m_unknownData.unkData18 = response->data[15];
  m_unknownData.unkData19 = response->data[16];
  m_unknownData.unkData20 = response->data[17];

  return ESP_OK;
}

esp_err_t update_data_from_table_D1() {
  command_result_t* response = update_data_from_table(0xD1);
  if (response == NULL) { return ESP_FAIL; }
  if (response->len != 0xb) { return ESP_FAIL; }

  m_vehicleData.state = response->data[4];

  m_unknownData.unkData21 = response->data[5];
  m_unknownData.unkData22 = response->data[6];
  m_unknownData.unkData23 = response->data[7];
  m_unknownData.unkData24 = response->data[8];
  m_unknownData.unkData25 = response->data[9];

  return ESP_OK;
}

void spin_once() {
  if (m_enableTable10) { update_data_from_table_10(); }
  if (m_enableTable11) { update_data_from_table_11(); }
  if (m_enableTable20) { update_data_from_table_20(); }
  if (m_enableTable21) { update_data_from_table_21(); }

  update_data_from_table_D0();
  update_data_from_table_D1();
}

void detect_active_tables() {
  update_data_from_table_0();

  if (update_data_from_table_10()) { m_enableTable10 = true; }
  if (update_data_from_table_11()) { m_enableTable11 = true; }
  if (update_data_from_table_20()) { m_enableTable20 = true; }
  if (update_data_from_table_21()) { m_enableTable21 = true; }
}
