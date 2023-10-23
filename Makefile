all:
	@clang++ $(wildcard src/*.cpp src/lexer/*.cpp src/parser/*.cpp src/transpiler/*.cpp) -o out/blue
	out/blue test/prog1.blu test/prog1
	./test/prog1

run:
	out/blue test/prog1.blu test/prog1
	./test/prog1