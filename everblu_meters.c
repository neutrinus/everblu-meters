 /*  the radian_trx SW shall not be distributed  nor used for commercial product*/
 /*  it is exposed just to demonstrate CC1101 capability to reader water meter indexes */

#define METER_YEAR              16
#define METER_SERIAL            123456

#define MQTT_HOST "localhost"
#define MQTT_PORT  1883
#define MQTT_USER "homeassistant"
#define MQTT_PASS "PASS"


#define MQTT_KEEP_ALIVE 60
#define MQTT_MSG_MAX_SIZE  512


#include <stdio.h>
#include <stdlib.h>
#include <mosquitto.h>
#include <string.h>

#include "everblu_meters.h"
#include "cc1101.c"

void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{

	if(message->payloadlen){
		printf("%s %s", message->topic, (char *)message->payload);
	}else{
		//printf("%s (null)\n", message->topic);
	}
	fflush(stdout);
}

void my_connect_callback(struct mosquitto *mosq, void *userdata, int result)
{
	if(!result){
		/* Subscribe to broker information topics on successful connect. */
		mosquitto_subscribe(mosq, NULL, "WaterUsage ", 2);
	}else{
		fprintf(stderr, "Connect failed\n");
	}
}

void my_subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos)
{
	int i;
	printf("Subscribed (mid: %d): %d", mid, granted_qos[0]);
	for(i=1; i<qos_count; i++){
		printf(", %d", granted_qos[i]);
	}
	printf("\n");
}

void my_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str)
{
	/* Pring all log messages regardless of level. */
	printf("%s\n", str);

}


void IO_init(void)
{
	wiringPiSetup();
	pinMode (GDO2, INPUT);
	pinMode (GDO0, INPUT);           

	cc1101_init();
}


int main(int argc, char *argv[])
{
	struct tmeter_data meter_data;
	struct mosquitto *mosq = NULL;
	char buff[MQTT_MSG_MAX_SIZE];
	char meter_id[12];
	char mqtt_topic[64];

	sprintf(meter_id, "%i_%i", METER_YEAR, METER_SERIAL);
	

	mosquitto_lib_init();
	mosq = mosquitto_new(NULL, true, NULL);
	if(!mosq){
		fprintf(stderr, "ERROR: Create MQTT client failed..\n");
		mosquitto_lib_cleanup();
		return 1;
	}
	
	//Set callback functions
	mosquitto_log_callback_set(mosq, my_log_callback);
	mosquitto_connect_callback_set(mosq, my_connect_callback);
	mosquitto_message_callback_set(mosq, my_message_callback);
	mosquitto_subscribe_callback_set(mosq, my_subscribe_callback);

	mosquitto_username_pw_set(mosq, MQTT_USER, MQTT_PASS);

	//Connect to MQTT server
	if(mosquitto_connect(mosq, MQTT_HOST, MQTT_PORT, MQTT_KEEP_ALIVE)){
		fprintf(stderr, "ERROR: Unable to connect to MQTT broker.\n");
		return 1;
	}
	//Start a thread, and call mosquitto? Loop() continuously in the thread to process network information
	int loop = mosquitto_loop_start(mosq);
	if(loop != MOSQ_ERR_SUCCESS)
	{
		fprintf(stderr, "ERROR: failed to create mosquitto loop");
		return 1;
	}

	IO_init();
	meter_data = get_meter_data();

	sprintf(buff, "%d", meter_data.liters);
	sprintf(mqtt_topic, "homeassistant/sensor/cyblemeter_%s/state", meter_id);

	printf("Liters: %i\n", meter_data.liters);
	mosquitto_publish(mosq, NULL, mqtt_topic, strlen(buff),buff,0,false);


	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();


	return 0;
}
