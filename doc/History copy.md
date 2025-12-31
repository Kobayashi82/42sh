# Historial de 42sh

## Estructura del Historial

El historial de 42sh mantiene dos componentes principales:

**Lista en memoria**: Comandos de la sesión actual que aún no se han guardado al disco.
**Archivo de historial**: Normalmente `~/.42sh_history`, donde se persisten los comandos entre sesiones.

### Anatomía de una entrada

Cada comando se almacena con:
- **Número de línea**: Posición en el historial (puede cambiar)
- **Timestamp** (opcional): Si `HISTTIMEFORMAT` está configurado
- **Comando**: El texto literal ejecutado

## Variables de Entorno Críticas

### `HISTFILE`
Ruta al archivo de historial. Por defecto: `~/.42sh_history`

```42sh
HISTFILE=~/.my_custom_history
```

### `HISTSIZE`
Número máximo de comandos en memoria durante la sesión.

```42sh
HISTSIZE=10000
```

### `HISTFILESIZE`
Número máximo de líneas que se guardan en el archivo al finalizar la sesión.

```42sh
HISTFILESIZE=20000
```

### `HISTTIMEFORMAT`
Formato de timestamp para cada comando. Si está vacío, no se guardan timestamps.

```42sh
HISTTIMEFORMAT="%F %T "  # Formato: YYYY-MM-DD HH:MM:SS
```

### `HISTCONTROL`
Controla qué comandos se guardan:
- `ignorespace`: Ignora comandos que empiezan con espacio
- `ignoredups`: Ignora comandos duplicados consecutivos
- `ignoreboth`: Combina ambos
- `erasedups`: Elimina duplicados en todo el historial

```42sh
HISTCONTROL=ignoreboth:erasedups
```

### `HISTIGNORE`
Patrón de comandos a ignorar (separados por `:`).

```42sh
HISTIGNORE="ls:cd:pwd:exit:clear"
```

## Navegación del Historial

### Atajos de teclado básicos

- **↑ / Ctrl+P**: Comando anterior
- **↓ / Ctrl+N**: Comando siguiente
- **Ctrl+R**: Búsqueda incremental hacia atrás
- **Ctrl+S**: Búsqueda incremental hacia adelante (requiere `stty -ixon`)
- **Alt+<**: Ir al primer comando del historial
- **Alt+>**: Ir al último comando (línea actual)

### Modo vi

Si usas `set -o vi`:
- **Esc + k**: Comando anterior
- **Esc + j**: Comando siguiente
- **Esc + /**: Búsqueda hacia atrás
- **Esc + ?**: Búsqueda hacia adelante

## Expansión del Historial (Event Designators)

La expansión del historial permite referenciar comandos previos usando `!`.

### Designadores de eventos

| Sintaxis | Descripción | Ejemplo |
|----------|-------------|---------|
| `!!` | Último comando | `sudo !!` |
| `!n` | Comando número n | `!532` |
| `!-n` | n comandos atrás | `!-3` |
| `!string` | Comando más reciente que empieza con "string" | `!vim` |
| `!?string?` | Comando más reciente que contiene "string" | `!?config?` |
| `^string1^string2^` | Último comando, reemplazando string1 por string2 | `^http^https^` |

### Designadores de palabras (Word Designators)

Extraen partes específicas de un comando previo. Se usan tras un designador de evento con `:`.

| Sintaxis | Descripción | Ejemplo |
|----------|-------------|---------|
| `!:0` | Comando (palabra 0) | `echo !-2:0` |
| `!:n` | n-ésima palabra (1-indexed) | `ls !vim:1` |
| `!:^` | Primera palabra (argumento 1) | `cat !:^` |
| `!:$` | Última palabra | `cd !:$` |
| `!:*` | Todas las palabras excepto la 0 | `echo !:*` |
| `!:n-m` | Rango de palabras | `!:2-4` |
| `!:n-$` | Desde palabra n hasta el final | `!:3-$` |
| `!:n*` | Desde palabra n hasta el final | `!:2*` |

**Atajos de teclado para palabras:**
- **Alt+.** o **Esc+.**: Inserta última palabra del comando anterior (repetir para ir hacia atrás)
- **Alt+Ctrl+Y**: Inserta primer argumento del comando anterior

### Modificadores

Los modificadores transforman el texto expandido. Se añaden tras el designador de palabra con `:`.

| Modificador | Descripción | Ejemplo |
|-------------|-------------|---------|
| `:h` | Head - elimina el último componente del path | `!$:h` → `/path/to` de `/path/to/file` |
| `:t` | Tail - mantiene solo el último componente | `!$:t` → `file` de `/path/to/file` |
| `:r` | Root - elimina la extensión | `!$:r` → `file` de `file.txt` |
| `:e` | Extension - mantiene solo la extensión | `!$:e` → `.txt` de `file.txt` |
| `:p` | Print - muestra sin ejecutar | `!vim:p` |
| `:q` | Quote - entrecomilla para evitar expansión | `!$:q` |
| `:x` | Quote por palabras (como :q pero cada palabra) | `!*:x` |
| `:s/old/new/` | Sustituye primera ocurrencia | `!$:s/http/https/` |
| `:gs/old/new/` | Sustituye todas las ocurrencias | `!$:gs/ /_/` |
| `:&` | Repite la última sustitución | `!$:&` |

**Combinaciones de modificadores:**
```42sh
!$:h:t      # Directorio padre de un archivo
!$:r:r      # Elimina dos extensiones (file.tar.gz → file)
```

## Búsqueda del Historial

### Búsqueda interactiva (Ctrl+R)

1. Presiona **Ctrl+R**
2. Escribe texto de búsqueda
3. **Ctrl+R** nuevamente para el siguiente resultado
4. **Enter** para ejecutar
5. **Ctrl+G** o **Esc** para cancelar
6. **→** o **Ctrl+E** para editar sin ejecutar

### Búsqueda con `history` y grep

```42sh
history | grep "docker"
history | grep "git commit" | tail -20
```

## Builtin: `history`

### Sintaxis básica
```42sh
history [n]              # Muestra últimos n comandos
history -c               # Limpia historial en memoria
history -d offset        # Elimina entrada en posición offset
history -a               # Añade nuevos comandos al archivo
history -r               # Lee el archivo y añade a memoria
history -w               # Escribe la memoria al archivo
history -n               # Lee comandos nuevos del archivo
history -p arg [arg...]  # Expansión de historial sin ejecutar
history -s arg [arg...]  # Añade argumentos como nueva entrada
```

### Opciones detalladas

**`-c` (clear)**: Borra todo el historial en memoria, pero no toca el archivo.

**`-d offset`**: Elimina la entrada específica. El offset puede ser:
- Positivo: Posición absoluta desde el inicio
- Negativo: Posición relativa desde el final

```42sh
history -d 450      # Elimina comando #450
history -d -2       # Elimina penúltimo comando
```

**`-a` (append)**: Añade las líneas nuevas de esta sesión al archivo `$HISTFILE`. No lee nada.

**`-r` (read)**: Lee el contenido del `$HISTFILE` y lo añade a la lista en memoria. Útil para sincronizar entre sesiones.

**`-w` (write)**: Sobrescribe el `$HISTFILE` con el contenido actual en memoria.

**`-n` (new)**: Lee líneas del `$HISTFILE` que no están en memoria y las añade.

**`-p` (print)**: Realiza expansión del historial sobre los argumentos y muestra el resultado sin añadirlo al historial.

```42sh
history -p !vim     # Muestra el último comando vim sin ejecutarlo
history -p !!:$     # Muestra la última palabra del último comando
```

**`-s` (store)**: Guarda los argumentos como una nueva entrada en el historial.

```42sh
history -s "comando inventado"   # Añade al historial sin ejecutar
```

### Ejemplos prácticos

```42sh
# Ver últimos 20 comandos
history 20

# Eliminar comando específico
history -d 1523

# Sincronizar historial entre terminales
history -a          # Guarda esta sesión
history -r          # Lee otras sesiones

# Limpiar todo
history -c
history -w          # Vacía el archivo también
```

## Builtin: `fc`

`fc` (fix command) es más potente que `history` para edición y re-ejecución.

### Sintaxis
```42sh
fc [-e editor] [-lnr] [first] [last]
fc -s [pat=rep] [command]
```

### Opciones principales

**`-l` (list)**: Lista comandos en lugar de editarlos.

```42sh
fc -l           # Lista últimos 16 comandos
fc -l 20 30     # Lista comandos del 20 al 30
fc -l -5        # Lista últimos 5 comandos
fc -l vim       # Lista desde último "vim" hasta el más reciente
```

**`-n`**: Suprime números de línea al listar.

```42sh
fc -ln -3       # Muestra últimos 3 comandos sin números
```

**`-r`**: Invierte el orden al listar.

```42sh
fc -lr -10      # Últimos 10 comandos en orden inverso
```

**`-e editor`**: Especifica el editor a usar (por defecto `$FCEDIT` o `$EDITOR`).

```42sh
fc -e vim 100   # Edita comando #100 en vim
fc -e nano      # Edita último comando en nano
```

**`-s` (substitute)**: Re-ejecuta comando con sustitución.

```42sh
fc -s           # Re-ejecuta último comando
fc -s vim       # Re-ejecuta último comando que empieza con "vim"
fc -s http=https    # Re-ejecuta último comando cambiando http por https
fc -s old=new 523   # Re-ejecuta comando #523 con sustitución
```

### Rangos en fc

Los argumentos `first` y `last` definen el rango:
- **Número**: Posición absoluta
- **Negativo**: Offset desde el final
- **String**: Último comando que empieza con ese string

```42sh
fc 100 105      # Edita comandos 100-105
fc -5 -1        # Edita últimos 5 comandos
fc vim git      # Edita desde último "vim" hasta último "git"
```

### Ejemplos prácticos

```42sh
# Editar y re-ejecutar último comando
fc

# Ver historial de forma limpia
fc -ln -50

# Re-ejecutar comando corrigiendo un typo
fc -s tset=test

# Editar bloque de comandos relacionados
fc vim1 vim4    # Si tienes varios comandos vim numerados

# Listar comandos de hoy (requiere HISTTIMEFORMAT)
fc -l | grep "$(date +%Y-%m-%d)"
```

## Opciones del Historial (`shopt`)

42sh tiene opciones adicionales para controlar el comportamiento del historial con `shopt`.

### Opciones disponibles

**`histappend`**: Añade al archivo de historial en lugar de sobrescribirlo al cerrar la sesión.

```42sh
shopt -s histappend     # Activar (recomendado)
```

**`cmdhist`**: Guarda comandos multi-línea como una sola entrada.

```42sh
shopt -s cmdhist
```

**`lithist`**: Si cmdhist está activo, guarda comandos multi-línea con newlines embebidos en lugar de punto y coma.

```42sh
shopt -s lithist
```

**`histreedit`**: Si una expansión del historial falla, permite re-editar.

```42sh
shopt -s histreedit
```

**`histverify`**: Muestra resultado de expansión del historial sin ejecutar inmediatamente (permite editar).

```42sh
shopt -s histverify
```

### Verificar estado

```42sh
shopt -p | grep hist    # Ver todas las opciones de historial
shopt histappend        # Ver estado de opción específica
```

## Configuración Recomendada

Añade esto a tu `~/.42shrc`:

```42sh
# Tamaño del historial
HISTSIZE=50000
HISTFILESIZE=50000

# No guardar duplicados ni comandos que empiezan con espacio
HISTCONTROL=ignoreboth:erasedups

# Ignorar comandos comunes
HISTIGNORE="ls:ll:cd:pwd:exit:clear:history"

# Timestamps en historial
HISTTIMEFORMAT="%F %T "

# Añadir al historial en lugar de sobrescribir
shopt -s histappend

# Guardar comandos multi-línea como una entrada
shopt -s cmdhist

# Verificar expansiones antes de ejecutar
shopt -s histverify

# Actualizar historial después de cada comando (opcional pero útil)
PROMPT_COMMAND="history -a; history -n"
```

## Sincronización entre Sesiones

Por defecto, cada terminal mantiene su propio historial en memoria y solo lo escribe al cerrar. Para sincronizar en tiempo real:

```42sh
# En ~/.42shrc
PROMPT_COMMAND="history -a; history -c; history -r"
```

- `history -a`: Añade comandos nuevos al archivo
- `history -c`: Limpia historial en memoria
- `history -r`: Recarga desde el archivo

**Advertencia**: Esto puede ser lento si tienes un historial muy grande.

## Trucos Avanzados

### Ejecutar comando n sin añadirlo al historial

```42sh
 comando_secreto    # Espacio inicial
```

(Requiere `HISTCONTROL=ignorespace`)

### Buscar y ejecutar con confirmación

```42sh
!vim:p              # Muestra sin ejecutar
!!                  # Si está bien, ejecuta
```

O activa `histverify`:
```42sh
shopt -s histverify
!vim                # Se muestra para editar, Enter ejecuta
```

### Reusar argumentos de forma eficiente

```42sh
mkdir /tmp/test
cd !$               # cd /tmp/test

vim archivo.txt
cat !$              # cat archivo.txt
nano !^             # nano archivo.txt (primer argumento de vim)
```

### Historial compartido en tmux

```42sh
# En ~/.42shrc
if [ -n "$TMUX" ]; then
    PROMPT_COMMAND="history -a; history -n"
fi
```

### Backup del historial

```42sh
# Copia de seguridad periódica
cp ~/.42sh_history ~/.42sh_history.backup.$(date +%Y%m%d)

# O en ~/.42shrc para backup automático
if [ ! -f ~/.42sh_history.backup.$(date +%Y%m%d) ]; then
    cp ~/.42sh_history ~/.42sh_history.backup.$(date +%Y%m%d)
fi
```

## Offsets y Numeración

### Offsets absolutos vs relativos

**Absolutos**: Número de línea en el historial (empieza en 1).
```42sh
history     # Muestra números absolutos
!532        # Ejecuta comando #532
```

**Relativos**: Offset desde el comando actual.
```42sh
!-1         # Último comando (equivale a !!)
!-5         # 5 comandos atrás
history -d -2   # Elimina penúltimo comando
```

### Cómo cambian los números

Los números absolutos cambian cuando:
- Se elimina un comando: `history -d 100`
- Se limpia el historial: `history -c`
- Se reinicia la shell y se relee: `history -r`

Los números relativos siempre son estables porque son posiciones desde "ahora".

### Event counter interno

42sh mantiene un contador interno de eventos que se incrementa con cada comando. Este número puede ser diferente de la posición en la lista si has eliminado comandos.

```42sh
echo $HISTCMD       # Número del comando actual
```

## Solución de Problemas

### El historial no se guarda

1. Verifica que `HISTFILE` apunte a un archivo escribible
2. Comprueba `HISTFILESIZE` (si es 0, no guarda)
3. Asegúrate de que la shell termine normalmente (no con `kill -9`)

### Historial duplicado entre sesiones

- Usa `HISTCONTROL=erasedups`
- O implementa sincronización con `PROMPT_COMMAND`

### Comando no aparece en el historial

Posibles causas:
- Empieza con espacio y tienes `ignorespace`
- Coincide con patrón en `HISTIGNORE`
- `HISTSIZE` es 0
- El comando falló y tienes alguna configuración personalizada

### Expansión del historial no funciona

- En scripts, está desactivada por defecto (requiere `set -H`)
- En modo interactivo, verifica: `set -o | grep histexpand`
- Para activar: `set -o histexpand` o `set -H`

## Seguridad

### Evitar almacenar contraseñas

```42sh
# Prefijo con espacio
 mysql -u root -p'secret'

# O mejor, usa variables de entorno
read -s PASSWORD
mysql -u root -p"$PASSWORD"

# O HISTIGNORE
HISTIGNORE="*password*:*secret*"
```

### Limpiar historial después de errores

```42sh
# Si ejecutaste comando con contraseña
history -d -1       # Elimina último comando
history -w          # Guarda cambios
```

### Historial separado para comandos sensibles

```42sh
# En shell temporal
unset HISTFILE      # No guarda nada
# o
HISTFILE=/dev/null  # Descarta todo
```

## Referencias Rápidas

### Variables principales
- `HISTFILE`, `HISTSIZE`, `HISTFILESIZE`
- `HISTCONTROL`, `HISTIGNORE`, `HISTTIMEFORMAT`
- `HISTCMD` (solo lectura)

### Comandos builtin
- `history`: Gestión básica
- `fc`: Edición y re-ejecución avanzada

### Opciones shopt
- `histappend`, `cmdhist`, `lithist`
- `histreedit`, `histverify`

### Atajos de teclado esenciales
- `Ctrl+R`: Búsqueda incremental
- `Alt+.`: Última palabra comando anterior
- `Ctrl+P/N`: Navegación arriba/abajo