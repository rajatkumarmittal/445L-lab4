#include <String.h>

extern char temperature[];

//Compares the first "length" characters of two strings. Returns True if equivalent 
int compareCharArray(char* str1, char* str2, int length);

//Parses the buffer for "temp:" and returns the temperature in Kelvin
void parseTemp(char* buffer);

float kelvinToCelcius(float kelvin);

void updateTemp(char* buffer);

