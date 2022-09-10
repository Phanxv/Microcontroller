#include <Wire.h>
#define I2C_SDA 4
#define I2C_SCL 5
#define Temp_I2C_ADDR 0x4D
float Temperature_value;
void setup(){
            Wire.begin(I2C_SDA, I2C_SCL),
            Serial.begin(115200); }
void loop(){
            byte Temperature_data[2];
            byte Temperature_data_maske[2];
            Wire.beginTransmission(Temp_I2C_ADDR); // start I2C communication and send slave address 0x4D (write data)
            Wire.write(0);                         // write temperature address
            Wire.endTransmission();                // stop I2C communication
            Wire.requestFrom(Temp_I2C_ADDR, 2);    // start I2C communication and read temperature data from sensor 2 byte (read data)
            Temperature_data[1] = Wire.read();     // receive MSB temp data 
            Temperature_data[0] = Wire.read();     // receive LSB temp data 
            //////////////////////////Temp conversion Method 1/////////////////
//            if((Temperature_data [1] & 0x80)== 1){                        // Check temperature under 0 degree
//                Temperature_data_maske[1] = ~Temperature_data [1];        // 2 complement
//                Temperature_data_maske[0] = (~Temperature_data [0] +1) >> 5;
//                Temperature_value = ((Temperature_data_maske[1]*2) + Temperature_data_maske[0]*0.25) *-1; }
//            else{
//                Temperature_data_maske[1] = Temperature_data[1] & 0b01111111;
//                Temperature_data_maske[0] = Temperature_data[0] & 0b11100000;
//                Temperature_data_maske[0] = Temperature_data_maske[0] >> 5;
//                Temperature_value = (Temperature_data_maske[1]*2) + (Temperature_data_maske[0]*0.25);}
            //////////////////////////Temp conversion Method 2/////////////////
            Temperature_value = (int16_t)((Temperature_data[1]<<8)|Temperature_data[0]) /128.0f;
            ///////////////////////////////////////////////////////////////////
            Serial.print("/////////////////////////////////");  
            Serial.print("Temperature in C degree = ");  // print Temperature data (MSB)
            Serial.println(Temperature_value);
            Serial.print(Temperature_data[1],BIN);
            Serial.println(Temperature_data[0],BIN);
            delay(500);
}
