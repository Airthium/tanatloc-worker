#include <catch2/catch.hpp>

#include "../../src/logger/Logger.hpp"

TEST_CASE("LOG") {
  INFO("Logger::LOG");
  Logger::LOG("log");
}

TEST_CASE("DISP") {
  INFO("Logger::DISP");
  Logger::DISP("info");
}

TEST_CASE("WARNING") {
  INFO("Logger::WARNING");
  Logger::WARNING("warning");
}

TEST_CASE("ERROR") {
  INFO("Logger::ERROR");
  Logger::ERROR("error");
}