#include <catch2/catch.hpp>

#include "../../src/logger/Logger.hpp"

TEST_CASE("LOG") { Logger::LOG("log"); }

TEST_CASE("WARNING") { Logger::WARNING("warning"); }

TEST_CASE("ERROR") { Logger::ERROR("error"); }