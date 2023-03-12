#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "init_functions.h"

void tf_main(void) {

    setup();
    while (true) {
        // loop();
    }
}

extern "C" void app_main() {
    xTaskCreate((TaskFunction_t)&tf_main, "tf_main", 4 * 1024, NULL, 8, NULL);
    vTaskDelete(NULL);
}
