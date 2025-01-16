# Internal Code Documentation: `cd.c`

[TOC](#toc)

## <a name="toc"></a>Table of Contents

* [1. Overview](#overview)
* [2. Function Behavior](#function-behavior)
* [3. Algorithm Details](#algorithm-details)
* [4.  `CDPATH` Variable Handling](#cdpath-handling)
* [5. Exit Status](#exit-status)


## <a name="overview"></a>1. Overview

The `cd.c` file implements the `cd` command functionality,  handling both absolute and relative path changes, symbolic links (`-L` vs `-P` options), and the `CDPATH` environment variable.  The code aims to emulate the behavior of the standard `cd` command, including its error handling and exit status.

## <a name="function-behavior"></a>2. Function Behavior

The `cd` command changes the current working directory.  The core functionality involves:

* **Handling Absolute and Relative Paths:** The code parses the given path argument and determines whether it's an absolute path (starting with `/`) or a relative path.  Relative paths are resolved relative to the current working directory.

* **Symbolic Link Resolution (`-L` vs `-P`):**  The `-L` option (logical) treats symbolic links as part of the logical directory structure.  The `-P` option (physical) resolves symbolic links and navigates the actual filesystem structure, ignoring the symbolic link itself.

* **`CDPATH` Variable Usage:** If a relative path is provided, the code searches for the path using the `CDPATH` environment variable. If found, the path specified within `CDPATH` is used as the base directory for resolution of the relative path. The `CDPATH` variable functions similar to the `PATH` variable, allowing multiple paths separated by colons.  Leading/trailing colons, and consecutive colons, are handled to represent the current directory.


* **Previous Directory (`cd -`):** The code also handles the special case of `cd -`, changing the current directory to the previous working directory (stored in the `$OLDPWD` environment variable).

* **Variable Expansion:**  If a valid path is not found using `CDPATH` and the `cdable_vars` option is enabled, the code attempts to treat the argument as a variable. If the variable exists and resolves to a valid path, it is used as the target directory.

* **Error Handling:** The code includes error handling for cases where the specified directory does not exist or is inaccessible.

## <a name="algorithm-details"></a>3. Algorithm Details

The algorithm for changing the directory can be summarized as follows:

1. **Parse Arguments:**  The input arguments are parsed to extract the target directory path and any options (`-L`, `-P`, `-e`).

2. **Resolve Path:**
    * **Absolute Path:** If the path is absolute, it's used directly.
    * **Relative Path:**
        * Search for the path using the `CDPATH` environment variable. If found, the path is resolved relative to the corresponding directory specified in `CDPATH`.
        * If not found in `CDPATH` and `cdable_vars` is enabled, the argument is treated as a variable for resolution.
        * If still not found, it's resolved relative to the current working directory.


3. **Handle Symbolic Links:**
    * **`-L`:** Symbolic links are treated as part of the path.
    * **`-P`:** Symbolic links are resolved, and the actual filesystem path is used. If `-e` option is set and the lower-level directory cannot be obtained, exit with a non-zero status code.

4. **Change Directory:** The `chdir()` system call is used to change the current working directory to the resolved path.

5. **Update `$PWD` and `$OLDPWD`:**  The `$PWD` (present working directory) environment variable is updated to reflect the new directory. The `$OLDPWD` variable is updated to store the previous working directory.

## <a name="cdpath-handling"></a>4. `CDPATH` Variable Handling

The `CDPATH` environment variable is a colon-separated list of directories.  The code iterates through each directory in `CDPATH`, attempting to resolve the relative path within each.  Special handling is included for leading/trailing colons and consecutive colons in the `CDPATH` variable to correctly interpret these as representing the current directory.  A `.` in the `CDPATH` also signifies the current directory.

## <a name="exit-status"></a>5. Exit Status

The `cd` command returns:

* **0:** If the directory is changed successfully, and `$PWD` is set successfully when `-P` is used.
* **Non-zero:** Otherwise (e.g., if the directory doesn't exist, is inaccessible, or an error occurs during path resolution).
