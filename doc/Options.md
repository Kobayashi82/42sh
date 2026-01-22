# Opciones de 42sh

Permiten configurar el comportamiento del shell y controlar distintas características, como el manejo de errores, la expansión de patrones, el historial o la visualización de variables y funciones.

- Las opciones de `set` configuran el shell y parámetros posicionales.  
- Las opciones de `shopt` configuran comportamientos internos específicos del shell.  

### Tabla de Opciones

| Opción larga           | Opción | `set` | `shopt` | Descripción                                                                                  |
|------------------------|--------|-------|---------|----------------------------------------------------------------------------------------------|
| `allexport`            | `-a`   | ✓     | ✓      | Exporta automáticamente todas las variables modificadas o creadas                             |
| `assoc_expand_once`    | -      | ✗     | ✓      | Expande variables asociativas solo una vez en asignaciones                                    |
| `autocd`               | -      | ✗     | ✓      | Un nombre de directorio solo se ejecuta como `cd` a ese directorio                            |
| `braceexpand`          | `-B`   | ✓     | ✓      | Habilita la expansión de llaves `{a,b,c}`                                                     |
| `cdable_vars`          | -      | ✗     | ✓      | Si `cd` falla, intenta usar el argumento como nombre de variable                              |
| `cdspell`              | -      | ✗     | ✓      | Corrige errores menores de ortografía en paths con `cd`                                       |
| `checkhash`            | -      | ✗     | ✓      | Verifica que los comandos hasheados existan antes de ejecutarlos                              |
| `checkjobs`            | -      | ✗     | ✓      | Lista trabajos detenidos/en background antes de salir del shell                               |
| `checkwinsize`         | -      | ✗     | ✓      | Actualiza `LINES` y `COLUMNS` después de cada comando                                         |
| `complete_fullquote`   | -      | ✗     | ✓      | Comillas todos los metacaracteres en completado de nombres de archivo                         |
| `direxpand`            | -      | ✗     | ✓      | Expande nombres de directorio durante completado de línea                                     |
| `dirspell`             | -      | ✗     | ✓      | Corrige errores menores de ortografía en nombres de directorio                                |
| `dotglob`              | -      | ✗     | ✓      | Incluye archivos que empiezan con `.` en expansión de pathname                                |
| `emacs`                | -      | ✓     | ✓      | Usa modo de edición de línea estilo emacs                                                     |
| `errexit`              | `-e`   | ✓     | ✓      | Sale inmediatamente si un comando retorna status distinto de cero                             |
| `errtrace`             | `-E`   | ✓     | ✓      | Hereda trap `ERR` en funciones y subshells                                                    |
| `execfail`             | -      | ✗     | ✓      | El shell no sale si `exec` falla                                                              |
| `expand_aliases`       | -      | ✗     | ✓      | Expande aliases (activado por defecto en shells interactivos)                                 |
| `extdebug`             | -      | ✗     | ✓      | Habilita comportamiento de depuración extendido para el debugger                              |
| `extglob`              | -      | ✗     | ✓      | Habilita patrones extendidos: `?(patron)`, `*(patron)`, `+(patron)`, `@(patron)`, `!(patron)` |
| `extquote`             | -      | ✗     | ✓      | Habilita sustitución `$'string'` y `$"string"` dentro de expansión de parámetros              |
| `failglob`             | -      | ✗     | ✓      | Falla si un patrón glob no encuentra coincidencias                                            |
| `force_fignore`        | -      | ✗     | ✓      | Fuerza el uso de sufijos en `FIGNORE` al completar palabras                                   |
| `functrace`            | `-T`   | ✓     | ✓      | Hereda trap `DEBUG` y `RETURN` en funciones                                                   |
| `globasciiranges`      | -      | ✗     | ✓      | Los rangos en patrones `[a-z]` usan orden ASCII en vez de locale                              |
| `globskipdots`         | -      | ✗     | ✓      | Los patrones de glob nunca coinciden con `.` y `..` incluso si el patrón empieza con punto    |
| `globstar`             | -      | ✗     | ✓      | El patrón `**` hace matching recursivo de directorios                                         |
| `gnu_errfmt`           | -      | ✗     | ✓      | Mensajes de error en formato estándar GNU                                                     |
| `hashall`              | `-h`   | ✓     | ✓      | Guarda la ubicación de comandos en tabla hash al buscarlos                                    |
| `histappend`           | -      | ✗     | ✓      | Añade al archivo de historial en vez de sobrescribirlo                                        |
| `histexpand`           | `-H`   | ✓     | ✗      | Habilita sustitución de historial estilo `!`                                                  |
| `history`              | -      | ✓     | ✗      | Habilita el historial de comandos                                                             |
| `histreedit`           | -      | ✗     | ✓      | Permite reeditar una sustitución de historial fallida                                         |
| `histverify`           | -      | ✗     | ✓      | Carga el resultado de sustitución de historial en el buffer de edición                        |
| `hostcomplete`         | -      | ✗     | ✓      | Intenta completar hostname cuando hay `@`                                                     |
| `huponexit`            | -      | ✗     | ✓      | Envía `SIGHUP` a todos los jobs al salir de un shell interactivo de login                     |
| `ignoreeof`            | -      | ✓     | ✓      | El shell no sale al recibir `EOF` (requiere `exit` o `logout`)                                |
| `inherit_errexit`      | -      | ✗     | ✓      | Subshells heredan el estado de `errexit`                                                      |
| `interactive_comments` | -      | ✗     | ✓      | Permite comentarios `#` en comandos interactivos                                              |
| `lastpipe`             | -      | ✗     | ✓      | Ejecuta el último comando de un pipeline en el shell actual (no en subshell)                  |
| `localvar_inherit`     | -      | ✗     | ✓      | Variables locales heredan el valor de variables con el mismo nombre en scope superior         |
| `localvar_unset`       | -      | ✗     | ✓      | Variables locales heredan el estado unset de variables en scope superior                      |
| `login_shell`          | -      | ✗     | ✓      | **Solo lectura**. Indica si el shell es un shell de login                                     |
| `monitor`              | `-m`   | ✓     | ✓      | Habilita control de jobs                                                                      |
| `no_empty_cmd_completion` | -   | ✗     | ✓      | No intenta completar comandos en una línea vacía (al presionar TAB)                           |
| `nocaseglob`           | -      | ✗     | ✓      | Expansión de pathname ignora mayúsculas/minúsculas                                            |
| `nocasematch`          | -      | ✗     | ✓      | Matching de patrones ignora mayúsculas/minúsculas en `case` y `[[`                            |
| `noclobber`            | `-C`   | ✓     | ✓      | Previene sobrescribir archivos existentes con redirección `>`                                 |
| `noexec`               | `-n`   | ✓     | ✓      | Lee comandos pero no los ejecuta (útil para verificar sintaxis)                               |
| `noglob`               | `-f`   | ✓     | ✓      | Desactiva expansión de pathname (globbing)                                                    |
| `notify`               | `-b`   | ✓     | ✓      | Reporta status de jobs en background inmediatamente                                           |
| `nounset`              | `-u`   | ✓     | ✓      | Trata variables no definidas como error al expandirlas                                        |
| `nullglob`             | -      | ✗     | ✓      | Patrones que no hacen match se expanden a string nulo (en vez de literal)                     |
| `patsub_replacement`   | -      | ✗     | ✓      | En `${var//pattern/replacement}`, el `&` en replacement referencia al texto que coincidió     |
| `physical`             | `-P`   | ✓     | ✓      | No sigue enlaces simbólicos al ejecutar comandos como `cd`                                    |
| `pipefail`             | -      | ✓     | ✗      | El valor de retorno de un pipeline es el del último comando que falló                         |
| `privileged`           | `-p`   | ✓     | ✗      | Modo privilegiado/restringido                                                                 |
| `progcomp`             | -      | ✗     | ✓      | Habilita facilidades de completado programable                                                |
| `progcomp_alias`       | -      | ✗     | ✓      | Intenta completado programable en aliases                                                     |
| `promptvars`           | -      | ✗     | ✓      | Expande variables y sustitución de comandos en strings de prompt                              |
| `restricted_shell`     | -      | ✗     | ✓      | **Solo lectura**. Indica si el shell está en modo restringido                                 |
| `shift_verbose`        | -      | ✗     | ✓      | `shift` imprime error si el conteo excede parámetros posicionales                             |
| `sourcepath`           | -      | ✗     | ✓      | Usa `PATH` para buscar el argumento del comando `source`                                      |
| `verbose`              | `-v`   | ✓     | ✓      | Imprime líneas de entrada del shell conforme se leen                                          |
| `vi`                   | -      | ✓     | ✓      | Usa modo de edición de línea estilo vi                                                        |
| `xpg_echo`             | -      | ✗     | ✓      | `echo` expande secuencias de escape estilo backslash sin necesidad de `-e`                    |
| `xtrace`               | `-x`   | ✓     | ✓      | Imprime comandos y sus argumentos conforme se ejecutan (debug)                                |

---

## `SET`

Permite configurar opciones del shell y ver el estado de variables, funciones y parámetros posicionales.

### Opciones

| Opción larga         | Opción | Descripción                                                                             |
|----------------------|--------|-----------------------------------------------------------------------------------------|
| -                    | `--`   | Marca el final de las opciones                                                          |
| -                    | `-`    | Marca el final de las opciones y desactiva las opciones `xtrace` y `verbose`            |
| -                    | `-o`   | Activa la opción larga indicada o muestra todas las opciones con su estado              |
| -                    | `+o`   | Desactiva la opción larga indicada o muestra todas las opciones en formato reutilizable |
| `positional`         | `-S`   | Muestra todos los parámetros posicionales del shell                                     |
| `variables`          | `-V`   | Muestra todas las variables (locales y exportadas)                                      |
| `variables-exported` | `-X`   | Muestra las variables exportadas                                                        |
| `variables-local`    | `-L`   | Muestra todas las variables locales                                                     |
| `functions`          | `-F`   | Muestra todas las funciones                                                             |
| `functions-name`     | `-N`   | Muestra solo los nombres de las funciones                                               |
|

```bash
# Activar opciones:
set -o opcion_larga
set -opcion_corta

# Desactivar opciones:
set +o opcion_larga
set +opcion_corta
```

### Parámetros Posicionales

Los parámetros posicionales representan los argumentos pasados a `42sh`, `script` o una `función`:

| Variable | Descripción                                                               |
|----------|---------------------------------------------------------------------------|
|`$0`      | Nombre del shell o script                                                 |
|`$[N]`    | Párametro posicional indicado por el número `N` y debe ser mayor que `0`  |
|`$#`      | Número de parámetros posicionales                                         |
|`$*`      | Todos los parámetros posicionales como una sola cadena                    |
|`$@`      | Todos los parámetros posicionales como lista separada (preserva espacios) |
|`shift`   | Builtin usado para desplazar parámetros posicionales                      |
|

```bash
# Mostrar los parámetros posicionales:
set --positional
set -S

# Modificar parámetros posicionales:
set arg1 arg2 arg3
set -- arg1 arg2 arg3
set - arg1 arg2 arg3        # Se desactivan las opciones '-x' y '-v'

# Eliminar parámetros posicionales:
set --
set -                       # Se desactivan las opciones '-x' y '-v'
```

## Modo `privileged`

Se activa con `42sh -p` o `42sh -o privileged` o una vez iniciado el shell con `set -p`, `set -o privileged`  
También se activa automáticamente cuando se detecta:

- user id ≠ real user id
- group id ≠ real group id

Cuando está activado, se restringe el acceso a variables de entorno peligrosas:

- $42_ENV - archivo ejecutado al iniciar
- $ENV - similar a 42_ENV
- $SHELLOPTS - opciones del shell heredadas
- $42OPTS - opciones de 42sh heredadas
- $CDPATH - rutas para el comando cd
- $GLOBIGNORE - patrones de globbing ignorados
- Funciones de shell del entorno - no se heredan

---

## `SHOPT`

Permite configurar opciones del shell específicas de `shopt` y ver su estado.

### Opciones

| Opción | Descripción                                                                                   |
|--------|-----------------------------------------------------------------------------------------------|
| `--`   | Marca el final de las opciones                                                                |
| `-o`   | Restringe las opciones a las compartidas con `set` o muestra todas las opciones con su estado |
| `-p`   | Muestra todas las opciones en formato reutilizable                                            |
| `-q`   | Establece el código de salida según el estado de la opción (0 si activada o 1 si desactivada) |
| `-s`   | Activa la opción indicada                                                                     |
| `-u`   | Desactiva la opción indicada                                                                  |
|

```bash
# Activar opciones:
shopt -s opcion_larga
shopt -s -o opcion_larga    # Válido solo para opciones compartidas con 'set'

# Desactivar opciones:
shopt -u opcion_larga
shopt -u -o opcion_larga    # Válido solo para opciones compartidas con 'set'
```

---

## Usos Comunes

```bash
42sh -x script.sh           # Ejecuta un script mostrando cada comando (depuración)
42sh -n script.sh           # Verifica la sintaxis sin ejecutar
42sh -c "comando"           # Ejecuta un comando directamente
set -euo pipefail           # Para scripts más seguros: sale en error, variables no definidas causan error, y detecta errores en pipelines
```
| Opción               | Descripción           |
|----------------------|-----------------------|
Permite que palabras reservadas (como `if`, `while`, `for`) se reconozcan en cualquier posición de un comando, no solo al principio |
Evita que las definiciones de funciones se guarden en el historial de comandos |
El shell sale después de leer y ejecutar un solo comando (usado principalmente para testing) |
Cambia el comportamiento de Bash para que cumpla estrictamente con el estándar POSIX cuando difiere del comportamiento normal |
Guarda comandos multi-línea como una sola entrada en el historial en lugar de múltiples líneas separadas |
Activa modo de compatibilidad con Bash 3.1 (restaura comportamientos antiguos) |
Activa modo de compatibilidad con Bash 3.2 (restaura comportamientos antiguos) |
Activa modo de compatibilidad con Bash 4.0 (restaura comportamientos antiguos) |
Activa modo de compatibilidad con Bash 4.1 (restaura comportamientos antiguos) |
Activa modo de compatibilidad con Bash 4.2 (restaura comportamientos antiguos) |
Activa modo de compatibilidad con Bash 4.3 (restaura comportamientos antiguos) |
Activa modo de compatibilidad con Bash 4.4 (restaura comportamientos antiguos) |
Guarda comandos multi-línea en el historial con newlines embebidos en lugar de usar punto y coma como separador |
Muestra un mensaje de advertencia si se accede a un archivo de correo desde la última vez que fue revisado |
Desactiva la traducción de cadenas $"..." (gettext) para internacionalización |
Hace que el shell cierre automáticamente los file descriptors asignados mediante redirección de variables cuando la variable sale de scope |

## No implementados

| Opción               | Descripción                                                                                                              |
|----------------------|--------------------------------------------------------------------------------------------------------------------------|
| keyword              | Permite que palabras reservadas se reconozcan en cualquier posición de un comando, no solo al principio                  |
| nolog                | Evita que las definiciones de funciones se guarden en el historial de comandos                                           |
| onecmd               | El shell sale después de leer y ejecutar un solo comando (usado principalmente para testing)                             |
| posix                | Cumple estrictamente con el estándar POSIX                                                                               |
| cmdhist              | Guarda comandos multi-línea como una sola entrada en el historial en lugar de múltiples líneas separadas                 |
| compat31             | Activa modo de compatibilidad con Bash 3.1 (restaura comportamientos antiguos)                                           |
| compat32             | Activa modo de compatibilidad con Bash 3.2 (restaura comportamientos antiguos)                                           |
| compat40             | Activa modo de compatibilidad con Bash 4.0 (restaura comportamientos antiguos)                                           |
| compat41             | Activa modo de compatibilidad con Bash 4.1 (restaura comportamientos antiguos)                                           |
| compat42             | Activa modo de compatibilidad con Bash 4.2 (restaura comportamientos antiguos)                                           |
| compat43             | Activa modo de compatibilidad con Bash 4.3 (restaura comportamientos antiguos)                                           |
| compat44             | Activa modo de compatibilidad con Bash 4.4 (restaura comportamientos antiguos)                                           |
| lithist              | Guarda comandos multi-línea en el historial con newlines embebidos en lugar de usar punto y coma como separador          |
| mailwarn             | Muestra un mensaje de advertencia si se accede a un archivo de correo desde la última vez que fue revisado               |
| noexpand_translation | Desactiva la traducción de cadenas $"..." (gettext) para internacionalización                                            |
| varredir_close       | Cierre automáticamente los file descriptors asignados mediante redirección de variables cuando la variable sale de scope |
|
