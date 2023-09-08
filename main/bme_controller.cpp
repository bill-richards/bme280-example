#include <bme_controller.h>

#include "font8x8_basic.h"
#include "freertos/task.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace gsdc_examples::bme {

    bool BmeController::Initialize(void)
    {
        connected_sensor = new gsdc_bme280::BME280IIC();

        Gpio_t * pins = connected_sensor->GetGpioSetupStruct();
        pins[gsdc_bme280::SDA_PIN_INDEX] = Gpio_t { 26 };
        pins[gsdc_bme280::SCL_PIN_INDEX] = Gpio_t { 25 };

        connected_sensor->SetGpio(pins);
        connected_sensor->Initialize();

        return connected_sensor->IsInitialized();
    }

    void BmeController::ReadData(char * data)
    {
        connected_sensor->ReadData(data);
    }
    
} // namespace gsdc_examples::bme


