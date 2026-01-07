# Evaluación 42sh

- **Si el programa termina de forma inesperada**: = 0
- **Si hay un solo memory leak**: = 0

Comando sugerido: `valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes -q ./42sh "$@"`

---

## Tests Básicos de Comandos

1. **Comando vacío:**
```bash
$>
# Debe no hacer nada y mostrar el prompt de nuevo
```

2. **Solo espacio:**
```bash
$> 
# Debe no hacer nada y mostrar el prompt de nuevo
```

3. **Solo espacios y tabulaciones:**
```bash
$>      	  	
# Debe no hacer nada y mostrar el prompt de nuevo
```

4. **Espacios/tabs innecesarios:**
```bash
$>    /bin/ls   -l   -a   
# Los espacios no deben interferir con la ejecución
```

5. **Test opción inexistente:**
```bash
$> echo $?                # Código de retorno igual a 0
$> ls -z ; echo $?        # Código de retorno diferente de 0
$> cd -z ; echo $?        # Código de retorno diferente de 0
```

6. **Comando inexistente:**
```bash
$> doesnotexist           # Mensaje de error indicando que el comando no existe/no se encuentra
$> echo ${?}              # Código de retorno diferente de 0
```

7. **Comando sin permisos:**
```bash
$> /etc/hosts             # Mensaje de error indicando permisos insuficientes
$> echo ${?}              # Código de retorno diferente de 0
```

8. **Comando con ruta absoluta:**
```bash
$> /bin/ls                # Salida del comando "ls"
$> echo ${?}              # Código de retorno igual a 0
```

9. **Comando sin ruta:**
```bash
$> ls                     # Salida del comando "ls"
$> echo ${?}              # Código de retorno igual a 0
```

10. **Comando con flags juntos:**
```bash
$> /bin/ls -alF           # Salida del comando "ls" con argumentos "alF"
$> echo ${?}              # Código de retorno igual a 0
```

11. **Comando con flags separados:**
```bash
$> /bin/ls -l -a -F       # Salida del comando "ls" con argumentos "alF"
$> echo ${?}              # Código de retorno igual a 0
```

---

## Redirecciones

2. **Redirección básica de salida:**
```bash
$> ls > /tmp/ftsh_ls_out /
$> cat /tmp/ftsh_ls_out
# {{ Salida del comando "ls" en la raíz del sistema }}
```

3. **Redirección de entrada y append:**
```bash
$> < /tmp/ftsh_ls_out cat -e >> /tmp/ftsh_ls_out
$> cat /tmp/ftsh_ls_out
# {{ 2 listados de la raíz, el segundo con $ al final de cada línea }}
```

4. **Redirección de stdout y stderr:**
```bash
$> echo 1 >out >&2 2>err
1
$> echo 2 >out 2>err
$> cat err
$> cat out
2
$>
```

5. **File descriptors no estándar:**
```bash
$> echo non-standard fd >&4 > dup_fd
$> cat <&4 dup_fd
non-standard fd$
$>
```

6. **File descriptor inválido:**
```bash
$> cat <&4
# {{ Mensaje de error indicando que el file descriptor es inválido }}
$>
```

7. **Redirección a múltiples FDs:**
```bash
$> echo abc >&9 >redir_one_to_all
$> cat <&9 redir_one_to_all
abc$
$>
```

8. **Cerrar file descriptor:**
```bash
$> cat <&- abc
$>
```

9. **Orden de redirecciones:**
```bash
$> ls doesnotexist . 2>&1 >/dev/null
ls: doesnotexist: No such file or directory
$> ls doesnotexist . >/dev/null 2>&1
$>
```

### Pipes

10. **Pipe básico:**
```bash
$> ls | sort -rn | cat -e
# {{ Contenido de la carpeta actual, ordenado, con '$' al final de cada línea }}
$>
```

11. **Pipe complejo:**
```bash
$> base64 < /dev/urandom | head -c 1000 | grep 42 | wc -l | sed -e s/1/Yes/g -e s/0/No/g
# {{ Muestra "Yes" o "No" aleatoriamente }}
$> ps a | grep 'base64' | grep -v 'grep'
$>
```

### Punto y coma (;)

12. **Comandos secuenciales:**
```bash
$> ls -1; touch test_file; ls -1
# {{ Muestra 2 'ls'. Un archivo adicional, "test_file", debe aparecer en la segunda salida }}
$>
```

13. **Exit en pipe:**
```bash
$> exit 1 | exit 2 | exit 3; echo "stayin' alive"
stayin' alive
$>
# Verificar que el 42sh NO ha terminado y el prompt está disponible
```

14. **Redirección a FD cerrado:**
```bash
$> echo out >&-; echo out2
# {{ Mensaje de error opcional indicando imposibilidad de escribir a stdout }}
out2
$> echo out >&- | echo out2
# {{ Mensaje de error opcional indicando imposibilidad de escribir a stdout }}
out2
$> echo out >&- && echo out2
# {{ Mensaje de error opcional indicando imposibilidad de escribir a stdout }}
$> echo out >&- || echo out2
# {{ Mensaje de error opcional indicando imposibilidad de escribir a stdout }}
out2
```

### Heredoc

15. **Heredoc básico:**
```bash
$> cat << END
heredoc> hello world
heredoc> and good
heredoc> morning!
heredoc> END
hello world
and good
morning!
$>
```

16. **Heredoc con escape:**
```bash
$> cat << EO\
> F
heredoc> hi
heredoc> EOF
hi
```

17. **Heredoc con append:**
```bash
$> cat > /tmp/heredoc-append << FIN
heredoc> abc
heredoc> FIN
$> cat -e >> /tmp/heredoc-append << FIN
heredoc> def
heredoc> ghi
heredoc> FIN
$> cat /tmp/heredoc-append
abc
def$
ghi$
$>
```

18. **Heredoc en pipe con sort:**
```bash
$> (cat <<EOF | sort -r | cat -e
heredoc> abd
heredoc> abc
heredoc> abb
heredoc> EOF
abb$
abc$
abd$
$>
```

---

## Built-ins

### Exit, Echo, CD y Type
**Si alguno falla: toda la sección falla**

1. **Exit básico:**
```bash
$> echo abc; exit; echo def
abc
$> echo ${?}
0
```

2. **Exit con número muy grande:**
```bash
$> exit 999999999999999999999999999999999999999999
# Verificar que el comportamiento del 42sh es consistente
# No debe haber crash ni comportamiento indeterminado
```

3. **Exit con argumento no numérico:**
```bash
$> exit abc
# {{ Mensaje de error indicando que se espera un valor numérico }}
$> echo ${?}
# {{ Código de retorno diferente de 0 }}
```

4. **Exit con múltiples argumentos:**
```bash
$> exit 1 2 3
# {{ Mensaje de error indicando demasiados argumentos }}
$> echo ${?}
# {{ Código de retorno diferente de 0 }}
```

5. **CD con ruta absoluta:**
```bash
$> cd {{ /ruta/absoluta/de/tu/elección }}
$> pwd
# {{ /ruta/absoluta/de/tu/elección }}
$>
```

6. **CD con ruta relativa:**
```bash
$> cd ruta/relativa/de/tu/elección
$> pwd
# {{ ruta/relativa/de/tu/elección }}
$>
```

7. **CD sin argumentos:**
```bash
$> cd /tmp
$> /bin/pwd
/tmp
$> cd
$> /bin/pwd
/Users/{{login_session}}
$>
```

8. **CD con guión:**
```bash
$> cd /tmp
$> pwd
/tmp
$> cd /bin
$> pwd
/bin
$> cd -
$> pwd
/tmp
$>
```

9. **CD con -L y -P:**
```bash
$> cd -L /tmp; cd -P ..
$> pwd
/private
$>
```

10. **Type:**
```bash
$> type type ls
# {{ Mensaje indicando que "type" es un builtin y "ls" un comando con su ruta }}
$>
```

**Tests adicionales sugeridos:**
- Comportamiento correcto del builtin "cd" con la variable "CDPATH"

---

## Operadores Lógicos

**Si alguno falla: toda la sección falla**

1. **AND (&&):**
```bash
$> ls -l && ls
# {{ Muestra el comando "ls" dos veces con parámetros diferentes }}
$>
```

2. **OR (||) con error:**
```bash
$> ls doesnotexist || echo "Notice me senpai"
ls: doesnotexist: No such file or directory
Notice me senpai
$> echo ${?}
0
$>
```

3. **OR (||) sin error:**
```bash
$> echo 'No error' || echo 'You cant see me'
No error
$> echo ${?}
0
$>
```

4. **Combinación de operadores:**
```bash
$> false && echo foo || echo bar
bar
$> true || echo foo && echo bar
bar
$>
```

---

## Gestión del Entorno

### Variables Internas y Externas + Built-ins set, export, unset
**Si alguno falla: toda la sección falla**

1. **Asignación de variables:**
```bash
$> a=hello b=world; b=42 echo ${a}_${b} && echo ${b}
hello_world
world
$>
```

2. **Variables en comandos:**
```bash
$> directory=/ ls_opt=-atr
$> ls ${ls_opt} ${directory}
# {{ Salida del comando "ls -atr" en la raíz del sistema }}
$>
```

3. **Variable vacía:**
```bash
$> echo ${empty}|cat -e
$
$>
```

4. **Comando set:**
```bash
$> set
# {{ Todas las variables internas del shell y del entorno }}
$> set | grep -E '(a|b)='
a=hello
b=world
$>
```

5. **Comando env:**
```bash
$> env
# {{ Solo variables de entorno }}
$> env | grep -E '(a|b)='
$>
```

6. **Export:**
```bash
$> export b
$> printenv b
world
$>
```

7. **Variable de un solo uso:**
```bash
$> ONESHOT= env | grep ONESHOT
ONESHOT=
$> env | grep ONESHOT
$>
```

8. **Unset:**
```bash
$> unset a b
$> env | grep -E '(a|b)='
$> set | grep -E '(a|b)='
$>
```

9. **PATH manual:**
```bash
$> unset PATH
$> PATH=/bin:/usr/bin
$> mkdir testdir
$> echo ${?}
0
$> ls -1 | grep testdir
testdir
$>
```

10. **Códigos de retorno:**
```bash
$> true; echo ${?}; false; echo ${?}
0
1
$>
```

**Tests adicionales sugeridos:**
- Opciones del builtin "export"

---

## Control de Trabajos (Job Control)

**Si alguno falla: toda la sección falla**

1. **Trabajo en background:**
```bash
$> mkfifo fifo
$> ls -lR /usr >fifo 2>&1 &
$> jobs
# {{ Mensaje indicando que el comando "ls" está ejecutándose }}
$>
```

2. **Múltiples trabajos en background:**
```bash
$> emacs -nw &
$> emacs -nw &
$> emacs -nw &
$> emacs -nw &
$> emacs -nw &
$> jobs
# {{ Mensaje indicando que 5 instancias de emacs están detenidas en background }}
$>
```

3. **Foreground (fg):**
```bash
$> fg %{{ número de trabajo de uno de los emacs }}
# El proceso Emacs debe volver al foreground y ser funcional
# Repetir tantas veces como sea necesario para traer todos los procesos "emacs"
```

4. **Verificación de jobs completados:**
```bash
$> jobs
# {{ El comando "ls" lanzado antes debe ser el único que queda }}
$> cat <&4 fifo | cat -e
# {{ Salida del comando "ls". No esperar hasta el final, cortar con CTRL-C }}
$> jobs
$>
```

5. **CTRL-Z (suspender):**
```bash
$> ls -Rl / 2>&1
# {{ Muestra del comando "ls". No esperar hasta el final, presionar CTRL-Z }}
# {{ Mensaje indicando que el comando está suspendido }}
$> jobs
# {{ Mensaje indicando que el comando está suspendido }}
$>
```

6. **Kill de proceso suspendido:**
```bash
$> ps a | grep "ls -Rl /" | grep -v 'grep' | cut -d ' ' -f 2
# {{ PID del comando "ls" del test anterior }}
$> kill {{ PID del comando "ls" del test anterior }}
$> jobs
# {{ Mensaje indicando que el comando ha terminado }}
$>
```

7. **Verificación de limpieza:**
```bash
$> jobs
$> ps a | grep "ls -Rl /" | grep -v 'grep'
$>
```

**Tests adicionales sugeridos:**
- Builtin "bg"

---

## Señales (Signals)

**Si alguno falla: toda la sección falla**

1. **Señales de hijos:**
```bash
$> python -c 'import os, signal;os.kill(os.getpid(), signal.SIGSEGV)'
# {{ Mensaje indicando la señal recibida }}
$>
```
- Reemplazar "SIGSEGV" con la señal que quieras enviar
- Probar TODAS las señales
- El shell NO debe terminar aunque un hijo termine con señal
- Ni siquiera con "SIGKILL"

2. **CTRL-C en prompt vacío:**
- Verificar que "CTRL-C" en prompt vacío redisplaya un prompt vacío

3. **CTRL-C con comando:**
```bash
$> cat
# Luego presionar "CTRL-\"
```
- El comando "cat" debe terminar con mensaje indicando la señal recibida
- El prompt debe estar disponible

---

## Parte Opcional

**IMPORTANTE:** Solo evaluar la parte modular si la parte obligatoria es PERFECTA.

### Formato de Tests
- Los tests están en archivos separados
- Debes realizar TODOS los tests
- Si al menos uno falla, toda la sección falla y se pasa a la siguiente
- **"$> "** representa el prompt normal
- **"> "** representa un comando incompleto esperando entrada del usuario

---

## Inhibidores (Quoting)

**Archivo de tests:** `42sh.quoting.txt`

Evaluar la presencia y corrección de:
- **"** (doble comilla)
- **'** (comilla simple)
- **\** (backslash)

**Prompt especial:**
- **quote>** indica que el shell espera entrada adicional para completar el comando

**Tests adicionales sugeridos:**
```bash
$> echo foo\
$> echo "\\'abcd\\'"
$> echo \'
```

Verificar que el shell reacciona igual que el shell de referencia elegido por el grupo.

---

## Pattern Matching (Globbing)

**Archivo de tests:** `42sh.pattern_matching.txt`

Evaluar el funcionamiento correcto de:
- **\*** (asterisco)
- **?** (interrogación)
- **[]** (corchetes)
- **-** (guión en rangos)
- **!** (negación)

**IMPORTANTE:**
- Buscar en las fuentes la implementación de globbing
- La función `glob(3)` NO debe ser usada

**Tests adicionales sugeridos:**
- Cómo se comporta el shell si un elemento del patrón está escapado con "\\"
- Cómo funciona si el patrón está entre comillas simples ("'")

---

## Expansiones Adicionales

**Archivo de tests:** `42sh.expansions.txt`

Probar la presencia y funcionamiento correcto de:
- Expansión de tilde (~)
- Expansión de parámetros (${...})

**Tests adicionales:**
Hay muchas pruebas posibles para expansión. ¡Sé inventivo!

---

## Controles Agrupados y Sub-shells

**Archivo de tests:** `42sh.grouped_commands.txt`

Probar la presencia y funcionamiento correcto de:
- Sub-shells: `( ... )`
- Comandos agrupados: `{ ... }`

**Tests adicionales - errores de sintaxis:**
```bash
$> ()
$> (echo a|)
$> (; echo b)
$> (echo c; ())
```

---

## Sustitución de Comandos

**Archivo de tests:** `42sh.command_sub.txt`

Probar la presencia y funcionamiento correcto de:
- Sustitución con backticks: `` `command` ``
- Sustitución con $(): `$(command)`

**Tests adicionales:**
Hay muchas pruebas posibles para sustitución de comandos. ¡Sé inventivo!

---

## Expansión Aritmética

**Archivo de tests:** `42sh.exp_arithm.txt`

Probar la presencia y funcionamiento correcto de:
- Expansión aritmética: `$(( ... ))`

**Tests adicionales:**
- Verificar que la expansión funciona entre comillas dobles ("")
- NO debe funcionar entre comillas simples ('')
- Probar valores inconsistentes como "999999999999999999999999999999999 + 1"

---

## Sustitución de Procesos

**Archivo de tests:** `42sh.process_sub.txt`

Probar la presencia y funcionamiento correcto de:
- Sustitución de procesos: `<(command)` y `>(command)`

**Tests adicionales:**
Hay muchas pruebas posibles para sustitución de procesos. ¡Sé inventivo!

---

## Historial (History)

**Archivo de tests:** `42sh.history.txt`

**Tests adicionales obligatorios:**

1. **Expansión con !:**
```bash
$> fc -l
# Obtener el historial con índices
# Ejecutar un comando con expansión "!"
# Verificar que se ejecuta el comando correcto
```

2. **Expansión con !-:**
```bash
# Usar expansión "!-"
# Verificar que se ejecuta el comando correcto
```

3. **Persistencia:**
- Verificar que los comandos se guardan en un archivo
- Cerrar y reiniciar el shell
- ¿El historial de la sesión anterior es accesible en el nuevo shell?

4. **Editor fc:**
```bash
$> fc -e vim -1 -10
# Verificar que los últimos 10 comandos aparecen en vim
# Editarlos si deseas, guardar y cerrar vim
# Los comandos presentes en vim al cerrar deben ejecutarse
```

5. **Búsqueda incremental:**
- Verificar que la búsqueda incremental funciona
- Via atajo CTRL-R (u otro)

**Tests adicionales:**
Hay muchas pruebas posibles para historial y su expansión. ¡Sé inventivo!

---

## Completado Dinámico Contextual

**Si alguno falla: toda la sección falla**

1. **Completado de comandos en PATH:**
- Verificar que el completado de comandos en el "PATH" funciona

2. **Completado de builtins:**
- Verificar que el completado de builtins funciona

3. **Completado contextual:**
- Verificar que el completado es contextual
- Si tienes "ls /sbin/" en la línea de comandos y presionas TAB (u otra tecla de completado)
- Solo los archivos del directorio "/sbin" deben aparecer

4. **Completado de variables:**
```bash
$> abc=def
$> echo ${a
# Verificar que el completado ofrece la variable "abc"
```

5. **Variables eliminadas:**
```bash
$> unset a
$> echo ${a
# Verificar que el completado ya NO ofrece la variable "abc"
```

**Tests adicionales:**
Hay muchas pruebas posibles para completado dinámico. ¡Sé inventivo!

---

## Modo de Edición Vi/Readline

**Si alguno falla: toda la sección falla**

1. **Cambio de modo:**
- Verificar que es posible cambiar el modo de edición con:
  - `set -o vi`
  - `set -o readline`

2. **Atajos:**
- Verificar que todos los atajos mencionados en el subject funcionan correctamente
- Probar para AMBOS modos

**Tests adicionales:**
Hay muchas pruebas posibles para edición de línea. ¡Sé inventivo!

---

## Built-ins alias/unalias

**Archivo de tests:** `42sh.alias.txt`

**Tests adicionales:**
- Probar nombres de alias inválidos como "=", "-" o "/"
- El shell debe mostrar un error

---

## Tabla Hash

**Archivo de tests:** `42sh.hash.txt`

Probar la presencia y funcionamiento correcto de:
- Tabla hash
- Builtin "hash"

**Tests adicionales:**
Hay muchas pruebas posibles para la tabla hash y el builtin "hash". ¡Sé inventivo!

---

## Built-in test

**Archivo de tests:** `42sh.test.txt`

**Verificación de errores internos:**
- En caso de error interno, se devuelve código de retorno mayor que 1
- Verificar con comando de ejemplo:
```bash
$> test zzz -eq; echo $?
```

---

## Bonificaciones

### Shell Script
- Verificar que la ejecución de shell scripts es funcional

### Autocompletado de Parámetros
- Verificar que el autocompletado de parámetros funciona

### POSIX
- El shell completo debe ser compatible con POSIX
