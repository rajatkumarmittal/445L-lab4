#include <stdint.h>
#include <stdlib.h>
#include "parse.h"
#include "fixed.h"

char temperature[]="Temp: ?\0\0\0\0\0\0";

//Parses the buffer for "temp:" and returns the temperature in Kelvin
void parseTemp(char* buffer){
	char search[]= "\"temp\":";
	char comma[]=",";
	char* index = strstr(buffer,search);
	char* end = strstr(index,comma);
	index=&index[7];
	int i=6;
	while( index != end){
		temperature[i]=index[0];
		i++;
		index=&index[1];
	}
	temperature[i]='C';
}

