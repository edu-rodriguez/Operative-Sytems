## Ciclo de vida del proceso

El ciclo de vida de un proceso abarca desde su creación hasta la liberación de todos los recursos que ocupaba, tanto en el espacio del usuario como en las tablas del sistema. Durante su existencia, la ejecución del proceso se verá interrumpida en múltiples ocasiones.

### Creación

El proceso solo se crea por petición expresa de otro proceso, mediante primitivas como `fork()` y `exec()`. Por ejemplo, cuando invocamos comandos en una shell (como `ls`, `cat`, `rm`, etc.), la shell actúa como el proceso padre de estos comandos. Las operaciones necesarias para crear un proceso incluyen:

- Asignar la imagen de memoria, lo que puede implicar la creación y el llenado de una tabla de páginas en sistemas que utilizan memoria virtual.
- Seleccionar un Bloque de Control de Proceso (BCP) libre en la tabla del kernel y llenarlo con los datos del nuevo proceso.
- Cargar el ejecutable y los datos estáticos inicializados en la memoria del proceso.
- Crear la pila y el heap del proceso.

Durante estas tareas, el estado del proceso se considera "Nuevo". Una vez completadas, se marca como "Listo" y se agrega a la cola del planificador.

### Activación

La activación de un proceso implica ponerlo en ejecución, y esto es responsabilidad de un componente del kernel llamado "dispatcher". Para activar un proceso, se deben recuperar los valores de los registros del procesador que el proceso tenía antes de la última interrupción, y estos datos se almacenan en el BCP.

El kernel entra en ejecución en modo privilegiado, guarda el contexto del proceso que ocupaba la CPU, carga el contexto del proceso seleccionado desde el BCP y, antes de regresar al modo usuario, coloca la dirección de retorno en el Program Counter (PC) para que el proceso se reanude desde donde se interrumpió. Este proceso se conoce como "cambio de contexto" y consume tiempo considerable. En sistemas con memoria virtual, la tabla de páginas se actualiza y la Traducción de Lookaside Buffer (TLB) se recarga.

### Interrupción

La interrupción es una fase crítica que activa al kernel y puede ocurrir debido a eventos hardware o interrupciones software (también llamadas "traps"). La interrupción provoca que el procesador ejecute en modo privilegiado.

En primer lugar, el kernel atiende la interrupción en sí, priorizándola sobre otras tareas. Luego, guarda los registros del procesador visibles para el usuario en la pila de usuario del proceso en ejecución.

La rutina de manejo de la interrupción puede ser compleja, pero en algún momento termina. El kernel puede haber bloqueado al proceso o haberlo colocado en una cola de procesos en espera de ejecución (estado "Listo"). Si alguna de estas circunstancias ocurre, el planificador debe decidir cuál será el nuevo proceso que ocupará la CPU y realizará un cambio de contexto. Si el proceso actual puede continuar ejecutándose, el kernel recuperará su contexto desde la pila de usuario y, al invocar la instrucción de retorno de interrupción, se volverá al entorno de usuario desde donde se interrumpió.

### Finalización

Un proceso finaliza cuando el programador así lo decide, por ejemplo, mediante las llamadas `exit()` o `return()` en la función `main()` de un programa en C. También puede finalizar de manera prematura al recibir una señal debido a situaciones anormales, como divisiones por cero o violaciones de segmento.

Sin importar la razón, la finalización del código de usuario no implica que el proceso desaparezca inmediatamente. Hay recursos del sistema operativo que deben liberarse, como el BCP o la tabla de memoria, y funciones que dependen del proceso finalizado, como `wait()`. Por lo tanto, un proceso padre puede quedar bloqueado esperando la terminación de un proceso hijo. Por estas razones, el proceso no muere instantáneamente, sino que pasa a un estado terminal llamado "zombie".
