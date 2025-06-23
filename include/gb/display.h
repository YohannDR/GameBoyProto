#ifndef DISPLAY_H
#define DISPLAY_H

#define SCREEN_SIZE_X 160u
#define SCREEN_SIZE_Y 144u

#define LCDC_BG_ENABLE              (1u << 0)
#define LCDC_OBJ_ENABLE             (1u << 1)
#define LCDC_LONG_OBJ               (1u << 2)
#define LCDC_BG_TILEMAP_SELECT      (1u << 3)
#define LCDC_ADDRESSING_MODE        (1u << 4)
#define LCDC_WINDOW_ENABLE          (1u << 5)
#define LCDC_WINDOW_TILEMAP_SELECT  (1u << 6)
#define LCDC_LCD_ENABLE             (1u << 7)

#endif /* DISPLAY_H */
