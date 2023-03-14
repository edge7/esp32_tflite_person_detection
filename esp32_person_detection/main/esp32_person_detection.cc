#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "init_functions.h"
#include "wifi_utility.h"
#include "esp_memory_utils.h"
#include "memory_utility.h"

const char * TAG = "MAIN";

void tf_main(void) {

    ESP_LOGI(TAG, "Starting App. ");
    print_memory_info();
    wifi_init_sta();
    print_memory_info();
    ESP_LOGI(TAG, "Wifi done, starting setup function, for TF & Camera");
    setup();
    print_memory_info();
    ESP_LOGI(TAG, "Setup done, starting looping");
    while (true) {
        loop();
    }
}

extern "C" void app_main() {
    xTaskCreate((TaskFunction_t)&tf_main, "tf_main", 4 * 1024, NULL, 8, NULL);
    vTaskDelete(NULL);
}
