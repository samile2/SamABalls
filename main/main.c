#include "esp_heap_caps.h"  // For memory free display logs
#include "assets.c"         // Contain Bitmap data
#include "math.h"           // For cos and sin function
#include "esp_timer.h"      // For millisecondes
#include "blob.h"
#include "init_lcd.h"

#define IMG_WIDTH 240
#define IMG_HEIGHT 240


uint16_t x_pos;
uint16_t y_pos; 
uint16_t framebuffer_index;
uint16_t x_min;
uint16_t x_max;
uint16_t y_min;
uint16_t y_max;
uint16_t x_center;
uint16_t y_center;
int64_t initial_time;
float time,t=0;

static uint16_t framebuffer01[IMG_WIDTH * IMG_HEIGHT];
static uint16_t framebuffer02[IMG_WIDTH * IMG_HEIGHT];
static uint16_t framebuffer03[IMG_WIDTH * IMG_HEIGHT];
static uint16_t * framebuffers[] = {framebuffer01, framebuffer02, framebuffer03};


void app_main(void)
{
	const uint8_t num_of_framebuffer=sizeof(framebuffers)/sizeof(framebuffers[0]);
	float max_display_time=30.0f;
    init_lcd_vars(IMG_WIDTH,IMG_HEIGHT,&x_pos, &y_pos, &framebuffer_index, &x_min, &x_max, &y_min, &y_max, &x_center, &y_center);
	lcd_init();
	
	init_frame_buffers(framebuffers, num_of_framebuffer,IMG_WIDTH, IMG_HEIGHT);
	blob_init(x_min,x_max,y_min,y_max);
	max_display_time=switchBlob(RANDOM_BLOB_FORMULA);
	
	// Log free ram
    ESP_LOGI("RAM", "RAM libre : %d octets\n",
    heap_caps_get_free_size(MALLOC_CAP_8BIT));
    ESP_LOGI("RAM", "Plus grand bloc : %d octets\n",
    heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));
    
	initial_time=esp_timer_get_time();
	while (1)
    {
        
		if(t>max_display_time){
			initial_time=esp_timer_get_time();
			init_frame_buffers(framebuffers, num_of_framebuffer, IMG_WIDTH, IMG_HEIGHT);
			max_display_time=switchBlob(RANDOM_BLOB_FORMULA);
		}
		
		//calculate time
		int64_t us = esp_timer_get_time();
		time=us-initial_time;
		t = time / 1000000.0f; // secondes
		
		ESP_LOGI("DEBUG", "time t : %.3f\n",t);
		
		
		Point pos = getBlobPosition(t);
		x_pos = pos.x;
		y_pos = pos.y;

		// Don't draw outside the screen
		if(x_pos<x_min) x_pos=x_min;
		if(y_pos<y_min) y_pos=y_min;
		if(x_pos>=x_max) x_pos=x_max;
		if(y_pos>=y_max) y_pos=y_max;
		

        drawSpriteMasked(
            framebuffers[framebuffer_index],
            x_pos,
            y_pos,
            (const uint16_t *) amigaball48x48,
            amigaballMASK48x48,
            AMIGABALLMASK48X48_WIDTH,
            AMIGABALLMASK48X48_HEIGHT,
			IMG_WIDTH
        );   


        // Draw image on screen
        lcd_draw(framebuffers[framebuffer_index], IMG_WIDTH, IMG_HEIGHT);
 
        framebuffer_index+=1;
        framebuffer_index%=sizeof(framebuffers)/sizeof(framebuffers[0]);
        /*
		ESP_LOGI("DEBUG", "framebuffer_index : %d\n",framebuffer_index);
        ESP_LOGI("DEBUG", "x_pos : %d\n",x_pos);
        ESP_LOGI("DEBUG", "y_pos : %d\n",y_pos);
		*/

        // Wait a moment please
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}