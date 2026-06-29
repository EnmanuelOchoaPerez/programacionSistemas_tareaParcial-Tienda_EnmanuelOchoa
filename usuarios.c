#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usuarios.h"

Usuario* cargar_usuarios(int* num_usuarios) {
    FILE* file = fopen("vendedores.txt", "r");
    if (!file) {
        *num_usuarios = 0;
        return NULL;
    }

    int capacidad = 10;
    Usuario* usuarios = (Usuario*)malloc(capacidad * sizeof(Usuario));
    *num_usuarios = 0;

    char linea[256];
    while (fgets(linea, sizeof(linea), file)) {
        if (*num_usuarios >= capacidad) {
            capacidad *= 2;
            usuarios = (Usuario*)realloc(usuarios, capacidad * sizeof(Usuario));
        }

        linea[strcspn(linea, "\r\n")] = 0;

        char* token = strtok(linea, ",");
        if (token) {
            strncpy(usuarios[*num_usuarios].usuario, token, sizeof(usuarios[*num_usuarios].usuario) - 1);
            usuarios[*num_usuarios].usuario[sizeof(usuarios[*num_usuarios].usuario) - 1] = '\0';
        }

        token = strtok(NULL, ",");
        if (token) {
            strncpy(usuarios[*num_usuarios].clave, token, sizeof(usuarios[*num_usuarios].clave) - 1);
            usuarios[*num_usuarios].clave[sizeof(usuarios[*num_usuarios].clave) - 1] = '\0';
        }

        token = strtok(NULL, ",");
        if (token) {
            strncpy(usuarios[*num_usuarios].nombre, token, sizeof(usuarios[*num_usuarios].nombre) - 1);
            usuarios[*num_usuarios].nombre[sizeof(usuarios[*num_usuarios].nombre) - 1] = '\0';
        }

        (*num_usuarios)++;
    }

    fclose(file);
    return usuarios;
}

int autenticar_usuario(Usuario* usuarios, int num_usuarios, const char* usuario, const char* clave) {
    for (int i = 0; i < num_usuarios; i++) {
        if (strcmp(usuarios[i].usuario, usuario) == 0 && strcmp(usuarios[i].clave, clave) == 0) {
            return 1;
        }
    }
    return 0;
}

void liberar_usuarios(Usuario* usuarios) {
    if (usuarios) {
        free(usuarios);
    }
}
