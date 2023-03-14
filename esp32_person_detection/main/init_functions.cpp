//
// Created by edge7 on 11/03/23.
//

#include "init_functions.h"
#include "esp32_model.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "image_provider.h"
#include "model_settings.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "memory_utility.h"
#include "init_functions.h"
#include "http_handler.h"
#include <esp_heap_caps.h>
#include <esp_timer.h>
#include <esp_log.h>
static const char* TAG = "INIT";


namespace {
    const tflite::Model* model = nullptr;
    tflite::MicroInterpreter* interpreter = nullptr;
    TfLiteTensor* input = nullptr;

    constexpr int kTensorArenaSize = 600 * 1024;
    static uint8_t *tensor_arena;
}  // namespace

// The name of this function is important for Arduino compatibility.
void setup() {
    // Map the model into a usable data structure. This doesn't involve any
    // copying or parsing, it's a very lightweight operation.
    model = tflite::GetModel(person_detection_quantized_tflite);
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        MicroPrintf("Model provided is schema version %d not equal to supported "
                    "version %d.", model->version(), TFLITE_SCHEMA_VERSION);
        return;
    }

    if (tensor_arena == NULL) {
        tensor_arena = (uint8_t *) heap_caps_malloc(kTensorArenaSize, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    }
    if (tensor_arena == NULL) {
        printf("Couldn't allocate memory of %d bytes\n", kTensorArenaSize);
        abort();
    }
    print_memory_info();

    static tflite::MicroMutableOpResolver<9> micro_op_resolver;
    micro_op_resolver.AddAveragePool2D();
    micro_op_resolver.AddConv2D();
    micro_op_resolver.AddDepthwiseConv2D();
    micro_op_resolver.AddReshape();
    micro_op_resolver.AddQuantize();
    micro_op_resolver.AddMean();
    micro_op_resolver.AddFullyConnected();
    micro_op_resolver.AddLogistic();
    micro_op_resolver.AddDequantize();


    static tflite::MicroInterpreter static_interpreter(
            model, micro_op_resolver, tensor_arena, kTensorArenaSize);
    interpreter = &static_interpreter;

    // Allocate memory from the tensor_arena for the model's tensors.
    TfLiteStatus allocate_status = interpreter->AllocateTensors();
    if (allocate_status != kTfLiteOk) {
        MicroPrintf("AllocateTensors() failed");
        ESP_LOGE(TAG, "AllocateTensors() failed");
        abort();
        return;
    }

    // Get information about the memory area to use for the model's input.
    input = interpreter->input(0);
    // Initialize Camera
    ESP_LOGI(TAG, "Trying to init camera");
    TfLiteStatus init_status = InitCamera();
    if (init_status != kTfLiteOk) {
        MicroPrintf("InitCamera failed\n");
        return;
    }

}

void loop() {
    // Get image from provider.
    if (kTfLiteOk != GetImage(input->data.f)) {
        MicroPrintf("Image capture failed.");
    }

    // Run the model on this input and make sure it succeeds.
    if (kTfLiteOk != interpreter->Invoke()) {
        MicroPrintf("Invoke failed.");
    }

    TfLiteTensor* output = interpreter->output(0);

    send_image_data(input->data.f, 128*128);
    // Process the inference results.
    float person_probability = output->data.f[0];
    // Respond to detection
    ESP_LOGI("TAG", "My float value is %f", person_probability);
    vTaskDelay(1); // to avoid watchdog trigger
}