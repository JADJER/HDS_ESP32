//
// Created by jadjer on 01.09.22.
//

#pragma once

#include "CommandResult.hpp"
#include "IProtocol.hpp"
#include "data/engine_data.h"
#include "data/error_data.h"
#include "data/sensors_data.h"
#include "data/unknown_data.h"
#include "data/vehicle_data.h"
#include <esp_err.h>
#include <memory>
#include <mutex>
#include <optional>
#include <string>

class ECU {
 public:
  explicit ECU(IProtocol* protocol);
  ~ECU();

 public:
  /**
 * @brief
 * @return
 */
  esp_err_t connect();

  /**
 * @brief
 */
  void detectAllTables();

  /**
 * @brief
 */
  void detectActiveTables();

  /**
 * @brief
 */
  void updateAllData();

  /**
 * @brief
 * @return
 */
  [[nodiscard]] std::string getId() const;

  /**
 * @brief
 * @return
 */
  [[nodiscard]] VehicleData_t getVehicleData() const;

  /**
 * @brief
 * @return
 */
  [[nodiscard]] EngineData_t getEngineData() const;

  /**
 * @brief
 * @return
 */
  [[nodiscard]] SensorsData_t getSensorsData() const;

  /**
 * @brief
 * @return
 */
  [[nodiscard]] ErrorData_t getErrorData() const;

  /**
 * @brief
 * @return
 */
  [[nodiscard]] UnknownData_t getUnknownData() const;

 private:
  std::string m_id;
  VehicleData_t m_vehicleData;
  EngineData_t m_engineData;
  SensorsData_t m_sensorsData;
  ErrorData_t m_errorData;
  UnknownData_t m_unknownData;
  bool m_enableTable10;
  bool m_enableTable11;
  bool m_enableTable20;
  bool m_enableTable21;
  std::mutex m_mutex;
  bool m_isInitialised;
  IProtocol* m_protocol;

 private:
  std::optional<CommandResult> updateDataFromTable(uint8_t table);
  esp_err_t updateDataFromTable0();
  esp_err_t updateDataFromTable10();
  esp_err_t updateDataFromTable11();
  esp_err_t updateDataFromTable20();
  esp_err_t updateDataFromTable21();
  esp_err_t updateDataFromTableD0();
  esp_err_t updateDataFromTableD1();
};
