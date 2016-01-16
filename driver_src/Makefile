CC		= g++
HEADERS	= hcsr04drv.h
OBJS	= hcsr04drv.o util.o  main.o
OUT		= okoshi_driver

$(OUT): $(OBJS) $(HEADERS)
	$(CC) -std=c++14 -o $(OUT) $(OBJS) -lbcm2835 -lrt

.cpp.o:
	$(CC) -std=c++14 -O2 -c $<

.PHONY: clean
clean:
	rm -f $(OBJS) $(OUT)
