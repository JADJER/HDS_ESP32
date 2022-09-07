//
// Created by jadjer on 29.08.22.
//

#include "Controller.hpp"
#include <Arduino.h>
#include <iostream>

Controller::Controller() : m_ble("HDS"), m_communication(16, 17), m_ecu(m_communication), m_indicator(2) {
  m_ble.createServiceNotify("9d1e62fc-e0a9-4250-8743-ba7be5820622", {
                                                                        "abb0a0d1-08d9-4522-87d8-38c1c3b575f7",
                                                                        "f6f83d7b-afb3-4715-8d54-9eb7f867b1ae",
                                                                        "420b960b-3fca-4b84-a224-2dddd55dc7d8",
                                                                    });

  m_ble.createServiceNotify("df874d19-7246-4370-9b4c-a80955191ad4", {
                                                                        "2f4c7890-5477-4803-80a3-46a066310a77",
                                                                        "ac986e86-034e-4561-8944-148c16712abc",
                                                                        "f269fec2-766d-4833-9a9b-0c0ad457cf7c",
                                                                        "5861773c-3d66-44f3-af15-74bea87d2557",
                                                                        "138b4ae7-61ef-4783-bee8-3e8dc1f745b9",
                                                                        "8d0edd85-0774-49bb-8cc5-294164429f50",
                                                                    });

  m_ble.createServiceNotify("d4b4fc17-fde2-45ce-b0cb-2a22910a67f1", {
                                                                        "519fa904-6022-4247-9a3d-35e72d75f2c6",
                                                                        "acc5be7a-af25-4c38-8ced-2a343eff00fb",
                                                                        "eb2a13ea-1e8c-4c27-9420-4c97b2c9d7d5",
                                                                        "0c7dd6fd-dac9-40d8-8a3d-52ffa8d81dc9",
                                                                        "f30cab3f-69ad-440b-b6ce-d632780e8a36",
                                                                        "06800dbe-bff0-4101-b679-4c0005c78405",
                                                                        "390e26f6-3026-4514-92cb-66ad7ff24a6c",
                                                                        "2bdc16dd-b921-4b60-82c4-f51fecf3b0f3",
                                                                    });

  m_ble.createService("af50deb7-2cc5-4436-87e0-ce784f35ae2d", {
                                                                  "dc639c42-f3c8-4bf2-8d0d-37ea801c9773",
                                                                  "3e0ec558-4e1f-4291-ade4-3837cccfe39f",
                                                                  "0e732023-f50e-47e2-88a9-5319ea804fe9",
                                                                  "80284955-22e5-4c65-8c65-bf95bc5e40eb",
                                                                  "1ee703e2-acbc-49ad-ab2e-c43907076f56",
                                                                  "190d2417-42a2-4bff-ad82-3f228de92771",
                                                                  "2867911e-7b6b-4e0b-a751-e1141f2b2e15",
                                                                  "b556d151-1adf-4429-8bfa-4884ebeac07b",
                                                                  "12157041-9589-4085-be0e-f50424ab3f9f",
                                                                  "dc8168c6-bda5-4f35-8c28-de890d24623f",
                                                                  "afdd4e3c-bbda-42c9-b5d6-4b51a7fb7570",
                                                                  "0644f5f0-d428-42d8-a2ba-e50c99700de2",
                                                                  "83a27dc7-9e2e-42a7-a938-46e930bd55d9",
                                                                  "d76dbe80-f26b-4f6b-8e8d-6e0d9cc629dc",
                                                                  "6b818f7f-434f-4b21-888c-a12a580e3f29",
                                                                  "2466fd5d-89fe-44a2-95de-20b073e33cc2",
                                                                  "76bc6808-d689-4c7b-98ed-2ec2ee99cc14",
                                                                  "2b8b0862-99c9-4197-a5c8-3e7fed57a3ee",
                                                                  "a0d4da5d-dff7-45f7-9304-dfee33484cf1",
                                                                  "bfcfa697-b98c-4ce0-9600-47ca4b5a9983",
                                                                  "701517b8-1b32-439a-ae84-7a8cab4d51a3",
                                                                  "3a8d236c-4c7c-48c9-9ad1-5d62f4c682c0",
                                                                  "770311c3-eb13-4b76-8e46-35ee5b761476",
                                                                  "402c312f-5ee5-4290-984f-9986db3fc4a4",
                                                                  "99d665c1-ce3a-42dc-bbbf-17c09d44c539",
                                                              });

  m_indicator.blink(500);

  m_ecuConnected = m_communication.connect();
  if (not m_ecuConnected) {
    std::cout << "ECU connection failed" << std::endl;
    m_indicator.enable(false);
    return;
  }

  m_indicator.blink(100);

  m_ecu.test();
  m_ecu.detectActiveTables();

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
  m_ecu.spinOnce();

  auto vehicleData = m_ecu.getVehicleData();
  m_ble.setValueString("abb0a0d1-08d9-4522-87d8-38c1c3b575f7", std::to_string(vehicleData.batteryVolts));
  m_ble.setValueString("f6f83d7b-afb3-4715-8d54-9eb7f867b1ae", std::to_string(vehicleData.speed));
  m_ble.setValueString("420b960b-3fca-4b84-a224-2dddd55dc7d8", std::to_string(vehicleData.state));

  auto engineData = m_ecu.getEngineData();
  m_ble.setValueString("2f4c7890-5477-4803-80a3-46a066310a77", std::to_string(engineData.rpm));
  m_ble.setValueString("ac986e86-034e-4561-8944-148c16712abc", std::to_string(engineData.fuelInject));
  m_ble.setValueString("f269fec2-766d-4833-9a9b-0c0ad457cf7c", std::to_string(engineData.ignitionAngle));
  m_ble.setValueString("5861773c-3d66-44f3-af15-74bea87d2557", std::to_string(engineData.unkData1));
  m_ble.setValueString("138b4ae7-61ef-4783-bee8-3e8dc1f745b9", std::to_string(engineData.unkData2));
  m_ble.setValueString("8d0edd85-0774-49bb-8cc5-294164429f50", std::to_string(engineData.unkData3));

  auto sensorsData = m_ecu.getSensorsData();
  m_ble.setValueString("519fa904-6022-4247-9a3d-35e72d75f2c6", std::to_string(sensorsData.tpsPercent));
  m_ble.setValueString("acc5be7a-af25-4c38-8ced-2a343eff00fb", std::to_string(sensorsData.tpsVolts));
  m_ble.setValueString("eb2a13ea-1e8c-4c27-9420-4c97b2c9d7d5", std::to_string(sensorsData.iatTemp));
  m_ble.setValueString("0c7dd6fd-dac9-40d8-8a3d-52ffa8d81dc9", std::to_string(sensorsData.iatVolts));
  m_ble.setValueString("f30cab3f-69ad-440b-b6ce-d632780e8a36", std::to_string(sensorsData.ectTemp));
  m_ble.setValueString("06800dbe-bff0-4101-b679-4c0005c78405", std::to_string(sensorsData.ectVolts));
  m_ble.setValueString("390e26f6-3026-4514-92cb-66ad7ff24a6c", std::to_string(sensorsData.mapPressure));
  m_ble.setValueString("2bdc16dd-b921-4b60-82c4-f51fecf3b0f3", std::to_string(sensorsData.mapVolts));

  auto errorData = m_ecu.getErrorData();

  auto unknownData = m_ecu.getUnknownData();
  m_ble.setValueString("dc639c42-f3c8-4bf2-8d0d-37ea801c9773", std::to_string(unknownData.unkData1));
  m_ble.setValueString("3e0ec558-4e1f-4291-ade4-3837cccfe39f", std::to_string(unknownData.unkData2));
  m_ble.setValueString("0e732023-f50e-47e2-88a9-5319ea804fe9", std::to_string(unknownData.unkData3));
  m_ble.setValueString("80284955-22e5-4c65-8c65-bf95bc5e40eb", std::to_string(unknownData.unkData4));
  m_ble.setValueString("1ee703e2-acbc-49ad-ab2e-c43907076f56", std::to_string(unknownData.unkData5));
  m_ble.setValueString("190d2417-42a2-4bff-ad82-3f228de92771", std::to_string(unknownData.unkData6));
  m_ble.setValueString("2867911e-7b6b-4e0b-a751-e1141f2b2e15", std::to_string(unknownData.unkData7));
  m_ble.setValueString("b556d151-1adf-4429-8bfa-4884ebeac07b", std::to_string(unknownData.unkData8));
  m_ble.setValueString("12157041-9589-4085-be0e-f50424ab3f9f", std::to_string(unknownData.unkData9));
  m_ble.setValueString("dc8168c6-bda5-4f35-8c28-de890d24623f", std::to_string(unknownData.unkData10));
  m_ble.setValueString("afdd4e3c-bbda-42c9-b5d6-4b51a7fb7570", std::to_string(unknownData.unkData11));
  m_ble.setValueString("0644f5f0-d428-42d8-a2ba-e50c99700de2", std::to_string(unknownData.unkData12));
  m_ble.setValueString("83a27dc7-9e2e-42a7-a938-46e930bd55d9", std::to_string(unknownData.unkData13));
  m_ble.setValueString("d76dbe80-f26b-4f6b-8e8d-6e0d9cc629dc", std::to_string(unknownData.unkData14));
  m_ble.setValueString("6b818f7f-434f-4b21-888c-a12a580e3f29", std::to_string(unknownData.unkData15));
  m_ble.setValueString("2466fd5d-89fe-44a2-95de-20b073e33cc2", std::to_string(unknownData.unkData16));
  m_ble.setValueString("76bc6808-d689-4c7b-98ed-2ec2ee99cc14", std::to_string(unknownData.unkData17));
  m_ble.setValueString("2b8b0862-99c9-4197-a5c8-3e7fed57a3ee", std::to_string(unknownData.unkData18));
  m_ble.setValueString("a0d4da5d-dff7-45f7-9304-dfee33484cf1", std::to_string(unknownData.unkData19));
  m_ble.setValueString("bfcfa697-b98c-4ce0-9600-47ca4b5a9983", std::to_string(unknownData.unkData20));
  m_ble.setValueString("701517b8-1b32-439a-ae84-7a8cab4d51a3", std::to_string(unknownData.unkData21));
  m_ble.setValueString("3a8d236c-4c7c-48c9-9ad1-5d62f4c682c0", std::to_string(unknownData.unkData22));
  m_ble.setValueString("770311c3-eb13-4b76-8e46-35ee5b761476", std::to_string(unknownData.unkData23));
  m_ble.setValueString("402c312f-5ee5-4290-984f-9986db3fc4a4", std::to_string(unknownData.unkData24));
  m_ble.setValueString("99d665c1-ce3a-42dc-bbbf-17c09d44c539", std::to_string(unknownData.unkData25));
}
