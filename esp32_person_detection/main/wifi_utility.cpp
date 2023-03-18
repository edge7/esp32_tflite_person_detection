//
// Created by edge7 on 14/03/23.
//

#include "wifi_utility.h"

#include "esp_log.h"
#include "nvs_flash.h"
#include "lwip/ip4_addr.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include <freertos/event_groups.h>
#include <esp_mac.h>
#include "esp_system.h"

#define WIFI_SSID "Wind3 HUB-74EC7C"
#define WIFI_PASS "1uwe2vv8p60q26fc"

static EventGroupHandle_t s_wifi_event_group;
#define WIFI_CONNECTED_BIT BIT0

static void event_handler(void* arg, esp_event_base_t event_base,
                          int32_t event_id, void* event_data)
{
    if (event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_id == WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
    } else if (event_id == IP_EVENT_STA_GOT_IP) {
        // Get the IP address event data
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        // Print the IP address
        ESP_LOGI("wifi", "Got IP address: %s", ip4addr_ntoa(reinterpret_cast<const ip4_addr_t *>(&event->ip_info.ip)));
    }
}

void wifi_init_sta() {
    const char *TAG = "CONNECT";

    s_wifi_event_group = xEventGroupCreate();
    uint8_t mac_addr[6] = {0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc};
    esp_err_t ret = esp_base_mac_addr_set(mac_addr);
    if (ret != ESP_OK) {
        printf("Error setting MAC address: %s\n", esp_err_to_name(ret));
    }
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    esp_event_handler_instance_register(WIFI_EVENT,
                                        ESP_EVENT_ANY_ID,
                                        &event_handler,
                                        NULL,
                                        &instance_any_id);
    esp_event_handler_instance_register(IP_EVENT,
                                        IP_EVENT_STA_GOT_IP,
                                        &event_handler,
                                        NULL,
                                        &instance_got_ip);

    wifi_config_t wifi_config = {
            .sta = {
                    .ssid = WIFI_SSID,
                    .password = WIFI_PASS,
            },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(static_cast<wifi_interface_t>(ESP_IF_WIFI_STA), &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_sta finished.");
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                           WIFI_CONNECTED_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           portMAX_DELAY);

    /* As later in the code, the image needs also to be sent, there is no point in going ahead without
     * being sure of having a WIFI connection set up.
     * */

    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "Connected to WIFI & IP received, ready to go");
    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }
}