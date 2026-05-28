#pragma once

#include <gtk/gtk.h>

typedef struct ThumbnailData ThumbnailData;

typedef struct
{
    GtkWidget *selected_bar;
    GPtrArray *thumbnails;
    
} EditorContext;

GtkWidget* create_editor_window(
    GtkApplication *app
);