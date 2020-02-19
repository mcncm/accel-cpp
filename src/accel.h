#ifndef ACCEL_H
#define ACCEL_H

#include <string>
#include <SDL2/SDL.h>

static std::string accel_dir = "/sys/bus/iio/devices/iio:device2/";

// TODO what is the "correct" way to do this?
/** Invensense MPU-6050
 * Range and sensitivity
 * Gyro FSR    Accel FSR (g)    Accel sensitivity (LSB/g)
 * +- 250      +- 2             16384
 * +- 500      +- 4             8192
 * +- 1000     +- 8             4096
 * +- 2000     +- 16            2048
 *
 * Output data rate
 * Min (Hz)     Max (Hz)
 * 4            8000
 *
 */
const int lsb_per_g = 16384;
// +/- maximum acceleration on each axis in g
const int range_g = 2;
// +/- maximum acceleration on each axis in native units
const int max_accel = lsb_per_g * range_g;

/**
 * Read the contents of a file containing a single integer, and
 * return that integer.
 * @param path The file path to read from
 * @return Integer in the file
 */
inline int read_int_file(const std::string &path);

/**
 * Scale an acceleration to [0, 255] e.g. for rendering as color channel.
 * @param accel The input acceleration
 * @return The acceleration, scaled to an unsigned integer.
 */
Uint8 scale_accel(int accel);

/**
 * Read the raw x acceleration
 */
int in_accel_x_raw();

/**
 * Read the raw y acceleration
 */
int in_accel_y_raw();

/**
 * Read the raw z acceleration
 */
int in_accel_z_raw();

#endif
