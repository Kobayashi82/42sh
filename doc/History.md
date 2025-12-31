# Historial de 42sh

## Estructura del Historial

El historial de 42sh se compone de:

**Lista en memoria**: Incluye comandos cargados desde el archivo y comandos ejecutados en la sesión.  
**Archivo de historial**: Almacena comandos de forma persistente entre sesiones. Por defecto `~/.42sh_history`.

Cada comando se almacena con:

- `Line`: Comando almacenado
- `Timestamp`: Fecha y hora en que se almacenó el comando
- `Event`: Número del evento asignado al comando
- `Length`: Longitud del comando almacenado

## Variables de Entorno

```bash
42_HISTFILE=~/.my_custom_history      # Ruta al archivo de historial. Por defecto: `~/.42sh_history`
42_HISTSIZE=1000                      # Número máximo de comandos en memoria durante la sesión
42_HISTFILESIZE=2000                  # Número máximo de líneas que se guardan en el archivo al finalizar la sesión
42_HISTCONTROL=ignoreboth:erasedups   # Controla como se guardan los comandos en el historial
42_HISTIGNORE="ls:cd:pwd:exit:clear"  # Patrón de comandos a ignorar (separados por `:`)
42_HISTTIMEFORMAT="%F %T "            # Formato de timestamp para cada comando (YYYY-MM-DD HH:MM:SS)
$42_HISTCMD                           # Variable con el numero de comando próximo (readonly)
```

- El prompt puedes usar `\!` y `\#` (`PS1='\!\#'`) que es número de comando y número de evento.

### Opciones para `42_HISTCONTROL`

- `ignorespace`: Ignora comandos que empiezan con espacio
- `ignoredups`: Ignora comandos duplicados consecutivos
- `ignoreboth`: Combina ambos
- `erasedups`: Elimina duplicados en todo el historial

### Notas sobre `42_HISTIGNORE`

- Los patrones se separan por el caracter `:`
- Se permiten `wildcards` para los comandos
- Se cuenta desde el principio del comando
- El caracter `&` tiene la misma funcionalidad que `ignoredups`
- Los espacios son literales
- Se determina si se añade antes del lexer y despues de la expasión de historial

### Formatos disponibles para `42_HISTTIMEFORMAT`

| Código | Significado                          | Ejemplo                |
|--------|--------------------------------------|------------------------|
| %Y     | Año con siglo                        | 2025                   |
| %y     | Año sin siglo                        | 25                     |
| %m     | Mes (01–12)                          | 12                     |
| %d     | Día del mes (01–31)                  | 31                     |
| %e     | Día del mes (1–31, espacio)          |  9                     |
| %H     | Hora 24h (00–23)                     | 18                     |
| %I     | Hora 12h (01–12)                     | 06                     |
| %M     | Minutos (00–59)                      | 42                     |
| %S     | Segundos (00–60)                     | 07                     |
| %p     | AM / PM                              | PM                     |
| %R     | Hora 24h HH:MM                       | 18:42                  |
| %T     | Hora 24h HH:MM:SS                    | 18:42:07               |
| %F     | Fecha ISO (YYYY-MM-DD)               | 2025-12-31             |
| %D     | Fecha MM/DD/YY                       | 12/31/25               |
| %c     | Fecha y hora locale                  | Wed Dec 31 18:42:07    |
| %x     | Fecha locale                         | 31/12/25               |
| %X     | Hora locale                          | 18:42:07               |
| %a     | Día abreviado                        | Wed                    |
| %A     | Día completo                         | Wednesday              |
| %b     | Mes abreviado                        | Dec                    |
| %B     | Mes completo                         | December               |
| %j     | Día del año (001–366)                | 365                    |
| %U     | Semana del año (domingo primero)     | 52                     |
| %W     | Semana del año (lunes primero)       | 52                     |
| %Z     | Zona horaria                         | CET                    |
| %%     | Literal '%'                          | %                      |
|

## Navegación del Historial

- `↑` / `Ctrl+P`: Comando anterior
- `↓` / `Ctrl+N`: Comando siguiente
- `Ctrl+R`: Búsqueda incremental hacia atrás
- `Ctrl+S`: Búsqueda incremental hacia adelante
- `k`: Comando anterior (modo vi)
- `j`: Comando siguiente (modo vi)

## Búsqueda interactiva (Ctrl+R)

1. Presiona `Ctrl+R`
2. Escribe texto de búsqueda
3. `Ctrl+R` nuevamente para el siguiente resultado
4. `Enter` para ejecutar
5. `Ctrl+G` o `Esc` para cancelar
6. `→` o `Ctrl+E` para editar sin ejecutar

## Expansión del Historial

La expansión del historial permite referenciar comandos previos usando `!`.

| Sintaxis            | Descripción                                      | Ejemplo        |
|---------------------|--------------------------------------------------|----------------|
| `!!`                | Último comando                                   | `sudo !!`      |
| `!n`                | Comando número n                                 | `!532`         |
| `!-n`               | n comandos atrás                                 | `!-3`          |
| `!string`           | Comando más reciente que empieza con "string"    | `!vim`         |
| `!?string`          | Comando más reciente que contiene "string"       | `!?config`     |
| `!?string?`         | Comando más reciente que contiene "string"       | `!?config?`    |
|

## Opciones del Historial

### `set`

```bash
set -o history           # Habilita el historial (por defecto ON)
set +o history           # Deshabilita el historial
set -o histexpand        # Habilita expansión ! (por defecto ON)
set +o histexpand        # Deshabilita expansión !
set -H                   # Habilita expansión ! (por defecto ON)
set +H                   # Deshabilita expansión !
```

- En modo no interactivo, `history` y `histexpand` está desactivado por defecto.

### `shopt`

```bash
shopt -s histappend      # Añade al archivo en lugar de sobreescribir
shopt -s histreedit      # Permite re-editar expansiones fallidas
shopt -s histverify      # Muestra expansión ! sin ejecutar (para confirmar)
```

## Builtins

### `history`

Permite administrar el historial de 42sh.

El argumento `offset` define una posición o rango:
- **Número**: Posición absoluta (no siempre coincide con la posición desde el inicio)
- **Número con signo +**: Posición desde el inicio
- **Número con signo -**: Posición desde el final
- **Rango**: Dos `offsets` separados por `-` (`offset`-`offset`)

```bash
history [n]              # Muestra últimos n comandos
history -c               # Limpia historial en memoria
history -d offset        # Elimina entrada en posición offset
history -a               # Añade solo comandos nuevos al archivo
history -n               # Lee solo las líneas nuevas del archivo (las que aún no están en memoria)
history -r               # Lee el archivo y añade a memoria
history -w               # Escribe la memoria al archivo
history -p arg [arg...]  # Expansión de historial sin ejecutar
history -s arg [arg...]  # Añade argumentos como nueva entrada
```

### `fc (fix command)`

Es más potente que `history` para edición y re-ejecución.

Los argumentos `first` y `last` definen el rango:
- **Número**: Posición absoluta
- **Negativo**: Offset desde el final
- **String**: Último comando que empieza con ese string

```bash
fc [-e ename] [-lnr] [first] [last]
fc -s [pat=rep] [command]

fc -l                    # Lista comandos en lugar de editarlos
fc -n                    # Suprime números de línea al listar
fc -r                    # Invierte el orden al listar
fc -e editor             # Especifica el editor a usar (por defecto `$FCEDIT` o `$EDITOR`)
fc -s                    # Re-ejecuta el comando con sustitución
```
