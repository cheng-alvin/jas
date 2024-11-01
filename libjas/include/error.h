/**
 * MIT License
 * Copyright (c) 2023-2024 Alvin Cheng (eventide1029@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @see `LICENSE`
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ERROR_H
#define ERROR_H

/**
 * Type wrapper for a void dictated function pointer that
 * takes a const char pointer as an argument; Which represents
 * the function that would be used to handle errors in the jas
 * library.
 *
 * This function takes in an error message as a string and passes
 * it into the handler from the `err()` function.
 *
 * @param msg A string that represents the error message.
 */
typedef void (*err_callback_t)(const char *msg);

/**
 * Function for throwing an error into this "error pipeline", and
 * finally hand-balling it into the user-defined error callback,
 * registered using `err_add_callback`. (For the caller to handle
 * the error accordingly)
 *
 * @param msg The error message to be passed down to the callback.
 *
 * @see `err_callback_t` For error callback type information/formats
 * @see `err_add_callback`
 */
void err(const char *msg);

/**
 * Function to add a callback function to the error handler, a bit like
 * a setter function for the very *object orientated* C language :)
 *
 * @param input The function pointer to the error handler.
 * @see `err_callback_t`
 */
void err_add_callback(err_callback_t input);

#endif

#ifdef __cplusplus
}
#endif
