#ifndef BATTERYSTATUS_H_
#define BATTERYSTATUS_H_

#define TEMP_MIN_VALUE   (0)
#define TEMP_MAX_VALUE   (45)

#define SOC_MIN_VALUE    (20)
#define SOC_MAX_VALUE    (80)

#define CHARGE_RATE_MIN_VALUE  (-1)
#define CHARGE_RATE_MAX_VALUE   (0.8)

typedef struct{
    float minValue;
    float maxValue;
}tst_BatParamLimits;

typedef struct{
    int tempStatus;
    int socStatus;
    int chargeRateStatus;
}tst_BatParmStatus;

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

int batteryIsOk(float temperature, float soc, float chargeRate);
int GetBatteryParamStatus(float value, tst_BatParamLimits limits,
                void (*printConsole)(float, char*, char*), char* printstr);

#endif
