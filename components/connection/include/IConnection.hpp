//
// Created by jadjer on 26.08.22.
//

#pragma once

#include <string>
#include <vector>

class IConnection {
 public:
  virtual ~IConnection() = default;

 public:
  virtual std::vector<uint8_t> readData(std::string const& logName) = 0;
  virtual int writeString(std::string const& logName, std::string const& data) = 0;
  virtual int writeData(std::string const& logName, std::vector<uint8_t> const& data) = 0;
};
