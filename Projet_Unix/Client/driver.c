#include "driver.h"

void driver(int* pfddriver)
{   
    char str_driver[700]= {};
    read(pfddriver[0],str_driver,sizeof(str_driver)+1);
    printf("Driver:\n%s", str_driver);
    
}