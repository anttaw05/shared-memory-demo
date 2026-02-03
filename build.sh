if [ ! -d "bin" ]; then
    mkdir bin
else
	rm bin/*
fi
gcc -g -O0 -I . -o bin/main Part2.5.c
gcc -g -O0 -I . -o bin/childP5 childP5.c