# Makefile

main:
	gcc everblu_meters.c -o everblu_meters -lwiringPi -lmosquitto -lpthread -Wall
