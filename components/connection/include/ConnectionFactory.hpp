//
// Created by jadjer on 26.08.22.
//

#pragma once

#include "IConnection.hpp"
#include <memory>

class ConnectionFactory {
 public:
  static std::shared_ptr<IConnection> createConnection();
};
