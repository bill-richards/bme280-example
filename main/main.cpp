#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "string.h"
#include <esp_logging.h>

#include <bme_controller.h>
#include <oled_controller.h>

typedef gsdc_examples::bme::BmeController BmeController;
typedef gsdc_examples::oled::OledController OledController;


const char * MAIN_TAG = "main";

extern "C" void app_main(void)
{
    BmeController Bme_Control;
    if(!Bme_Control.Initialize())
    {
        ESP_LOGE(MAIN_TAG, "BME280 Sensor is not initialized...");
        return;
    }

    OledController Oled_Controller;
    if(!Oled_Controller.Initialize(false))
    {
        ESP_LOGE(MAIN_TAG, "Oled Display is not initialized...");
        return;
    }
    Oled_Controller.DisplayUpTime();
    
    char *reading = (char*)calloc(1024, sizeof(char));
    char *saveptr, *inner_pos, *value, *pos;
    char *line = (char*)calloc(17, sizeof(char));

    while(true)
    {
        Bme_Control.ReadData(reading);
        
        int line_number = 1;
        pos = strtok_r(reading, "|", &saveptr);
        while(pos != NULL)
        {
            inner_pos = strtok_r(pos, ":", &value);

            if(strchr(inner_pos, 'T')) {
                sprintf(line, "Temp : %s", value);
            } else if(strchr(inner_pos, 'H')) {
                sprintf(line, "Humid: %s%c", value, '%');
            }else if(strchr(inner_pos, 'P')) {
                sprintf(line, "Press: %1.9s", value);
            }
            pos = strtok_r(saveptr, "|", &saveptr);
            Oled_Controller.DisplayText(line, gsdc_examples::oled::TOP+(line_number++));
        }

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}