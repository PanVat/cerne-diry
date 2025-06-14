CPP=g++
SRC= main.cpp screen.cpp kerr_ray.cpp kepler_disk.cpp kerr.cpp pline.cpp num.cpp
FILES=$(addprefix ./src/,$(SRC))
LIBS=-lgsl -lgslcblas -lm
LIBDIR=-L../lib		# Knihovny se nacházejí v kořenovém adresáři projektu ve složce \lib
INCDIR=-I../include # Hlavičkové soubory se nacházejí v kořenovém adresáři projektu ve složce \include

build: $(FILES)
	$(CPP) -o ./run/kray $(FILES) $(INCDIR) $(LIBDIR) $(LIBS)