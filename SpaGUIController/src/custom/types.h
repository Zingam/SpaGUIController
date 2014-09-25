#ifndef C_TYPES_H
#define C_TYPES_H

enum class SensorState
{
    TemperatureHigher,
    TemperatureLower,
    TemperatureNormal
};

enum class SensorError
{
    Disconnected,
    OK,
    Undefined
};

#endif // C_TYPES_H
