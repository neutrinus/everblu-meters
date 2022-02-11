 /*  the radian_trx SW shall not be distributed  nor used for commercial product*/
 /*  it is exposed just to demonstrate CC1101 capability to reader water meter indexes */
 /*  there is no Warranty on radian_trx SW */
 
#include "everblu_meters.h"

void IO_init(void)
{
   wiringPiSetup () ;
   pinMode (GDO2, INPUT);
   pinMode (GDO0, INPUT);           

   cc1101_init();
}


int main(int argc, char *argv[])
{ 
	fflush(stdout);
	IO_init();
	scenario_releve();

	return 0;
}
