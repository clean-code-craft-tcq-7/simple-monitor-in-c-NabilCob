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

const int Langaugedefined = English;
tst_SensorParm sensorParm;


int batteryIsOk(float temperature, float soc, float chargeRate) {

  int lp;

  tst_SensorInputs tempInput = sensorParm.data[BATTERY_TEMPERATURE];
  tst_SensorInputs socInput = sensorParm.data[SOC];
  tst_SensorInputs chargeRateInput = sensorParm.data[CHARGE_RATE];

  tempInput.status = GetBatteryParamStatus(temperature, &sensorParm, &tempInput);
  socInput.status = GetBatteryParamStatus(soc, &sensorParm, &socInput);
  chargeRateInput.status = GetBatteryParamStatus(chargeRate, &sensorParm, &chargeRateInput);

  //Copy to Global
  sensorParm.data[BATTERY_TEMPERATURE] = tempInput;
  sensorParm.data[SOC] = socInput;
  sensorParm.data[CHARGE_RATE] = chargeRateInput;

  for(lp = 0; lp < MAX_BATTERY_PARAMETERS; lp ++) {
      if(sensorParm.data[lp].status != Valid)
      {
         return sensorParm.data[lp].status;
      }
  }
  return Valid;
}

void init_Limits(void *inputList, tst_BatteryParmLimits limit, int tolerance){
    tst_SensorInputs* input = (tst_SensorInputs*)inputList;
    input->limits.minValue = limit.minValue;
    input->limits.maxValue = limit.maxValue;
    input->tolerancelimits.tolerence = tolerance;
}

void init_registerCallback(void *inputList, void *callback_fn){
    tst_SensorInputs* input = (tst_SensorInputs*)inputList;
    input->callback_warning = callback_fn;
}

void init_ConfigureMessage(void *inputList, void * errormsg, void * warningmsg){
    tst_SensorInputs* input = (tst_SensorInputs*)inputList;
    input->errorMessage = errormsg;
    input->warningMessage = warningmsg;
}


int GetBatteryParamStatus(float sensorValue, void * inputList, void* inputValue){
     tst_SensorParm *sensorParm =  (tst_SensorParm*)inputList;
     tst_SensorInputs *input = (tst_SensorInputs*)inputValue;
     tst_PrintParm printParm;
     if(sensorValue < input->limits.minValue ){
        printParm.value = sensorValue;
        printParm.breachLevel = Breach_Low;
        printParm.Warning = Default;
        printParm.errorMessage = input->errorMessage;
        sensorParm->printMessage(&printParm);
        return Breach_Low;
     }
     else if(sensorValue > input->limits.maxValue){
        printParm.value = sensorValue;
        printParm.breachLevel = Breach_High;
        printParm.Warning = Default;
        printParm.errorMessage = input->errorMessage;
        sensorParm->printMessage(&printParm);
        return Breach_High;
     }
     else{

       return input->callback_warning(sensorValue, sensorParm, input);;
     }
}

int GetBatteryParamWarningStatus(float sensorValue, void *inputList, void * inputValue){

    tst_SensorParm* sensorParm =  (tst_SensorParm*)inputList;
    tst_SensorInputs* input = (tst_SensorInputs*)inputValue;
    tst_PrintParm printParm;
    int tolerence = CalculateTolerance(input->limits.maxValue,
                    input->tolerancelimits.tolerence);

    input->tolerancelimits.limits.minValue =  input->limits.minValue + tolerence;
    input->tolerancelimits.limits.maxValue= input->limits.maxValue - tolerence;

    if(sensorValue < input->tolerancelimits.limits.minValue){
        printParm.value = sensorValue;
        printParm.breachLevel = Default;
        printParm.Warning = Approaching_discharge;
        printParm.errorMessage = input->warningMessage;
        sensorParm->printMessage(&printParm);
        return Approaching_discharge;
    }
    else if(sensorValue > input->tolerancelimits.limits.maxValue){
        printParm.value = sensorValue;
        printParm.breachLevel = Default;
        printParm.Warning = Approaching_charge_peak;
        printParm.errorMessage = input->warningMessage;
        sensorParm->printMessage(&printParm);
        return Approaching_charge_peak;
    }
    else{
        return Valid;
    }
}

int CalculateTolerance(int value, int tolerance){

    return value*tolerance /100;
}

int main() {

    //init parameters
    sensorParm.printMessage = PrintMessage;

    tst_SensorInputs tempInput ;
    init_Limits(&tempInput, TempLimit, Temp_Tolerance);
    init_registerCallback(&tempInput, GetBatteryParamWarningStatus);
    init_ConfigureMessage(&tempInput,(void*)Temperature_out_of_range,
                             (void*)Temperature_warning);
    sensorParm.data[BATTERY_TEMPERATURE] = tempInput;

    tst_SensorInputs socInput;
    init_Limits(&socInput,SocLimit, Soc_Tolerance);
    init_registerCallback(&socInput, GetBatteryParamWarningStatus);
    init_ConfigureMessage(&socInput,(void*)State_of_Charge_out_of_range,
                  (void*)Soc_Warning);
    sensorParm.data[SOC] = socInput;


    tst_SensorInputs chargeRateInput;
    init_Limits(&chargeRateInput,ChargeRateLimit, ChargeRate_Tolerance);
    init_registerCallback(&chargeRateInput, GetBatteryParamWarningStatus);
    init_ConfigureMessage(&chargeRateInput,(void*)Charge_Rate_out_of_range,
                  (void*)ChargeRate_Warning);
    sensorParm.data[CHARGE_RATE] = chargeRateInput;

    assert(batteryIsOk(25, 70, 0.7)== Valid);
    assert(batteryIsOk(50, 85, 0) != Valid);
    assert(batteryIsOk(70, 0, 0) != Valid);
    assert(batteryIsOk(45, 80, 0.8) == Approaching_charge_peak);
    assert(batteryIsOk(0, 20, 0.8) == Approaching_discharge);
  return 0;
}
