#include <oled_controller.h>
#include <oled_semaphore.h>

#include "sdkconfig.h"
#include "font8x8_basic.h"
#include "esp_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char * OLED_TAG = "OledController";

namespace gsdc_examples::oled {

    const char time_format_string[] = "%03i     %02i:%02i:%02i";
    const char time_label[] = "UP TIME        ";

    char * time; 
    int seconds{}, minutes{}, hours{}, days{};

    void display_time_task(void * parameters)
    {
        auto dev = (SSD1306_t *)parameters;
        TickType_t lastTime = xTaskGetTickCount();
        while(true)
        {
            vTaskDelayUntil(&lastTime, pdMS_TO_TICKS(100));
            take_semaphore();
            ssd1306_display_text(dev, 0, (char *)&time_label, strlen(time_label), false);
            ssd1306_display_text(dev, 1, time, strlen(time), false);
            give_semaphore();
        }
    }

    void calculate_up_time_task(void * parameters)
    {
        auto width = (int)parameters;
        time = (char *)calloc(width, sizeof(char));
        sprintf(time, time_format_string, days, hours, minutes, seconds);

        TickType_t lastTime = xTaskGetTickCount();
        while(true)
        {
            vTaskDelayUntil(&lastTime, pdMS_TO_TICKS(1000));
            seconds +=1;
            if(seconds %60 == 0) { 
                seconds = 0; minutes+=1; 
                if(minutes %60 == 0) { 
                    minutes = 0; hours+=1; 
                    if(hours %24 == 0) { hours = 0; days+=1;}
                }
            }
            sprintf(time, time_format_string, days, hours, minutes, seconds);
        }
    }

    void OledController::DisplayUpTime(void) 
    {
        xTaskCreatePinnedToCore(calculate_up_time_task, "calculate_up_time_task", 2048, (void*)dev._width, 2, NULL, 0);
        xTaskCreatePinnedToCore(display_time_task, "display_time_task", 2048, &dev, 1, NULL, 0);
    }

    // SSD1306_t * OledController::GetOledDisplay(void){ return &dev; }

    bool OledController::Initialize(bool flipDisplay) {

        gsdc_ssd1306_i2c_master_init(&dev, 26, 25, CONFIG_RESET_GPIO);
        if(flipDisplay) {
            dev._flip = true;
            ESP_LOGW(OLED_TAG, "Flip upside down");
        }

        ssd1306_init(&dev, 128, 64);
        ssd1306_contrast(&dev, 0xff);
        ssd1306_clear_screen(&dev, false);

        // Display Count Down
        //uint8_t image[24];
        // memset(image, 0, sizeof(image));
        // ssd1306_display_image(&dev, top, (6*8-1), image, sizeof(image));
        // ssd1306_display_image(&dev, top+1, (6*8-1), image, sizeof(image));
        // ssd1306_display_image(&dev, top+2, (6*8-1), image, sizeof(image));
       
        // for(int font=0x35;font>0x30;font--) {
        //     memset(image, 0, sizeof(image));
        //     ssd1306_display_image(&dev, top+1, (7*8-1), image, 8);
        //     memcpy(image, font8x8_basic_tr[font], 8);
        //     if (dev._flip) ssd1306_flip(image, 8);
        //     ssd1306_display_image(&dev, top+1, (7*8-1), image, 8);
        //     vTaskDelay(1000 / portTICK_PERIOD_MS);
        // }
        // ssd1306_clear_screen(&dev, false);

        // DisplayUpTime();
	
        // Scroll Up
        // ssd1306_clear_screen(&dev, false);
        // ssd1306_contrast(&dev, 0xff);
        // ssd1306_display_text(&dev, 0, "---Scroll  UP---", 16, true);
        // //ssd1306_software_scroll(&dev, 7, 1);
        // ssd1306_software_scroll(&dev, (dev._pages - 1), 1);
        // for (int line=0;line<bottom+10;line++) {
        //     lineChar[0] = 0x01;
        //     sprintf(&lineChar[1], " Line %02d", line);
        //     ssd1306_scroll_text(&dev, lineChar, strlen(lineChar), false);
        //     vTaskDelay(500 / portTICK_PERIOD_MS);
        // }
        // vTaskDelay(3000 / portTICK_PERIOD_MS);
        
        // // Scroll Down
        // ssd1306_clear_screen(&dev, false);
        // ssd1306_contrast(&dev, 0xff);
        // ssd1306_display_text(&dev, 0, "--Scroll  DOWN--", 16, true);
        // //ssd1306_software_scroll(&dev, 1, 7);
        // ssd1306_software_scroll(&dev, 1, (dev._pages - 1) );
        // for (int line=0;line<bottom+10;line++) {
        //     lineChar[0] = 0x02;
        //     sprintf(&lineChar[1], " Line %02d", line);
        //     ssd1306_scroll_text(&dev, lineChar, strlen(lineChar), false);
        //     vTaskDelay(500 / portTICK_PERIOD_MS);
        // }
        // vTaskDelay(3000 / portTICK_PERIOD_MS);

        // // Page Down
        // ssd1306_clear_screen(&dev, false);
        // ssd1306_contrast(&dev, 0xff);
        // ssd1306_display_text(&dev, 0, "---Page	DOWN---", 16, true);
        // ssd1306_software_scroll(&dev, 1, (dev._pages-1) );
        // for (int line=0;line<bottom+10;line++) {
        //     //if ( (line % 7) == 0) ssd1306_scroll_clear(&dev);
        //     if ( (line % (dev._pages-1)) == 0) ssd1306_scroll_clear(&dev);
        //     lineChar[0] = 0x02;
        //     sprintf(&lineChar[1], " Line %02d", line);
        //     ssd1306_scroll_text(&dev, lineChar, strlen(lineChar), false);
        //     vTaskDelay(500 / portTICK_PERIOD_MS);
        // }
        // vTaskDelay(3000 / portTICK_PERIOD_MS);

        // // Horizontal Scroll
        // ssd1306_clear_screen(&dev, false);
        // ssd1306_contrast(&dev, 0xff);
        // ssd1306_display_text(&dev, center, "Horizontal", 10, false);
        // ssd1306_hardware_scroll(&dev, SCROLL_RIGHT);
        // vTaskDelay(5000 / portTICK_PERIOD_MS);
        // ssd1306_hardware_scroll(&dev, SCROLL_LEFT);
        // vTaskDelay(5000 / portTICK_PERIOD_MS);
        // ssd1306_hardware_scroll(&dev, SCROLL_STOP);
        
        // // Vertical Scroll
        // ssd1306_clear_screen(&dev, false);
        // ssd1306_contrast(&dev, 0xff);
        // ssd1306_display_text(&dev, center, "Vertical", 8, false);
        // ssd1306_hardware_scroll(&dev, SCROLL_DOWN);
        // vTaskDelay(5000 / portTICK_PERIOD_MS);
        // ssd1306_hardware_scroll(&dev, SCROLL_UP);
        // vTaskDelay(5000 / portTICK_PERIOD_MS);
        // ssd1306_hardware_scroll(&dev, SCROLL_STOP);
        
        // // Invert
        // ssd1306_clear_screen(&dev, true);
        // ssd1306_contrast(&dev, 0xff);
        // ssd1306_display_text(&dev, center, "  Good Bye!!", 12, true);
        // vTaskDelay(5000 / portTICK_PERIOD_MS);


        // // Fade Out
        // ssd1306_fadeout(&dev);
        
        // // Fade Out
        // for(int contrast=0xff;contrast>0;contrast=contrast-0x20) {
        //     ssd1306_contrast(&dev, contrast);
        //     vTaskDelay(40);
        // }

        return true;
    }

    void OledController::DisplayText(char * text, int lineNumber, bool invert)
    {
        take_semaphore();
        ssd1306_display_text(&dev, lineNumber, text, strlen(text), invert);
        give_semaphore();
    }
    
    void OledController::HorizontalScrollText(char text[], bool scroll_left, bool invert)
    {
        take_semaphore();
        ssd1306_contrast(&dev, 0xff);
        ssd1306_display_text(&dev, CENTER, text, strlen(text), invert);

        if(scroll_left) {
            ssd1306_hardware_scroll(&dev, SCROLL_LEFT);
        } else {
            ssd1306_hardware_scroll(&dev, SCROLL_RIGHT);
        }
        give_semaphore();
        vTaskDelay(5000 / portTICK_PERIOD_MS);

        take_semaphore();
        ssd1306_hardware_scroll(&dev, SCROLL_STOP);
        ssd1306_clear_line(&dev, CENTER, false);
        give_semaphore();
    }

}