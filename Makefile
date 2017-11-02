CXX := g++ -g -std=c++11

all: bin/evaluateSolution 

bin/evaluateSolution: build/evaluateSolution.o build/SharedElements.o
	@ echo linking evaluateSolution
	@ mkdir -p bin
	@$(CXX) build/evaluateSolution.o build/SharedElements.o -o bin/evaluateSolution

build/%.o: src/%.cpp src/%.h
	@ echo building $<
	@ mkdir -p build
	@$(CXX)  -c $< -o $@

build/%.o: src/%.cpp 
	@ echo building $<
	@ mkdir -p build
	@$(CXX)  -c $< -o $@

clean:
	rm -rf build

distclean: clean
	rm -rf bin
