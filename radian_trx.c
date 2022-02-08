 /*  the radian_trx SW shall not be distributed  nor used for commercial product*/
 /*  it is exposed just to demonstrate CC1101 capability to reader water meter indexes */
 /*  there is no Warranty on radian_trx SW */
 
#include "radian_trx.h"
/**********************************************************************/
/*                           initialization                           */
/**********************************************************************/ 

void IO_init(void)
{
   wiringPiSetup () ;
   pinMode (LED, OUTPUT);
   digitalWrite (LED, 1) ;
   pinMode (GDO2, INPUT);
   pinMode (GDO0, INPUT);           
   
   cc1101_init();
   digitalWrite (LED, 0) ;
}

/**********************************************************************/
/*                           MAIN                                     */
/**********************************************************************/ 
void display_command_help (void)
{
   printf("\r\ncommand list :\r\n");
   printf("exit:x ;cc1101 version:v; read config : c\r\n");
   printf("m:MARCSTATE a:MCSM1_val s:full_status\r\n");
   printf("H:scenario de l'outil de reléve !\r\n");
   printf("R:Reset CC1101\r\n\r\n");
   
}

int main(int argc, char *argv[])
{ 
  char c_command;
  uint8_t ret8;
  
  fflush(stdout);
  IO_init();
 
  //printf("argc=%i\r\n",argc);
  //printf("argv=%i\r\n",argv[0]);
  if (argc == 2) // if one parameters 
  {
	  debug_out=0;
	  ret8=scenario_releve();
	  if (ret8 < 30)
	   {
	      sleep(3);
		  ret8=scenario_releve();
	      if (ret8 < 30)
		  {
		    sleep(3);
		    ret8=scenario_releve();
		  }
	   }
	  //test_rx();
	  return 0;
  }	  
  if (debug_out)printf("raspian radian trx builded:%s %s\r\n",DateOFCompil,TimeOFCompil);  
  if (debug_out)display_command_help();
  c_command = getch();
  while (c_command != 'x')
  {
    switch (c_command)
	{
	   case 'a': printf("MCSM1: 0x%02X\r\n", halRfReadReg(MCSM1));break;
	   case 'H': ret8=scenario_releve(); break;  //scenario de l'outil de reléve !	   
	   case 'R': cc1101_reset();break;
	   case 's': echo_cc1101_full_status();break;
	   case 'm': echo_cc1101_MARCSTATE(); break;
	   case 'c': show_cc1101_registers_settings(); break;
	   case 'v': echo_cc1101_version(); break;
	   case '?': display_command_help(); break;
	   default : break;
    } //switch c_command
    c_command = getch();
  }// while command

  return 0;
}
