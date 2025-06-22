#ifndef DISPLAY_H
#define DISPLAY_H

#define SCREEN_SIZE_X 160
#define SCREEN_SIZE_Y 144

#define LCDC_BG_ENABLE              ((u8)(1 << 0))
#define LCDC_OBJ_ENABLE             ((u8)(1 << 1))
#define LCDC_LONG_OBJ               ((u8)(1 << 2))
#define LCDC_BG_TILEMAP_SELECT      ((u8)(1 << 3))
#define LCDC_ADDRESSING_MODE        ((u8)(1 << 4))
#define LCDC_WINDOW_ENABLE          ((u8)(1 << 5))
#define LCDC_WINDOW_TILEMAP_SELECT  ((u8)(1 << 6))
#define LCDC_LCD_ENABLE             ((u8)(1 << 7))

#endif /* DISPLAY_H */
