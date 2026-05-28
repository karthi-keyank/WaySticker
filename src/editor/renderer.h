#pragma once

#include <gtk/gtk.h>

#include "thumbnail.h"

GtkWidget* create_runtime_sticker_window(
    ThumbnailData *thumbnail_data
);

void destroy_runtime_sticker_window(
    ThumbnailData *thumbnail_data
);