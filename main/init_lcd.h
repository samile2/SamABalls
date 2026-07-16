#include "bsp_spi.h"        // Bus
#include "bsp_display.h"    // Display
#include "esp_log.h"        // For ESP32 Logs system
#include "driver/gpio.h"    // For Back Light GPIO definition
#include "blob.h"
#include "assets.c"  
#include "math.h"           // For cos and sin function

void lcd_init(void);

void drawSpriteMasked(
    uint16_t *fb,
    int dstX,
    int dstY,
    const uint16_t *sprite,
    const uint8_t *mask,
    int w,
    int h,
	int IMG_WIDTH);
	
void lcd_draw(uint16_t *framebuffer,int IMG_WIDTH, int IMG_HEIGHT);

void init_lcd_vars(
	int IMG_WIDTH,
	int IMG_HEIGHT,
    uint16_t *x_pos,
    uint16_t *y_pos,
    uint16_t *framebuffer_index,
    uint16_t *x_min,
    uint16_t *x_max,
    uint16_t *y_min,
    uint16_t *y_max,
    uint16_t *x_center,
    uint16_t *y_center
);

void init_frame_buffers(
    uint16_t *framebuffers[],
    uint8_t num_buffers,
    int width,
    int height
);