idf_component_register(SRCS "main.c" "blob.c" "init_lcd.c" 
                    INCLUDE_DIRS "." "../components" 
		REQUIRES esp_bsp
)
