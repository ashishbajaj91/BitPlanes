CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`
main : main.cpp
		g++ $(CFLAGS) -o $@ $< $(LIBS)
