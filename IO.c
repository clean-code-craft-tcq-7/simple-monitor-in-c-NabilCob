#include <stdio.h>
#include "IO.h"


void PrintConsole(float value, char* breachlevel, char *print_str){
    printf("\n abnormal measure =%f, breach - %s, error = %s", value, breachlevel,print_str);
}
