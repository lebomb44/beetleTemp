#include <CnC.h>
#include <OneWire.h>
#include <DallasTemperature.h>


const char nodeName[] PROGMEM = "beetleTemp";
const char sepName[] PROGMEM = " ";
const char hkName[] PROGMEM = "val";
const char cmdGetName[] PROGMEM = "get";
const char cmdSetName[] PROGMEM = "set";
const char tempSensorsName[] PROGMEM = "tempSensors";

OneWire oneWire(A2);
DallasTemperature tempSensors(&oneWire);
uint8_t tempSensorsNb = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  cncInit(nodeName, &Serial);
  cnc_hkName_set(hkName);
  cnc_cmdGetName_set(cmdGetName);
  cnc_cmdSetName_set(cmdSetName);
  cnc_sepName_set(sepName);
}

void loop() {
  tempSensors.begin();
  tempSensorsNb = tempSensors.getDeviceCount();
  tempSensors.requestTemperatures();
  for(uint8_t i=0; i<tempSensorsNb; i++)  {
    DeviceAddress sensorAddr;
    tempSensors.getAddress(sensorAddr, i);
    //digitalWrite(LED_BUILTIN, HIGH); 
    cnc_print_hk_temp_sensor(tempSensorsName, "BEETLETE", tempSensors.getTempCByIndex(i));
    digitalWrite(LED_BUILTIN, LOW); 
  }

  delay(1000);
}
