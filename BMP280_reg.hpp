#pragma once

#include <cstdint>

#define BMP280_REG_CALIB 0x88
#define BMP280_REG_CHIP_ID 0xD0
#define BMP280_REG_RESET 0xE0
#define BMP280_REG_CTRL_MEAS 0xF4
#define BMP280_REG_CONFIG 0xF5
#define BMP280_REG_PRESSURE 0xF7
#define BMP280_REG_TEMPERATURE 0xFA

#define BMP280_DEFAULT_CHIP_ID 0x58
#define BMP280_RESET_CMD 0xB6

enum class BMP280_OVERSAMPLING : uint8_t
{
    SKIP = 0b000,
    OS_1X = 0b001,
    OS_2X = 0b010,
    OS_4X = 0b011,
    OS_8X = 0b100,
    OS_16X = 0b101
};

enum class BMP280_POWER_MODE : uint8_t
{
    SLEEP = 0b00,
    FORCED = 0b01,
    NORMAL = 0b11
};

enum class BMP280_STANDBY_TIME : uint8_t
{
    MS_0_5 = 0b000,
    MS_62_5 = 0b001,
    MS_125 = 0b010,
    MS_250 = 0b011,
    MS_500 = 0b100,
    MS_1000 = 0b101,
    MS_2000 = 0b110,
    MS_4000 = 0b111
};

enum class BMP280_FILTER_COEFFICIENT : uint8_t
{
    OFF = 0b000,
    COEFF_2 = 0b001,
    COEFF_4 = 0b010,
    COEFF_8 = 0b011,
    COEFF_16 = 0b100
};