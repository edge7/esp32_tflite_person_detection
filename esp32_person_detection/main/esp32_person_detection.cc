#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "init_functions.h"
#include "wifi_utility.h"
#include "memory_utility.h"


void tf_main(void) {
    const char * TAG = "MAIN";
    ESP_LOGI(TAG, "Starting App. ");
    print_memory_info();
    wifi_init_sta();
    print_memory_info();
    ESP_LOGI(TAG, "Wifi done, starting setup function, for TF & Camera");
    setup();
    print_memory_info();
    ESP_LOGI(TAG, "Setup done, starting looping");
    while (true) {
        print_memory_info();
        loop();
    }
}

extern "C" void app_main() {
    xTaskCreate((TaskFunction_t)&tf_main, "tf_main", 4 * 1024, NULL, 8, NULL);
    vTaskDelete(NULL);
}
