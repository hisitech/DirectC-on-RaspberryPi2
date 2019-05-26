VPATH = Release:Release/G3Algo:Release/G4Algo:Release/JTAG

object = main.o \
         dpalg.o dpcom.o dpuser.o dputil.o \
         dpcore.o dpfrom.o dpG3alg.o dpnvm.o dpsecurity.o \
         dpG4alg.o \
         dpchain.o dpjtag.o

main : main.o $(object)
	cc -o main $(object) -l wiringPi

main.o :
dpalg.o : dpuser.h dputil.h dpalg.h dpG3alg.h dpG4alg.h dpjtag.h
dpcom.o : dpuser.h dpcom.h dpjtag.h
dpuser.o : dpuser.h dpalg.h dputil.h
dputil.o : dpuser.h dputil.h dpcom.h dpalg.h
dpcore.o : dpcore.h dputil.h dpalg.h dpG3alg.h dpnvm.h dpsecurity.h dpcom.h dpjtag.h
dpfrom.o : dpfrom.h dpuser.h dputil.h dpalg.h dpG3alg.h dpsecurity.h dpcom.h dpjtag.h
dpG3alg.o : dpG3alg.h dpuser.h dputil.h dpcom.h dpalg.h dpcore.h dpfrom.h dpnvm.h dpsecurity.h dpjtag.h
dpnvm.o : dpnvm.h dpuser.h dputil.h dpalg.h dpG3alg.h dpcom.h dpjtag.h
dpsecurity.o : dpsecurity.h dpuser.h dputil.h dpalg.h dpG3alg.h dpcom.h dpjtag.h
dpG4alg.o : dpG4alg.h dpuser.h dputil.h dpcom.h dpalg.h dpjtag.h
dpchain.o : dpchain.h dpuser.h dpjtag.h dpcom.h
dpjtag.o : dpjtag.h dpuser.h dputil.h dpcom.h dpchain.h dpalg.h dpG3alg.h

clean :
	-rm main $(object)
