/*
 * global.c
 *
 *  Created on: 2022. 3. 29.
 *      Author: luckyman
 */


/*
 * global.c
 *
 *  Created on: 2022. 3. 24.
 *      Author: luckyman
 */


#include "global.h"


const char hextable[] = {
 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,-1,-1,-1,-1,-1,-1,
-1,10,11,12,13,14,15,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,10,11,12,13,14,15,
};

const char chtable[] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};
long ch2hex(char *hex, uint8_t size) {
   long ret = 0;

   while(size > 0)
   {
	   if(*hex < 48 || *hex > 102)
		   return -0x100;

	   if(hextable[*hex - 48] == -1)
		   return -0x100;

	   ret = (ret << 4) | hextable[*hex - 48];

	   size--;
	   hex++;
   }
   return ret;
}

void hex2ch(uint8_t hex, char *chBuff)
{
	for(int i = 0; i < 2; i++)
	{
		chBuff[i] = chtable[(hex >> (4 - i*4)) & 0x0F];
	}
	return;
}




