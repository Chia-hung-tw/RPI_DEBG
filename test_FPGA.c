#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define MODE_READ 0
#define MODE_WRITE 1
#define MODE_SET   1      // redundant
#define MODE_CLR 2
#define MODE_INPUT_READ 3

#define PULL_UP 0
#define PULL_DOWN 1
#define NO_PULL 2

#define GPIO_BEGIN 0
#define GPIO_END 1
#define NO_ACTION 2

static unsigned char BusMode;         // global to remember status of gpio lines (IN or OUT)

#define STpin RPI_V2_GPIO_P1_12
#define RWpin RPI_V2_GPIO_P1_11
#define AD0pin RPI_V2_GPIO_P1_07
#define AD1pin RPI_V2_GPIO_P1_13
#define AD2pin RPI_V2_GPIO_P1_15
#define AD3pin RPI_V2_GPIO_P1_29
#define D0pin RPI_V2_GPIO_P1_37
#define D1pin RPI_V2_GPIO_P1_36
#define D2pin RPI_V2_GPIO_P1_22
#define D3pin RPI_V2_GPIO_P1_18
#define D4pin RPI_V2_GPIO_P1_38
#define D5pin RPI_V2_GPIO_P1_40
#define D6pin RPI_V2_GPIO_P1_33
#define D7pin RPI_V2_GPIO_P1_35
#define ACKpin RPI_V2_GPIO_P1_16


/*****************************************************/
/* define commands for Master FPGA */
/*****************************************************/
#define CMD_IDLE         0x80
#define CMD_RESETPULSE   0x88
#define CMD_WRPRBITS     0x90
#define CMDH_WRPRBITS    0x12 
#define CMD_SETSTARTACQ  0x98
#define CMD_STARTCONPUL  0xA0
#define CMD_STARTROPUL   0xA8
#define CMD_SETSELECT    0xB0
#define CMD_RSTBPULSE    0xD8
#define CMD_READSTATUS   0xC0
#define CMDH_READSTATUS  0x18
#define CMD_LOOPBRFIFO   0xF0
#define CMDH_LOOPBRFIFO  0x1E
#define CMD_LOOPBACK     0xF8
#define CMDH_LOOPBACK    0x1F
#define DAC_HIGH_WORD    0x42
#define DAC_LOW_WORD     0x0A
#define TRIGGER_DELAY    0x15// 0x00 to 0x07


int set_bus_init();
int set_bus_read_mode();
int set_bus_write_mode();
int send_command(unsigned char);
int read_command(void);
int read_usedwl();
int read_usedwh();
int write_local_fifo(unsigned char );
int read_local_fifo();
void assign(int);


unsigned char tar;

int main(){

  int res;

  /*  Initialize RPI */
  if (!bcm2835_init())
    return 1;
  set_bus_init();
  // Set the pin to be an output
  bcm2835_gpio_fsel(RWpin, BCM2835_GPIO_FSEL_OUTP);

  /* empty local fifo by forcing extra reads, ignore results */
  int i = 0;
  for(i=0; i < 33000; i = i+1){
    res = read_local_fifo();	
  }

  tar = 0xE8;
  set_bus_write_mode();

  int repeat =256;
  for(i = 0; i < repeat ; ++i){
    assign(i);
    write_local_fifo(tar);
  }

  for(i = 0; i < repeat ; ++i){
  res = read_local_fifo();
  printf ("res: %d \n", res);
  
  }
  
}

void assign(int towrite){
  if(towrite > 255) return;
  int b;
  int bit_content;
  tar = 0;
  for(b = 0 ; b < 8 ; ++b){
      bit_content = towrite >> b & 1;
      tar |= bit_content << b;
  }
  
  printf("input:%d , fill: %x\n", towrite,tar);
}
