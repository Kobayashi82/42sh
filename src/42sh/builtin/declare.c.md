# Internal Code Documentation: `declare.c`

## Table of Contents

* [1. Overview](#1-overview)
* [2. Function Details](#2-function-details)
    * [2.1 `print_help()`](#21-print_help)
    * [2.2 `add_declare()`](#22-add_declare)
    * [2.3 `delete_declare()`](#23-delete_declare)
    * [2.4 `declare()`](#24-declare)


## 1. Overview

The `declare.c` file implements the `declare` builtin command for managing shell variables.  This command allows users to create, modify, and display shell variables, along with their attributes (integer, readonly, export, lowercase, uppercase).  The command supports various options to control variable creation and manipulation.


## 2. Function Details

### 2.1 `print_help()`

This function displays the help message for the `declare` command.  It prints a formatted string containing the command's synopsis, options, and exit status information to the standard output.

| Element          | Description                                                                   |
|-----------------|-------------------------------------------------------------------------------|
| `msg`            | A string containing the formatted help message.                               |
| `print(STDOUT_FILENO, msg, RESET_PRINT)` | Prints the help message to standard output with formatting reset.        |
| Return Value    | 0 (success)                                                                    |


### 2.2 `add_declare()`

This function handles the addition or modification of variables. It processes a single command-line argument (`arg`), which can either be a variable name (for creation) or a variable name with an assignment (e.g., `name=value`). The function determines if the assignment is a concatenation based on a trailing '+' sign. It then uses the `variables_validate`, `variables_find`, `variables_add`, and `variables_concatenate` functions to perform the necessary operations. Readonly variables cannot be modified.

| Parameter | Description                                                                        |
|-----------|------------------------------------------------------------------------------------|
| `arg`      | The command-line argument (variable name or name=value pair).                       |
| `key`      | The variable name extracted from `arg`.                                          |
| `value`    | The variable value extracted from `arg` (NULL if not provided).                   |
| `concatenate` | Boolean indicating whether to concatenate the new value to an existing value.     |
| `result`   | Integer indicating success (0) or failure (1).                                  |
| Return Value | 0 (success), 1 (failure)                                                         |

**Algorithm:**

1. Extract the key (variable name) and value from the input string using `get_key_value`.
2. Check for a trailing '+' in the key; if present, set the `concatenate` flag and remove the '+'.
3. Validate the key and value using `variables_validate`. If validation fails return an error.
4. If the variable exists and is readonly, print an error and return.
5. If `concatenate` is true, call `variables_concatenate` to append to the existing value.
6. Otherwise, call `variables_add` to add or update the variable.
7. Free the allocated memory for `key` and `value`.
8. Return the result of the variable operation.


### 2.3 `delete_declare()`

This function handles the removal of attributes from existing variables or the setting of variables to unexported. It operates similarly to `add_declare`, but instead modifies existing attributes.  Readonly variables cannot be modified, and an error is reported if attempted.

| Parameter | Description                                                                        |
|-----------|------------------------------------------------------------------------------------|
| `arg`      | The command-line argument (variable name or name=value pair).                       |
| `key`      | The variable name extracted from `arg`.                                          |
| `value`    | The variable value extracted from `arg` (NULL if not provided).                   |
| `concatenate` | Boolean indicating whether to concatenate the new value to an existing value.     |
| `result`   | Integer indicating success (0) or failure (1).                                  |
| Return Value | 0 (success), 1 (failure)                                                         |

**Algorithm:**

1. Extract the key (variable name) and value from the input string using `get_key_value`.
2. Check for a trailing '+' in the key; if present, set the `concatenate` flag and remove the '+'.
3. Validate the key and value using `variables_validate`. If validation fails return an error.
4. If the variable exists and is readonly, print an error and return.
5. If `concatenate` is true, call `variables_concatenate` to remove the value.
6. Otherwise, call `variables_add` to remove attributes.
7. Free the allocated memory for `key` and `value`.
8. Return the result of the variable operation.


### 2.4 `declare()`

This is the main function for the `declare` builtin. It parses command-line arguments using `parse_options`, differentiating between options with '-' (add attributes) and '+' (remove attributes). It then processes the arguments based on the parsed options, calling `add_declare` or `delete_declare` accordingly.  It handles help and version requests and gracefully handles the case where no arguments are provided.

| Parameter | Description                                            |
|-----------|--------------------------------------------------------|
| `args`     | A structure containing command-line arguments.        |
| `opts`     | Parsed options with '-' prefix.                        |
| `opts_plus`| Parsed options with '+' prefix.                        |
| `result`   | Integer indicating success (0) or failure (1).        |
| Return Value | 0 (success), 1 (failure)                              |


**Algorithm:**

1. Parse command-line arguments using `parse_options` to separate options with '-' and '+'.
2. Handle invalid options by calling `invalid_option` and return an error.
3. Handle `-?` or `-#` options to print help or version information respectively.
4. If no arguments are provided, print all variables and return.
5. Iterate through the arguments and call either `add_declare` or `delete_declare` depending on the presence of 'n' in `opts->valid`.
6. Free the allocated memory for options structure.
7. Return the final result.


