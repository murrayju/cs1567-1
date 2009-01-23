wall: wall.c ../PIDlib/PIDlib.h ../PIDlib/PIDlib.o
	gcc `pkg-config --cflags playerc` -o wall `pkg-config --libs playerc` wall.c ../PIDlib/PIDlib.h ../PIDlib/PIDlib.o
	
PIDlib.o: ../PIDlib/PIDlib.c ../PIDlib/PIDlib.h
	cd ../PIDlib
	make
	cd ../wall

