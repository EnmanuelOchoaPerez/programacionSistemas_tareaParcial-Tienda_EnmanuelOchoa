#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "usuarios.h"
#include "productos.h"

void obtener_fecha_actual(char* fecha) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(fecha, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

int main() {
    int num_usuarios = 0;
    Usuario* usuarios = cargar_usuarios(&num_usuarios);

    if (!usuarios) {
        printf("Error: No se pudo cargar el archivo vendedores.txt\n");
        return 1;
    }

    char usr[50];
    char clave[50];
    int intentos = 0;
    int autenticado = 0;

    printf("=== SISTEMA DE VENTAS ===\n");
    while (intentos < 3 && !autenticado) {
        printf("Usuario: ");
        scanf("%49s", usr);
        printf("Clave: ");
        scanf("%49s", clave);

        if (autenticar_usuario(usuarios, num_usuarios, usr, clave)) {
            autenticado = 1;
            printf("\nBienvenido al sistema, %s.\n\n", usr);
        } else {
            intentos++;
            printf("Credenciales incorrectas. Intento %d de 3.\n", intentos);
        }
    }

    liberar_usuarios(usuarios);

    if (!autenticado) {
        printf("Acceso denegado. Cerrando sistema.\n");
        return 1;
    }

    int num_productos = 0;
    Producto* inventario = cargar_productos(&num_productos);

    if (!inventario) {
        printf("Error: No se pudo cargar el archivo producto.txt\n");
        return 1;
    }

    ItemVenta carrito[100];
    int num_items = 0;
    char continuar = 's';
    char codigo_buscar[50];
    int cantidad_vender;

    while (continuar == 's' || continuar == 'S') {
        printf("\nIngrese el codigo del producto a vender: ");
        scanf("%49s", codigo_buscar);

        int idx = buscar_producto(inventario, num_productos, codigo_buscar);

        if (idx == -1) {
            printf("Producto no encontrado.\n");
        } else {
            printf("Producto: %s | Disponible: %d | Precio: %.2f\n",
                    inventario[idx].nombre, inventario[idx].cantidad, inventario[idx].precio_venta);
            printf("Cantidad a vender: ");
            if (scanf("%d", &cantidad_vender) != 1) {
                printf("Entrada invalida.\n");
                int c; while ((c = getchar()) != '\n' && c != EOF);
                continue;
            }

            if (cantidad_vender > inventario[idx].cantidad) {
                printf("Error: No hay suficiente inventario disponible. Maximo %d.\n", inventario[idx].cantidad);
            } else if (cantidad_vender > 0) {
                if (num_items >= 100) {
                    printf("Error: El carrito esta lleno (max 100 items).\n");
                } else {
                    carrito[num_items].producto = inventario[idx];
                    carrito[num_items].cantidad_vendida = cantidad_vender;
                    num_items++;
                    printf("Agregado al carrito.\n");

                    inventario[idx].cantidad -= cantidad_vender;
                }
            } else {
                printf("Cantidad invalida.\n");
            }
        }

        printf("\n¿Desea registrar otro producto? (s/n): ");
        scanf(" %c", &continuar);
    }

    if (num_items > 0) {
        char fecha_hoy[15];
        obtener_fecha_actual(fecha_hoy);
        int factura_actual = obtener_siguiente_factura();

        printf("\n============================================\n");
        printf("FACTURA NUMERO: %d\n", factura_actual);
        printf("FECHA: %s\n", fecha_hoy);
        printf("============================================\n");
        printf("%-10s %-20s %-10s %-10s %-10s\n", "CODIGO", "PRODUCTO", "CANT", "P.UNIT", "TOTAL");

        float gran_total = 0;
        for (int i = 0; i < num_items; i++) {
            float total_item = carrito[i].cantidad_vendida * carrito[i].producto.precio_venta;
            gran_total += total_item;
            printf("%-10s %-20s %-10d %-10.2f %-10.2f\n",
                    carrito[i].producto.codigo,
                    carrito[i].producto.nombre,
                    carrito[i].cantidad_vendida,
                    carrito[i].producto.precio_venta,
                    total_item);
        }
        printf("============================================\n");
        printf("TOTAL A PAGAR: %.2f\n", gran_total);
        printf("============================================\n");

        printf("\n¿Confirmar venta? (s/n): ");
        char confirmar;
        scanf(" %c", &confirmar);

        if (confirmar == 's' || confirmar == 'S') {
            registrar_venta(carrito, num_items, fecha_hoy, factura_actual);
            guardar_productos(inventario, num_productos);
            printf("Venta confirmada y registrada con exito.\n");
        } else {
            printf("Venta cancelada.\n");
        }
    } else {
        printf("No se registraron productos en la venta.\n");
    }

    liberar_productos(inventario);
    return 0;
}
