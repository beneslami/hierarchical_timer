TARGET:exe

exe: main.o WheelTimer.o LinkedListApi.o
	gcc -g main.o WheelTimer.o LinkedListApi.o -o exe -lpthread

main.o:
	gcc -g -c main.c -o main.o

LinkedListApi.o:
		gcc -g -c LinkedListApi.c -o LinkedListApi.o

WheelTimer.o:
	gcc -g -c WheelTimer.c -o WheelTimer.o



clean:
	rm -rf *.o exe
