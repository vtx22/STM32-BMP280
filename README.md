# STM32-BMP280
STM32 library for Bosch BMP280 pressure and temperature sensor

## Usage example
### Simple Code
```c++
#include "BMP280.hpp"

// Create BMP object with I2C interface and I2C address
BMP280 bmp(&hi2c1, 0x76);

// Perform some configurations
bmp.set_power_mode(BMP280_POWER_MODE::NORMAL);
bmp.set_pressure_oversampling(BMP280_OVERSAMPLING::OS_16X);
bmp.set_temperature_oversampling(BMP280_OVERSAMPLING::OS_2X);
bmp.set_filter_coefficient(BMP280_FILTER_COEFFICIENT::COEFF_16);

// Simple loop to read temperature [°C] and pressure [Pa] every 100ms
while(true)
{
  float temperature = bmp.get_temperature();
  float pressure = bmp.get_pressure();
  HAL_Delay(100);
}
```

### Build
Copy the `BMP280.cpp`, `BMP280.hpp` and `BMP280_reg.hpp` to your source and include directory.

To build, specify a build flag for your STM32 MCU. For a STM32F1XXX for example, use `-D STM32F1`.

In the **Cube IDE**, paste the flag in the `Preprocessor` tab in the C/C++ build settings under `Project > Properties`

## API
### Constructor
Construct a BMP sensor object using the hi2c interface and the I2C address (e.g. 0x76)
```c++
BMP280(I2C_HandleTypeDef *hi2c, uint8_t address)
```
### Software Reset
Perform a software reset
```c++
void reset();
```
### Chip ID
Get the sensors fixed chip ID 0x58
```c++
uint8_t get_chip_id(); // Returns 0x58 if the sensor is reachable
```
### Check sensor availability
Returns true if the chip ID was successfully read as 0x58
```c++
bool is_available();
```
### Read pressure and temperature
Get the current temperature and pressure reading
```c++
float get_temperature();  // Returns temperature in [°C]
float get_pressure();     // Returns pressure in [Pa]
```
### Oversampling
Set the pressure and temperature oversampling
```c++
void set_pressure_oversampling(BMP280_OVERSAMPLING oversampling);
void set_temperature_oversampling(BMP280_OVERSAMPLING oversampling);
```
### Power Mode
Set the sensor power mode
```c++
void set_power_mode(BMP280_POWER_MODE mode);
```
### Standby Time
```c++
void set_standby_time(BMP280_STANDBY_TIME time);
```
### IIR filter coefficient
```c++
void set_filter_coefficient(BMP280_FILTER_COEFFICIENT coeff);
```

## Recommende settings for different use cases
| Use case | Power Mode | Pressure<br>Oversampling | Temperature<br>Oversampling | IIR filter<br>coeff. |
| --- | :---: | :---: | :---: | :---: |
| Handheld device,<br>low power | Normal | x16 | x2 | 4 |
| Handheld device,<br>dynamic | Normal | x4 | x1 | 16 |
| Weather monitor | Forced | x1 | x1 | Off |
| Elevator / Floor <br>change detection | Normal | x4 | x1 | 4 |
| Drop detection | Normal | x2 | x1 | Off |
| Indoor navigation | Normal | x16 | x2 | 16 |
