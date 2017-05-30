run:
	gcc -c ../libseno/libseno.c -o build/libseno.o -std=c99
	ar rcs build/libseno.a build/libseno.o
	gcc -static src/main.c -L./build -I../libseno -lseno -o build/main.run -lm

clean:
	rm build/*
