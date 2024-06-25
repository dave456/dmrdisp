//
//    Copyright (C) 2024 Dave Lindner
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

#include <ssd1306_i2c.h>

#define OLED_HEIGHT 32
#define OLED_WIDTH 128
#define I2C_BUS_ID 0x3c

static const char *oledDevice = "/dev/i2c-1";


int main(int argc, char *argv[])
{
    ssd1306_graphics_options_t opts[2];
    ssd1306_framebuffer_box_t bbox;
    
    fprintf(stderr, "DEBUG: Using library version: %s\n", ssd1306_i2c_version());
    ssd1306_i2c_t *oled = ssd1306_i2c_open(oledDevice, I2C_BUS_ID, OLED_WIDTH, OLED_HEIGHT, NULL);
    if (!oled) {
	fprintf(stderr, "ERROR: Failed to open oled device\n");
        return -1;
    }

    if (ssd1306_i2c_display_initialize(oled) < 0) {
        fprintf(stderr, "ERROR: Failed to initialize oled display\n");
        ssd1306_i2c_close(oled);
        return -1;
    }
    

    ssd1306_framebuffer_t *fbp = ssd1306_framebuffer_create(oled->width, oled->height, oled->err);

    ssd1306_i2c_display_clear(oled);
    for (uint8_t i = oled->height - 1; i < oled->height; ++i) {
        for (uint8_t j = 0; j < oled->width; ++j)
            ssd1306_framebuffer_put_pixel(fbp, j, i, true);
    }

    //ssd1306_framebuffer_hexdump(fbp);
    //ssd1306_framebuffer_bitdump(fbp);
    ssd1306_framebuffer_clear(fbp);

    opts[0].type = SSD1306_OPT_FONT_FILE;
    opts[0].value.font_file = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
    opts[1].type = SSD1306_OPT_ROTATE_PIXEL;
    opts[1].value.rotation_degrees = 180;
    //opts[1].type = SSD1306_OPT_ROTATE_FONT;
    //opts[1].value.rotation_degrees = 180;
    
    ssd1306_framebuffer_draw_text_extra(fbp, "JumboSpot  13:45", 0, 0, 9, SSD1306_FONT_DEFAULT, 3, opts, 2, &bbox);
    ssd1306_framebuffer_bitdump(fbp);
    ssd1306_i2c_display_update(oled, fbp);

    sleep(15);

    ssd1306_i2c_run_cmd(oled, SSD1306_I2C_CMD_POWER_OFF, 0, 0);
    ssd1306_framebuffer_destroy(fbp);
    fbp = NULL;
    ssd1306_i2c_close(oled);
    oled = NULL;
	return 0;
}

