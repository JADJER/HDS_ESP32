//
// Created by jadjer on 01.09.22.
//

#include "ECU.hpp"
#include "CommandResult.hpp"
#include "Protocol.hpp"
#include "utils.hpp"

ECU::ECU(IProtocol* protocol) {
  m_protocol = protocol;
}

ECU::~ECU() = default;

esp_err_t ECU::connect() {
  if (m_isInitialised) { return ESP_OK; }

  esp_err_t err = m_protocol->connect();
  if (err == ESP_OK) {
    m_isInitialised = true;
  }

  return err;
}

void ECU::detectAllTables() {
  if (!m_isInitialised) { return; }

  std::lock_guard<std::mutex> lock(m_mutex);

  uint8_t address = 0x0;

  for (size_t i = 0; i <= 255; i++) {
    uint8_t message[5] = {0x72, 0x05, 0x71, address, 0x00};
    message[4] = calcChecksum(message, 4);

    address++;

    m_protocol->writeData(message, 5);
    m_protocol->readData();
  }
}

void ECU::detectActiveTables() {
  if (!m_isInitialised) { return; }

  std::lock_guard<std::mutex> lock(m_mutex);

  updateDataFromTable0();

  if (updateDataFromTable10()) { m_enableTable10 = true; }
  if (updateDataFromTable11()) { m_enableTable11 = true; }
  if (updateDataFromTable20()) { m_enableTable20 = true; }
  if (updateDataFromTable21()) { m_enableTable21 = true; }
}

void ECU::updateAllData() {
  if (!m_isInitialised) { return; }

  std::lock_guard<std::mutex> lock(m_mutex);

  if (m_enableTable10 == 1) { updateDataFromTable10(); }
  if (m_enableTable11 == 1) { updateDataFromTable11(); }
  if (m_enableTable20 == 1) { updateDataFromTable20(); }
  if (m_enableTable21 == 1) { updateDataFromTable21(); }

  updateDataFromTableD0();
  updateDataFromTableD1();
}

std::string ECU::getId() const {
  return m_id;
}

VehicleData_t ECU::getVehicleData() const {
  return m_vehicleData;
}

EngineData_t ECU::getEngineData() const {
  return m_engineData;
}

SensorsData_t ECU::getSensorsData() const {
  return m_sensorsData;
}

ErrorData_t ECU::getErrorData() const {
  return m_errorData;
}

UnknownData_t ECU::getUnknownData() const {
  return m_unknownData;
}

std::optional<CommandResult> ECU::updateDataFromTable(uint8_t table) {
  uint8_t message[5] = {0x72, 0x05, 0x71, table, 0x00};
  message[4] = calcChecksum(message, 4);

  m_protocol->writeData(message, 5);

  return m_protocol->readData();
}

esp_err_t ECU::updateDataFromTable0() {
  auto response = updateDataFromTable(0x0);
  if (response == std::nullopt) { return ESP_FAIL; }
  if (response->len != 0xf) {
    free(response->data);
    return ESP_FAIL;
  }

  //  for (size_t i = 4; i < response->len - 1; i++) {
  //    m_id += std::to_string(static_cast<int>(response->data[i]));
  //  }

  return ESP_OK;
}

esp_err_t ECU::updateDataFromTable10() {
  auto response = updateDataFromTable(0x10);
  if (response == std::nullopt) { return ESP_FAIL; }
  if (response->len != 0x16) {
    free(response->data);
    return ESP_FAIL;
  }

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
  m_engineData.ignitionAdvance = calcValueDivide10(response->data[20]);

  return ESP_OK;
}

esp_err_t ECU::updateDataFromTable11() {
  auto response = updateDataFromTable(0x11);
  if (response == std::nullopt) { return ESP_FAIL; }
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
  m_engineData.ignitionAdvance = calcValueDivide10(response->data[20]);

  m_unknownData.unkData1 = response->data[21];
  m_unknownData.unkData2 = response->data[22];
  m_unknownData.unkData3 = response->data[23];

  return ESP_OK;
}

esp_err_t ECU::updateDataFromTable20() {
  auto response = updateDataFromTable(0x20);
  if (response == std::nullopt) { return ESP_FAIL; }
  if (response->len != 0x8) { return ESP_FAIL; }

  m_unknownData.unkData4 = response->data[4];
  m_unknownData.unkData5 = response->data[5];
  m_unknownData.unkData6 = response->data[6];

  return ESP_OK;
}

esp_err_t ECU::updateDataFromTable21() {
  auto response = updateDataFromTable(0x21);
  if (response == std::nullopt) { return ESP_FAIL; }
  if (response->len != 0xb) { return ESP_FAIL; }

  m_unknownData.unkData1 = response->data[4];
  m_unknownData.unkData2 = response->data[5];
  m_unknownData.unkData3 = response->data[6];
  m_unknownData.unkData4 = response->data[7];
  m_unknownData.unkData5 = response->data[8];
  m_unknownData.unkData6 = response->data[9];

  return ESP_OK;
}

esp_err_t ECU::updateDataFromTableD0() {
  auto response = updateDataFromTable(0xD0);
  if (response == std::nullopt) { return ESP_FAIL; }
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

esp_err_t ECU::updateDataFromTableD1() {
  auto response = updateDataFromTable(0xD1);
  if (response == std::nullopt) { return ESP_FAIL; }
  if (response->len != 0xb) { return ESP_FAIL; }

  m_vehicleData.state = response->data[4];

  m_unknownData.unkData21 = response->data[5];
  m_unknownData.unkData22 = response->data[6];
  m_unknownData.unkData23 = response->data[7];
  m_unknownData.unkData24 = response->data[8];
  m_unknownData.unkData25 = response->data[9];

  return ESP_OK;
}
