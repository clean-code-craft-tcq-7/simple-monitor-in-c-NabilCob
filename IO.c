#include <stdio.h>
#include "IO.h"
#include "LanguageString.h"
#include "ApplicationConfig.h"


void PrintMessage(void* printmessage){
    tst_PrintParm* printValue = (tst_PrintParm*)printmessage;
    printf("\n%f, %s, %s, %s", printValue->value, stringsArray[printValue->Warning].LangArray[Langaugedefined],
                  stringsArray[printValue->breachLevel].LangArray[Langaugedefined],
                  stringsArray[printValue->errorMessage].LangArray[Langaugedefined]);
}

