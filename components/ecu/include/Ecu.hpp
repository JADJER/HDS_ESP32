//
// Created by jadjer on 24.08.22.
//

#pragma once

#include <memory>

class ECU {
 public:
  ECU();
  ~ECU();

 public:
  bool connect() const;
  void test() const;

 private:
  int m_rx;
  int m_tx;

 private:
  void wakeup() const;
  bool initialize() const;
};
