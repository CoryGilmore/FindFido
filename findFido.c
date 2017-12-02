#include "getword.h"

int main()
{
	
  int c;
  char buffer[STORAGE];

  for(;;)
  {
   /* This function grabs one word at a time
    * from the ping response and sticks it in 
    * buffer
    */	  
    c = getword(buffer);
    /* when ping message fails packet loss is 100% */
    if(strcmp(buffer, "100%") == 0)
    {
      printf("FAIL");
      return -1;
    }
    if( c == -1)
	    break;
   
  }
  printf("OK");
}
