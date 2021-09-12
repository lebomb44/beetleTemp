#include <CnC.h>
#include <OneWire.h>
#include <DallasTemperature.h>


const char nodeName[] PROGMEM = "dining";
const char sepName[] PROGMEM = " ";
const char hkName[] PROGMEM = "val";
const char cmdGetName[] PROGMEM = "get";
const char cmdSetName[] PROGMEM = "set";
const char tempSensorsName[] PROGMEM = "tempSensors";

OneWire oneWire(A2);
DallasTemperature tempSensors(&oneWire);
uint8_t tempSensorsNb = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  cncInit(nodeName, &Serial1);
  cnc_hkName_set(hkName);
  cnc_cmdGetName_set(cmdGetName);
  cnc_cmdSetName_set(cmdSetName);
  cnc_sepName_set(sepName);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("RF_TRX Starting...");
  tempSensors.begin();
  tempSensorsNb = tempSensors.getDeviceCount();
  tempSensors.requestTemperatures();
  for(uint8_t i=0; i<tempSensorsNb; i++)  {
    DeviceAddress sensorAddr;
    cnc_print_hk_temp_sensor(tempSensorsName, sensorAddr, tempSensors.getTempCByIndex(i));
  }

  delay(1000);
}
