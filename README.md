# QuinapantaE-bash
Proyecto - Bash
Este proyecto implementa un mini intérprete de comandos usando únicamente system calls de Linux. El objetivo es comprender cómo funcionan internamente los shells: lectura de comandos, parsing, ejecución de procesos y manejo básico de archivos.

¿Qué hace este mini-bash?

El programa:

Muestra un prompt: mini - bash >

Lee comandos del usuario usando read().

Separa los comandos en argumentos con una función split().

Ejecuta comandos internos:

cd

pwd

mkdir

rm

mv

cp

cat

Si no es interno, lo ejecuta con fork() y execvp().

Permite ejecutar procesos en segundo plano usando &.

Usa solo system calls como read, write, open, close, fork, execvp, wait, mkdir, unlink, rmdir, rename.

2. Funcionamiento del programa
Lectura del comando

El shell muestra un prompt y lee lo que escribe el usuario con read().
El texto se coloca en un buffer para ser procesado.

Separación en argumentos

La función split() divide el comando en tokens, eliminando espacios y saltos de línea.
Cada token se almacena en args[], similar a cómo trabaja un shell real.

3. Comandos internos implementados
cd

Cambia el directorio actual usando chdir().
No usa fork() porque debe ejecutarse en el mismo proceso.

pwd

Muestra el directorio actual usando getcwd().

mkdir

Crea un directorio con permisos 755 usando mkdir().

rm

Elimina:

archivos usando unlink()

directorios vacíos usando rmdir()

mv

Renombra o mueve archivos con rename().

cp

Copia archivos manualmente usando open(), read(), write() y close().
Lee bloques de 512 bytes y los escribe en el archivo destino.

cat

Abre un archivo y muestra su contenido usando read() y write().

4. Ejecución de comandos externos

Si el comando no coincide con uno interno, el shell:

Crea un proceso hijo con fork().

En el proceso hijo llama a execvp() para ejecutar el comando real del sistema.

El proceso padre espera con wait(), excepto si se usa &.

Ejecución en background

Si el usuario escribe:

comando &


El shell no espera al proceso hijo y continúa inmediatamente.

5. Compilación

Compilar con:

gcc mini_bash.c -o mini_bash

6. Ejecución

Ejecutar con:

./mini_bash


Ejemplos de comandos:

pwd
mkdir carpeta
cd carpeta
cp ../archivo.txt copia.txt
cat copia.txt
ls &

7. Limitaciones

Este mini-bash no soporta:

1. ¿Qué hace este mini-bash?

El programa:

Muestra un prompt: mini - bash >

Lee comandos del usuario usando read().

Separa los comandos en argumentos con una función split().

Ejecuta comandos internos:

cd

pwd

mkdir

rm

mv

cp

cat

Si no es interno, lo ejecuta con fork() y execvp().

Permite ejecutar procesos en segundo plano usando &.

Usa solo system calls como read, write, open, close, fork, execvp, wait, mkdir, unlink, rmdir, rename.

2. Funcionamiento del programa
Lectura del comando

El shell muestra un prompt y lee lo que escribe el usuario con read().
El texto se coloca en un buffer para ser procesado.

Separación en argumentos

La función split() divide el comando en tokens, eliminando espacios y saltos de línea.
Cada token se almacena en args[], similar a cómo trabaja un shell real.

3. Comandos internos implementados
cd

Cambia el directorio actual usando chdir().
No usa fork() porque debe ejecutarse en el mismo proceso.

pwd

Muestra el directorio actual usando getcwd().

mkdir

Crea un directorio con permisos 755 usando mkdir().

rm

Elimina:

archivos usando unlink()

directorios vacíos usando rmdir()

mv

Renombra o mueve archivos con rename().

cp

Copia archivos manualmente usando open(), read(), write() y close().
Lee bloques de 512 bytes y los escribe en el archivo destino.

cat

Abre un archivo y muestra su contenido usando read() y write().

4. Ejecución de comandos externos

Si el comando no coincide con uno interno, el shell:

Crea un proceso hijo con fork().

En el proceso hijo llama a execvp() para ejecutar el comando real del sistema.

El proceso padre espera con wait(), excepto si se usa &.

Ejecución en background

Si el usuario escribe:

comando &


El shell no espera al proceso hijo y continúa inmediatamente.

5. Compilación

Compilar con:

gcc mini_bash.c -o mini_bash

6. Ejecución

Ejecutar con:

./mini_bash


Ejemplos de comandos:

pwd
mkdir carpeta
cd carpeta
cp ../archivo.txt copia.txt
cat copia.txt
ls &

7. Limitaciones

Este mini-bash no soporta:

pipes (|)

redirecciones (>, <)

variables de entorno

historial

autocompletado

scripts .sh

Su propósito es mostrar el funcionamiento básico interno de un shell.pipes (|)

redirecciones (>, <)

variables de entorno

historial

autocompletado

scripts .sh

Su propósito es mostrar el funcionamiento básico interno de un shell.
