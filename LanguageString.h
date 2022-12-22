#ifndef LANGUAGE_STRING_H_
#define LANGUAGE_STRING_H_

typedef enum{
    Temperature_out_of_range,
    State_of_Charge_out_of_range,
    Charge_Rate_out_of_range,
    Breach_Low,
    Breach_High,
    Approaching_discharge,
    Approaching_charge_peak,
    Temperature_warning,
    Soc_Warning,
    ChargeRate_Warning,
    Default,
    Valid,
    Max_error_messages,
}errorList;


typedef enum{
    English,
    German,
    MaxsupportedLanguages,
}LanguagesSupported;


typedef struct{
    char *LangArray[MaxsupportedLanguages];
}LangArray_t;

extern const LangArray_t stringsArray[Max_error_messages];
#endif

