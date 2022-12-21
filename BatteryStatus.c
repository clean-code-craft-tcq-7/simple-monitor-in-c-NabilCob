#include <assert.h>
#include "BatteryStatus.h"
#include "LanguageString.h"
#include "IO.h"

const tst_BatteryParmLimits TempLimit = {
 .minValue = TEMP_MIN_VALUE,
 .maxValue = TEMP_MAX_VALUE,
};
const tst_BatteryParmLimits SocLimit = {
 .minValue = SOC_MIN_VALUE,
 .maxValue = SOC_MAX_VALUE,

};
const tst_BatteryParmLimits ChargeRateLimit = {
 .minValue =CHARGE_RATE_MIN_VALUE,
 .maxValue =CHARGE_RATE_MAX_VALUE,
};


int batteryIsOk(float temperature, float soc, float chargeRate) {

  int lp;
  tst_SensorParm sensorParm ;
  sensorParm.printMessage = PrintMessage;

  tst_SensorInputs tempInput ;
  init_Limits(&tempInput, temperature, TempLimit, Temp_Tolerance);
  init_registerCallback(&tempInput, GetBatteryParamWarningStatus);
  init_ConfigureMessage(&tempInput,Temperature_out_of_range,
                             Temperature_warning);
  tempInput.status = GetBatteryParamStatus(&sensorParm, &tempInput);
  sensorParm.data[BATTERY_TEMPERATURE] = tempInput;

  tst_SensorInputs socInput;
  init_Limits(&socInput, soc, SocLimit, Soc_Tolerance);
  init_registerCallback(&socInput, GetBatteryParamWarningStatus);
  init_ConfigureMessage(&socInput,State_of_Charge_out_of_range,
                  Soc_Warning);
  socInput.status = GetBatteryParamStatus(&sensorParm, &socInput);
  sensorParm.data[SOC] = socInput;

  tst_SensorInputs chargeRateInput;
  init_Limits(&chargeRateInput, chargeRate, ChargeRateLimit, ChargeRate_Tolerance);
  init_registerCallback(&chargeRateInput, GetBatteryParamWarningStatus);
  init_ConfigureMessage(&chargeRateInput,Charge_Rate_out_of_range,
                  ChargeRate_Warning);
  chargeRateInput.status = GetBatteryParamStatus(&sensorParm, &chargeRateInput);
  sensorParm.data[CHARGE_RATE] = chargeRateInput;

  for(lp = 0; lp < MAX_BATTERY_PARAMETERS; lp ++) {
      if(sensorParm.data[lp].status > VALID_RANGE)
      {
         return 0;
      }
  }
  return 1;
}

void init_Limits(void *inputList, float value, tst_BatteryParmLimits limit, int tolerance){
    tst_SensorInputs* input = (tst_SensorInputs*)inputList;
    input->value = value;
    input->limits.minValue = limit.minValue;
    input->limits.maxValue = limit.maxValue;
    input->tolerancelimits.tolerence = tolerance;
}

void init_registerCallback(void *inputList, void *callback_fn){
    tst_SensorInputs* input = (tst_SensorInputs*)inputList;
    input->callback_warning = callback_fn;
}

void init_ConfigureMessage(void *inputList, void * errormessage, void * warningmessage){
    tst_SensorInputs* input = (tst_SensorInputs*)inputList;
    input->errorMessage = errormessage;
    input->warningMessage = warningmessage;
}

int GetBatteryParamStatus(void * inputList, void* inputValue){
     tst_SensorParm *sensorParm =  (tst_SensorParm*)inputList;
     tst_SensorInputs *input = (tst_SensorInputs*)inputValue;
     tst_PrintParm printParm;
    if(input->value < input->limits.minValue ){
        printParm.value = input->value;
        printParm.breachLevel = Breach_Low;
        printParm.Warning = Default;
        printParm.errorMessage = input->errorMessage;
        sensorParm->printMessage(&printParm);
        return INVALID_RANGE_LOW;
    }
    else if(input->value > input->limits.maxValue){
        printParm.value = input->value;
        printParm.breachLevel = Breach_High;
        printParm.Warning = Default;
        printParm.errorMessage = input->errorMessage;
        sensorParm->printMessage(&printParm);
        return INVALID_RANGE_HIGH;
    }
    else{
        input->callback_warning(sensorParm, input);
       return VALID_RANGE;
    }
}

int GetBatteryParamWarningStatus(void *inputList, void * inputValue){

    tst_SensorParm* sensorParm =  (tst_SensorParm*)inputList;
    tst_SensorInputs* input = (tst_SensorInputs*)inputValue;
    tst_PrintParm printParm;
    int tolerence = CalculateTolerance(input->limits.maxValue,
                    input->tolerancelimits.tolerence);

    input->tolerancelimits.limits.minValue =  input->limits.minValue + tolerence;
    input->tolerancelimits.limits.maxValue= input->limits.maxValue - tolerence;
    if(input->value < input->tolerancelimits.limits.minValue){
        printParm.value = input->value;
        printParm.breachLevel = Default;
        printParm.Warning = Approaching_discharge;
        printParm.errorMessage = input->warningMessage;
        sensorParm->printMessage(&printParm);
        return 0;
    }
    else if(input->value > input->tolerancelimits.limits.maxValue){
        printParm.value = input->value;
        printParm.breachLevel = Default;
        printParm.Warning = Approaching_charge_peak;
        printParm.errorMessage = input->warningMessage;
        sensorParm->printMessage(&printParm);
        return 0;
    }
    else{
        return 1;
    }
}

int CalculateTolerance(int value, int tolerance){
    return value*tolerance /100;
}

int main() {
  assert(batteryIsOk(25, 70, 0.7));
  assert(!batteryIsOk(50, 85, 0));
  assert(!batteryIsOk(70, 0, 0));
  assert(batteryIsOk(45, 80, 0.8));
  assert(batteryIsOk(0, 20, 0.8));


  return 0;
}


