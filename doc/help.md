Categorías principales:
1. builtins
Los comandos internos del shell. Aquí entran:

cd, echo, exit, export, unset, source, pwd
history, alias, unalias
read, set, shift
break, continue, return
jobs, fg, bg, kill (si los tienes)
help (sí, help se explica a sí mismo)

Acceso:

help builtins → lista todos + descripción breve de qué es un builtin
help cd → ayuda específica del builtin cd

2. variables
Todo lo relacionado con variables:

Declaración y asignación: VAR=value
Expansión: $VAR, ${VAR}
Variables especiales: $?, $$, $!, $0, $1..., $@, $*
Arrays (si los implementas): arr=(a b c), ${arr[0]}
Variables de entorno vs locales

Acceso:

help variables → explicación general + lista subtemas
Podrías tener help variables special para las especiales

3. expansions
Mecanismos de expansión del shell:

Parameter expansion: ${var:-default}, ${var#pattern}
Command substitution: $(command), `command`
Arithmetic expansion: $((expresión))
Brace expansion: {a,b,c}, {1..10} (si lo tienes)
Tilde expansion: ~, ~user

Acceso:

help expansions → lista todos los tipos
help arithmetic → detalle de $(( ))

4. redirections
Redirecciones de entrada/salida:

>, >>, <
<< (here-document), <<< (here-string)
>&, <& (duplicación de descriptores)
2>, &>, 2>&1
Redirecciones con números de fd personalizados

Acceso:

help redirections → explicación + ejemplos de cada tipo

5. globbing
Expansión de patrones:

*, ?, [...]
Patrones extendidos (si los tienes): @(...), *(...), etc.
Opciones que afectan globbing: dotglob, nullglob

Acceso:

help globbing → explicación completa

6. operators
Operadores de control y ejecución:

Pipelines: |, |&
Listas: ;, &, &&, ||
Agrupación: ( ), { }
Background: &

Acceso:

help operators → explica todos

7. subshells
Ejecución en subshells:

Cuándo se crean: ( ), pipes, command substitution
Diferencia entre subshell y child process
Scope de variables
Exit status

Acceso:

help subshells → explicación conceptual + ejemplos

8. readline
Edición de línea de comandos (si usas readline):

Modos emacs/vi
Shortcuts principales
Configuración básica

Acceso:

help readline → lista modos + shortcuts básicos
help readline emacs y help readline vi para detalles

9. options
Opciones del shell (set -o, shopt):

set -e, set -u, set -x, etc.
Opciones de comportamiento
Cómo activar/desactivar

Acceso:

help options → lista opciones disponibles

Implementación sugerida:
Prioridad ALTA (implementa primero):

builtins (cada uno individualmente)
redirections
variables
operators

Prioridad MEDIA:

expansions
globbing
subshells

Prioridad BAJA (después):

readline
options

Sobre las man pages:
Si vas a tener man pages, podrías hacer:

help [tema] → ayuda rápida, concisa, con ejemplos
man 42sh → documentación completa del shell
man 42sh-builtins → página dedicada a todos los builtins

La ventaja: help es para consultas rápidas en el terminal, man para documentación exhaustiva.




SHELL HELP CATEGORIES

    Use 'help [category]' for information on a specific category.
    Use 'help [builtin]' for information on a specific builtin command.
    Use 'help -m [topic]' for man-page style format.

CATEGORIES:
    redirections    - Input/output redirection operators
    variables       - Shell variables and parameter usage
    expansions      - Shell expansion mechanisms
    arithmetic      - Arithmetic evaluation and expressions
    globbing        - Filename expansion patterns
    operators       - Control and execution operators
    subshells       - Subshell execution contexts

BUILTINS:
    Use 'help builtins' to see a list of all builtin commands.

SEE ALSO:
    help builtins, man 42sh

---

REDIRECTIONS
    Input/output redirection operators

DESCRIPTION
    Redirection allows you to control where a command reads its input
    from and where it sends its output. Before a command is executed,
    its input and output may be redirected using special notation
    interpreted by the shell.

    Redirection operators may precede or appear anywhere within a
    simple command or may follow a command. Redirections are processed
    in the order they appear, from left to right.

REDIRECTION OPERATORS:
    >file       Redirect standard output to file (overwrite)
    >>file      Redirect standard output to file (append)
    <file       Redirect standard input from file
    <<EOF       Here-document (read input until EOF delimiter)
    <<<string   Here-string (use string as input)
    
    n>file      Redirect file descriptor n to file
    n>>file     Redirect file descriptor n to file (append)
    n<file      Redirect file descriptor n from file
    n>&m        Duplicate output file descriptor m to n
    n<&m        Duplicate input file descriptor m to n
    &>file      Redirect both stdout and stderr to file
    &>>file     Redirect both stdout and stderr to file (append)
    2>&1        Redirect stderr to wherever stdout is going

EXAMPLES
    # Basic output redirection
    echo "hello" > output.txt
    
    # Append to file
    echo "world" >> output.txt
    
    # Input redirection
    sort < unsorted.txt
    
    # Redirect stderr to file
    command 2> errors.txt
    
    # Redirect both stdout and stderr
    command &> all_output.txt
    
    # Redirect stderr to stdout
    command 2>&1 | grep error
    
    # Here-document
    cat <<EOF
    Line 1
    Line 2
    EOF
    
    # Here-string
    grep pattern <<<"text to search"
    
    # Close file descriptor
    exec 3>&-

NOTES
    File descriptor 0 is standard input (stdin)
    File descriptor 1 is standard output (stdout)
    File descriptor 2 is standard error (stderr)
    
    Redirections using file descriptors greater than 9 should be used
    with care, as they may conflict with file descriptors used
    internally by the shell.

SEE ALSO
    operators, subshells

---

VARIABLES
    Shell variables and parameters

DESCRIPTION
    A variable is a parameter denoted by a name. A variable has a value
    and zero or more attributes. Variables are created with the
    assignment operator (=). Variable names must start with a letter or
    underscore and contain only letters, numbers, and underscores.

ASSIGNMENT
    name=value              Assign value to variable
    name=(value1 value2)    Assign array values (if supported)
    
    No spaces are allowed around the = operator.

PARAMETER EXPANSION
    $name or ${name}        Expand variable value
    ${name:-word}           Use default value if unset or null
    ${name:=word}           Assign default value if unset or null
    ${name:?message}        Display error if unset or null
    ${name:+word}           Use alternate value if set
    ${#name}                Length of variable value
    ${name#pattern}         Remove shortest match from beginning
    ${name##pattern}        Remove longest match from beginning
    ${name%pattern}         Remove shortest match from end
    ${name%%pattern}        Remove longest match from end

SPECIAL PARAMETERS
    $0      Name of the shell or shell script
    $1-$9   Positional parameters
    $*      All positional parameters as single word
    $@      All positional parameters as separate words
    $#      Number of positional parameters
    $?      Exit status of last command
    $$      Process ID of the shell
    $!      Process ID of last background command
    $_      Last argument of previous command

ARRAYS (if supported)
    arr=(val1 val2 val3)    Create array
    ${arr[0]}               Access element
    ${arr[@]}               All elements as separate words
    ${arr[*]}               All elements as single word
    ${#arr[@]}              Number of elements

ENVIRONMENT VARIABLES
    Variables can be exported to child processes using the export
    builtin. Exported variables are called environment variables.
    
    export VAR=value        Create and export variable
    export VAR              Export existing variable

VARIABLE SCOPE
    Variables are local to the shell process unless exported.
    Subshells inherit exported variables but cannot modify parent's
    variables. Functions can create local variables with 'local'.

EXAMPLES
    # Basic assignment and expansion
    name="John"
    echo "Hello, $name"
    
    # Default values
    echo ${undefined:-"default value"}
    
    # String manipulation
    path="/usr/local/bin"
    echo ${path##*/}        # Output: bin
    echo ${path%/*}         # Output: /usr/local
    
    # Length
    echo ${#name}           # Output: 4
    
    # Arrays
    fruits=(apple orange banana)
    echo ${fruits[1]}       # Output: orange
    echo ${#fruits[@]}      # Output: 3
    
    # Special parameters
    echo $?                 # Exit status
    echo $$                 # Current PID

SEE ALSO
    export, unset, set, expansions

---

EXPANSIONS
    Shell expansion mechanisms

DESCRIPTION
    Expansion is performed on the command line after it has been split
    into tokens. There are several types of expansion performed in a
    specific order: brace expansion, tilde expansion, parameter and
    variable expansion, command substitution, arithmetic expansion,
    word splitting, and filename expansion.

TYPES OF EXPANSION

    Brace Expansion
        {string1,string2,...}   Generate arbitrary strings
        {start..end}            Generate sequences
        
        Examples:
            echo {a,b,c}        # a b c
            echo {1..5}         # 1 2 3 4 5
            echo file{1..3}.txt # file1.txt file2.txt file3.txt

    Tilde Expansion
        ~           Home directory of current user
        ~/path      Path relative to home directory
        ~user       Home directory of specified user
        
        Examples:
            cd ~/Documents
            ls ~root

    Parameter Expansion
        See 'help variables' for complete parameter expansion syntax.
        
        Examples:
            $VAR
            ${VAR}
            ${VAR:-default}

    Command Substitution
        $(command)      Execute command and substitute output
        `command`       Legacy syntax (backticks)
        
        Examples:
            echo "Today is $(date)"
            files=$(ls *.txt)
            current_dir=`pwd`

    Arithmetic Expansion
        $((expression))     Evaluate arithmetic expression
        
        Examples:
            echo $((2 + 2))
            count=$((count + 1))
            echo $((5 * 3 + 2))

    Word Splitting
        Results of parameter expansion, command substitution, and
        arithmetic expansion are subject to word splitting using
        IFS (Internal Field Separator) characters.

    Filename Expansion (Globbing)
        See 'help globbing' for pattern matching details.

ORDER OF EXPANSION
    1. Brace expansion
    2. Tilde expansion
    3. Parameter, variable, and arithmetic expansion
    4. Command substitution (left-to-right)
    5. Word splitting
    6. Filename expansion

QUOTING
    Single quotes preserve literal value of all characters
    Double quotes preserve literal value except $, `, \, and !
    Backslash escapes the next character
    
    Examples:
        echo '$USER'        # Output: $USER
        echo "$USER"        # Output: john (expanded)
        echo "Total: \$100" # Output: Total: $100

EXAMPLES
    # Combining expansions
    backup_dir=~/backups/$(date +%Y%m%d)
    
    # Command substitution in assignment
    file_count=$(ls | wc -l)
    
    # Arithmetic in expansion
    for i in {1..10}; do
        echo "Number: $((i * 2))"
    done
    
    # Nested expansions
    echo "User ${USER:-unknown} has $(($(ls | wc -l))) files"

SEE ALSO
    variables, arithmetic, globbing, quoting

---

ARITHMETIC
    Arithmetic evaluation and expressions

DESCRIPTION
    The shell allows arithmetic expressions to be evaluated in several
    contexts. Evaluation is done in fixed-width integers with no check
    for overflow. Division by zero is detected and flagged as an error.

ARITHMETIC CONTEXTS

    Arithmetic Expansion
        $((expression))     Evaluates and expands to result
        
        Used for substituting arithmetic results in commands.

    Arithmetic Evaluation
        ((expression))      Evaluates expression (no expansion)
        
        Used in conditional contexts. Exit status is 0 if result
        is non-zero, 1 if result is zero.

    Let Command
        let arg [arg ...]   Evaluate arithmetic expressions
        
        Each argument is evaluated as an arithmetic expression.

OPERATORS (highest to lowest precedence)
    id++, id--          Post-increment, post-decrement
    ++id, --id          Pre-increment, pre-decrement
    -, +                Unary minus, unary plus
    !, ~                Logical NOT, bitwise NOT
    **                  Exponentiation
    *, /, %             Multiplication, division, remainder
    +, -                Addition, subtraction
    <<, >>              Bitwise left shift, right shift
    <, <=, >, >=        Comparison operators
    ==, !=              Equality, inequality
    &                   Bitwise AND
    ^                   Bitwise XOR
    |                   Bitwise OR
    &&                  Logical AND
    ||                  Logical OR
    expr ? expr : expr  Conditional operator
    =, *=, /=, %=,      Assignment operators
    +=, -=, <<=, >>=,
    &=, ^=, |=

CONSTANTS
    Numeric constants can be specified in different bases:
        [base#]number   Number in specified base (2-64)
        0x number       Hexadecimal
        0 number        Octal
        
    Examples:
        2#1010          # Binary: 10
        0xFF            # Hexadecimal: 255
        077             # Octal: 63

VARIABLES IN ARITHMETIC
    Shell variables are allowed as operands. Variables need not be
    prefixed with $ within arithmetic expressions.
    
    Examples:
        count=5
        echo $((count + 1))     # 6
        ((count++))
        echo $count             # 6

EXAMPLES
    # Arithmetic expansion
    echo $((5 + 3))                     # 8
    echo $((10 / 3))                    # 3 (integer division)
    echo $((2 ** 8))                    # 256
    
    # Variables
    x=10
    y=3
    echo $((x * y))                     # 30
    echo $((x % y))                     # 1
    
    # Increment/decrement
    count=0
    ((count++))
    echo $count                         # 1
    
    # Arithmetic evaluation (conditional)
    ((5 > 3)) && echo "true"           # prints "true"
    ((5 < 3)) && echo "true"           # prints nothing
    
    # Let command
    let "a = 5 + 5"
    let "b = a * 2"
    echo $b                             # 20
    
    # Complex expressions
    echo $((3 + 5 * 2))                # 13
    echo $(((3 + 5) * 2))              # 16
    
    # Bitwise operations
    echo $((5 & 3))                    # 1
    echo $((5 | 3))                    # 7
    echo $((5 ^ 3))                    # 6
    
    # Conditional operator
    max=$((a > b ? a : b))
    
    # Assignment in arithmetic
    echo $((x += 5))                   # increment and show result

NOTES
    Arithmetic is performed using long integers (typically 64-bit).
    Arithmetic expansion can be nested.
    Spaces around operators are optional within (( )).

SEE ALSO
    let, expansions, operators

---

GLOBBING
    Filename expansion with pattern matching

DESCRIPTION
    After word splitting, unless the -f option has been set, the shell
    scans each word for the characters *, ?, and [. If one of these
    characters appears, the word is regarded as a pattern and replaced
    with an alphabetically sorted list of filenames matching the
    pattern. If no matching filenames are found, and the shell option
    nullglob is not enabled, the word is left unchanged.

PATTERN MATCHING CHARACTERS
    *               Match any string, including empty string
    ?               Match any single character
    [...]           Match any one of enclosed characters
    [!...]          Match any character not enclosed
    [^...]          Same as [!...] (match any not enclosed)

CHARACTER CLASSES
    Within [...]:
        [a-z]       Match any character in range
        [[:class:]] Match any character in character class
    
    Character classes:
        [:alnum:]   Alphanumeric characters
        [:alpha:]   Alphabetic characters
        [:digit:]   Numeric characters
        [:lower:]   Lowercase letters
        [:upper:]   Uppercase letters
        [:space:]   Whitespace characters
        [:punct:]   Punctuation characters

EXTENDED PATTERNS (if enabled with extglob)
    ?(pattern)      Match zero or one occurrence
    *(pattern)      Match zero or more occurrences
    +(pattern)      Match one or more occurrences
    @(pattern)      Match exactly one occurrence
    !(pattern)      Match anything except pattern
    
    Multiple patterns can be separated by |

SPECIAL CASES
    Dot files (filenames starting with .) are not matched by *,
    ?, or [...] unless the pattern explicitly starts with a dot.
    
    The character / must always be matched explicitly.
    
    A pattern-list is a list of patterns separated by |.

SHELL OPTIONS AFFECTING GLOBBING
    nullglob        Patterns matching nothing expand to null string
    dotglob         Include dot files in * expansion
    extglob         Enable extended pattern matching
    nocaseglob      Match patterns case-insensitively
    globstar        ** matches directories recursively

EXAMPLES
    # Basic patterns
    ls *.txt                    # All .txt files
    ls file?.txt                # file1.txt, fileA.txt, etc.
    ls [abc]*.txt               # Files starting with a, b, or c
    ls [!0-9]*                  # Files not starting with digit
    
    # Character classes
    ls *[[:digit:]].txt         # Files ending with digit
    ls [[:upper:]]*             # Files starting with uppercase
    
    # Multiple patterns
    ls *.{txt,md}               # .txt and .md files (with brace expansion)
    
    # Extended patterns (if extglob enabled)
    ls +(*.txt|*.md)            # One or more .txt or .md
    ls !(*.o)                   # Everything except .o files
    
    # Recursive (if globstar enabled)
    ls **/*.txt                 # All .txt files in all subdirectories
    
    # Escaping special characters
    ls \*.txt                   # Literally "*.txt"
    ls "*.txt"                  # Also literally "*.txt"

NOTES
    Globbing is performed after other expansions.
    Failed matches (when nullglob is off) remain as literal strings.
    Quoting disables pattern matching.

SEE ALSO
    expansions, set (for shell options), shopt

---

OPERATORS
    Control and execution operators

DESCRIPTION
    Operators control the flow of command execution and allow
    combining multiple commands. The shell recognizes several
    operators that have special meaning and are treated as
    separate tokens.

CONTROL OPERATORS

    Semicolon (;)
        command1 ; command2
        
        Execute commands sequentially. Shell waits for each command
        to terminate before executing the next.
        
        Example:
            cd /tmp ; ls ; pwd

    Ampersand (&)
        command &
        
        Execute command in background. Shell does not wait for
        command to finish and immediately returns with exit status 0.
        
        Example:
            long_process &
            echo "Process started"

    AND (&&)
        command1 && command2
        
        Execute command2 only if command1 succeeds (returns 0).
        Short-circuit evaluation.
        
        Example:
            mkdir newdir && cd newdir
            test -f file && cat file

    OR (||)
        command1 || command2
        
        Execute command2 only if command1 fails (returns non-zero).
        Short-circuit evaluation.
        
        Example:
            command || echo "Command failed"
            cd /tmp || exit 1

    Pipe (|)
        command1 | command2
        
        Connect stdout of command1 to stdin of command2.
        Commands execute concurrently in subshells.
        
        Example:
            ls -l | grep txt
            cat file | sort | uniq

    Pipe with stderr (|&)
        command1 |& command2
        
        Connect both stdout and stderr of command1 to stdin of
        command2. Equivalent to 2>&1 |
        
        Example:
            command |& tee output.log

GROUPING OPERATORS

    Subshell ( )
        (commands)
        
        Execute commands in a subshell. Variable assignments and
        directory changes do not affect parent shell.
        
        Example:
            (cd /tmp; ls)       # pwd unchanged after
            x=1; (x=2); echo $x # prints 1

    Command group { }
        { commands; }
        
        Execute commands in current shell context. Note: requires
        semicolon or newline before closing brace.
        
        Example:
            { echo "Line 1"; echo "Line 2"; } > file
            x=1; { x=2; }; echo $x  # prints 2

COMMAND SEPARATORS

    Newline
        Equivalent to semicolon; terminates a command.

    &
        Terminates command and runs it in background.

    ;
        Terminates command; next command runs after completion.

PIPELINES
    A pipeline is a sequence of one or more commands separated by |.
    The exit status of a pipeline is the exit status of the last
    command, unless pipefail option is set.

LISTS
    A list is a sequence of one or more pipelines separated by
    ;, &, &&, or || operators, and optionally terminated by
    ;, &, or newline.

OPERATOR PRECEDENCE (highest to lowest)
    1. |, |&                    Pipelines
    2. &&, ||                   Logical operators
    3. ;, &, newline            Command terminators/separators

EXAMPLES
    # Sequential execution
    echo "First" ; echo "Second" ; echo "Third"
    
    # Conditional execution
    make && make install
    grep pattern file || echo "Not found"
    
    # Complex conditions
    test -f file && cat file || echo "File not found"
    
    # Pipelines
    ps aux | grep process | awk '{print $2}'
    
    # Background jobs
    sleep 10 &
    long_task & another_task &
    
    # Grouping
    { echo "Header"; cat file; echo "Footer"; } > output
    (cd /tmp && ls) | grep txt
    
    # Combining operators
    command1 && command2 || command3
    (command1; command2) | command3 &

NOTES
    && and || have equal precedence and are evaluated left-to-right.
    Pipelines are executed in subshells.
    Background processes inherit stdin from /dev/null.

SEE ALSO
    redirections, subshells, job control (jobs, fg, bg)

---

SUBSHELLS
    Subshell execution contexts

DESCRIPTION
    A subshell is a separate instance of the shell that runs in a
    child process. Subshells are created implicitly in several
    situations and explicitly with parentheses. Changes made in a
    subshell do not affect the parent shell.

WHEN SUBSHELLS ARE CREATED

    Explicit Subshells
        (commands)
        
        Parentheses create an explicit subshell for command execution.

    Pipelines
        command1 | command2
        
        Each command in a pipeline runs in its own subshell.

    Command Substitution
        $(command)
        `command`
        
        The command runs in a subshell; its output is captured.

    Background Jobs
        command &
        
        Background commands execute in a subshell.

    Coprocesses (if supported)
        coproc command
        
        Creates a subshell with bidirectional pipe.

SUBSHELL CHARACTERISTICS

    Inheritance:
        - Subshells inherit environment variables
        - Subshells inherit exported variables
        - Subshells inherit file descriptors
        - Subshells inherit current directory
        - Subshells inherit umask and ulimits

    Isolation:
        - Variable changes don't affect parent
        - Directory changes don't affect parent
        - Function definitions local to subshell
        - Trap settings local to subshell
        - Shell options can be modified independently

    Process Properties:
        - Different process ID (PID)
        - Separate memory space
        - Exit status returned to parent

SUBSHELL LEVEL
    The BASH_SUBSHELL variable (if available) indicates the subshell
    level. It's 0 in the main shell and increments with each subshell.

SUBSHELLS vs CHILD PROCESSES
    Subshell:
        - Created by shell for shell constructs
        - Inherits shell environment completely
        - Used for: ( ), pipelines, command substitution
    
    Child Process:
        - Created by exec/fork for external commands
        - Inherits only exported variables
        - New program replaces shell image

EXAMPLES
    # Explicit subshell
    x=1
    (x=2; echo "Inside: $x")    # Prints: Inside: 2
    echo "Outside: $x"           # Prints: Outside: 1
    
    # Directory change in subshell
    pwd                          # /home/user
    (cd /tmp; pwd)              # /tmp
    pwd                          # /home/user (unchanged)
    
    # Pipeline subshells
    echo "test" | read var
    echo $var                    # Empty (var set in subshell)
    
    # Avoid subshell with process substitution
    while read line; do
        var="$line"
    done < <(echo "test")
    echo $var                    # Prints: test
    
    # Command substitution
    result=$(x=5; echo $x)
    echo $result                 # Prints: 5
    echo $x                      # Empty (x set in subshell)
    
    # Background job
    count=0
    { sleep 1; count=10; } &
    wait
    echo $count                  # Still 0 (changed in subshell)
    
    # Subshell for cleanup
    (
        cd /tmp
        mktemp -d
        # work in temp dir
    )  # automatically returns to original dir
    
    # Grouping without subshell
    x=1
    { x=2; echo "Inside: $x"; }  # Prints: Inside: 2
    echo "Outside: $x"           # Prints: Outside: 2 (same shell)

PERFORMANCE CONSIDERATIONS
    Subshells have overhead due to process creation. For performance-
    critical code, avoid unnecessary subshells:
    
    Inefficient:
        cat file | while read line; do
            process "$line"
        done
    
    Efficient:
        while read line; do
            process "$line"
        done < file

DEBUGGING SUBSHELLS
    Use $BASHPID to get actual process ID in subshells:
        echo "Main: $$, PID: $BASHPID"
        (echo "Sub: $$, PID: $BASHPID")

NOTES
    $$ always shows parent shell PID, even in subshells
    $BASHPID shows actual current process ID
    Exit status of subshell is exit status of last command
    Subshells can be nested

SEE ALSO
    operators, variables, command substitution, job control

---

