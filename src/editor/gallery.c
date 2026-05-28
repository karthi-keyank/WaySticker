#include <gtk/gtk.h>

#include <dirent.h>
#include <string.h>
#include <stdio.h>

#include "gallery.h"
#include "thumbnail.h"
#include "../common/sticker.h"

static gboolean is_image_file(const char *filename)
{
    return
        g_str_has_suffix(filename, ".png")  ||
        g_str_has_suffix(filename, ".jpg")  ||
        g_str_has_suffix(filename, ".jpeg") ||
        g_str_has_suffix(filename, ".webp");
}



GtkWidget* create_gallery(EditorContext *context)
{
    GtkWidget *flowbox;

    DIR *dir;
    struct dirent *entry;

    int thumbnail_capacity = 100;

    flowbox = gtk_flow_box_new();

    context->thumbnails =
        g_ptr_array_new();

    gtk_flow_box_set_row_spacing(
        GTK_FLOW_BOX(flowbox),
        15
    );

    gtk_flow_box_set_column_spacing(
        GTK_FLOW_BOX(flowbox),
        15
    );

    gtk_flow_box_set_min_children_per_line(
        GTK_FLOW_BOX(flowbox),
        4
    );

    gtk_flow_box_set_max_children_per_line(
        GTK_FLOW_BOX(flowbox),
        8
    );

    gtk_flow_box_set_selection_mode(
        GTK_FLOW_BOX(flowbox),
        GTK_SELECTION_NONE
    );

    gtk_widget_set_margin_top(flowbox, 20);
    gtk_widget_set_margin_bottom(flowbox, 20);
    gtk_widget_set_margin_start(flowbox, 20);
    gtk_widget_set_margin_end(flowbox, 20);

    dir = opendir(ASSETS_DIR);

    if (!dir)
    {
        printf("Failed to open assets directory\n");
        return flowbox;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (!is_image_file(entry->d_name))
        {
            continue;
        }

        char filepath[512];

        snprintf(
            filepath,
            sizeof(filepath),
            "%s/%s",
            ASSETS_DIR,
            entry->d_name
        );

        ThumbnailData *thumbnail_data;
        thumbnail_data = g_malloc(sizeof(ThumbnailData));

        snprintf(
            thumbnail_data->filepath,
            sizeof(thumbnail_data->filepath),
            "%s",
            filepath
        );

        thumbnail_data->selected =  FALSE;
        thumbnail_data->context = context;
        thumbnail_data->chip = NULL;

        thumbnail_data->x = 100;

        thumbnail_data->y = 100;

        thumbnail_data->scale = 1.0;

        thumbnail_data->sticker_window = NULL;

        g_ptr_array_add(
            context->thumbnails,
            thumbnail_data
        );

        GtkWidget *thumbnail = create_thumbnail(thumbnail_data);

        GtkWidget *child =
            gtk_flow_box_child_new();

        gtk_flow_box_child_set_child(
            GTK_FLOW_BOX_CHILD(child),
            thumbnail
        );

        gtk_flow_box_append(
            GTK_FLOW_BOX(flowbox),
            child
        );
    }

    closedir(dir);

    return flowbox;
}