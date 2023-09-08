#ifndef __OLED_CONTROLLER_H__
#define __OLED_CONTROLLER_H__

#include <gsdc_ssd1306.h>

namespace gsdc_examples { 

    namespace oled {

        const int TOP = 2;
        const int CENTER = 3;
        const int BOTTOM = 8;

        class OledController {
        private:
            SSD1306_t dev;
            char lineChar[20];
            bool _invertText = false;
        public:
            /**
             * @brief Displays a text string
             * @param text (char *) the text to display
             * @param lineNumber (int) the "page" upon which to display the text
             * @param invert (bool -defaults to false) whether or not to invert the text
             */
            void DisplayText(char * text, int lineNumber, bool invert = false);
            /**
             * @brief Show the "up time" on the top two lines of the display
             */
            void DisplayUpTime(void);
            // SSD1306_t * GetOledDisplay(void);
            void HorizontalScrollText(char text[], bool scroll_left = true, bool invert = false);
            bool Initialize(bool flipDisplay);
        };

    } // namespace OLED

} // namespace bme

#endif // __OLED_CONTROLLER_H__