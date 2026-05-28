#pragma once

#include <gtk/gtk.h>

#include "editor.h"

typedef struct ThumbnailData
{
    char filepath[512];

    gboolean selected;

    int x;
    int y;

    double scale;

    EditorContext *context;

    GtkWidget *chip;
    GtkWidget *widget;
    GtkWidget *picture;
    GtkWidget *sticker_window;

} ThumbnailData;

GtkWidget* create_thumbnail(
    ThumbnailData *thumbnail_data
);