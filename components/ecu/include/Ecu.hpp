//
// Created by jadjer on 24.08.22.
//

#pragma once

#include "IConnection.hpp"
#include <memory>

class ECU {
 public:
  explicit ECU(std::shared_ptr<IConnection> const& connection);
  ~ECU();

 public:
  void init() const;

 private:
  int m_rx;
  int m_tx;
  std::shared_ptr<IConnection> m_connection;

 private:
  void wakeup() const;
};
