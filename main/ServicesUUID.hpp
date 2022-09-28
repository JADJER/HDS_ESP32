// Copyright 2022 Pavel Suprunov
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//
// Created by jadjer on 24.09.22.
//

#pragma once

#include <BLEUUID.h>

static auto serviceAdvertiseUUID = BLEUUID("7def33bd-e4b5-409b-86e8-805f6e882d69");

static auto serviceVehicleUUID = BLEUUID("9d1e62fc-e0a9-4250-8743-ba7be5820622");
static auto vehicleBatteryUUID = BLEUUID("abb0a0d1-08d9-4522-87d8-38c1c3b575f7");
static auto vehicleSpeedUUID = BLEUUID("f6f83d7b-afb3-4715-8d54-9eb7f867b1ae");
static auto vehicleStateUUID = BLEUUID("420b960b-3fca-4b84-a224-2dddd55dc7d8");

static auto serviceEngineUUID = BLEUUID("df874d19-7246-4370-9b4c-a80955191ad4");
static auto engineRpmUUID = BLEUUID("2f4c7890-5477-4803-80a3-46a066310a77");
static auto engineFuelInjectUUID = BLEUUID("ac986e86-034e-4561-8944-148c16712abc");
static auto engineIgnitionAdvanceUUID = BLEUUID("f269fec2-766d-4833-9a9b-0c0ad457cf7c");
static auto engineUnknown1UUID = BLEUUID("5861773c-3d66-44f3-af15-74bea87d2557");
static auto engineUnknown2UUID = BLEUUID("138b4ae7-61ef-4783-bee8-3e8dc1f745b9");
static auto engineUnknown3UUID = BLEUUID("8d0edd85-0774-49bb-8cc5-294164429f50");

static auto serviceSensorsUUID = BLEUUID("d4b4fc17-fde2-45ce-b0cb-2a22910a67f1");
static auto sensorTpsPercentUUID = BLEUUID("519fa904-6022-4247-9a3d-35e72d75f2c6");
static auto sensorTpsVoltageUUID = BLEUUID("acc5be7a-af25-4c38-8ced-2a343eff00fb");
static auto sensorEctTempUUID = BLEUUID("eb2a13ea-1e8c-4c27-9420-4c97b2c9d7d5");
static auto sensorEctVoltageUUID = BLEUUID("0c7dd6fd-dac9-40d8-8a3d-52ffa8d81dc9");
static auto sensorIatTempUUID = BLEUUID("f30cab3f-69ad-440b-b6ce-d632780e8a36");
static auto sensorIatVoltageUUID = BLEUUID("06800dbe-bff0-4101-b679-4c0005c78405");
static auto sensorMapPressureUUID = BLEUUID("390e26f6-3026-4514-92cb-66ad7ff24a6c");
static auto sensorMapVoltageUUID = BLEUUID("2bdc16dd-b921-4b60-82c4-f51fecf3b0f3");

static auto serviceSettingsUUID = BLEUUID("045cc171-7d99-436f-826c-28fad2ef0df9");
static auto settingDistanceUUID = BLEUUID("29bbb961-1de7-414c-931d-10af437ac2bb");
static auto settingDelayUUID = BLEUUID("19f3ba3d-049a-4ea8-82eb-d9a7952cc3c1");

static auto serviceMonitorUUID = BLEUUID("2d0d2cba-85b8-428f-ab5c-fc5d12b22d1a");
static auto monitorOilLevelUUID = BLEUUID("d30e60b4-4097-4b32-bf2b-5cb3b2306b52");

#define SERVICE_UNKNOWN_UUID "af50deb7-2cc5-4436-87e0-ce784f35ae2d"
#define UNKNOWN_DATA_1_UUID "dc639c42-f3c8-4bf2-8d0d-37ea801c9773"
#define UNKNOWN_DATA_2_UUID "3e0ec558-4e1f-4291-ade4-3837cccfe39f"
#define UNKNOWN_DATA_3_UUID "0e732023-f50e-47e2-88a9-5319ea804fe9"
#define UNKNOWN_DATA_4_UUID "80284955-22e5-4c65-8c65-bf95bc5e40eb"
#define UNKNOWN_DATA_5_UUID "1ee703e2-acbc-49ad-ab2e-c43907076f56"
#define UNKNOWN_DATA_6_UUID "190d2417-42a2-4bff-ad82-3f228de92771"
#define UNKNOWN_DATA_7_UUID "2867911e-7b6b-4e0b-a751-e1141f2b2e15"
#define UNKNOWN_DATA_8_UUID "b556d151-1adf-4429-8bfa-4884ebeac07b"
#define UNKNOWN_DATA_9_UUID "12157041-9589-4085-be0e-f50424ab3f9f"
#define UNKNOWN_DATA_10_UUID "dc8168c6-bda5-4f35-8c28-de890d24623f"
#define UNKNOWN_DATA_11_UUID "afdd4e3c-bbda-42c9-b5d6-4b51a7fb7570"
#define UNKNOWN_DATA_12_UUID "0644f5f0-d428-42d8-a2ba-e50c99700de2"
#define UNKNOWN_DATA_13_UUID "83a27dc7-9e2e-42a7-a938-46e930bd55d9"
#define UNKNOWN_DATA_14_UUID "d76dbe80-f26b-4f6b-8e8d-6e0d9cc629dc"
#define UNKNOWN_DATA_15_UUID "6b818f7f-434f-4b21-888c-a12a580e3f29"
#define UNKNOWN_DATA_16_UUID "2466fd5d-89fe-44a2-95de-20b073e33cc2"
#define UNKNOWN_DATA_17_UUID "76bc6808-d689-4c7b-98ed-2ec2ee99cc14"
#define UNKNOWN_DATA_18_UUID "2b8b0862-99c9-4197-a5c8-3e7fed57a3ee"
#define UNKNOWN_DATA_19_UUID "a0d4da5d-dff7-45f7-9304-dfee33484cf1"
#define UNKNOWN_DATA_20_UUID "bfcfa697-b98c-4ce0-9600-47ca4b5a9983"
#define UNKNOWN_DATA_21_UUID "701517b8-1b32-439a-ae84-7a8cab4d51a3"
#define UNKNOWN_DATA_22_UUID "3a8d236c-4c7c-48c9-9ad1-5d62f4c682c0"
#define UNKNOWN_DATA_23_UUID "770311c3-eb13-4b76-8e46-35ee5b761476"
#define UNKNOWN_DATA_24_UUID "402c312f-5ee5-4290-984f-9986db3fc4a4"
#define UNKNOWN_DATA_25_UUID "99d665c1-ce3a-42dc-bbbf-17c09d44c539"
