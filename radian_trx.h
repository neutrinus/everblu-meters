 /*  the radian_trx SW shall not be distributed  nor used for commercial product*/
 /*  it is exposed just to demonstrate CC1101 capability to reader water meter indexes */
 /*  there is no Warranty on radian_trx SW */

//gcc radian_trx.c -o radian_trx -lwiringPi -lpthread -Wall
#include "time.h"
#include "stdio.h"
#include "stdarg.h"
#include "stdlib.h"
#include "pthread.h"
#include "stdint.h"
#include "string.h"

#include "wiringPi.h"
/* wiringPi install 
tar xfz wiringPi-xxxx.tar.gz
cd wiringPi-xxxx
./build

run the gpio command to check the installation:
gpio -v
gpio readall

to use SPI 
pi@MinePi ~ $ gpio load spi
*/

#ifdef __unix__
# include <time.h>
# include <unistd.h>    // pour sleep
# include <termios.h>
//# include <ncurses.h>   // pour getch mais fonctionne bizarement
	char getch(){
		/*#include <unistd.h>   //_getch*/
		/*#include <termios.h>  //_getch*/
		char buf=0;
		struct termios old={0};
		fflush(stdout);
		if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
		old.c_lflag&=~ICANON;
		old.c_lflag&=~ECHO;
		old.c_cc[VMIN]=1;
		old.c_cc[VTIME]=0;
		if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
		if(read(0,&buf,1)<0)
        perror("read()");
		old.c_lflag|=ICANON;
		old.c_lflag|=ECHO;
		if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
		printf("%c",buf);
		return buf;
	}

#elif defined _WIN32
# include <windows.h>
# include <conio.h> // pour getch
#define sleep(x) Sleep(1000 * x)
#endif

const char *DateOFCompil = __DATE__; //The compilation date of the current source file.
const char *TimeOFCompil = __TIME__; //The most recent compilation time of the current source file.

typedef unsigned char T_BOOL;       //1 octets

//RH PIN allocation  to GPIO number
/*P1: The Main GPIO connector
WiringPi Pin = WPP
WPP Name 	Header	Name 	WPP
	3.3v	1	2	5v
8	SDA		3 	4	5v
9  	SCL 	5 	6 	0v
7  	GPIO7 	7 	8 	TxD 	15
	0v		9 	10 	RxD 	16
0	GPIO0 	11 	12 	GPIO1 	1
2	GPIO2	13 	14 	0v
3	GPIO3	15	16	GPIO4   4
	3.3v	17	18	GPIO5 	5
12	MOSI 	19 	20	0v
13	MISO 	21 	22	GPIO6	6
14  SCLK 	23	24	CE0		10
	0v		25	26	CE1		11
WPP Name 	Header	Name 	WPP*/
#define GDO2 2 //header  13
#define GDO1_MISO 13
#define GDO0 0 //header  11
#define MOSI 12
#define cc1101_CSn 10 ////header  24 
#define LED  3 //header 15


#include "utils.c"
#include "wiringPi.h"
#include "wiringPiSPI.h"
#include "cc1101.c"
