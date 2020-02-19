#ifndef ACCEL_H
#define ACCEL_H

#include <string>
#include <SDL2/SDL.h>

// TODO what is the "correct" way to do this?
const int max_accel = 20000;

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
