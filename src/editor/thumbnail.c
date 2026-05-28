#include <gtk/gtk.h>

#include "thumbnail.h"
#include "chip.h"
#include "renderer.h"
#include "../common/sticker.h"

static void update_thumbnail_visual(
    GtkWidget *widget,
    ThumbnailData *thumbnail_data
)
{
    if (thumbnail_data->selected)
    {
        gtk_widget_add_css_class(
            widget,
            "selected-thumbnail"
        );
    }
    else
    {
        gtk_widget_remove_css_class(
            widget,
            "selected-thumbnail"
        );
    }
}

static void on_thumbnail_clicked(
    GtkGestureClick *gesture,
    int n_press,
    double x,
    double y,
    gpointer data
)
{
    ThumbnailData *thumbnail_data = data;

    thumbnail_data->selected =
        !thumbnail_data->selected;

    if (thumbnail_data->selected)
    {
        create_runtime_sticker_window(
            thumbnail_data
        );
    }
    else
    {
        destroy_runtime_sticker_window(
            thumbnail_data
        );
    }

        GtkWidget *widget;

    widget =
        gtk_event_controller_get_widget(
            GTK_EVENT_CONTROLLER(gesture)
        );

    update_thumbnail_visual(
        widget,
        thumbnail_data
    );

    update_selected_chip(
        thumbnail_data
    );
}

GtkWidget* create_thumbnail(
    ThumbnailData *thumbnail_data
)
{
    GtkWidget *box;
    GtkWidget *picture;
    GtkWidget *label;

    GtkGesture *click_gesture;

    box = gtk_box_new(
        GTK_ORIENTATION_VERTICAL,
        5
    );

    thumbnail_data->widget = box;

    gtk_widget_set_size_request(
        box,
        150,
        180
    );

    picture =
        gtk_picture_new_for_filename(
            thumbnail_data->filepath
        );

    gtk_picture_set_content_fit(
        GTK_PICTURE(picture),
        GTK_CONTENT_FIT_CONTAIN
    );

    gtk_widget_set_size_request(
        picture,
        THUMBNAIL_SIZE,
        THUMBNAIL_SIZE
    );

    gtk_picture_set_can_shrink(
        GTK_PICTURE(picture),
        TRUE
    );

    label = gtk_label_new(
        g_path_get_basename(
            thumbnail_data->filepath
        )
    );

    gtk_label_set_wrap(
        GTK_LABEL(label),
        TRUE
    );

    gtk_label_set_max_width_chars(
        GTK_LABEL(label),
        16
    );

    gtk_label_set_justify(
        GTK_LABEL(label),
        GTK_JUSTIFY_CENTER
    );

    gtk_box_append(
        GTK_BOX(box),
        picture
    );

    gtk_box_append(
        GTK_BOX(box),
        label
    );

    click_gesture =
        gtk_gesture_click_new();

    g_signal_connect(
        click_gesture,
        "pressed",
        G_CALLBACK(on_thumbnail_clicked),
        thumbnail_data
    );

    gtk_widget_add_controller(
        box,
        GTK_EVENT_CONTROLLER(click_gesture)
    );

    return box;
}