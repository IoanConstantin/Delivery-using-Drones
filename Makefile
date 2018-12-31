build: livrare

livrare: livrare.c
	gcc -Wall livrare.c -o livrare -std=c99

clean:
	rm -rf livrare