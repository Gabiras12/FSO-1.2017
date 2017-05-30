run:
	gcc -shared -fPIC ../libseno/libseno.c -std=c99 -o build/libseno.so
	gcc -Wl,-rpath=build -o build/main.run src/main.c -L./build -I../libseno -lseno -lm -ldl

clean:
	rm build/*
