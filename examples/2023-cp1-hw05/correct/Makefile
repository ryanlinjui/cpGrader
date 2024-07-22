all:
	gcc -c tas.c -o tas.o
	gcc -c decode.c -o decode.o
	gcc hw0505.c tas.o decode.o -o hw0505

clean:
	rm hw0505 tas.o decode.o