#pragma once

#include "BMP280_reg.hpp"
#include <cstdint>

// Use the following flags for compiling the right library, e.g.: -D STM32F1
#if defined(STM32F0)
#include "stm32f0xx_hal.h"
#elif defined(STM32F1)
#include "stm32f1xx_hal.h"
#elif defined(STM32F2)
#include "stm32f2xx_hal.h"
#elif defined(STM32F3)
#include "stm32f3xx_hal.h"
#elif defined(STM32F4)
#include "stm32f4xx_hal.h"
#elif defined(STM32F7)
#include "stm32f7xx_hal.h"
#elif defined(STM32H7)
#include "stm32h7xx_hal.h"
#else
#error "Unsupported STM32 microcontroller. Make sure you build with -D STM32F1 for example!"
#endif

class BMP280
{
public:
    BMP280(I2C_HandleTypeDef *hi2c, uint8_t address);

    bool init();

    uint8_t get_chip_id();
    bool is_available();

    float get_pressure();
    float get_temperature();

    void reset();

    void set_pressure_oversampling(BMP280_OVERSAMPLING oversampling);
    void set_temperature_oversampling(BMP280_OVERSAMPLING oversampling);
    void set_power_mode(BMP280_POWER_MODE mode);
    void set_standby_time(BMP280_STANDBY_TIME time);
    void set_filter_coefficient(BMP280_FILTER_COEFFICIENT coeff);

private:
    void get_calibration_data();

    void get_pressure_and_temperature();

    float _temperature = 0.0f;
    float _pressure = 0.0f;

    uint16_t dig_T1 = 0;
    int16_t dig_T2 = 0;
    int16_t dig_T3 = 0;
    uint16_t dig_P1 = 0;
    int16_t dig_P2 = 0;
    int16_t dig_P3 = 0;
    int16_t dig_P4 = 0;
    int16_t dig_P5 = 0;
    int16_t dig_P6 = 0;
    int16_t dig_P7 = 0;
    int16_t dig_P8 = 0;
    int16_t dig_P9 = 0;

    int32_t t_fine = 0;

    uint8_t _calib_bytes[24] = {0};

    I2C_HandleTypeDef *_hi2c = nullptr;
    uint8_t _address;
};