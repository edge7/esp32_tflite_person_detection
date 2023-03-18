//
// Created by edge7 on 14/03/23.
//

#include <esp_log.h>
#include <esp_heap_caps.h>
#include <freertos/FreeRTOS.h>

void print_memory_info(){
    const char * TAG = "MEMORY";
    ESP_LOGI(TAG, "xPortGetFreeHeapSize %ld = DRAM", xPortGetFreeHeapSize());

    int DRam = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    int IRam = heap_caps_get_free_size(MALLOC_CAP_32BIT) - heap_caps_get_free_size(MALLOC_CAP_8BIT);

    ESP_LOGI(TAG, "DRAM \t\t %d", DRam);
    ESP_LOGI(TAG, "IRam \t\t %d", IRam);
    int free = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
    ESP_LOGI(TAG, "free = %d", free);
}