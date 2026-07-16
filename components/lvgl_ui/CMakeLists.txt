idf_component_register(
    SRCS ${SRC_FILES}
    INCLUDE_DIRS "."
    REQUIRES
        "lvgl__lvgl"
        "esp_bsp"
        "freertos"
        "spi_flash"
        "esp_psram"
        "driver"
        "esp_hw_support"
        "esp_driver_tsens"
        "espressif__esp_lvgl_port"
)