#include "BMP280.hpp"

BMP280::BMP280(I2C_HandleTypeDef *hi2c, uint8_t address) : _hi2c(hi2c), _address(address)
{
}

uint8_t BMP280::get_chip_id()
{
    uint8_t chip_id = 0;
    HAL_I2C_Mem_Read(_hi2c, _address << 1, BMP280_REG_CHIP_ID, I2C_MEMADD_SIZE_8BIT, &chip_id, 1, HAL_MAX_DELAY);
    return chip_id;
}

bool BMP280::is_available()
{
    return get_chip_id() == BMP280_DEFAULT_CHIP_ID;
}

void BMP280::get_pressure_and_temperature()
{
    uint8_t data[6];
    HAL_I2C_Mem_Read(_hi2c, _address << 1, BMP280_REG_PRESSURE, I2C_MEMADD_SIZE_8BIT, data, 6, HAL_MAX_DELAY);

    int32_t adc_T = (data[3] << 12) | (data[4] << 4) | (data[5] >> 4);
    int32_t adc_P = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);

    double var1, var2, T, p;

    var1 = (((double)adc_T) / 16384.0 - ((double)dig_T1) / 1024.0) * ((double)dig_T2);
    var2 = ((((double)adc_T) / 131072.0 - ((double)dig_T1) / 8192.0) *
            (((double)adc_T) / 131072.0 - ((double)dig_T1) / 8192.0)) *
           ((double)dig_T3);
    t_fine = (int32_t)(var1 + var2);
    _temperature = (var1 + var2) / 5120.0;

    var1 = ((double)t_fine / 2.0) - 64000.0;
    var2 = var1 * var1 * ((double)dig_P6) / 32768.0;
    var2 = var2 + var1 * ((double)dig_P5) * 2.0;
    var2 = (var2 / 4.0) + (((double)dig_P4) * 65536.0);
    var1 = (((double)dig_P3) * var1 * var1 / 524288.0 + ((double)dig_P2) * var1) / 524288.0;
    var1 = (1.0 + var1 / 32768.0) * ((double)dig_P1);
    if (var1 == 0.0)
    {
        return 0; // avoid exception caused by division by zero
    }
    p = 1048576.0 - (double)adc_P;
    p = (p - (var2 / 4096.0)) * 6250.0 / var1;
    var1 = ((double)dig_P9) * p * p / 2147483648.0;
    var2 = p * ((double)dig_P8) / 32768.0;
    _pressure = p + (var1 + var2 + ((double)dig_P7)) / 16.0;
}

float BMP280::get_temperature()
{
    get_pressure_and_temperature();
    return _temperature;
}

float BMP280::get_pressure()
{
    get_pressure_and_temperature();
    return _pressure;
}

bool BMP280::init()
{
    if (!is_available())
    {
        return false;
    }

    get_calibration_data();

    uint8_t config[2] = {0b01011111, 0b00011100};
    HAL_I2C_Mem_Write(_hi2c, _address << 1, BMP280_REG_CTRL_MEAS, I2C_MEMADD_SIZE_8BIT, config, 2, HAL_MAX_DELAY);

    return true;
}

void BMP280::get_calibration_data()
{
    HAL_I2C_Mem_Read(_hi2c, _address << 1, BMP280_REG_CALIB, I2C_MEMADD_SIZE_8BIT, _calib_bytes, 24, HAL_MAX_DELAY);

    dig_T1 = (_calib_bytes[1] << 8) | _calib_bytes[0];
    dig_T2 = (_calib_bytes[3] << 8) | _calib_bytes[2];
    dig_T3 = (_calib_bytes[5] << 8) | _calib_bytes[4];
    dig_P1 = (_calib_bytes[7] << 8) | _calib_bytes[6];
    dig_P2 = (_calib_bytes[9] << 8) | _calib_bytes[8];
    dig_P3 = (_calib_bytes[11] << 8) | _calib_bytes[10];
    dig_P4 = (_calib_bytes[13] << 8) | _calib_bytes[12];
    dig_P5 = (_calib_bytes[15] << 8) | _calib_bytes[14];
    dig_P6 = (_calib_bytes[17] << 8) | _calib_bytes[16];
    dig_P7 = (_calib_bytes[19] << 8) | _calib_bytes[18];
    dig_P8 = (_calib_bytes[21] << 8) | _calib_bytes[20];
    dig_P9 = (_calib_bytes[23] << 8) | _calib_bytes[22];
}