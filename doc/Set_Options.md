## Opciones de Shell (set)

Estas opciones se pueden activar con `set -<opción>` o desactivar con `set +<opción>`:

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

### `-E` (errtrace)
Si está activada, la trampa ERR se hereda por funciones de shell.

### `-H` (histexpand)
Habilita la sustitución de historial con `!`. Está activada por defecto en shells interactivas.

### `-P` (physical)
No sigue enlaces simbólicos al ejecutar comandos como `cd` que cambian el directorio actual.

### `-T` (functrace)
Si está activada, las trampas DEBUG y RETURN se heredan por funciones de shell.

## Opciones especiales sin letra

### `--`
Asigna los argumentos restantes a los parámetros posicionales. Si no quedan argumentos, los parámetros posicionales se desactivan (unset).

### `-`
Asigna los argumentos restantes a los parámetros posicionales. Las opciones -x y -v se desactivan.

### `-o <nombre_opción>`
Establece opciones por nombre largo. Algunas opciones importantes:
`allexport` Igual que -a.
`braceexpand` Igual que -B.
`emacs` Usa el estilo de edición de línea de emacs.
`errexit` Igual que -e.
`errtrace` Igual que -E.
`functrace` Igual que -T.
`hashall` Igual que -h.
`histexpand` Igual que -H.
`history` Habilita el historial de comandos.
`ignoreeof` La shell no sale al leer EOF (Ctrl+D).
`interactive-comments` Permite comentarios en shells interactivas.
`keyword` Igual que -k.
`monitor` Igual que -m.
`noclobber` Igual que -C.
`noexec` Igual que -n.
`noglob` Igual que -f.
`nolog` (Actualmente ignorada).
`notify` Igual que -b.
`nounset` Igual que -u.
`onecmd` Igual que -t.
`physical` Igual que -P.
`pipefail` El valor de retorno de un pipeline es el estado del último comando que salió con un valor diferente de cero (o cero si todos tuvieron éxito).
`posix` Cambia el comportamiento de bash para coincidir con el estándar POSIX.
`privileged` Igual que -p.
`verbose` Igual que -v.
`vi` Usa el estilo de edición de línea de vi.
`xtrace` Igual que -x.

## Combinaciones Comunes

- `bash -x script.sh` - Ejecuta un script mostrando cada comando (depuración)
- `bash -n script.sh` - Verifica la sintaxis sin ejecutar
- `bash -c "comando"` - Ejecuta un comando directamente
- `set -euo pipefail` - Combinación común para scripts más seguros: sale en error, variables no definidas causan error, y detecta errores en pipelines
