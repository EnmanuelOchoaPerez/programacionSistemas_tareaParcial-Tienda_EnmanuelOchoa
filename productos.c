#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "productos.h"

Producto* cargar_productos(int* num_productos) {
    FILE* file = fopen("producto.txt", "r");
    if (!file) {
        *num_productos = 0;
        return NULL;
    }

    int capacidad = 10;
    Producto* productos = (Producto*)malloc(capacidad * sizeof(Producto));
    *num_productos = 0;

    char linea[256];
    while (fgets(linea, sizeof(linea), file)) {
        if (*num_productos >= capacidad) {
            capacidad *= 2;
            productos = (Producto*)realloc(productos, capacidad * sizeof(Producto));
        }

        linea[strcspn(linea, "\r\n")] = 0;

        char* token = strtok(linea, ",");
        if (token) {
            strncpy(productos[*num_productos].codigo, token, sizeof(productos[*num_productos].codigo) - 1);
            productos[*num_productos].codigo[sizeof(productos[*num_productos].codigo) - 1] = '\0';
        }

        token = strtok(NULL, ",");
        if (token) {
            strncpy(productos[*num_productos].nombre, token, sizeof(productos[*num_productos].nombre) - 1);
            productos[*num_productos].nombre[sizeof(productos[*num_productos].nombre) - 1] = '\0';
        }

        token = strtok(NULL, ",");
        if (token) productos[*num_productos].cantidad = atoi(token);

        token = strtok(NULL, ",");
        if (token) productos[*num_productos].costo = atof(token);

        token = strtok(NULL, ",");
        if (token) productos[*num_productos].precio_venta = atof(token);

        (*num_productos)++;
    }

    fclose(file);
    return productos;
}

void guardar_productos(Producto* productos, int num_productos) {
    FILE* file = fopen("producto.txt", "w");
    if (!file) return;

    for (int i = 0; i < num_productos; i++) {
        fprintf(file, "%s,%s,%d,%.2f,%.2f\n",
                productos[i].codigo,
                productos[i].nombre,
                productos[i].cantidad,
                productos[i].costo,
                productos[i].precio_venta);
    }
    fclose(file);
}

int buscar_producto(Producto* productos, int num_productos, const char* codigo) {
    for (int i = 0; i < num_productos; i++) {
        if (strcmp(productos[i].codigo, codigo) == 0) {
            return i;
        }
    }
    return -1;
}

int obtener_siguiente_factura() {
    FILE* file = fopen("ventas.txt", "r");
    if (!file) {
        return 1;
    }

    int ultima_factura = 0;
    char linea[256];
    while (fgets(linea, sizeof(linea), file)) {
        linea[strcspn(linea, "\r\n")] = 0;
        char* token = strtok(linea, ",");
        if (token) {
            int num = atoi(token);
            if (num > ultima_factura) {
                ultima_factura = num;
            }
        }
    }
    fclose(file);
    return ultima_factura + 1;
}

void registrar_venta(ItemVenta* carrito, int num_items, const char* fecha, int num_factura) {
    FILE* file = fopen("ventas.txt", "a");
    if (!file) return;

    for (int i = 0; i < num_items; i++) {
        fprintf(file, "%d,%s,%s,%d,%.2f,%.2f,%s\n",
                num_factura,
                carrito[i].producto.codigo,
                carrito[i].producto.nombre,
                carrito[i].cantidad_vendida,
                carrito[i].producto.costo,
                carrito[i].producto.precio_venta,
                fecha);
    }
    fclose(file);
}

void liberar_productos(Producto* productos) {
    if (productos) {
        free(productos);
    }
}
