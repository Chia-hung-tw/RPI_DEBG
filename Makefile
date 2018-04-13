all: fpga_debug

fpga_debug: test_FPGA.o gpiolib.o bcm2835.o
	gcc $^ -lm -o $@

test_FPGA.o: test_FPGA.c
	gcc -c $^ -I ./RPi_software/bcm2835-1.52/src ./RPi_software/bcm2835-1.52/src/bcm2835.c       

gpiolib.o: gpiolib.c
	gcc -c -I ./RPi_software/bcm2835-1.52/src ./RPi_software/bcm2835-1.52/src/bcm2835.c gpiolib.c


clean:
	rm -f fpga_debug
	rm -f *.o
