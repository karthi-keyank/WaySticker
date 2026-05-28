#include <gtk/gtk.h>

#include "resize.h"
#include "../common/sticker.h"

static void update_sticker_scale(
    ThumbnailData *thumbnail_data
)
{
    int size;

    size =
        BASE_STICKER_SIZE *
        thumbnail_data->scale;

    gtk_widget_set_size_request(
        thumbnail_data->picture,
        size,
        size
    );

    gtk_window_set_default_size(
        GTK_WINDOW(
            thumbnail_data->sticker_window
        ),
        size,
        size
    );
}

static void clamp_scale(
    ThumbnailData *thumbnail_data
)
{
    if (
        thumbnail_data->scale <
        MIN_SCALE
    )
    {
        thumbnail_data->scale =
            MIN_SCALE;
    }

    if (
        thumbnail_data->scale >
        MAX_SCALE
    )
    {
        thumbnail_data->scale =
            MAX_SCALE;
    }
}

static gboolean on_scroll(
    GtkEventControllerScroll *controller,
    double dx,
    double dy,
    gpointer data
)
{
    ThumbnailData *thumbnail_data = data;

    double scroll_delta;

    (void)controller;
    (void)dx;

    /*
     * Scroll up (negative dy)
     * Scroll down (positive dy)
     * Normalize scroll delta to prevent
     * extreme changes on first scroll
     */

    scroll_delta = dy < 0 ? 1.0 : -1.0;

    thumbnail_data->scale +=
        SCALE_STEP * scroll_delta;

    clamp_scale(
        thumbnail_data
    );

    update_sticker_scale(
        thumbnail_data
    );

    return TRUE;
}

void setup_sticker_resize(
    ThumbnailData *thumbnail_data
)
{
    GtkEventController *scroll;

    scroll =
        gtk_event_controller_scroll_new(
            GTK_EVENT_CONTROLLER_SCROLL_VERTICAL
        );

    g_signal_connect(
        scroll,
        "scroll",
        G_CALLBACK(on_scroll),
        thumbnail_data
    );

    gtk_widget_add_controller(
        thumbnail_data->sticker_window,
        scroll
    );
}