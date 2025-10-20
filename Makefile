#makefile para la tarea 2 de datos

#variables
CC = g++
CFLAGS = -Wall -std=c++11
TARGET = DataBalatro

#regla por defecto: compilar el programa
all: $(TARGET)

#regla para compilar el .cpp
$(TARGET): main.cpp
	$(CC) $(CFLAGS) -o $(TARGET) main.cpp

#regla para limpiar los archivos generados
clean:
	rm -f $(TARGET)

#regla para ejecutar el programa
run: $(TARGET)
	./$(TARGET)
