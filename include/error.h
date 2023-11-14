/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Registers an error to the error module. The error module is used
 * to store errors that occur during the compilation process. And
 * report them into the user's output.
 *
 * @note Can be portable to ensure reported correctly
 *
 * @param error The string of the error message to be registered.
 */

void jasRegisterError(char *error);

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Polls for the last error that has be reported with the
 * `jasRegisterError()` function. Can be used to report errors
 * to the user via your own error reporting functions.
 *
 * @note Only polls for the last error that has occurred in
 * the `errors` array.
 *
 * @note Return value should be freed with `free()`!
 *
 * ~UWU~ Daddy
 */

char *jasPollLastError();

/**
 * @author cheng-alvin
 * @since v0.0.1
 *
 * Prints all the errors that have been registered with the
 * `jasRegisterError()` function. Can be used to report errors
 * quickly without the need of setting up more boilerplate code.
 *
 * @see `jasPollLastError()`
 * @note depends on `jasPollLastError()` to poll for the last
 * error on the "stack". (If you will)
 */

void jasPrintErrors();