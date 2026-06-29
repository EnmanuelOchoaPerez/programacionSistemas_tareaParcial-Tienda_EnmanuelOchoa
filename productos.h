#ifndef PRODUCTOS_H
#define PRODUCTOS_H

typedef struct {
    char codigo[50];
    char nombre[100];
    int cantidad;
    float costo;
    float precio_venta;
} Producto;

typedef struct {
    Producto producto;
    int cantidad_vendida;
} ItemVenta;

Producto* cargar_productos(int* num_productos);

void guardar_productos(Producto* productos, int num_productos);

int buscar_producto(Producto* productos, int num_productos, const char* codigo);

void registrar_venta(ItemVenta* carrito, int num_items, const char* fecha, int num_factura);

int obtener_siguiente_factura();

void liberar_productos(Producto* productos);

#endif
