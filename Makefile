CPP=g++
SRC=main.cpp screen.cpp kerr_ray.cpp kepler_disk.cpp kerr.cpp pline.cpp num.cpp
FILES=$(addprefix ./src/,$(SRC))
LIBS=-Llib -lgsl -lgslcblas -lm
INCDIR=-Iinclude    # Hlavičkové soubory jsou v ./include (relativně k Makefile)
# LIBDIR=-Llib      # Sloučeno do LIBS

build: $(FILES)
	$(CPP) -o ./run/kray $(FILES) $(INCDIR) $(LIBS)