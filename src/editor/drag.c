#include <gtk/gtk.h>

#include <gtk4-layer-shell.h>
#include <stdlib.h>

#include "drag.h"

typedef struct
{
    int start_x;
    int start_y;

    int last_x;
    int last_y;

} DragData;

static void on_drag_begin(
    GtkGestureDrag *gesture,
    double start_x,
    double start_y,
    gpointer data
)
{
    ThumbnailData *thumbnail_data = data;

    DragData *drag_data;

    (void)start_x;
    (void)start_y;

    drag_data =
        g_new0(DragData, 1);

    drag_data->start_x =
        thumbnail_data->x;

    drag_data->start_y =
        thumbnail_data->y;

    drag_data->last_x =
        thumbnail_data->x;

    drag_data->last_y =
        thumbnail_data->y;

    g_object_set_data_full(
        G_OBJECT(gesture),
        "drag-data",
        drag_data,
        g_free
    );
}

static void update_window_position(
    ThumbnailData *thumbnail_data
)
{
    gtk_layer_set_margin(
        GTK_WINDOW(
            thumbnail_data
                ->sticker_window
        ),
        GTK_LAYER_SHELL_EDGE_LEFT,
        thumbnail_data->x
    );

    gtk_layer_set_margin(
        GTK_WINDOW(
            thumbnail_data
                ->sticker_window
        ),
        GTK_LAYER_SHELL_EDGE_TOP,
        thumbnail_data->y
    );
}

static void on_drag_update(
    GtkGestureDrag *gesture,
    double offset_x,
    double offset_y,
    gpointer data
)
{
    ThumbnailData *thumbnail_data = data;

    DragData *drag_data;

    int new_x;
    int new_y;

    drag_data =
        g_object_get_data(
            G_OBJECT(gesture),
            "drag-data"
        );

    if (!drag_data)
    {
        return;
    }

    new_x =
        drag_data->start_x +
        (int)offset_x;

    new_y =
        drag_data->start_y +
        (int)offset_y;

    /*
     * Ignore tiny compositor spam updates
     */

    if (
        abs(new_x - drag_data->last_x) < 2 &&
        abs(new_y - drag_data->last_y) < 2
    )
    {
        return;
    }

    thumbnail_data->x = new_x;
    thumbnail_data->y = new_y;

    drag_data->last_x = new_x;
    drag_data->last_y = new_y;

    update_window_position(
        thumbnail_data
    );
}

void setup_sticker_drag(
    ThumbnailData *thumbnail_data
)
{
    GtkGesture *drag_gesture;

    drag_gesture =
        gtk_gesture_drag_new();

    g_signal_connect(
        drag_gesture,
        "drag-begin",
        G_CALLBACK(on_drag_begin),
        thumbnail_data
    );

    g_signal_connect(
        drag_gesture,
        "drag-update",
        G_CALLBACK(on_drag_update),
        thumbnail_data
    );

    gtk_widget_add_controller(
        thumbnail_data->sticker_window,
        GTK_EVENT_CONTROLLER(
            drag_gesture
        )
    );
}