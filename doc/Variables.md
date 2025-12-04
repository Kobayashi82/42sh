# Bash Variables Reference

## History Variables

| Variable | Description | Exported | Value |
|----------|-------------|----------|-------|
| `_` | Last argument to the last command executed | Yes | var |
| `42HISTCMD` | The history number of the current command | No | 2000 |
| `42HISTCONTROL` | Settings for controlling the history list behavior | No | ignoreboth |
| `42HISTFILE` | File where the history list is stored | No | /home/kobayashi/.bash_history |
| `42HISTFILESIZE` | Maximum number of lines to save in the history file | No | 2000 |
| `42HISTSIZE` | Number of commands to retain in the history list | No | 1000 |

## Shell Configuration

| Variable | Description | Exported | Value |
|----------|-------------|----------|-------|
| `BASH` | Path to the bash executable | No | /usr/bin/bash |
| `BASH_ALIASES` | List of currently defined aliases | No | |
| `BASH_ARGC` | Array containing the number of arguments for each function call | No | 0 |
| `BASH_ARGV` | Array containing the arguments for each function call | No | |
| `BASH_ARGV0` | The name of the script or shell | No | bash |
| `BASH_CMDS` | Associative array of remembered commands | No | |
| `BASH_COMMAND` | The last command being executed | No | printf \"%s=%s\\n\" \"\$var\" \"\${!var}\" |
| `BASH_COMPLETION_VERSINFO` | Version of the Bash completion framework | No | 2 |
| `BASH_LINENO` | Array of line numbers for each function call | No | |
| `BASH_LOADABLES_PATH` | Directories Bash uses to search for loadable builtins | No | /usr/local/lib/bash:/usr/lib/bash:/opt/local/lib/bash:/usr/pkg/lib/bash:/opt/pkg/lib/bash |
| `BASH_SOURCE` | Array of source file paths for the current function call | No | |
| `BASH_SUBSHELL` | Subshell level (increments with each subshell) | No | 1 |
| `BASH_VERSINFO` | Array containing version details of Bash | No | 5 |
| `BASH_VERSION` | Version of Bash as a string | No | 5.2.21(1)-release |

## Shell Options

| Variable | Description | Exported | Value |
|----------|-------------|----------|-------|
| `BASHOPTS` | Enabled shell options | No | checkwinsize:cmdlist:complete_fullquote:expand_aliases:extglob:extquote:force_fignore:globasciiranges:globskipdots:histappend... |
| `SHELLOPTS` | Enabled shell options as a colon-separated list | No | braceexpand:emacs:hashall:histexpand:history:interactive-comments:monitor |

## Process Information

| Variable | Description | Exported | Value |
|----------|-------------|----------|-------|
| `BASHPID` | Process ID of the current Bash process | No | 64016 |
| `PPID` | Parent process ID of the current Bash process | No | 17847 |
| `SHLVL` | Shell level; incremented each time a new shell is spawned | Yes | 1 |
| `UID` | User ID of the current user | No | 1000 |
| `EUID` | Effective user ID of the current user | No | 1000 |
| `GROUPS` | List of groups the current user belongs to | No | 1000 |

## Terminal & Display

| Variable | Description | Exported | Value |
|----------|-------------|----------|-------|
| `COLUMNS` | Number of columns for the terminal display | No | 133 |
| `LINES` | Number of lines for the terminal display | No | 19 |
| `TERM` | Terminal type (e.g., xterm, dumb) | Yes | dumb |
| `HOSTTYPE` | Hardware type of the machine (e.g., x86_64) | No | x86_64 |
| `MACHTYPE` | System type (architecture, vendor, and OS) | No | x86_64-pc-linux-gnu |
| `OSTYPE` | Operating system type (e.g., linux-gnu) | No | linux-gnu |

## Directories & Paths

| Variable | Description | Exported | Value |
|----------|-------------|----------|-------|
| `DIRSTACK` | Stack of directories managed by the dirs and pushd/popd commands | No | /mnt/d/Programas/Drivers/Mis\ Programas/C/Active/42sh |
| `HOME` | Home directory of the current user | Yes | /home |
| `OLDPWD` | Previous working directory (before the last cd command) | Yes | /mnt/d/Programas/Drivers/Mis\ Programas/C/Active/42sh |
| `PATH` | List of directories where executables are searched | Yes | /usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin |
| `PWD` | Current working directory | Yes | /mnt/d/Programas/Drivers/Mis\ Programas/C/Active/42sh |

## Prompts

| Variable | Description | Exported | Value |
|----------|-------------|----------|-------|
| `PS1` | Primary prompt string | No | \$\(debian_chroot\+\(\$debian_chroot\)\)\u@\h:\w\\$ |
| `PS2` | Secondary prompt string (e.g., for multiline commands) | No | > |
| `PS4` | Prompt string for tracing (set -x) | No | \+\ |

## System Information

| Variable | Description | Exported | Value |
|----------|-------------|----------|-------|
| `HOSTNAME` | Name of the current host | No | PADRE |
| `SHELL` | Path to the user's default shell | No | /bin/bash |
| `IFS` | Internal field separator (used for word splitting) | No | $' \t\n' |

## Less Pager

| Variable | Description | Exported | Value |
|----------|-------------|----------|-------|
| `LESSCLOSE` | Command executed by less to clean up after quitting | Yes | /usr/bin/lesspipe\) %s\ %s |
| `LESSOPEN` | Command executed by less to preprocess files before viewing | Yes | \| /usr/bin/lesspipe\(\) %s |

## Script & Line Information

| Variable | Description | Exported | Value |
|----------|-------------|----------|-------|
| `LINENO` | Current line number in the script | No | 1 |

## Miscellaneous

| Variable | Description | Exported | Value |
|----------|-------------|----------|-------|
| `COMP_WORDBREAKS` | Characters that delimit words for completion | No | \|n'\"><=;|&\(: |
| `EPOCHREALTIME` | Seconds since the epoch as a floating-point number | No | 1733590631932600 |
| `EPOCHSECONDS` | Seconds since the epoch as an integer | No | 1733590631 |
| `MAILCHECK` | Time interval (in seconds) to check for mail | No | 60 |
| `NVM_DIR` | Directory where Node Version Manager (NVM) is installed | Yes | /root |
| `OPTERR` | If set to 1, bash displays error messages from getopts | No | 1 |
| `OPTIND` | Index of the next argument to be processed by getopts | No | 1 |
| `PIPESTATUS` | Array of exit statuses from the last pipeline executed | No | 0 |
| `RANDOM` | Generates a random number each time accessed | No | 26355 |
| `SECONDS` | Number of seconds since the shell was started | No | 9 |
| `SRANDOM` | Generates a random number seeded by the current time | No | 253357727 |
| `LS_COLORS` | Define colors for ls output (e.g., for file types) | Yes |
---

## Notes

- **Exported = Yes**: Variable is available to child processes
- **Exported = No**: Variable is local to the current shell session