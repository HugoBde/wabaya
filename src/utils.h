#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include <string>

typedef uint8_t byte;
typedef uint16_t halfword;
typedef uint32_t word;

void log_error(std::string message);
void log_success(std::string message);
void log_warning(std::string message);
void crash();

#endif