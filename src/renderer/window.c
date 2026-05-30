#include <gtk/gtk.h>

#include <gtk4-layer-shell.h>

#include "window.h"
#include "../common/sticker.h"

static gboolean css_loaded = FALSE;

static void load_css(void)
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

GtkWidget* create_sticker_window(
    GtkApplication *app,
    StickerConfig *config
)
{
    GtkWidget *window;
    GtkWidget *picture;

    int size;

    /*
     * Load CSS once
     */

    if (!css_loaded)
    {
        load_css();

        css_loaded = TRUE;
    }

    size =
        BASE_STICKER_SIZE *
        config->scale;

    /*
     * Create window
     */

    window =
        gtk_application_window_new(app);

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

    /*
     * Layer shell
     */

    gtk_layer_init_for_window(
        GTK_WINDOW(window)
    );

    gtk_layer_set_layer(
        GTK_WINDOW(window),
        GTK_LAYER_SHELL_LAYER_BACKGROUND
    );

    /*
     * Anchor top-left
     */

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

    /*
     * Position
     */

    gtk_layer_set_margin(
        GTK_WINDOW(window),
        GTK_LAYER_SHELL_EDGE_LEFT,
        config->x
    );

    gtk_layer_set_margin(
        GTK_WINDOW(window),
        GTK_LAYER_SHELL_EDGE_TOP,
        config->y
    );

    /*
     * Sticker image
     */

    picture =
        gtk_picture_new_for_filename(
            config->path
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

    gtk_window_set_default_size(
        GTK_WINDOW(window),
        size,
        size
    );

    gtk_widget_set_hexpand(
        picture,
        FALSE
    );

    gtk_widget_set_vexpand(
        picture,
        FALSE
    );

    gtk_window_set_child(
        GTK_WINDOW(window),
        picture
    );

    gtk_window_present(
        GTK_WINDOW(window)
    );

    return window;
}