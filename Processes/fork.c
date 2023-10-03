/*
Este código es un programa en C que crea varios procesos hijos utilizando la función fork() en un bucle for. 
Cada proceso hijo incrementa una variable llamada variable, y luego imprime información sobre sí mismo, su padre y el valor de la variable. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


int main() {
    int variable = 1;
    for (int i = 0; i < 4; i++) {
        int result = fork();
        if (result != 0) {
            variable++;
            break;
        } else {
            variable += 2;
        }
    }
    printf("Soy el proceso %i, mi padre es %i, el valor es: %i\n", getpid(), getppid(), variable);
}
