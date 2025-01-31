
(Este README es un template y no representa el estado actual del proyecto.)

# 42sh

## Descripción
42sh es una implementación personalizada de un shell Unix compatible con POSIX.

Diseñado como un proyecto educativo, este shell proporciona una interfaz potente para la ejecución de comandos, scripting y manipulación de procesos, con soporte para edición de línea de comandos en modo `readline` y `vi`.

Además, se han implementado características avanzadas como globbing, redirecciones y ejecución de scripts.

## Características

- **Modo interactivo y no interactivo**: Permite ejecutar comandos de manera interactiva o a través de scripts.
- **Builtins personalizados**: Implementación de comandos internos esenciales para la funcionalidad del shell.
- **Edición de línea de comandos**: Soporte para `readline` y `vi`, permitiendo navegación y edición eficiente.
- **Globbing avanzado**: Soporte para los patrones `**` y `**/` para coincidencias recursivas de archivos y directorios.
- **Redirecciones**: Manejo de entrada y salida con redirecciones estándar (`>`, `>>`, `<`, etc.).
- **Pipes**: Soporte para encadenamiento de comandos mediante `|`.
- **Scripting**: Ejecución de scripts con estructura de control (`if`, `while`, `for`, etc.).
- **Gestión de señales**: Manejo adecuado de `SIGINT`, `SIGQUIT` y `SIGTSTP`.
- **Expansión de variables**: Soporte para variables de entorno y sustitución de comandos.

## Builtins Implementados

42sh incluye los siguientes builtins:

### Alias
- `alias` - Define alias para comandos.
- `unalias` - Elimina alias definidos.

### Variables de entorno
- `declare` - Declara variables y atributos.
- `export` - Define variables de entorno.
- `readonly` - Marca variables como de solo lectura.
- `set` - Muestra o define opciones del shell.
- `shopt` - Configura opciones del shell.
- `unset` - Elimina variables de entorno.

### Sistema de archivos
- `cd` - Cambia el directorio actual.
- `hash` - Mantiene un caché de comandos.
- `pwd` - Muestra el directorio de trabajo actual.

### Historial
- `fc` - Edita y ejecuta comandos previos.
- `history` - Muestra el historial de comandos.

### Gestión de trabajos
- `bg` - Reanuda un trabajo en segundo plano.
- `fg` - Trae un trabajo al primer plano.
- `jobs` - Lista los trabajos en ejecución.
- `kill` - Envía señales a procesos.
- `wait` - Espera la finalización de un proceso.

### Expresiones matemáticas
- `let` - Evalúa expresiones aritméticas.

### Scripting
- `.` - Ejecuta un script en el contexto actual.
- `eval` - Evalúa y ejecuta una cadena de comandos.
- `exec` - Reemplaza el shell actual con un comando.
- `source` - Ejecuta un script en el contexto actual.
- `test` - Evalúa expresiones condicionales.

### Shell
- `banner` - Muestra el banner de 42sh.
- `builtin` - Muestra información sobre comandos internos.
- `command` - Ejecuta un comando sin aplicar alias o funciones.
- `echo` - Imprime texto en la salida estándar.
- `enable` - Activa o desactiva builtins.
- `exit` - Cierra la sesión del shell.
- `help` - Muestra ayuda sobre 42sh.
- `type` - Indica cómo se interpretará un comando.

## Globbing

El shell implementa globbing para la expansión de patrones en nombres de archivos y directorios. Se han integrado los siguientes patrones:

- `*` - Coincide con cualquier número de caracteres en un solo nivel de directorio.
- `?` - Coincide con un solo carácter.
- `[abc]` - Coincide con cualquiera de los caracteres especificados.
- `**` - Coincide de manera recursiva con cualquier número de subdirectorios.
- `**/` - Coincide con cualquier número de subdirectorios, sin incluir archivos.

## Redirecciones y Pipes

Se soportan las siguientes redirecciones:

- `>`   - Redirige la salida estándar a un archivo (sobrescribe).
- `>>`  - Redirige la salida estándar a un archivo (añade al final).
- `<`   - Toma la entrada de un archivo.
- `<<`  - Permite la redirección de entrada con un _heredoc_.
- `<<<` - Redirige una cadena como entrada.
- `>&`  - Duplica descriptores de archivo de salida.
- `<&`  - Duplica descriptores de archivo de entrada.
- `|`   - Conecta la salida de un comando con la entrada de otro.
- `|&`  - Conecta tanto la salida estándar como la de error con la entrada de otro comando.

## Scripting

42sh permite la ejecución de scripts con estructuras de control y operadores lógicos. Ejemplo de script:

```sh
#!/usr/bin/env 42sh

echo "Hola, bienvenido a 42sh"

if [ -f "archivo.txt" ]; then
    echo "El archivo existe"
else
    echo "El archivo no existe"
fi

for i in {1..5}; do
    echo "Iteración $i"
done
```

## Instalación y Uso

Para compilar el shell:

```sh
make
```

Para ejecutar:

```sh
./42sh
```

Para ejecutar un script:

```sh
./42sh script.sh
```

## Futuras Mejoras

- Implementación de autocompletado.
- Soporte para más patrones de globbing.
- Mejoras en la gestión de trabajos en segundo plano.

---

42sh es un proyecto en constante evolución, pensado para aprender y mejorar la comprensión del funcionamiento de un shell Unix desde cero.
