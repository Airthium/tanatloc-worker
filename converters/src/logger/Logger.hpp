#ifndef _LOGGER_
#define _LOGGER_

#include <string>

class Logger {
public:
  static void LOG(std::string);
  static void WARN(std::string);
  static void ERROR(std::string);
};

#endif