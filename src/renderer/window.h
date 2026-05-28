#pragma once

#include <gtk/gtk.h>

#include "../common/config.h"

GtkWidget* create_sticker_window(
    GtkApplication *app,
    StickerConfig *config
);