#include "accel.h"
#include <iostream>
#include <fstream>
#include <sstream>

#ifdef DEBUG
#define TEST_FILE "test.txt"
#endif

static std::string accel_dir = "/sys/bus/iio/devices/iio:device2/";

// TODO does this really belong here?
inline int read_int_file(const std::string &path) {
  std::stringstream buf;
  std::ifstream input(path);
  buf << input.rdbuf();
  return std::stoi(buf.str());
}

// TODO this implementation might not be *correct*.
Uint8 scale_accel(int accel) {
  // 0 <= magnitude <= 1, nominally
  float magnitude = ((float)(accel + max_accel) / max_accel);
  return (Uint8) (magnitude * UINT8_MAX);
}

int in_accel_x_raw() {
#ifdef DEBUG
  return read_int_file(TEST_FILE);
#else
  return read_int_file(accel_dir + "in_accel_x_raw");
#endif
}

int in_accel_y_raw() {
#ifdef DEBUG
  return read_int_file(TEST_FILE);
#else
  return read_int_file(accel_dir + "in_accel_y_raw");
#endif
}

int in_accel_z_raw() {
#ifdef DEBUG
  return read_int_file(TEST_FILE);
#else
  return read_int_file(accel_dir + "in_accel_z_raw");
#endif
}
