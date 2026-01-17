# Builtins

## 1. Job Control

- `jobs` - List jobs
- `fg` - Bring job to foreground
- `bg` - Continue job in background
- `wait` - Wait for processes to terminate
- `disown` - Disown jobs from the shell

## 2. Process/Execution Control

- `exec` - Replace the shell with another command
- `exit` - Exit the shell
- `eval` - Evaluate and execute arguments
- `command` - Execute command bypassing functions
- `builtin` - Execute specific builtin
- `source` / `.` - Execute script in current shell

## 3. Flow Control

- `break` - Exit from loops
- `continue` - Skip to next loop iteration
- `return` - Exit from function/script

## 4. File System/Directories

- `cd` - Change directory
- `pwd` - Print working directory
- `dirs` - Display directory stack
- `pushd` - Push to directory stack
- `popd` - Pop from directory stack

## 5. Variables and Attributes

- `export` - Export variables to environment
- `unset` - Remove variables/functions
- `set` - Set shell options and positional parameters
- `declare` / `typeset` - Declare variables with attributes
- `readonly` - Mark variables as read-only
- `local` - Declare local variables in functions
- `shift` - Shift positional parameters
- `read` - Read input
- `readarray` / `mapfile` - Read lines into array

## 6. Arithmetic

- `let` - Evaluate arithmetic expressions

## 7. I/O and Formatting

- ✓ `echo` - Print text
- `printf` - Print with format

## 8. Traps, Signals and Control

- `trap` - Configure signal handlers
- `kill` - Send signals to processes
- `suspend` - Suspend the shell

## 9. Information and Query

- `type` - Show command type
- `help` - Help for builtins
- `times` - Show accumulated CPU time
- `umask` - Show/set permission mask
- `ulimit` - Show/set resource limits
- `hash` - Manage command hash table

## 10. Shell Options

- `shopt` - Set shell options (42sh specific)
- `set` - Also controls options (POSIX)

## 11. History

- `history` - Manage command history
- `fc` - Edit and execute commands from history

## 12. Aliases

- `alias` - Define alias
- `unalias` - Remove alias

## 13. Testing/Conditionals

- `test` / `[` - Evaluate conditional expressions
- `true` - Always returns success
- `false` - Always returns failure
- `:` - No-op, always returns success
