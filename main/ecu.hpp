//
// Created by jadjer on 24.08.22.
//

#pragma once

/*
L9637D Wiring
ESP32       L9637D
------------------
17          TX
16          RX
3V3         VCC
GND         GND
*/

struct TABLE11RESPONSE {

  uint16_t rpm;
  uint8_t speedKPH;

  uint8_t tpsPercent;
  uint8_t tpsVolts;

  float ectTemp;
  uint8_t ectVolts;

  float iatTemp;
  uint8_t iatVolts;

  float battVolts;

  float mapPressure;
  uint8_t mapVolts;
};

struct TABLED1RESPONSE {
  uint8_t engState;
  uint8_t switchState;
};

class ECU {
 public:
  ECU();
  ~ECU();

 public:
  void wakeup() const;
  void setup();

 public:
  bool isConnected() const;

 public:
  TABLE11RESPONSE showDataTable11();
  TABLED1RESPONSE showDataTableD1();

 private:
  bool m_isConnected;
  uint8_t const m_rx;
  uint8_t const m_tx;
  uint8_t const m_bufferSize;

 private:
  void bufferECUResponse();
  void printBuffer(byte* buffer, size_t buffer_size);
};
