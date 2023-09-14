#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "string.h"
#include <esp_logging.h>

#include <bme_controller.h>
#include <ssd1306_controller.h>

typedef gsdc_sensor::bme280::BmeController BmeController;
typedef gsdc_oled::SSD1306Controller OledController;
typedef gsdc_oled::SPEED SCROLL_SPEED;
typedef gsdc_oled::DISPLAY_POSITION DISPLAY_POSITION;

const char * MAIN_TAG = "main";

const uint16_t IIC_DATA_PIN = 26;
const uint16_t IIC_CLOCK_PIN = 25;

extern "C" void app_main(void)
{
    BmeController Bme_Controller;
    if(!Bme_Controller.Initialize(IIC_DATA_PIN, IIC_CLOCK_PIN))
    {
        ESP_LOGE(MAIN_TAG, "BME280 Sensor is not initialized...");
        return;
    }

    OledController Oled_Controller;
    if(!Oled_Controller.Initialize(false, IIC_DATA_PIN, IIC_CLOCK_PIN))
    {
        ESP_LOGE(MAIN_TAG, "Oled Display is not initialized...");
        return;
    }
    Oled_Controller.DisplayUpTime();
    
    char *bmeData = (char*)calloc(1024, sizeof(char));
    char *saveptr, *inner_pos, *value, *pos;
    char *line = (char*)calloc(OledController::DISPLAY_WIDTH, sizeof(char));
    const char message[] = "This message scrolls @ SPEED::SLOW. The raw BME data, which are read every 5 seconds, scroll @ SPEED::FAST; they are parsed & displayed immediately below.";
    Oled_Controller.HorizontalScrollText((char*)message, DISPLAY_POSITION::TOP, SCROLL_SPEED::SLOW, true, true);
    
    TickType_t lastTime = xTaskGetTickCount();
    while(true)
    {
        Bme_Controller.ReadData(bmeData);
        Oled_Controller.HorizontalScrollText(bmeData, DISPLAY_POSITION::CENTER, SCROLL_SPEED::FAST);
        
        int line_number_adjustment = 1;
        pos = strtok_r(bmeData, "|", &saveptr);
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
            Oled_Controller.DisplayText(line, DISPLAY_POSITION::CENTER+(line_number_adjustment++));
        }
        vTaskDelayUntil(&lastTime, pdMS_TO_TICKS(5000));
    }
}