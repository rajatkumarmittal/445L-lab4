// January 17, 2018
// Andrew Ferrari (acf2282)
// TA: Sam
// Last Change 01/21/18
// Lab 1
// Fixed Point Output Module
// Runs on TM4C123
// Uses ST7735.c LCD.

// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected 
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO)
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "fixed.h"
#include "ST7735.h"

/****************ST7735_sDecOut2***************
 converts fixed point number to LCD
 format signed 32-bit with resolution 0.01
 range -99.99 to +99.99
 Inputs:  signed 32-bit integer part of fixed-point number
 Outputs: none
 send exactly 6 characters to the LCD 
Parameter LCD display
 12345    " **.**"
  2345    " 23.45"  
 -8100    "-81.00"
  -102    " -1.02" 
    31    "  0.31" 
-12345    "-**.**"
 */ 
void ST7735_sDecOut2(int32_t n){
	int x;
	char val[6] = "   .  ";
	int32_t absVal = n<0 ? -n : n;
	
	//Calculate ASCII Values
	val[5]=48+absVal%10;
	val[4]=48+(absVal%100)/10;
	val[2]=48+(absVal%1000)/100;
	val[1]=48+(absVal%10000)/1000;
	val[0]=' ';
	
	//Shift Blank Space if needed
	if(val[1]=='0') {
		val[1]=' ';
	}
	
	//Handle Error Values
	if(n>9999 || n<-9999)	{
		val[5]=val[4]=val[2]=val[1]='*';
	}
	
	//Handle and Shift Negative Values
	if (n<0) {
		val[0]='-';
		if (val[1]==' '){
			val[0]=' ';
			val[1]='-';
		}
	}
	
	//Output to LCD
	for(x=0;x<6;x++){
		fputc(val[x],NULL);
	}
}


/**************divRoundClosest***************
 divide n by d and return the answer rounded to the nearest integer
 Inputs:  two unsigned 32-bit integers
 Outputs: one unsigned 32-bit integer
*/

int divRoundClosest(int n, int d)
{
  return ( (n < 0) ^ (d < 0) ) ? ((n - d/2)/d) : ((n + d/2)/d);
}

/**************ST7735_uBinOut6***************
 unsigned 32-bit binary fixed-point with a resolution of 1/64. 
 The full-scale range is from 0 to 999.99. 
 If the integer part is larger than 256000, it signifies an error. 
 The ST7735_uBinOut6 function takes an unsigned 32-bit integer part 
 of the binary fixed-point number and outputs the fixed-point value on the LCD
 Inputs:  unsigned 32-bit integer part of binary fixed-point number
 Outputs: none
 send exactly 6 characters to the LCD 
Parameter LCD display
     0	  "  0.00"
     1	  "  0.01"
    16    "  0.25"
    25	  "  0.39"
   125	  "  1.95"
   128	  "  2.00"
  1250	  " 19.53"
  7500	  "117.19"
 63999	  "999.99"
 64000	  "***.**"
*/
void ST7735_uBinOut6(uint32_t n){
	int x;
	char val[6] = "   .  ";
	int num = divRoundClosest(100*n,64);
	
	//Calculate ASCII values
	val[5]=48+num%10;
	val[4]=48+(num%100)/10;
	val[2]=48+(num%1000)/100;
	val[1]=48+(num%10000)/1000;
	val[0]=48+(num%100000)/10000;
	
	//Shift blank space if needed
	if(val[0]=='0') {
		val[0]=' ';
		if(val[1]=='0') {
			val[1]=' ';
		}
	}
	
	//Error Value Handler
	if(num>99999 || num<0) val[5]=val[4]=val[2]=val[1]=val[0]='*';
	
	//Output to LCD
	for(x=0;x<6;x++){
		fputc(val[x], NULL);
	}
} 
	
int max_X,min_X,max_Y,min_Y;
	
/**************ST7735_XYplotInit***************
 Specify the X and Y axes for an x-y scatter plot
 Draw the title and clear the plot area
 Inputs:  title  ASCII string to label the plot, null-termination
          minX   smallest X data value allowed, resolution= 0.001
          maxX   largest X data value allowed, resolution= 0.001
          minY   smallest Y data value allowed, resolution= 0.001
          maxY   largest Y data value allowed, resolution= 0.001
 Outputs: none
 assumes minX < maxX, and miny < maxY
*/
void ST7735_XYplotInit(char *title, int32_t minX, int32_t maxX, int32_t minY, int32_t maxY){
	ST7735_OutString(title);
	ST7735_FillRect(0, 32, 128, 128, ST7735_Color565(228,228,228)); // light grey
	max_X=maxX; min_X=minX; max_Y=maxY; min_Y=minY;
}

/**************ST7735_XYplot***************
 Plot an array of (x,y) data
 Inputs:  num    number of data points in the two arrays
          bufX   array of 32-bit fixed-point data, resolution= 0.001
          bufY   array of 32-bit fixed-point data, resolution= 0.001
 Outputs: none
 assumes ST7735_XYplotInit has been previously called
 neglect any points outside the minX maxY minY maxY bounds
*/
void ST7735_XYplot(uint32_t num, int32_t bufX[], int32_t bufY[]){
	int next;
	float xval;
	float yval;
	for(next=0;next<num;next++){
		if( (bufX[next]<=max_X) && (bufX[next]>=min_X) && (bufY[next]<=max_Y) && (bufY[next]>=min_Y) ){
			xval = (127 * (bufX[next]-min_X)  ) / (max_X-min_X) ;
			yval = 32 + (127 - ((127 * (bufY[next]-min_Y)  ) / (max_Y-min_Y))) ;
			ST7735_DrawPixel( xval, yval,ST7735_BLACK);
		}
	}
}

//************* ST7735_Line********************************************
//  Draws one line on the ST7735 color LCD
//  Inputs: (x1,y1) is the start point
//          (x2,y2) is the end point
// x1,x2 are horizontal positions, columns from the left edge
//               must be less than 128
//               0 is on the left, 126 is near the right
// y1,y2 are vertical positions, rows from the top edge
//               must be less than 160
//               159 is near the wires, 0 is the side opposite the wires
//        color 16-bit color, which can be produced by ST7735_Color565() 
// Output: none
void ST7735_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color){
	float delta_x = abs(x2-x1); 
	float delta_y = abs(y2-y1); 
	int sx = x1 < x2 ? 1 : -1;
	int sy = y1 < y2 ? 1 : -1;
	float delta_err = (delta_x>delta_y ? delta_x : -delta_y)/2, error;

	while(1){  /* loop */
			ST7735_DrawPixel(x1,y1,color);
			if (x1 == x2 && y1 == y2) break;
			error = delta_err;
			if (error >= -delta_x) { delta_err -= delta_y; x1 += sx; } 
			if (error <= delta_y) { delta_err += delta_x; y1 += sy; } 
  }
}

