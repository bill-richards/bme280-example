#ifndef __BME_CONTROLLER_H__
#define __BME_CONTROLLER_H__

#include <gsdc_bme280_iic.h>
#include <gsdc_ssd1306.h>

typedef gsdc_bme280::Gpio_t Gpio_t;

namespace gsdc_examples { 
    namespace bme {

        class BmeController {
        private:
            gsdc_bme280::GSDC_SENSOR * connected_sensor = nullptr;
        public:
            bool Initialize(void);
            /**
             * @brief The caller is responsible for freeing the returned char *
             */
            void ReadData(char *);
        };

    } // namespace gsdc_examples 

} // namespace bme

#endif // __BME_CONTROLLER_H__