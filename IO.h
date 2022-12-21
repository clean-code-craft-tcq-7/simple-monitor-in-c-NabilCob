#ifndef IO_H_
#define IO_H_

typedef struct{
    float value;
    int breachLevel;
    int Warning;
    int errorMessage;
}tst_PrintParm;

void PrintMessage(void*);

#endif

