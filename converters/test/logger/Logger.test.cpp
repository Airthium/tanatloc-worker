#include <catch2/catch.hpp>

#include "../../src/logger/Logger.hpp"

TEST_CASE("Logger") {
  SECTION("LOG") {
    INFO("Logger::LOG");
    Logger::LOG("log");
  }

  SECTION("DISP") {
    INFO("Logger::DISP");
    Logger::DISP("info");
  }

  SECTION("WARNING") {
    INFO("Logger::WARNING");
    Logger::WARNING("warning");
  }

  SECTION("ERROR") {
    INFO("Logger::ERROR");
    Logger::ERROR("error");
  }
}