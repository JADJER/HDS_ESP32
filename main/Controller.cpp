//
// Created by jadjer on 29.08.22.
//

#include "Controller.hpp"
#include <Arduino.h>
#include <iostream>

Controller::Controller() : m_ble("HDS"), m_ecu(16, 17), m_engine(m_ecu), m_indicator(2) {
  m_ble.createService("d6f3f0a5-e35b-463e-834b-d7a69cb9ba30", {
                                                                  "89d26cc3-0df7-4d3d-aebf-381d595d9395",//ID
                                                                  "96902795-9837-4345-949f-706cdb7b76f6",//RPM
                                                                  "ce914ca7-5b8b-4133-af2f-27a15f7c7d89",//SPEED
                                                                  "41ea8aa3-9541-4c36-949e-45d63a0da48b",//BATTERY
                                                                  "7f237aee-d163-4f32-a6aa-f008b0727e26",
                                                              });
  m_ble.createService("2b3c1105-ff38-4963-b5af-c2631372f9ad", {
                                                                  "a4c6be92-0060-4702-a63a-5d7eb122eac8",//TPS_PERCENT
                                                                  "2b2ee537-9bb5-4cd3-9e44-72122f1c82ca",//TPS_VOLTAGE
                                                              });
  m_ble.createService("7cdd9e93-844f-4313-b500-91e8a374c8a7", {
                                                                  "ddbea267-80cb-4bc3-992a-159bb45b32d2",//ECT_TEMP
                                                                  "3bf76097-4b24-428a-81a2-bb2f073e273e",//ECT_VOLTAGE
                                                              });
  m_ble.createService("1311712d-1ef6-4161-be8e-6b60aab4c643", {
                                                                  "6b9679b7-3fc4-4f30-ae75-df2f2179dfca",//IAT_TEMP
                                                                  "63c6853f-1d91-49eb-bbe5-ec4a05174bdc",//IAT_VOLTAGE
                                                              });
  m_ble.createService("5ed5a50d-7b41-4bd5-a937-dbd58b5e469f", {
                                                                  "96f9b873-6c05-42d6-8245-06abffe4abfb",//MAP_PRESSURE
                                                                  "b7df0786-d4c3-43d9-a1ff-ba98af91ce52",//MAP_VOLTAGE
                                                              });
  m_ble.createService("248bb324-f9f0-4e81-a0eb-78e046ee2ffa", {
                                                                  "ee2880a1-0ef8-4448-a488-bc44414de6f9",//FUEL_INJECT
                                                                  "85ca5a88-5980-40ab-be6c-16b26ae9d020",//IGNITION_ANGLE
                                                                  "7cb4a1b8-c692-49c7-b7f3-4eb940d5d2f0",//UNK1
                                                                  "0ddc7e88-c531-46a9-9183-1222db4a6875",//UNK2
                                                                  "9fdd5c0a-9d9b-42ff-9bfa-fe5e03861b45",//UNK3
                                                              });

  m_indicator.blink(500);

  m_ecuConnected = m_ecu.connect();
  if (not m_ecuConnected) {
    std::cout << "ECU connection failed" << std::endl;
    m_indicator.enable(false);
    return;
  }

  m_indicator.blink(100);

  m_engine.test();
  m_engine.detectActiveTables();

  m_ble.start();
  m_ble.advertising();
}

Controller::~Controller() = default;

[[noreturn]] void Controller::spin() {
  while (true) {
    if (m_ecuConnected) {
      m_indicator.blink(2000);
      spinOnce();
    }
    delay(1);
  }
}

void Controller::spinOnce() {
  m_engine.spinOnce();

  m_ble.setValueString("89d26cc3-0df7-4d3d-aebf-381d595d9395", m_engine.getId());

  m_ble.setValueString("96902795-9837-4345-949f-706cdb7b76f6", std::to_string(m_engine.getRpm()));
  m_ble.setValueString("ce914ca7-5b8b-4133-af2f-27a15f7c7d89", std::to_string(m_engine.getSpeed()));
  m_ble.setValueString("41ea8aa3-9541-4c36-949e-45d63a0da48b", std::to_string(m_engine.getBatteryVolts()));
  m_ble.setValueString("7f237aee-d163-4f32-a6aa-f008b0727e26", std::to_string(m_engine.getState()));

  m_ble.setValueString("a4c6be92-0060-4702-a63a-5d7eb122eac8", std::to_string(m_engine.getTpsPercent()));
  m_ble.setValueString("2b2ee537-9bb5-4cd3-9e44-72122f1c82ca", std::to_string(m_engine.getTpsVolts()));

  m_ble.setValueString("ddbea267-80cb-4bc3-992a-159bb45b32d2", std::to_string(m_engine.getEctTemp()));
  m_ble.setValueString("3bf76097-4b24-428a-81a2-bb2f073e273e", std::to_string(m_engine.getEctVolts()));

  m_ble.setValueString("6b9679b7-3fc4-4f30-ae75-df2f2179dfca", std::to_string(m_engine.getIatTemp()));
  m_ble.setValueString("63c6853f-1d91-49eb-bbe5-ec4a05174bdc", std::to_string(m_engine.getIatVolts()));

  m_ble.setValueString("96f9b873-6c05-42d6-8245-06abffe4abfb", std::to_string(m_engine.getMapPressure()));
  m_ble.setValueString("b7df0786-d4c3-43d9-a1ff-ba98af91ce52", std::to_string(m_engine.getMapVolts()));

  m_ble.setValueString("ee2880a1-0ef8-4448-a488-bc44414de6f9", std::to_string(m_engine.getFuelInject()));
  m_ble.setValueString("85ca5a88-5980-40ab-be6c-16b26ae9d020", std::to_string(m_engine.getIgnitionAngle()));
  m_ble.setValueString("7cb4a1b8-c692-49c7-b7f3-4eb940d5d2f0", std::to_string(m_engine.getUnknown1()));
  m_ble.setValueString("0ddc7e88-c531-46a9-9183-1222db4a6875", std::to_string(m_engine.getUnknown2()));
  m_ble.setValueString("9fdd5c0a-9d9b-42ff-9bfa-fe5e03861b45", std::to_string(m_engine.getUnknown3()));
}
