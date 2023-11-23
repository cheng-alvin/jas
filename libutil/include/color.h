#ifndef COLOR_H
#define COLOR_H

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Color codes for the ASCII escape sequences. Used for
 * reporting and outputting error messages to the terminal.
 *
 * @note Terminal must support ASCII escape!
 */

#define RESET "\033[0m"

#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define PURPLE "\033[38;5;129m"
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"

#endif
