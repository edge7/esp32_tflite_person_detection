//
// Created by edge7 on 11/03/23.
//

#ifndef ESP32_PERSON_DETECTION_INIT_FUNCTIONS_H
#define ESP32_PERSON_DETECTION_INIT_FUNCTIONS_H
#ifdef __cplusplus
extern "C" {
#endif

// Initializes all data needed for the example. The name is important, and needs
// to be setup() for Arduino compatibility.
void setup();

// Runs one iteration of data gathering and inference. This should be called
// repeatedly from the application code. The name needs to be loop() for Arduino
// compatibility.
void loop();

#ifdef __cplusplus
}
#endif

#endif //ESP32_PERSON_DETECTION_INIT_FUNCTIONS_H
