 /*  the radian_trx SW shall not be distributed  nor used for commercial product*/
 /*  it is exposed just to demonstrate CC1101 capability to reader water meter indexes */
 /*  there is no Warranty on radian_trx SW */

#include "time.h"
#include "stdio.h"
#include "stdarg.h"
#include "stdlib.h"
#include "pthread.h"
#include "stdint.h"
#include "string.h"

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

#include "cc1101.h"

typedef unsigned char T_BOOL;       //1 octets

#define GDO2 2 //header  13
#define GDO0 0 //header  11 

#define METER_YEAR              16
#define METER_SERIAL            123456

#define MQTT_HOST "localhost"
#define MQTT_PORT  1883
#define MQTT_USER "homeassistant"
#define MQTT_PASS "PASS"


#define MQTT_KEEP_ALIVE 60
#define MQTT_MSG_MAX_SIZE  512


