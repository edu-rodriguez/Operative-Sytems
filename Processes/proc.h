// Segmentos en proc->gdt.
#define NSEGS     7

// Estado por CPU
struct cpu {
  uchar id;                    // ID de APIC local; índice en cpus[] a continuación
  struct context *planificador; // swtch() aquí para entrar al planificador
  struct taskstate ts;         // Utilizado por x86 para encontrar la pila para interrupciones
  struct segdesc gdt[NSEGS];   // Tabla global de descriptores x86
  volatile uint started;       // ¿La CPU ha iniciado?
  int ncli;                    // Profundidad de anidación de pushcli.
  int intena;                  // ¿Estaban habilitadas las interrupciones antes de pushcli?

  // Variables de almacenamiento locales a la CPU; ver abajo
  struct cpu *cpu;
  struct proc *proc;           // El proceso actualmente en ejecución.
};

extern struct cpu cpus[NCPU];
extern int ncpu;

// Variables por CPU, que mantienen punteros al
// CPU actual y al proceso actual.
// El sufijo asm le indica a gcc que use "%gs:0" para referirse a cpu
// y "%gs:4" para referirse a proc. seginit configura el
// registro de segmento %gs para que %gs se refiera a la memoria
// que contiene esas dos variables en la estructura cpu local.
// Esto es similar a cómo se implementan las variables locales a hilos
// en bibliotecas de hilos como pthreads en Linux.
extern struct cpu *cpu asm("%gs:0");       // &cpus[cpunum()]
extern struct proc *proc asm("%gs:4");     // cpus[cpunum()].proc

// PAGEBREAK: 17
// Registros guardados para cambios de contexto del kernel.
// No es necesario guardar todos los registros de segmento (%cs, etc),
// porque son constantes en los contextos del kernel.
// No es necesario guardar %eax, %ecx, %edx, ya que la
// convención x86 es que el llamante los ha guardado.
// Los contextos se almacenan en la parte inferior de la pila que
// describen; el puntero de pila es la dirección del contexto.
// El diseño del contexto coincide con el diseño de la pila en swtch.S
// en el comentario "Cambiar pilas". Switch no guarda eip explícitamente,
// pero está en la pila y allocproc() lo manipula.
struct context {
  uint edi;
  uint esi;
  uint ebx;
  uint ebp;
  uint eip;
};

enum procstate { SIN_USAR, EMBRIONARIO, DURMIENDO, LISTO, EJECUTANDO, ZOMBI };

// La dirección virtual donde aparece la memoria compartida, si se solicita
#define SHARED_V 0x70000000

// El número máximo de páginas compartidas en el sistema
#define NSHARED 10

struct shared {
  int refcount;
  void *page; 
};

// Estado por proceso
struct proc {
  uint sz;                     // Tamaño de la memoria del proceso (bytes)
  pde_t* pgdir;                // Tabla de páginas
  char *kstack;                // Base de la pila del kernel para este proceso
  enum procstate state;        // Estado del proceso
  volatile int pid;            // ID del proceso
  struct proc *parent;         // Proceso padre
  struct trapframe *tf;        // Marco de trampa para la llamada al sistema actual
  struct context *context;     // swtch() aquí para ejecutar el proceso
  void *chan;                  // Si no es cero, durmiendo en chan
  int killed;                  // Si no es cero, ha sido asesinado
  struct file *ofile[NOFILE];  // Archivos abiertos
  struct inode *cwd;           // Directorio actual
  struct shared *shared;       // Registro de memoria compartida (0 -> ninguna)
  char name[16];               // Nombre del proceso (depuración)
};

// La memoria del proceso se organiza de manera contigua, con las direcciones bajas primero:
//   texto
//   datos originales y bss
//   pila de tamaño fijo
//   montón expansible
//   (opcionalmente) segmento de memoria compartida de tamaño fijo, 1 página @ 0x7000000
