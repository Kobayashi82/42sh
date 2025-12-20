
# Builtins

## 42sh

- `banner` - displays 42sh banner
- `welcome` - displays 42sh welcome message
- `builtin` - executes builtin ignoring functions with same name
- `declare` - declares variables with attributes
- `typeset` - declares variables with attributes (alias of `declare`)
- `dirs` - shows directory stack
- `disown` - detaches jobs from shell
- `enable` - enables/disables builtins
- `help` - help about builtins
- `history` - shows command history
- `let` - arithmetic evaluation
- `local` - declares local variables inside functions
- `logout` - logs out from login shell
- `mapfile` - reads file lines into array
- `readarray` - reads file lines into array (alias of `mapfile`)
- `popd` - removes directory from stack
- `pushd` - adds directory to stack
- `shopt` - configures 42sh-specific options
- `suspend` - suspends shell execution

## Regular
- `alias` - creates command aliases
- `bg` - resumes job in background
- `cd` - changes directory
- `command` - executes command ignoring functions
- `echo` - prints text
- `false` - returns failure status (exit code 1)
- `fc` - "fix command", interactive history editor
- `fg` - resumes job in foreground
- `getopts` - processes options
- `hash` - caches external command locations
- `jobs` - lists active jobs
- `kill` - sends signals
- `printf` - formats and prints text
- `pwd` - shows current directory
- `read` - reads input
- `test` - conditional evaluation
- `[` - conditional evaluation (alias of `test`)
- `true` - returns success status (exit code 0)
- `type` - shows command type
- `ulimit` - shows/configures system resource limits
- `umask` - permission mask
- `unalias` - removes aliases
- `wait` - waits for processes

## Special
- `break` - exits from loop
- `continue` - skips to next loop iteration
- `.` - executes script in current context
- `source` - executes script in current context (alias of `.`)
- `:` - null command, always returns success (exit code 0)
- `eval` - evaluates arguments as command
- `exec` - replaces current shell
- `exit` - terminates shell
- `export` - exports variables to environment
- `readonly` - makes variables read-only
- `return` - returns from function
- `set` - configures shell options
- `shift` - shifts positional parameters
- `times` - displays execution times
- `trap` - handles signals
- `unset` - removes variables or functions

---

## Keywords (POSIX)
- `if`, `then`, `else`, `elif`, `fi` - conditional structure
- `case`, `esac`, `in` - pattern matching structure
- `for`, `while`, `until`, `do`, `done` - loop structures
- `!` - negation/pipeline negation

## Keywords (42sh-specific)
- `function` - alternative function declaration syntax
- `select` - menu generation loop
- `[[`, `]]` - conditional expression (enhanced test)
- `coproc` - coprocess execution
- `time` - command timing (also exists as external command)

## Semi-keywords (context-dependent)
- `in` - only keyword inside `for` and `case`
- `do`, `done` - only with loops
- `then` - only with `if`/`elif`
- `esac` - only closes `case`
- `fi` - only closes `if`

---

### NOTES
Some like `do`, `done`, `then`, `fi`, `esac` are only keywords in certain syntactic contexts. Outside those contexts they could be used as variable names or commands (although it's bad practice).

Also `time` is special - in 42sh it's a keyword for timing commands, but it also exists as an external command `/usr/bin/time`.
