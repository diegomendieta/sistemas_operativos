# Makefile Múltiple 
# Por Vicente Errázuriz
# Para el curso de Estructuras de Datos y Algoritmos, 2017 - 1, PUC

# El compilador a usar: Gnu C Compiler, Standard 2011 with GNU extensions
CC=gcc -std=gnu11

# La carpeta donde va todo el código
SRC=src

# La carpeta donde van todos los archivos de objeto
OBJ=obj

################################################################################
# OPTIMIZACIÓN                                                                 #
################################################################################

# Comentar uno, descomentar el otro
OPT=-g  # No optimiza, guarda toda la info para debugging.
# OPT=-O3  # Optimiza al máximo, descarta info para debugging.

################################################################################
# PARÁMETROS                                                                   #
################################################################################

#   -Wunused avisa de variables sin usar.
#   -Wall avisa de todos los posibles errores de compilación.
CFLAGS=-Wunused -Wall $(OPT)

################################################################################
# LIBRERÍAS                                                                    #
################################################################################

# Librería de matemáticas
MTH=-lm
LIB=$(MTH)

################################################################################
# MÓDULOS Y PROGRAMAS                                                          #
################################################################################

# Directorios con elementos de uso común
COMMON=common

# Directorios que serán compilados a un programa
PROGRAMS=crtree

# Todos los directorios que contienen archivos de código
# SRCDIR=$(COMMON) $(PROGRAMS)
SRCDIR=$(COMMON) $(PROGRAMS)

################################################################################
# DEPENDENCIAS Y DIRECTORIOS                                                   #
################################################################################

# Todos los archivos .h de las carpetas comunes
# DEPS := $(foreach i, $(COMMON), $(shell find $(SRC)/$(i) -name '*.h'))

# Todos los archivos .h
HDRFILES := $(shell find $(SRC) -name '*.h')

# Todos los archivos .c
SRCFILES := $(shell find $(SRC) -name '*.c')

# Archivos de objeto .o, un estado intermedio de compilación
OBJFILES := $(foreach i, $(SRCFILES), $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(i)))

# Los directorios para los archivos de objeto .o
OBJDIR := $(patsubst $(SRC)/%, $(OBJ)/%, $(shell find $(SRC) -type d))

################################################################################
# REGLAS                                                                       #
################################################################################

# Imprime cuando todo está listo.
all: $(OBJDIR) $(PROGRAMS)
	@echo "done compiling"

# Elimina registros de compilación.
clean:
	@rm -fv $(PROGRAMS) && rm -rfv obj && echo "done cleaning"

# Crea directorios para guardar archivos .o
$(OBJDIR):
	@mkdir -p $@

# Indica que las siguientes reglas deben ser ejecutadas en dos pasadas.
.SECONDEXPANSION:

# Dependencias locales para un archivo .o
LOCAL_DEPS = $(filter $(patsubst $(OBJ)/%, $(SRC)/%, $(dir $(1)))%, $(HDRFILES))

# Compila archivos .o si corresponde.
obj/%.o: src/%.c $$(call LOCAL_DEPS,$$@) $(DEPS) Makefile
	@$(CC) $(CFLAGS) $< -c -o $@ $(LIB) && echo "compiled '$@'"

# Compila a partir de archivos .o
$(PROGRAMS): $$(filter obj/$$@/% $(foreach i, $(COMMON), obj/$(i)/%), $(OBJFILES))
	@$(CC) $(CFLAGS) $^ -o $@ $(LIB) && echo "compiled '$@'"
