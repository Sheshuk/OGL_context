CFLAGS=-O3 -Wall -std=c++0x `sdl-config --cflags` -D _USE_OPENGL
# CFLAGS+=-D NCOLOR
# CFLAGS+=-D NDEBUG
LIBS=-lSDL -lSDL_image -lSDL_ttf -lGL -lGLU -ljpeg
INCDIR=include/

CC=g++

%.o: src/%.cxx
	$(CC) -c $(CFLAGS) -I $(INCDIR) $< -o $@


%: %.o
	$(CC) -o $@ $^ $(OBJ) $(LIBS)
	strip -s $@

all: test_gl 

test_gl: GLcontext.o taylor.o GLshader.o GLcamera.o GLerror.o


run: test_gl
	./test_gl

clean:  
	rm -f *.o *~ test_gl

fresh : clean clearscr all

clearscr:
	clear
