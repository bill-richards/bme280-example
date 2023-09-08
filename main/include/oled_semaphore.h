#ifndef __OLED_SEMAPHORE_H__
#define __OLED_SEMAPHORE_H__

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

SemaphoreHandle_t oled_mux = NULL;

void take_semaphore() 
{
    if(oled_mux == NULL)
        oled_mux = xSemaphoreCreateMutex();

    xSemaphoreTake(oled_mux, portMAX_DELAY);
}

void give_semaphore()
{
    xSemaphoreGive(oled_mux);
}


#endif // __OLED_SEMAPHORE_H__