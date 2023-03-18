//
// Created by edge7 on 14/03/23.
//

#include <esp_log.h>
#include "http_handler.h"
#include "esp_http_client.h"

const char * TAG = "HTTP_HANDLER";

esp_err_t send_image_data(float *image_data, int num_elements) {
    // Set up the HTTP client configuration
    esp_http_client_config_t config = {
            .url = "http://192.168.1.133:5000/upload_image", //Modify this in according to your setting
            .method = HTTP_METHOD_POST,
            .buffer_size = 1024,
    };


    // Create the HTTP client instance
    esp_http_client_handle_t client = esp_http_client_init(&config);

    // Set the content type header to "application/octet-stream" for binary data
    esp_http_client_set_header(client, "Content-Type", "application/octet-stream");

    // Set the content length header to the size of the image data in bytes
    size_t data_size = num_elements * sizeof(float);
    printf("num_elements = %d\n", num_elements);
    printf("float size = %d\n", sizeof(float));

    char content_length[32];
    sprintf(content_length, "%u", (unsigned int)data_size);
    printf("%s", content_length);
    esp_http_client_set_header(client, "Content-Length", content_length);
    printf("Content-Length: %s\n", content_length);


    // Send the image data in the request body
    esp_err_t err = esp_http_client_open(client, data_size);
    if (err != ESP_OK) {
        // Handle error
        printf("Error opening HTTP client: %s\n", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        ESP_LOGE(TAG, "HTTP INIT failed");
        return err;
    } else {
        // Send the image data as binary data in the request body
        esp_http_client_write(client, reinterpret_cast<const char *>(image_data), data_size);
    }

    // Close the HTTP client
    esp_http_client_close(client);
    esp_http_client_cleanup(client);

    return ESP_OK;
}