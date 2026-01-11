## REFERENCE

declare -n REF=TARGET

- declare -n  REF	-> convierte REF a scalar
- REF[0]=value		-> convierte TARGET a array
- declare -r  REF	-> readonly TARGET
- declare -a  REF	-> convierte TARGET a array
- declare -A  REF	-> convierte TARGET a array asociativo
- declare -i  REF	-> convierte TARGET a integer
- declare -rn REF	-> readonly REF
- declare -an REF	-> convierte TARGET a array
- declare -An REF	-> convierte TARGET a array asociativo
- declare -in REF	-> convierte TARGET a integer
- unset    REF		-> elimina TARGET
- unset -n REF		-> elimina REF
- local REF=value	-> modifica TARGET (si existe)

## SCALAR

declare SCLR=VALUE

- SCLR[0]=value		-> convierte SCLR a array
- declare -r  SCLR	-> readonly SCLR
- declare -a  SCLR	-> convierte SCLR a array
- declare -A  SCLR	-> convierte SCLR a array asociativo
- declare -i  SCLR	-> convierte SCLR a integer
- declare -n  SCLR	-> convierte SCLR a referencia a VALUE (si no es array ni asociativo)
- declare -rn SCLR	-> convierte SCLR a referencia a VALUE (readonly)
- declare -an SCLR	-> convierte SCLR a array (error al convertir a -n)
- declare -An SCLR	-> convierte SCLR a array asociativo (error al convertir a -n)
- declare -in SCLR	-> convierte SCLR a referencia a VALUE (ignora integer)
- unset    SCLR		-> elimina SCLR
- unset -n SCLR		-> no elimina nada
- local SCLR=value	-> modifica SCLR (si existe)

## ARRAY

declare ARRAY=VALUE

- ARRAY[0]=value	-> modifica o crea el indice
- declare -r  ARRAY	-> readonly ARRAY
- declare -a  ARRAY	-> crea ARRAY
- declare +a  ARRAY	-> error, no se puede desconvetir
- declare -A  ARRAY	-> error, no se puede convetir
- declare -i  ARRAY	-> convierte ARRAY a integer (todos sus elementos)
- declare -n  ARRAY	-> error, no se puede convetir
- declare -rn ARRAY	-> error, no se puede convetir (error al convertir a -n)
- declare -an ARRAY	-> error, no se puede convetir (error al convertir a -n)
- declare -An ARRAY	-> error, no se puede convetir (error al convertir a -n)
- declare -in ARRAY	-> convierte ARRAY a integer (todos sus elementos) (error al convertir a -n)
- unset    ARRAY		-> elimina ARRAY
- unset -n ARRAY		-> no elimina nada
- local ARRAY=value	-> modifica ARRAY (si existe)

## ASSOC

declare ASSOC=VALUE

- ASSOC[key]=value	-> modifica o crea el indice
- declare -r  ASSOC	-> readonly ASSOC
- declare -A  ASSOC	-> crea ASSOC
- declare +A  ASSOC	-> error, no se puede desconvetir
- declare -a  ASSOC	-> error, no se puede convetir
- declare -i  ASSOC	-> convierte ASSOC a integer (todos sus elementos)
- declare -n  ASSOC	-> error, no se puede convetir
- declare -rn ASSOC	-> error, no se puede convetir (error al convertir a -n)
- declare -an ASSOC	-> error, no se puede convetir (error al convertir a -n)
- declare -An ASSOC	-> error, no se puede convetir (error al convertir a -n)
- declare -in ASSOC	-> convierte ASSOC a integer (todos sus elementos) (error al convertir a -n)
- unset    ASSOC		-> elimina ASSOC
- unset -n ASSOC		-> no elimina nada
- local ASSOC=value	-> modifica ASSOC (si existe)

## Builtins

- local
- declare
- export
- unset
- readonly
- read
- "assing"
- "expand"

### local

- scalar y array

### declare

- scalar, array, associative y reference

### export

- scalar, array

### unset

-  scalar, array, associative y reference

### readonly

- scalar, array, associative

### read

- scalar, array

### "assign"

- VAR=VALUE
- VAR[n]=VALUE

### "expand"

- $VAR
- $VAR[n]
- $VAR[key]
- ${VAR}
