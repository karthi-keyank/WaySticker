#pragma once

#include <gtk/gtk.h>

#include "thumbnail.h"

GtkWidget* create_selected_chip(
    ThumbnailData *thumbnail_data
);

void update_selected_chip(
    ThumbnailData *thumbnail_data
);