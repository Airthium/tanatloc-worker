#include "./Logger.hpp"

#include <iostream>

void Logger::LOG(const std::string &message) {
  try {
    std::cout << message << std::endl;
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}

void Logger::DISP(const std::string &message) {
  try {
    std::cout << message << std::endl;
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}

void Logger::WARNING(const std::string &message) {
  try {
    std::cout << message << std::endl;
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}

void Logger::ERROR(const std::string &message) {
  try {
    std::cout << message << std::endl;
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}