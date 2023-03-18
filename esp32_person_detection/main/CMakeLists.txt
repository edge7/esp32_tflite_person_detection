idf_component_register(SRCS "esp32_person_detection.cc"
        "init_functions.cpp"
        "esp32_model.cc"
        "image_provider.cc"
        "app_camera_esp.c"
        "wifi_utility.cpp"
        "memory_utility.cpp"
        "http_handler.cpp"
        INCLUDE_DIRS ".")
