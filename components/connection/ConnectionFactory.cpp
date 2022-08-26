//
// Created by jadjer on 26.08.22.
//

#include "ConnectionFactory.hpp"
#include "Connection.hpp"

std::shared_ptr<IConnection> ConnectionFactory::createConnection() {
  return std::make_shared<Connection>();
}
