#ifndef BATTERYSTATUS_H_
#define BATTERYSTATUS_H_

#define TEMP_MIN_VALUE   (0)
#define TEMP_MAX_VALUE   (45)
#define Temp_Tolerance   (5)

#define SOC_MIN_VALUE    (20)
#define SOC_MAX_VALUE    (80)
#define Soc_Tolerance   (5)

#define CHARGE_RATE_MIN_VALUE  (-1)
#define CHARGE_RATE_MAX_VALUE   (0.8)
#define ChargeRate_Tolerance   (5)

typedef enum{
    BATTERY_TEMPERATURE = 0,
    SOC,
    CHARGE_RATE,
    MAX_BATTERY_PARAMETERS,
}tst_BatParm;

typedef enum{
    VALID_RANGE =0,
    INVALID_RANGE_LOW,
    INVALID_RANGE_HIGH,
} tst_BatParmStates;


typedef struct{
    float minValue;
    float maxValue;
}tst_BatteryParmLimits;


typedef struct{
    int tolerence;
    tst_BatteryParmLimits limits;
}tst_TolerenceLimits;

typedef struct{
    float value;
    tst_BatteryParmLimits limits;
    tst_TolerenceLimits tolerancelimits;
    int status;
    int errorMessage;
    int warningMessage;
    int (*callback_warning)(void*, void*);
}tst_SensorInputs;

typedef struct{
    tst_SensorInputs data[MAX_BATTERY_PARAMETERS];
    void (*printMessage)(void*);
}tst_SensorParm;


int batteryIsOk(float temperature, float soc, float chargeRate);
int GetBatteryParamStatus(void*, void*);
int GetBatteryParamWarningStatus(void*, void*);
int CalculateTolerance(int value, int tolerance);
void init_Limits(void *inputList, float value, tst_BatteryParmLimits limit, int tolerance);
void init_registerCallback(void *inputList, void *callback_fn);
void init_ConfigureMessage(void *inputList, void * errormsg, void * warningmsg);
#endif

