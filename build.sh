if [ ! -d "bin" ]; then
    mkdir bin
else
	rm bin/*
fi
gcc -g -O0 -I . -o bin/main Parent.c
gcc -g -O0 -I . -o bin/Child Child.c
