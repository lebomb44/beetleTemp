#include <CnC.h>
#include <OneWire.h>
#include <DallasTemperature.h>


const char nodeName[] PROGMEM = "beetleTemp";
const char sepName[] PROGMEM = " ";
const char hkName[] PROGMEM = "val";
const char cmdGetName[] PROGMEM = "get";
const char cmdSetName[] PROGMEM = "set";
const char tempSensorsName[] PROGMEM = "tempSensors";
const char lightModeName[] PROGMEM = "lightMode";

OneWire oneWire(A2);
DallasTemperature tempSensors(&oneWire);
uint8_t tempSensorsNb = 0;
#define LIGHT_PIN A1
#define BLINK_PIN A0
uint8_t lightMode = 0;
uint32_t previousTime_light = 0;
uint16_t loopNb = 0;

void lightMode_cmdGet(int arg_cnt, char **args) { cnc_print_cmdGet_u32(lightModeName, lightMode); }
void lightMode_cmdSet(int arg_cnt, char **args) {
  if(4 == arg_cnt) {
    lightMode = strtoul(args[3], NULL, 16);
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  cncInit(nodeName, &Serial);
  cnc_hkName_set(hkName);
  cnc_cmdGetName_set(cmdGetName);
  cnc_cmdSetName_set(cmdSetName);
  cnc_sepName_set(sepName);
  cnc_cmdGet_Add(lightModeName, lightMode_cmdGet);
  cnc_cmdSet_Add(lightModeName, lightMode_cmdSet);
  tempSensorsNb = 0;
  loopNb = 0;

  previousTime_light = millis();
  pinMode(LIGHT_PIN, OUTPUT);
  digitalWrite(LIGHT_PIN, HIGH);
  pinMode(BLINK_PIN, OUTPUT);
  digitalWrite(BLINK_PIN, HIGH);
}

void runLight(void) {
  /* Light blink */
  uint32_t currentTime_ = 0;
  currentTime_ = millis();
  if((uint32_t)(currentTime_ - previousTime_light) >= 100) {
    if(0 == lightMode) { digitalWrite(LIGHT_PIN, LOW); digitalWrite(BLINK_PIN, LOW); }
    else if(1 == lightMode) { digitalWrite(LIGHT_PIN, HIGH); }
    else if(2 == lightMode) { digitalWrite(LIGHT_PIN, !digitalRead(LIGHT_PIN)); }
    else if(3 == lightMode) { digitalWrite(BLINK_PIN, HIGH); }
    else { digitalWrite(LIGHT_PIN, LOW); }
    previousTime_light = currentTime_;
  }
}

void loop() {
  cncPoll();
  runLight();

  if(0 == (loopNb%1000)) {
    tempSensors.begin();
    tempSensorsNb = tempSensors.getDeviceCount();
    tempSensors.requestTemperatures();
    for(uint8_t i=0; i<tempSensorsNb; i++)  {
      DeviceAddress sensorAddr;
      tempSensors.getAddress(sensorAddr, i);
      //digitalWrite(LED_BUILTIN, HIGH); 
      cnc_print_hk_temp_sensor(tempSensorsName, "BEETLETE", tempSensors.getTempCByIndex(i));
      //digitalWrite(LED_BUILTIN, LOW); 
    }
    cnc_print_hk_u32(lightModeName, (uint32_t) lightMode);
  }
  if(2000 > loopNb) {
    if(1000 > loopNb) {
      analogWrite(LED_BUILTIN, loopNb/4);
    }
    else {
      analogWrite(LED_BUILTIN, (1999-loopNb)/4);
    }
  }
  else {
    analogWrite(LED_BUILTIN, 0);
  }

  delay(1);
  loopNb++;
  if(5999<loopNb) {
    loopNb = 0;
  }
}
