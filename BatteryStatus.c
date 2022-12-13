#include <assert.h>
#include "BatteryStatus.h"
#include "IO.h"

//
const tst_BatParamLimits TempLimit = {
 .minValue = TEMP_MIN_VALUE,
 .maxValue = TEMP_MAX_VALUE
};
const tst_BatParamLimits SocLimit = {
 .minValue = SOC_MIN_VALUE,
 .maxValue = SOC_MAX_VALUE
};
const tst_BatParamLimits ChargeRateLimit = {
 .minValue =CHARGE_RATE_MIN_VALUE,
 .maxValue =CHARGE_RATE_MAX_VALUE
};
//
int BatteryState[MAX_BATTERY_PARAMETERS] = {0};

//
int batteryIsOk(float temperature, float soc, float chargeRate) {

  int lp;
  BatteryState[BATTERY_TEMPERATURE] = GetBatteryParamStatus(temperature, TempLimit,
                          PrintConsole, "Temperature out of range!\n");
  BatteryState[SOC] = GetBatteryParamStatus(soc, SocLimit,
                  PrintConsole, "State of Charge out of range!\n");
  BatteryState[CHARGE_RATE] = GetBatteryParamStatus(chargeRate, ChargeRateLimit,
                  PrintConsole, "Charge Rate out of range!\n");

  for(lp = 0; lp < MAX_BATTERY_PARAMETERS; lp ++) {
      if(BatteryState[lp] > VALID_RANGE)
      {
         return 0;
      }
  }
  return 1;
}


//
int GetBatteryParamStatus(float value, tst_BatParamLimits limits,
                void (*printConsole)(float, char*, char*), char* printstr){

    if(value < limits.minValue ){
         (*printConsole)(value, "LOW", printstr);
        return INVALID_RANGE_LOW;
    }
    else if(value > limits.maxValue){
         (*printConsole)(value, "HIGH", printstr);
        return INVALID_RANGE_HIGH;
    }
    else{
       return VALID_RANGE;
    }
}


int main() {
  assert(batteryIsOk(25, 70, 0.7));
  assert(!batteryIsOk(50, 85, 0));
  assert(!batteryIsOk(70, 0, 0));
  assert(batteryIsOk(45, 80, 0.8));
  assert(batteryIsOk(0, 20, 0.8));
 
 
  return 0;
}
