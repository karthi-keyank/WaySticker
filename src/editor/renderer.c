#include <gtk/gtk.h>

#include <gtk4-layer-shell.h>

#include "renderer.h"
#include "drag.h"
#include "resize.h"

#define BASE_STICKER_SIZE 256

static gboolean runtime_css_loaded = FALSE;

static void load_runtime_css(void)
{
    GtkCssProvider *provider;

    provider = gtk_css_provider_new();

    gtk_css_provider_load_from_string(
        provider,
        "window { background-color: transparent; }"
    );

    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );

    g_object_unref(provider);
}

static void setup_runtime_window(
    GtkWidget *window
)
{
    gtk_window_set_decorated(
        GTK_WINDOW(window),
        FALSE
    );

    gtk_window_set_resizable(
        GTK_WINDOW(window),
        FALSE
    );

    gtk_widget_set_focusable(
        window,
        FALSE
    );
}

static void setup_layer_shell(
    GtkWidget *window,
    ThumbnailData *thumbnail_data
)
{
    gtk_layer_init_for_window(
        GTK_WINDOW(window)
    );

    gtk_layer_set_layer(
        GTK_WINDOW(window),
        GTK_LAYER_SHELL_LAYER_BACKGROUND
    );

    gtk_layer_set_anchor(
        GTK_WINDOW(window),
        GTK_LAYER_SHELL_EDGE_TOP,
        TRUE
    );

    gtk_layer_set_anchor(
        GTK_WINDOW(window),
        GTK_LAYER_SHELL_EDGE_LEFT,
        TRUE
    );

    gtk_layer_set_margin(
        GTK_WINDOW(window),
        GTK_LAYER_SHELL_EDGE_TOP,
        thumbnail_data->y
    );

    gtk_layer_set_margin(
        GTK_WINDOW(window),
        GTK_LAYER_SHELL_EDGE_LEFT,
        thumbnail_data->x
    );
}

static GtkWidget* create_sticker_picture(
    ThumbnailData *thumbnail_data
)
{
    GtkWidget *picture;

    int size;

    size =
        BASE_STICKER_SIZE *
        thumbnail_data->scale;

    picture =
        gtk_picture_new_for_filename(
            thumbnail_data->filepath
        );

    gtk_picture_set_can_shrink(
        GTK_PICTURE(picture),
        TRUE
    );

    gtk_picture_set_content_fit(
        GTK_PICTURE(picture),
        GTK_CONTENT_FIT_CONTAIN
    );

    gtk_widget_set_size_request(
        picture,
        size,
        size
    );

    thumbnail_data->picture =
        picture;

    return picture;
}

GtkWidget* create_runtime_sticker_window(
    ThumbnailData *thumbnail_data
)
{
    GtkWidget *window;
    GtkWidget *picture;

    /*
     * Prevent duplicate windows
     */

    if (thumbnail_data->sticker_window)
    {
        return thumbnail_data->sticker_window;
    }

    /*
     * Load CSS once
     */

    if (!runtime_css_loaded)
    {
        load_runtime_css();

        runtime_css_loaded = TRUE;
    }

    /*
     * Create window
     */

    window = gtk_window_new();

    thumbnail_data->sticker_window =
        window;

    /*
     * Setup
     */

    setup_runtime_window(
        window
    );

    setup_layer_shell(
        window,
        thumbnail_data
    );

    /*
     * Create sticker picture
     */

    picture =
        create_sticker_picture(
            thumbnail_data
        );

    gtk_window_set_child(
        GTK_WINDOW(window),
        picture
    );

    /*
     * Interactions
     */

    setup_sticker_drag(
        thumbnail_data
    );

    setup_sticker_resize(
        thumbnail_data
    );

    gtk_window_present(
        GTK_WINDOW(window)
    );

    return window;
}

void destroy_runtime_sticker_window(
    ThumbnailData *thumbnail_data
)
{
    GtkWidget *window;

    if (
        !thumbnail_data ||
        !thumbnail_data->sticker_window
    )
    {
        return;
    }

    window =
        thumbnail_data->sticker_window;

    thumbnail_data->sticker_window =
        NULL;

    thumbnail_data->picture =
        NULL;

    gtk_window_destroy(
        GTK_WINDOW(window)
    );
}