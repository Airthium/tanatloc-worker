#include "./Logger.hpp"

#include <iostream>

void Logger::DEBUG(const std::string &message) {
#if DEBUG
  try {
    std::cout << "\033[1;34m";
    std::cout << message;
    std::cout << "\033[0m" << std::endl;
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
#endif
}

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
    std::cout << "\033[1;33m";
    std::cout << "Warning: ";
    std::cout << message;
    std::cout << "\033[0m" << std::endl;
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}

void Logger::ERROR(const std::string &message) {
  try {
    std::cout << "\033[1;31m";
    std::cout << "Error: ";
    std::cout << message;
    std::cout << "\033[0m" << std::endl;
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}