#ifndef USUARIOS_H
#define USUARIOS_H

typedef struct {
    char usuario[50];
    char clave[50];
    char nombre[100];
} Usuario;

Usuario* cargar_usuarios(int* num_usuarios);

int autenticar_usuario(Usuario* usuarios, int num_usuarios, const char* usuario, const char* clave);

void liberar_usuarios(Usuario* usuarios);

#endif
