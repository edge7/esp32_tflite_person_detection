//
// Created by edge7 on 14/03/23.
//

#ifndef ESP32_PERSON_DETECTION_HTTP_HANDLER_H
#define ESP32_PERSON_DETECTION_HTTP_HANDLER_H

#include <esp_err.h>

esp_err_t send_image_data(float *image_data, int num_elements);
#endif //ESP32_PERSON_DETECTION_HTTP_HANDLER_H
