tienda.exe: main.o usuarios.o productos.o
	gcc -o tienda main.o usuarios.o productos.o

main.o: main.c usuarios.h productos.h
	gcc -c main.c -o main.o

usuarios.o: usuarios.c usuarios.h
	gcc -c usuarios.c -o usuarios.o

productos.o: productos.c productos.h
	gcc -c productos.c -o productos.o

objdump: main.o usuarios.o productos.o
	objdump -d main.o > main_objdump.s
	objdump -d usuarios.o > usuarios_objdump.s
	objdump -d productos.o > productos_objdump.s

clean:
	rm -f *.o *.s tienda
