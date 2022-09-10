#include <Wire.h>
#define I2C_SDA 4
#define I2C_SCL 5
#define ADS_I2C_ADDR 0x48

int err_status,pot_val0,pot_val1,pot_val2,pot_val3;
float volt_val0,volt_val1,volt_val2,volt_val3;

int ADC_read(int channel){
  byte channel_config_byte[4] = {0b11010000,0b11100000,0b11110000,0b11000000},pot_data[2];
  Wire.beginTransmission(ADS_I2C_ADDR);
  Wire.write(0b00000001);
  Wire.write(channel_config_byte[channel]);
  Wire.write(0b11100011);
  Wire.endTransmission();
  Wire.beginTransmission(ADS_I2C_ADDR);
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.requestFrom(ADS_I2C_ADDR, 2);
  pot_data[1] = Wire.read();
  pot_data[0] = Wire.read();
  return (int16_t)((pot_data[1]<<8)|(pot_data[1]));
}

float VOLT_calc(int ADC_val){
  return ADC_val / 26728.00 * 5.00;
}

void setup() {
  Wire.begin();
  Serial.begin(115200);
  Wire.beginTransmission(ADS_I2C_ADDR);
}

void loop() {
  pot_val0 = ADC_read(0);
  volt_val0 = VOLT_calc(pot_val0);
  Serial.print("POT0 : ");
  Serial.print(pot_val0);
  Serial.print("\tVOLT0 : "); 
  Serial.println(volt_val0);
  delay(20);
  pot_val1 = ADC_read(1);
  volt_val1 = VOLT_calc(pot_val1);
  Serial.print("POT1 : ");
  Serial.print(pot_val1);
  Serial.print("\tVOLT1 : "); 
  Serial.println(volt_val1);
  delay(20);
  pot_val2 = ADC_read(2);
  volt_val2 = VOLT_calc(pot_val2);
  Serial.print("POT2 : ");
  Serial.print(pot_val2);
  Serial.print("\tVOLT2 : "); 
  Serial.println(volt_val2);
  delay(20);
  pot_val3 = ADC_read(3);
  volt_val3 = VOLT_calc(pot_val3);
  Serial.print("POT3 : ");
  Serial.print(pot_val3);
  Serial.print("\tVOLT3 : "); 
  Serial.println(volt_val3);
  delay(520);
}
