# Internal Code Documentation: alias.c

## Table of Contents

* [1. Overview](#1-overview)
* [2. Function Details](#2-function-details)
    * [2.1 `print_help()`](#21-print_help)
    * [2.2 `add_alias()`](#22-add_alias)
    * [2.3 `print_alias()`](#23-print_alias)
    * [2.4 `alias()`](#24-alias)
* [3. Data Structures](#3-data-structures)


## 1. Overview

The `alias.c` file implements the `alias` shell built-in command. This command allows users to define and manage aliases, which are shortcuts for longer commands.  The functionality includes defining new aliases, displaying existing aliases, and providing help information.


## 2. Function Details

### 2.1 `print_help()`

This function prints the help message for the `alias` command to standard output.

| Aspect | Description |
|---|---|
| **Purpose:** | Displays help information for the `alias` command. |
| **Algorithm:** | A statically defined string containing the help message is printed using the `print()` function. |
| **Return Value:** | 0 (success) |
| **Error Handling:** | No explicit error handling; assumes `print()` will handle potential errors. |

### 2.2 `add_alias()`

This function adds a new alias to the shell's alias table.

| Aspect | Description |
|---|---|
| **Purpose:** | Adds a new alias definition. |
| **Algorithm:** | Takes a string argument (`arg`) representing the alias definition (e.g., "NAME=VALUE"). It uses `get_key_value()` to split this string into the alias name (`key`) and value (`value`) based on the `=` character.  It then validates the alias name using `alias_validate()`. If validation fails, it frees allocated memory and returns an error code.  If validation succeeds, it adds the alias using `alias_add()`. Finally, it frees allocated memory and returns a success or failure code. |
| **Parameters:** | `arg` (char*): The alias definition string. |
| **Return Value:** | 0 (success), 1 (failure) |
| **Error Handling:** | Checks for `NULL` input and uses `alias_validate()` for name validation. Memory allocated for `key` and `value` is freed using `sfree()`. |


### 2.3 `print_alias()`

This function prints an alias or an error message if the alias is not found.


| Aspect | Description |
|---|---|
| **Purpose:** | Prints a specific alias or an error message if the alias doesn't exist.  |
| **Algorithm:** | Takes an alias name (`arg`). It searches for the alias using `alias_find()`. If found, it constructs a formatted string using `ft_strjoin_sep()` and appends it to `*values`. If not found, it constructs an error message using `ft_strjoin_sep()` and appends it to `*invalues`. |
| **Parameters:** | `arg` (char*): The name of the alias to print. | `values` (char**): Pointer to a string to store the output of found aliases. | `invalues` (char**): Pointer to a string to store error messages. |
| **Return Value:** | 0 (success) |
| **Error Handling:** | Handles the case where the alias is not found.  The `ft_strjoin_sep` functions likely handle potential memory allocation errors (although this is not explicitly shown in the code). |


### 2.4 `alias()`

This is the main function for the `alias` built-in command.

| Aspect | Description |
|---|---|
| **Purpose:** |  Handles the parsing of options and arguments for the `alias` command, and delegates the work to other helper functions. |
| **Algorithm:** |  It parses command line options using `parse_options()`. Based on the options (`opts`), it either prints the help message (`print_help()`), prints the version (`print_version()`), adds aliases (`add_alias()`), prints aliases (`print_alias()`), or prints all aliases (`alias_print()`).  It manages error handling and memory allocation/deallocation. |
| **Parameters:** | `args` (t_arg*): A structure containing the command-line arguments. |
| **Return Value:** | 0 (success), 1 (failure) |
| **Error Handling:** | Checks for invalid options, handles help and version requests, and checks for errors from `add_alias()`.  It frees memory allocated for the options using `sfree()`. |



## 3. Data Structures

The code uses a custom data structure `t_alias` (not shown, but implied by usage) to represent an alias, likely containing at least `name` and `value` fields.  The code also uses a linked list of arguments (`t_arg`) and options (`t_opt`). The exact structure of these types is not provided in the given code snippet, but they appear to be custom structs used for argument and option parsing.
