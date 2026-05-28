#pragma once
#include <gtk/gtk.h>

typedef struct
{
    char path[512];

    int x;
    int y;

    double scale;

} StickerConfig;

void save_stickers_config(
    GPtrArray *thumbnails
);

GPtrArray* load_stickers_config(void);