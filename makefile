CC = g++
CFLAGS = -std=c++11 -c
LD = g++
LFLAGS = -std=c++11

source = $(wildcard src/*.cpp)
objects = $(patsubst src/%,obj/%, $(patsubst %.cpp, %.o, $(source)))

raytracer.exe: $(objects)
	@echo "Linking raytracer"
	@g++ $^ -std=c++11 -o $@
	@echo "Done making raytracer.exe"

debug: $(objects)
	@echo "Linking debug raytracer.exe"
	@g++ $^ -std=c++11 -DDEBUG -g -o raytracer.exe
	@echo "Done making debug raytracer.exe"

obj/%.o: src/%.cpp obj
	@echo "Compiling $<"
	@g++ -c $< -std=c++11 -o $@

obj:
	@mkdir obj

clean: 
	@rm *.exe *.out *.ppm obj/*.o
