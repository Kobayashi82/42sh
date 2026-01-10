# Guía completa de Arrays en Bash

## Tabla de contenidos
1. [Variables y Stacks](#variables-y-stacks)
2. [Arrays básicos](#arrays-básicos)
3. [Arrays asociativos](#arrays-asociativos)
4. [Conversiones de tipos](#conversiones-de-tipos)
5. [Interacción con local y export](#interacción-con-local-y-export)
6. [Parsing y contextos](#parsing-y-contextos)
7. [Evaluación aritmética en índices](#evaluación-aritmética-en-índices)
8. [Estructura de datos recomendada](#estructura-de-datos-recomendada)

---

## Variables y Stacks

### Comportamiento de `variable_delete(key)`

Cuando llamas a `variable_delete(key)`, debe eliminar **solo la primera variable que encuentre** buscando desde el scope actual hacia arriba en el stack.

```bash
export PATH="/global"

funcion_sin_local() {
    unset PATH  # Borra la variable global
}

funcion_con_local() {
    local PATH="/local"
    unset PATH  # Solo borra la local, la global se mantiene
}
```

**Regla:** `unset` elimina la primera ocurrencia encontrada subiendo por el stack, no todas las variables con ese nombre.

---

## Arrays básicos

### Declaración y tipos

**Arrays indexed (normales):**
```bash
declare -a array           # Declaración explícita (opcional)
array=(uno dos tres)       # Asignación directa
array[0]="primero"         # Asignación individual
```

**Características:**
- Índices numéricos (0, 1, 2, ...)
- Pueden ser sparse (tener huecos): `array[0]` y `array[5]` sin índices intermedios
- Los índices empiezan en 0

### Acceso a elementos

```bash
array=(uno dos tres)

${array[0]}      # Accede al índice 0: "uno"
${array[@]}      # Todos los elementos como palabras separadas
${array[*]}      # Todos los elementos (diferente con quotes)
${#array[@]}     # Número de elementos
${!array[@]}     # Lista de índices existentes
$array           # Solo el índice 0 (equivale a ${array[0]})
```

**Importante:** `$array[0]` NO accede al índice 0, expande `$array` y concatena el literal `[0]`:
```bash
array=(uno dos tres)
echo $array[0]    # Imprime: uno[0]  (no "uno")
```

### Espacios en índices

Los espacios dentro de `[]` son válidos:
```bash
array[3]=valor
array[ 3 ]=valor
array[  3  ]=valor
```

Todos son equivalentes. **Pero cuidado:**
```bash
array[3]=valor     # ✓ OK
array [3]=valor    # ✗ ERROR - espacio antes de [
```

### Quote handling

**`"${array[@]}"` vs `"${array[*]}"`:**

```bash
array=("uno dos" "tres")

# Con [@] - cada elemento es palabra separada
for x in "${array[@]}"; do echo "[$x]"; done
# [uno dos]
# [tres]

# Con [*] - todo en una sola palabra
for x in "${array[*]}"; do echo "[$x]"; done
# [uno dos tres]
```

Sin quotes, ambos hacen word splitting normal.

---

## Arrays asociativos

### Declaración obligatoria

**Deben declararse explícitamente con `declare -A`:**
```bash
declare -A asociativo
asociativo["nombre"]="Juan"
asociativo["edad"]="30"
echo ${asociativo["nombre"]}  # Juan
```

**Sin `declare -A` no funciona:**
```bash
assoc["key"]="valor"  # ERROR si no hiciste declare -A antes
```

### Diferencias con arrays normales

1. **Índices son strings arbitrarios**, no solo números
2. **No tienen orden garantizado** al iterar
3. **No se pueden exportar** (igual que arrays normales)
4. Los índices numéricos se tratan como strings:
   ```bash
   declare -A assoc
   assoc[0]="valor"      # La clave es el STRING "0"
   assoc["0"]="valor"    # Exactamente lo mismo
   ```

---

## Conversiones de tipos

### Tabla de conversiones

| Desde | Hacia | ¿Permitido? | Cómo |
|-------|-------|-------------|------|
| SCALAR → INDEXED | ✓ | Automático con `var[numero]=valor` |
| SCALAR → ASSOCIATIVE | ✗ | Solo con `declare -A` (variable no debe existir) |
| INDEXED → SCALAR | ✗ | Requiere `unset` primero |
| INDEXED → ASSOCIATIVE | ✗ | Requiere `unset` primero |
| ASSOCIATIVE → SCALAR | ✗ | Requiere `unset` primero |
| ASSOCIATIVE → INDEXED | ✗ | Requiere `unset` primero |

### Conversión automática: escalar a indexed

```bash
variable="escalar"
echo $variable        # Imprime: escalar

variable[3]="hola"
echo ${variable[0]}   # Imprime: escalar (valor original en índice 0)
echo ${variable[3]}   # Imprime: hola
```

El valor escalar original se convierte en el elemento del índice 0.

### Conversión bloqueada: entre tipos de arrays

```bash
array=(uno dos tres)      # Array indexed
declare -A array          # ERROR: cannot convert indexed to associative

# Forma correcta:
unset array
declare -A array
array[clave]=valor        # Ahora sí
```

### Arrays con un solo elemento siguen siendo arrays

```bash
array=(uno dos tres)
unset array[1]
unset array[2]
# Solo queda array[0]="uno"

export array  # ERROR - sigue siendo array, no se puede exportar
```

Bash mantiene el tipo internamente. Un array con un elemento sigue siendo array.

---

## Interacción con local y export

### Arrays y local

Funcionan igual que con variables escalares:

```bash
mi_array=(global1 global2)

funcion() {
    local mi_array=(local1 local2 local3)
    echo ${mi_array[@]}  # local1 local2 local3
}

funcion
echo ${mi_array[@]}  # global1 global2
```

### Arrays y export

**Los arrays NO se pueden exportar:**

```bash
array=(uno dos tres)
export array          # Bash lo ignora o da error
env | grep array      # No aparece
```

Solo las variables escalares se pueden exportar.

**Pérdida de export al convertir a array:**

```bash
variable="valor"
export variable
env | grep variable    # Aparece

variable[0]="valor"    # Convierte a array
env | grep variable    # Ya NO aparece - perdió el export
```

Incluso asignando al mismo índice (0) con el mismo valor, pierde el flag de export al convertirse a array.

### Modificar array como escalar

```bash
array=(uno dos tres)
array="nuevo"           # Asigna solo al índice 0
echo ${array[0]}        # nuevo
echo ${array[1]}        # dos (los demás siguen ahí)
```

Asignar sin `[]` solo modifica el índice 0, no convierte el array en escalar.

---

## Parsing y contextos

### Diferenciación de `[]`

El significado de `[]` depende del contexto:

1. **Test/comando:** `[` en posición de comando
   ```bash
   [ -f archivo ]
   ```

2. **Asignación de array:** Con `=` pegado
   ```bash
   var[3]=valor
   var=(a b c)
   ```

3. **Expansión de array:** Con `$` o `${`
   ```bash
   ${array[3]}
   $array[3]    # Expande $array, luego literal [3]
   ```

4. **Globbing:** En cualquier otro contexto
   ```bash
   echo array[3]     # Busca archivos que matcheen
   ls file[123]
   ```

### Asignación con `=()`

Debe estar **pegado** al `=`:

```bash
var=(a b c)       # ✓ Correcto
var= (a b c)      # ✗ var="" + subshell
var =(a b c)      # ✗ comando "var" con arg
```

### Expansión con variable inexistente

```bash
$noexiste[3]
```

1. Token en posición de comando: `$noexiste[3]`
2. Se expande `$noexiste` → `""` (vacío)
3. Queda: `[3]` en posición de comando
4. Intenta ejecutar comando `[3]` (palabra única, no el builtin `[`)
5. Error: command not found

Para ejecutar el builtin `[` necesitarías espacios: `$noexiste [ 3 ]`

---

## Evaluación aritmética en índices

### Arrays indexed evalúan expresiones aritméticas

Dentro de `[]` en arrays indexed se evalúa como expresión aritmética:

```bash
array[5 + 3]=valor              # array[8]
array[2 * 4]=valor              # array[8]
array[(5 + 3) * 2]=valor        # array[16]
```

### Con variables

```bash
i=10
j=5
array[i + j]=valor              # array[15]
array[$i + $j]=valor            # También funciona
```

El `$` es opcional en contexto aritmético dentro de `[]`.

### Con sustitución de comandos

```bash
array[$(echo 6)]=valor          # array[6]
array[5 + $(echo 3)]=valor      # array[8]
array[$((2 + 3))]=valor         # Arithmetic expansion anidada
```

### Variables no numéricas

```bash
patata=5
array[patata]=valor    # Asigna a array[5]

patata="hola"
array[patata]=valor    # Se evalúa como 0, asigna a array[0]

array[patata]=valor    # patata no existe, se evalúa como 0
```

Si la variable no existe o no es numérica, se evalúa como 0.

### Arrays asociativos NO evalúan

```bash
declare -A assoc
assoc[patata]=valor    # La clave es literalmente "patata"
```

En arrays asociativos, el contenido entre `[]` es la clave literal (string), no se evalúa aritméticamente.

---

## Estructura de datos recomendada

### Usar hash map (tabla hash)

**Ventajas:**
- O(1) para acceso, inserción y eliminación
- Sparse arrays naturalmente (no desperdicias memoria)
- Preparado para arrays asociativos
- Las colisiones se manejan automáticamente

**Las colisiones no son problema:**
El hash map compara la clave completa, no solo el hash. Ejemplo:

```
hash("3") = 17  → bucket 17
hash("48") = 17 → bucket 17 (colisión)

Bucket 17: ["3" -> "valor1"] -> ["48" -> "valor2"]

Al buscar "3": encuentra bucket 17, compara "3" == "3" → devuelve "valor1"
```

### Estructura recomendada

```c
typedef enum {
    SCALAR,
    INDEXED_ARRAY,
    ASSOCIATIVE_ARRAY
} var_type_t;

typedef struct variable {
    char *key;
    var_type_t type;
    
    union {
        char *scalar_value;
        hash_map_t *array_map;  // índice/clave -> valor
    } data;
    
    int flags;  // export, readonly, local, etc.
} variable_t;
```

### Mantener orden en arrays indexed

El hash map no mantiene orden automáticamente. Dos opciones:

**Opción 1: Ordenar al expandir (recomendado)**
```
Al expandir ${array[@]}:
    indices = obtener_claves_del_hash(array)
    ordenar_numericamente(indices)
    for cada indice en indices:
        agregar valor[indice] al resultado
```

**Opción 2: Lista ordenada adicional**
Mantener una lista de índices ordenada además del hash. Más complejo pero más eficiente si expandes frecuentemente.

**Para arrays asociativos:**
No necesitas mantener orden, bash no lo garantiza.

---

## Implementación paso a paso

### Recomendación

**No implementes arrays al principio.** Son una feature compleja que requiere:

1. Parser especial para `var[indice]=valor` y `var=(val1 val2)`
2. Evaluador de expresiones aritméticas para índices
3. Expansiones especiales: `${array[@]}`, `${#array[@]}`, `${!array[@]}`
4. Manejo de conversión de tipos
5. Integración con export (rechazar arrays)
6. Manejo de memoria para hash maps

**Implementa primero:**
- Variables escalares
- Stack de scopes (funciones)
- Local y export
- Expansiones básicas

**Luego añade arrays** como una extensión cuando lo demás funcione perfectamente.

---

## Casos especiales y edge cases

### Array vacío
```bash
array=()
echo $array       # Nada (índice 0 no existe)
echo ${array[@]}  # Nada
```

### Unset con arrays
```bash
unset array       # Borra TODO el array
unset array[2]    # Borra solo el elemento en índice 2
```

### Escalar como array
```bash
variable="hola"
echo ${variable[0]}  # hola (bash trata escalares como arrays de 1 elemento)
echo ${variable[1]}  # vacío
```

Bash permite usar sintaxis de array en escalares. `${escalar[0]}` devuelve el valor escalar.

### Array en posición de comando
```bash
array=(echo hola mundo)
${array[@]}  # Ejecuta: echo hola mundo
```

Cada elemento del array se convierte en un token separado en la línea de comandos.

---

Add/Concatenate variable
- Si local, la crea/modifica en el env actual.
- busca en env actual y va retrocediendo. Si no existe, la crea en la última (base).

Delete variable
- busca en env actual y va retrocediendo. La primera que encuentre la borra y ya.

Find variable
- busca en env actual y va retrocediendo. Si la encuentra, la devuelve.

Variables_join (lo uso)?

Variables_to_array (está es tocha)

Variables_add tiene que tener:
Table
Key
Value
Join
Exported
Readonly
Integer
Force (esto no se si es necesario)
(Que pasa con los arrays)

Y se elimina variables_concatenate