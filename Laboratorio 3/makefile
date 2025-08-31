# Variables
SRC_DIR = .
BIN_DIR = bin
JAVAC = javac
JAVA = java
JFLAGS = -g

# Archivos fuente
SOURCES = $(SRC_DIR)/Lab3.java

# Archivos de clase (deben ir en el directorio bin)
CLASSES = $(BIN_DIR)/Lab3.class

# Objetivo por defecto
all: $(CLASSES)

# Crear el directorio bin si no existe
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compilar Lab3.java a Lab3.class
$(BIN_DIR)/Lab3.class: $(SOURCES) | $(BIN_DIR)
	$(JAVAC) $(JFLAGS) -d $(BIN_DIR) $(SOURCES)

# Ejecutar el programa (asume que Lab3 tiene un método main())
run: $(CLASSES)
	$(JAVA) -cp $(BIN_DIR) Lab3

# Limpiar archivos generados
clean:
	rm -rf $(BIN_DIR)

# Depuración
debug:
	@echo "Fuentes: $(SOURCES)"
	@echo "Clase generada: $(CLASSES)"
