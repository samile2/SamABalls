#include "init_lcd.h"    

#define LCD_BL_GPIO GPIO_NUM_6

esp_lcd_panel_io_handle_t io_handle = NULL;
esp_lcd_panel_handle_t panel_handle = NULL;

void lcd_init(void)
{
    ESP_LOGI("LCD", "Init BSP display");

    bsp_spi_init();

    bsp_display_brightness_init();
    bsp_display_set_brightness(100);

    bsp_display_init(
        &io_handle,
        &panel_handle,
        240 * 50
    );

    ESP_LOGI("LCD", "LCD initialized");
}

void drawSpriteMasked(
    uint16_t *fb,
    int dstX,
    int dstY,
    const uint16_t *sprite,
    const uint8_t *mask,
    int w,
    int h,
	int IMG_WIDTH)
{
    int bytesPerLine = (w + 7) / 8;

    for (int y = 0; y < h; y++)
    {
        const uint8_t *m = &mask[y * bytesPerLine];

        for (int x = 0; x < w; x++)
        {
            if (m[x >> 3] & (0x80 >> (x & 7)))
            {
                fb[(dstY + y) * IMG_WIDTH + dstX + x] =
                    sprite[y * w + x];
            }
        }
    }
}

void lcd_draw(uint16_t *framebuffer,int IMG_WIDTH, int IMG_HEIGHT){
        ESP_ERROR_CHECK(
            esp_lcd_panel_draw_bitmap(
                panel_handle,
                0,
                0,
                IMG_WIDTH,
                IMG_HEIGHT,
                framebuffer
            )
        );
}

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
)
{
    *x_pos = 0;
    *y_pos = 0;

    *framebuffer_index = 0;

    *x_min = 0;
    *x_max = IMG_WIDTH - AMIGABALLMASK48X48_WIDTH;

    *y_min = 0;
    *y_max = IMG_HEIGHT - AMIGABALLMASK48X48_HEIGHT;

    *x_center = round(*x_max / 2.0f);
    *y_center = round(*y_max / 2.0f);
}

void init_frame_buffers(
    uint16_t *framebuffers[],
    uint8_t num_buffers,
    int width,
    int height
)
{
    for(uint8_t f = 0; f < num_buffers; f++)
    {
        for(uint32_t i = 0; i < width * height; i++)
        {
            framebuffers[f][i] = 0x0000;
        }
    }
}