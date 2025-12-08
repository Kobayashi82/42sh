# Guía Completa de Opciones de Bash

## Opciones Largas de GNU

### `--debug`
Activa el modo de depuración. Produce trazas de depuración antes de ejecutar cada comando.

### `--debugger`
Habilita el soporte para el depurador de bash. Permite el uso de características extendidas de depuración.

### `--dump-po-strings`
Equivale a `-D`, pero con salida en formato gettext po (portable object).

### `--dump-strings`
Equivale a `-D`. Muestra todas las cadenas traducibles en el script sin ejecutarlo.

### `--help`
Muestra la información de ayuda y sale.

### `--init-file <archivo>` / `--rcfile <archivo>`
Ejecuta comandos desde el archivo especificado en lugar de `~/.bashrc` cuando se inicia una shell interactiva.

### `--login`
Hace que bash actúe como una shell de login.

### `--noediting`
No usa la biblioteca GNU readline para leer líneas de comando en shells interactivas.

### `--noprofile`
No lee los archivos de inicio del sistema (`/etc/profile`) ni ninguno de los archivos de inicio personales (`~/.bash_profile`, `~/.bash_login`, `~/.profile`).

### `--norc`
No lee ni ejecuta el archivo de inicio personal `~/.bashrc` si la shell es interactiva.

### `--posix`
Cambia el comportamiento de bash donde la operación por defecto difiere del estándar POSIX para que coincida con el estándar.

### `--pretty-print`
Muestra los comandos de la shell en un formato más legible sin ejecutarlos.

### `--restricted`
La shell se vuelve restringida. Limita lo que el usuario puede hacer (sin cambiar directorios, sin redirigir salida, sin ejecutar comandos con `/`, etc.).

### `--verbose`
Equivale a `-v`. Imprime las líneas de entrada de la shell a medida que se leen.

### `--version`
Muestra información de versión de bash y sale.

## Opciones de Invocación

### `-c <comando>`
Lee y ejecuta comandos desde el argumento de cadena. Si hay argumentos después del comando, se asignan a los parámetros posicionales, comenzando con `$0`.

### `-i`
Fuerza a la shell a ejecutarse en modo interactivo.

### `-l`
Hace que bash actúe como si hubiera sido invocado como una shell de login.

### `-r`
Shell restringida. Equivale a `--restricted`.

### `-s`
Lee comandos desde la entrada estándar. Si hay argumentos después de `-s`, se asignan a los parámetros posicionales.

### `-D`
Lista todas las cadenas entre comillas dobles precedidas por `$` sin ejecutar el script. Útil para internacionalización.

### `-O <shopt_option>`
Establece la opción de shopt especificada antes de ejecutar el script.

## Opciones de Shell (set)

Estas opciones se pueden activar con `set -<letra>` o desactivar con `set +<letra>`:

### `-a` (allexport)
Marca automáticamente para exportar todas las variables y funciones que se modifiquen o creen.

### `-b` (notify)
Notifica la terminación de trabajos en segundo plano inmediatamente, en lugar de esperar al siguiente prompt.

### `-e` (errexit)
Sale inmediatamente si un comando sale con un estado diferente de cero.

### `-f` (noglob)
Deshabilita la expansión de nombres de archivo (globbing).

### `-h` (hashall)
Recuerda la ubicación de comandos cuando se buscan para ejecutarlos.

### `-k` (keyword)
Todos los argumentos de asignación se colocan en el entorno del comando, no solo los que preceden al nombre del comando.

### `-m` (monitor)
Habilita el control de trabajos. Esta opción está activada por defecto para shells interactivas.

### `-n` (noexec)
Lee comandos pero no los ejecuta. Útil para verificar la sintaxis de scripts.

### `-p` (privileged)
Modo privilegiado. No se procesan los archivos `$ENV` y no se heredan funciones de shell del entorno.

### `-t` (onecmd)
Sale después de leer y ejecutar un comando.

### `-u` (nounset)
Trata las variables no establecidas como un error cuando se sustituyen.

### `-v` (verbose)
Imprime las líneas de entrada de la shell a medida que se leen.

### `-x` (xtrace)
Imprime comandos y sus argumentos a medida que se ejecutan. Muy útil para depuración.

### `-B` (braceexpand)
Habilita la expansión de llaves. Está activada por defecto.

### `-C` (noclobber)
Previene la sobrescritura de archivos existentes mediante redirección de salida (`>`).

### `-H` (histexpand)
Habilita la sustitución de historial con `!`. Está activada por defecto en shells interactivas.

### `-P` (physical)
No sigue enlaces simbólicos al ejecutar comandos como `cd` que cambian el directorio actual.

### `-o <nombre_opción>`
Establece opciones por nombre largo. Algunas opciones importantes:
- `pipefail`: El valor de retorno de un pipeline es el estado del último comando que salió con un valor diferente de cero.
- `emacs`: Usa el estilo de edición de línea de emacs.
- `vi`: Usa el estilo de edición de línea de vi.
- `noclobber`: Igual que `-C`.
- `errexit`: Igual que `-e`.

## Combinaciones Comunes

- `bash -x script.sh` - Ejecuta un script mostrando cada comando (depuración)
- `bash -n script.sh` - Verifica la sintaxis sin ejecutar
- `bash -c "comando"` - Ejecuta un comando directamente
- `set -euo pipefail` - Combinación común para scripts más seguros: sale en error, variables no definidas causan error, y detecta errores en pipelines
