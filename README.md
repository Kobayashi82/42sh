
<div align="center">

![System & Kernel](https://img.shields.io/badge/System-brown?style=for-the-badge)
![Unix Shell](https://img.shields.io/badge/Unix-Shell-blue?style=for-the-badge)
![POSIX Compliance](https://img.shields.io/badge/Compliance-POSIX-green?style=for-the-badge)
![C Language](https://img.shields.io/badge/Language-C-red?style=for-the-badge)

*A from-scratch implementation of an interactive POSIX-compliant shell*

</div>

<div align="center">
  <img src="/42sh.png">
</div>

# 42sh

[README en Español](README_es.md)

### <span style="color:yellow;">**🚨 THIS README IS A TEMPLATE AND DOES NOT REFLECT THE CURRENT PROJECT STATUS 🚨**</span>

`42sh` is a custom implementation of a POSIX-compliant Unix shell.

Designed as an educational project, this shell provides a powerful interface for command execution, scripting, and process management, with command-line editing support in `readline` and `vi` modes.

In addition, advanced features have been implemented such as globbing, redirections, job control, and script execution.

## ✨ Features

- `Custom readline`: Built from scratch with history, search, autocomplete, and real-time editing.
- `Editing modes`: Support for **emacs** and **vi** modes for efficient navigation and editing.
- `Globbing`: Support for a wide variety of patterns.
- `Redirections`: Input/output handling with all types of redirections.
- `Pipes`: Support for chaining commands via pipelines.
- `Builtins`: A large set of builtins essential to shell functionality.
- `Scripting`: Full support for script execution.
- `Signals`: Proper handling of all signals.
- `Variables`: Support for shell and environment variables.

### Implemented Builtins

42sh includes the following builtins:

#### Alias
- `alias` - Defines command aliases.
- `unalias` - Removes defined aliases.

#### Environment variables
- `declare` - Declares variables and attributes.
- `export` - Defines environment variables.
- `readonly` - Marks variables as read-only.
- `set` - Displays or defines shell options.
- `shopt` - Configures shell options.
- `unset` - Removes environment variables.

#### Filesystem
- `cd` - Changes the current directory.
- `hash` - Maintains a command cache.
- `pwd` - Prints the current working directory.

#### History
- `fc` - Edits and executes previous commands.
- `history` - Shows the command history.

#### Job control
- `bg` - Resumes a job in the background.
- `fg` - Brings a job to the foreground.
- `jobs` - Lists running jobs.
- `kill` - Sends signals to processes.
- `wait` - Waits for a process to finish.

#### Arithmetic expressions
- `let` - Evaluates arithmetic expressions.

#### Scripting
- `.` - Executes a script in the current context.
- `eval` - Evaluates and executes a command string.
- `exec` - Replaces the current shell with a command.
- `source` - Executes a script in the current context.
- `test` - Evaluates conditional expressions.

#### Shell
- `banner` - Displays the 42sh banner.
- `builtin` - Shows information about built-in commands.
- `command` - Executes a command without applying aliases or functions.
- `echo` - Prints text to standard output.
- `enable` - Enables or disables builtins.
- `exit` - Closes the shell session.
- `help` - Shows help about 42sh.
- `type` - Indicates how a command will be interpreted.

### Globbing

The shell implements globbing for pattern expansion in file and directory names. The following patterns are supported:

- `*` - Matches any number of characters in a single directory level.
- `?` - Matches a single character.
- `[abc]` - Matches any of the specified characters.
- `**` - Recursively matches any number of subdirectories.
- `**/` - Matches any number of subdirectories, excluding files.

### Redirections and Pipes

The following redirections are supported:

- `>`   - Redirects standard output to a file (overwrite).
- `>>`  - Redirects standard output to a file (append).
- `<`   - Takes input from a file.
- `<<`  - Enables input redirection with a _heredoc_.
- `<<<` - Redirects a string as input.
- `>&`  - Duplicates output file descriptors.
- `<&`  - Duplicates input file descriptors.
- `|`   - Connects the output of one command to the input of another.
- `|&`  - Connects both standard output and error to the input of another.

### Scripting

42sh supports script execution with control structures and logical operators. Example script:

```sh
#!/usr/bin/env 42sh

echo "Hello, welcome to 42sh"

if [ -f "archivo.txt" ]; then
  echo "The file exists"
else
  echo "The file does not exist"
fi

for i in {1..5}; do
  echo "Iteration $i"
done
```

## 🔧 Installation

To build the shell:

```sh
make
```

To run:

```sh
./42sh
```

To run a script:

```sh
./42sh script.sh
```

---

## 📄 License

This project is licensed under the WTFPL – [Do What the Fuck You Want to Public License](http://www.wtfpl.net/about/).

---

<div align="center">

**📟 Developed as part of the 42 School curriculum 📟**

*"It's not magic... just pipes and processes"*

</div>

<div align="center">
  <img src="/doc/42sh.jpg">
</div>
