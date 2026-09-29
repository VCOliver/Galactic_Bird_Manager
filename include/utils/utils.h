#ifndef UTILS_H
#define UTILS_H

/**
 * @brief Reads a single character from standard input without line buffering.
 *
 * Temporarily disables terminal canonical mode (line buffering) to allow
 * immediate character capture without requiring the user to press Enter.
 * Gracefully falls back to standard buffered input if stdin is not a terminal.
 *
 * @return The character read as an int, or EOF on error/end-of-file.
 */
int read_char(void);

#endif // UTILS_H
