# Internal Code Documentation: `command.c`

## Table of Contents

* [1. Overview](#1-overview)
* [2. Function Details](#2-function-details)
    * [2.1 `print_help()`](#21-print_help)
    * [2.2 `check_alias()`](#22-check_alias)
    * [2.3 `check_builtin()`](#23-check_builtin)
    * [2.4 `check_function()`](#24-check_function)
    * [2.5 `check_command()`](#25-check_command)
    * [2.6 `command()`](#26-command)


<a name="1-overview"></a>
## 1. Overview

The `command.c` file implements the `command` builtin for the 42sh shell. This builtin allows users to execute simple commands, suppressing shell function lookup if necessary, and provides information about commands using various options.  The core functionality involves checking for aliases, builtins, functions, and finally, searching the filesystem for the command.


<a name="2-function-details"></a>
## 2. Function Details

<a name="21-print_help"></a>
### 2.1 `print_help()`

This function prints the help message for the `command` builtin to standard output.  The message includes descriptions of the command's functionality, available options (`-p`, `-v`, `-V`), and exit status.

```c
static int print_help() {
    char *msg =
        "command: command [-pVv] command [arg ...]\n"
        "    Execute a simple command or display information about commands.\n\n"
        // ... (rest of the help message) ...
    ;
    print(STDOUT_FILENO, msg, RESET_PRINT);
    return (0);
}
```

<a name="22-check_alias"></a>
### 2.2 `check_alias(char *arg)`

This function checks if the given argument `arg` is an alias. It uses the `alias_find()` function (defined elsewhere) to search the alias table. If found, it prints the alias name and its value to standard output and returns 1; otherwise, it returns 0.

```c
static int check_alias(char *arg) {
    if (!arg) return (0);
    t_alias *alias = alias_find(arg);
    if (alias) {
        print(STDOUT_FILENO, alias->name, JOIN);
        print(STDOUT_FILENO, ft_strjoin_sep(" is aliased to `", alias->value, "'\\n", 0), FREE_JOIN);
        return (1);
    }
    return (0);
}
```

<a name="23-check_builtin"></a>
### 2.3 `check_builtin(char *arg)`

This function checks if the given argument `arg` is a built-in command.  It first checks if it's an alias (to avoid conflicts) and then uses `builtin_find()` (defined elsewhere) to search the built-in command table. If found and not disabled, it prints the builtin's name and returns 1; otherwise, it returns 0.

```c
static int check_builtin(char *arg) {
    if (!arg) return (0);
    if (alias_find(arg)) return (0);
    t_builtin *builtin = builtin_find(arg);
    if (builtin && !builtin->disabled) {
        print(STDOUT_FILENO, ft_strjoin(builtin->name, "\\n", 0), FREE_JOIN);
        return (1);
    }
    return (0);
}
```

<a name="24-check_function"></a>
### 2.4 `check_function(char *arg)`

This function checks if the given argument `arg` is a shell function.  It returns 0.  The commented-out code suggests previous functionality that was removed.


```c
static int check_function(char *arg) {
    if (!arg) return (0);

    if (alias_find(arg) || builtin_isactive(arg)) return (0);

    return (0);
}
```

<a name="25-check_command"></a>
### 2.5 `check_command(char *arg, char *opts)`

This function checks if the given argument `arg` is a command by searching the filesystem.  It uses `path_find_first()` (defined elsewhere) to search the PATH environment variable (or a default PATH if the `-p` option is given). If found, it prints the path and returns 1; otherwise, it returns 0.


```c
static int check_command(char *arg, char *opts) {
    if (!arg) return (0);
    if (alias_find(arg) || builtin_isactive(arg)) return (0);
    char *path = path_find_first(arg, ft_strchr(opts, 'p') ? PATH : NULL);
    if (path) {
        print(STDOUT_FILENO, path, JOIN);
        return (sfree(path), 1);
    }
    return (sfree(path), 0);
}
```

<a name="26-command"></a>
### 2.6 `command(t_arg *args)`

This is the main function of the `command` builtin. It parses options using `parse_options()` (defined elsewhere), handles help and version requests, and then determines the type of command based on the provided arguments and options (`-v`, `-V`, `-p`).

The core logic is a loop that iterates through the arguments, using the helper functions (`check_alias`, `check_builtin`, `check_function`, `check_command`) to identify each argument's type.  The exit status is carefully managed to reflect success or failure. The `-v` option provides concise information, while `-V` provides more verbose details.  The `-p` option forces the use of a default PATH. If no options are provided, the command is executed using the path found.


```c
int command(t_arg *args) {
    t_opt *opts = parse_options(args, "pVv", '-', false);

    // ... (Option parsing and error handling) ...

    if (ft_strchr(opts->valid, 'V')) {
        // ... (Verbose type information using 'type' function) ...
    } else if (ft_strchr(opts->valid, 'v')) {
        // ... (Concise type information loop) ...
    } else {
        // ... (Execute command using default path if '-p' is set) ...
    }

    return (sfree(opts), result); //Return value reflects success/failure
}
```

The commented-out sections in the code represent either removed or potential future features.  The code uses a series of helper functions to modularize its functionality, improving readability and maintainability.  Error handling is implemented throughout, checking for invalid arguments and options.  Memory management is also addressed using functions like `sfree()`.
