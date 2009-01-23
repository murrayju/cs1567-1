wall: wall.c ../PIDlib/PIDlib.h PIDlib.o
	gcc `pkg-config --cflags playerc` -o wall `pkg-config --libs playerc` wall.c ../PIDlib/PIDlib.h PIDlib.o
	
PIDlib.o: ../PIDlib/PIDlib.c ../PIDlib/PIDlib.h
	gcc `pkg-config --cflags playerc` -c `pkg-config --libs playerc` ../PIDlib/PIDlib.h ../PIDlib/PIDlib.c

