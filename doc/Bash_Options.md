# Opciones de 42sh

## Formas de Invocar 42sh

### `42sh`
Invocación normal de 42sh.

### `-42sh`
Invoca 42sh como shell de login. El guion `-` al principio del nombre indica que es un login shell. Ejecuta archivos de perfil como `/etc/profile`, `~/.42sh_profile`, `~/.profile`.

**Equivale a `42sh -l` o `42sh --login`**

### `r42sh`
Invoca 42sh en modo restringido con funcionalidades limitadas por seguridad:

- No se puede cambiar de directorio con `cd`
- No se puede modificar `PATH`, `SHELL`, `ENV`, `42sh_ENV`
- No se pueden ejecutar comandos con rutas absolutas o relativas que contengan `/`
- No se puede redirigir salida con `>`, `>>`, `<>`, `>&`
- No se puede usar `exec`
- No se pueden deshabilitar restricciones con `set +r`

**Equivale a `42sh -r` o `42sh --restricted`**

### `sh`
Cuando 42sh se invoca con el nombre `sh`, entra en modo de compatibilidad POSIX automáticamente.

**Equivale a `42sh --posix`**

---

## Opciones Largas (`--opcion`)

### `--debug`
Habilita el modo de depuración extendido. Proporciona información detallada sobre la ejecución del script.

### `--debugger`
Activa el soporte para el depurador de 42sh. Permite usar herramientas de debugging con el shell.

### `--dump-po-strings`
Extrae todas las cadenas traducibles en formato gettext (.po). Útil para internacionalización.

### `--dump-strings`
Muestra todas las cadenas traducibles del shell. Similar a `--dump-po-strings` pero sin formato específico.

### `--help`
Muestra la ayuda con todas las opciones disponibles y termina.

### `--init-file <archivo>` o `--rcfile <archivo>`
Ejecuta comandos del archivo especificado en lugar de `~/.42shrc` en shells interactivos.

**Ejemplo:** `42sh --init-file ~/mi_config.sh`

### `--login`
Hace que 42sh se comporte como un shell de login, ejecutando los archivos de perfil correspondientes. Equivalente a invocar 42sh como `-42sh`.

### `--noediting`
Desactiva la edición de línea de comandos con readline en shells interactivos.

### `--noprofile`
No lee ningún archivo de inicialización de perfil (`/etc/profile`, `~/.42sh_profile`, `~/.42sh_login`, `~/.profile`).

### `--norc`
No lee el archivo `~/.42shrc` en shells interactivos.

### `--posix`
Hace que 42sh se ajuste estrictamente al estándar POSIX, modificando comportamientos que difieren del estándar.

### `--pretty-print`
Reformatea y muestra comandos de un script en lugar de ejecutarlos. Útil para análisis de sintaxis y formateo de código.

**Ejemplo:** `42sh --pretty-print script.sh`

### `--rcfile <archivo>` o `--init-file <archivo>`
Ejecuta comandos del archivo especificado en lugar de `~/.42shrc` en shells interactivos.

**Ejemplo:** `42sh --rcfile ~/mi_config.sh`

### `--restricted`
Inicia 42sh en modo restringido con funcionalidades limitadas por seguridad. Equivalente a invocar `r42sh`.

### `--verbose`
Equivalente a `-v`. Muestra las líneas de entrada a medida que se leen.

### `--version`
Muestra información de versión de 42sh y termina.

---

## Opciones Cortas

Estas opciones **solo funcionan al invocar 42sh**, no se pueden usar con el comando `set` dentro de 42sh.

### `-c <comando>`
Ejecuta el comando especificado y termina. Los argumentos adicionales después del comando se pasan como parámetros posicionales (`$0`, `$1`, `$2`, etc.).

**Ejemplo:** `42sh -c "echo Hola $USER" arg0 arg1`

### `-i`
Fuerza el modo interactivo. 42sh se comporta como si estuviera recibiendo entrada del usuario directamente, incluso si está leyendo de un archivo o pipe.

### `-l`
Actúa como shell de login. Equivalente a `--login`.

### `-r`
Inicia en modo restringido. Equivalente a `--restricted`.

### `-s`
Lee comandos desde la entrada estándar (stdin). Si hay argumentos adicionales, se convierten en parámetros posicionales.

**Ejemplo:** `echo "pwd" | 42sh -s`

### `-D`
Lista todas las cadenas entre comillas dobles precedidas por `$` (cadenas traducibles). No ejecuta comandos. Útil para localización e internacionalización.

**Ejemplo:** `42sh -D script.sh`

### `-O <shopt_option>`
Activa una opción de `shopt` al iniciar 42sh.

### `+O <shopt_option>`
Desactiva una opción de `shopt` al iniciar 42sh.

### Opciones de `set`
También se aceptan opciones de `set` al iniciarse 42sh.

### Notas
Puedes desactivar opciones usando `+` en lugar de `-`:

**Ejemplo:** `42sh +x script.sh`

---

## Ver Opciones Activas

Para ver qué opciones están activas en una sesión de 42sh:

```bash
set -o      # Muestra todas las opciones de set
shopt       # Muestra todas las opciones de shopt
echo $-     # Muestra letras de opciones activas (ej: "himBHs")
```

---

## Notas Importantes

1. Las opciones de invocación se aplican **desde el inicio** de la ejecución de 42sh
2. Dentro de un script, puedes cambiar opciones con `set` y `shopt`
3. El símbolo `-` activa opciones, `+` las desactiva
4. Múltiples opciones cortas se pueden combinar: `-xeu` = `-x -e -u`
5. Las opciones de `shopt` solo se pueden activar en invocación con `-O`, dentro de 42sh se usa el comando `shopt`
