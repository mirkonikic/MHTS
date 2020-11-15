#include "main.h"

//DODATI AES NA SVE OVO

void encrypt(char *msg, int len)
{

	//M1RK0+ => postoji odredjen key string duzine 4096 bajta
	for(int j = 0; j<len; j++)
	{
		msg[j] = msg[j]+M_KEY[(j+len)%MSG_SIZE];
	}

	//M1Rk0xor => Xor sa X_KEY svaki char i posle xor unazad svaki char
	for(int z = 0; z<len; z++)
	{
		msg[z] = msg[z]^X_KEY;
	}

	//MIRKOrot13 => da se doda svakom slovu duzina cele reci
	for(int i = 0; i<len; i++)
	{
		msg[i] = msg[i]-(len%KEY);
	}

	//AES
}

void decrypt(char *msg, int len)
{
	//AES

	//MIRKOrot13
	for(int i = 0; i<len; i++)
	{
		msg[i] = msg[i]+(len%KEY);
	}

	//M1Rk0XOR
	for(int z = 0; z<len; z++)
	{
		msg[z] = msg[z]^X_KEY;
	}

	//M1Rk0+
	for(int j = 0; j<len; j++)
	{
		msg[j] = msg[j]-M_KEY[(j+len)%MSG_SIZE];
	}
}
