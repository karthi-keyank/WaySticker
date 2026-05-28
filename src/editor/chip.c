#include <gtk/gtk.h>

#include "chip.h"
#include "renderer.h"

static void on_chip_remove_clicked(
    GtkButton *button,
    gpointer data
)
{
    ThumbnailData *thumbnail_data = data;

    thumbnail_data->selected = FALSE;

    destroy_runtime_sticker_window(
        thumbnail_data
    );

    gtk_widget_remove_css_class(
        thumbnail_data->widget,
        "selected-thumbnail"
    );

    if (thumbnail_data->chip)
    {
        gtk_flow_box_remove(
            GTK_FLOW_BOX(
                thumbnail_data
                    ->context
                    ->selected_bar
            ),
            thumbnail_data->chip
        );

        thumbnail_data->chip = NULL;
    }
}

GtkWidget* create_selected_chip(
    ThumbnailData *thumbnail_data
)
{
    GtkWidget *chip_box;

    GtkWidget *label;
    GtkWidget *remove_button;

    chip_box = gtk_box_new(
        GTK_ORIENTATION_HORIZONTAL,
        4
    );

    gtk_widget_add_css_class(
        chip_box,
        "selected-chip"
    );

    gtk_widget_set_halign(
        chip_box,
        GTK_ALIGN_START
    );

    gtk_widget_set_hexpand(
        chip_box,
        FALSE
    );

    label = gtk_label_new(
        g_path_get_basename(
            thumbnail_data->filepath
        )
    );

    gtk_widget_set_margin_start(
        label,
        1
    );

    remove_button =
        gtk_button_new_with_label("✕");

    gtk_widget_add_css_class(
        remove_button,
        "chip-remove-button"
    );

    gtk_box_append(
        GTK_BOX(chip_box),
        label
    );

    gtk_box_append(
        GTK_BOX(chip_box),
        remove_button
    );

    g_signal_connect(
        remove_button,
        "clicked",
        G_CALLBACK(on_chip_remove_clicked),
        thumbnail_data
    );

    return chip_box;
}

void update_selected_chip(
    ThumbnailData *thumbnail_data
)
{
    if (thumbnail_data->selected)
    {
        thumbnail_data->chip =
            create_selected_chip(
                thumbnail_data
            );

        gtk_flow_box_append(
            GTK_FLOW_BOX(
                thumbnail_data
                    ->context
                    ->selected_bar
            ),
            thumbnail_data->chip
        );
    }
    else
    {
        if (thumbnail_data->chip)
        {
            gtk_flow_box_remove(
                GTK_FLOW_BOX(
                    thumbnail_data
                        ->context
                        ->selected_bar
                ),
                thumbnail_data->chip
            );

            thumbnail_data->chip = NULL;
        }
    }
}