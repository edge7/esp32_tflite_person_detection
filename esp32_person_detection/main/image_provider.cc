
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_spi_flash.h"
#include "app_camera_esp.h"
#include "esp_camera.h"
#include "image_provider.h"
#include "http_handler.h"

static const char *TAG = "app_camera";

// Get the camera module ready
TfLiteStatus InitCamera() {

    int ret = app_camera_init();
    if (ret != 0) {
        MicroPrintf("Camera init failed\n");
        ESP_LOGE(TAG, "Camera INIT failed");
        return kTfLiteError;
    }
    MicroPrintf("Camera Initialized\n");
    ESP_LOGI(TAG, "Camera INIT OK");
    return kTfLiteOk;
}

void process_camera_image(camera_fb_t *fb, float *image_data, int image_width, int image_height) {
    if (fb != NULL) {
        uint8_t *pixels = fb->buf;
        int width = fb->width;
        int height = fb->height;
        int bytes_per_pixel = 1;  // Grayscale format

        // Allocate memory for the resized image buffer
        uint8_t *resized_pixels = (uint8_t *) heap_caps_malloc(image_width * image_height * bytes_per_pixel,
                                                               MALLOC_CAP_8BIT);

        // Calculate the scaling factors for the width and height
        float scale_width = (float) width / (float) image_width;
        float scale_height = (float) height / (float) image_height;

        // Resize the image using nearest neighbor interpolation
        for (int y = 0; y < image_height; y++) {
            for (int x = 0; x < image_width; x++) {
                // Calculate the corresponding pixel in the original image
                int orig_x = (int) (x * scale_width);
                int orig_y = (int) (y * scale_height);
                int orig_index = orig_y * width + orig_x;

                // Copy the pixel value to the corresponding pixel in the resized image
                int new_index = y * image_width + x;
                resized_pixels[new_index] = pixels[orig_index];
            }
            vTaskDelay(1);
        }

        // Normalize the pixel values and store them in the float buffer
        for (int i = 0; i < image_width * image_height; i++) {
            float pixel_value = (float) resized_pixels[i] / 255.0f;
            image_data[i] = pixel_value;
        }

        // Free the resized image buffer
        heap_caps_free(resized_pixels);

    }
}

// Get an image from the camera module
TfLiteStatus GetImage(float *image_data) {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        ESP_LOGE(TAG, "Camera capture failed");
        return kTfLiteError;
    }
    MicroPrintf("Image Captured\n");
    ESP_LOGI(TAG, "length %d", fb->len);
    ESP_LOGI(TAG, "width %d", fb->width);
    ESP_LOGI(TAG, "height %d", fb->height);
    vTaskDelay(1);
    process_camera_image(fb, image_data, 128, 128);
    send_image_data(image_data, 128*128);
    esp_camera_fb_return(fb);
    /* here the esp camera can give you grayscale image directly */
    return kTfLiteOk;
}
