# Resumen: Registros de la CPU y Datos del Proceso

## Registros de la CPU
Durante la ejecución de un proceso, los registros de propósito general contienen datos relevantes, como el contador de programa (PC), puntero de pila (SP), registro de interrupción (IR) y otros. 
Estos registros se guardan en el Bloque de Control de Proceso (BCP) si el proceso es expulsado de la CPU para poder retomar su ejecución desde el punto correcto.

## Entorno
Los datos globales heredados por un proceso al nacer, como $PATH o $HOME, se almacenan como pares clave/valor y se pasan al proceso a través del stack al crearse.

## Metadatos
El Sistema Operativo genera datos sobre el proceso durante su ejecución, que residen en la memoria del kernel.
El BCP contiene información clave sobre el proceso, como su identidad, prioridad y estado. La tabla de páginas mapea las páginas de memoria virtual a los marcos en los que se carga el proceso. 
El mapa de E/S almacena información sobre los dispositivos de entrada/salida y archivos utilizados por el proceso.

## Almacenamiento en Disco
El código ejecutable del programa se guarda en el disco. También se almacenan las páginas de la zona de swap cuando se utiliza memoria virtual.
