CC		= g++
HEADERS	= hcsr04drv.h
OBJS	= hcsr04drv.o  main.o
OUT		= okoshi_driver

$(OUT): $(OBJS) $(HEADERS)
	g++ -std=c++14 -o $(OUT) $(OBJS) -lbcm2835 -lrt

.cpp.o:
	g++ -std=c++14 -O2 -c $<

.PHONY: clean
clean:
	rm -f *.o $(OUT)
