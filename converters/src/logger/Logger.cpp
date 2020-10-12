#include "./Logger.hpp"

#include <iostream>

static void LOG(std::string message) {
  try {
    std::cout << message << std::endl;
  } catch (...) {
    //
  }
}

static void WARN(std::string message) {
  try {
    std::cout << message << std::endl;
  } catch (...) {
    //
  }
}

static void ERROR(std::string message) {
  try {
    std::cout << message << std::endl;
  } catch (...) {
    //
  }
}