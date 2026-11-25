#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include "DatosMemCompartida.h"

int main(int argc, char **argv) {
    int fd;
    DatosMemCompartida *datos_bot;

    // Abre el fichero
    if ((fd = open("mmap_bot", O_RDWR)) < 0) {
        perror("No puede abrirse el fichero para mmap");
        return 1;
    }

    // Realiza la proyección en memoria
    if ((void *)(datos_bot = (DatosMemCompartida *)(mmap(NULL, sizeof(DatosMemCompartida), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))) == (void *)MAP_FAILED) {
        perror("Error en la proyeccion del archivo");
        close(fd);
        return 1;
    }
    close(fd);

    while (datos_bot->cerrar == 0) {
        float centro_raqueta = datos_bot->raqueta1.y2 + ((datos_bot->raqueta1.y1 - datos_bot->raqueta1.y2) / 2.0f);

        if (datos_bot->esfera.centro.y > 1.0f + centro_raqueta) {
            datos_bot->accion = 1;
        }
        else if (datos_bot->esfera.centro.y < -1.0f + centro_raqueta) {
            datos_bot->accion = -1;
        }
        else {
            datos_bot->accion = 0;
        }

        usleep(25000);
    }

    munmap(datos_bot, sizeof(DatosMemCompartida));
    return 0;
}
