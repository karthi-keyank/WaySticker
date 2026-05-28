#include <gtk/gtk.h>

#include "editor.h"
#include "gallery.h"
#include "style.h"
#include "../common/config.h"

static void on_close_clicked(
    GtkButton *button,
    gpointer data
)
{
    GtkWindow *window = data;

    gtk_window_destroy(window);
}

static void on_save_clicked(
    GtkButton *button,
    gpointer data
)
{
    EditorContext *context = data;

    save_stickers_config(
        context->thumbnails
    );
}

GtkWidget* create_editor_window(GtkApplication *app)
{
    GtkWidget *window;

    GtkWidget *main_box;

    GtkWidget *selected_bar;
    GtkWidget *separator;

    GtkWidget *scroll;
    GtkWidget *flowbox;

    GtkWidget *action_bar;

    GtkWidget *save_button;
    GtkWidget *close_button;

    EditorContext *context;

    window = gtk_application_window_new(app);

    load_css();

    gtk_window_set_title(
        GTK_WINDOW(window),
        "Sticker Editor"
    );

    gtk_window_set_default_size(
        GTK_WINDOW(window),
        1000,
        700
    );

    gtk_window_set_resizable(
        GTK_WINDOW(window),
        TRUE
    );

    /*
     * Main vertical layout
     */

    main_box = gtk_box_new(
        GTK_ORIENTATION_VERTICAL,
        10
    );

    gtk_window_set_child(
        GTK_WINDOW(window),
        main_box
    );

    /*
     * Selected chips bar
     */

    selected_bar = gtk_flow_box_new();

    context =
        g_new0(EditorContext, 1);

    context->selected_bar =
        selected_bar;

    gtk_flow_box_set_selection_mode(
        GTK_FLOW_BOX(selected_bar),
        GTK_SELECTION_NONE
    );

    gtk_flow_box_set_row_spacing(
        GTK_FLOW_BOX(selected_bar),
        8
    );

    gtk_flow_box_set_column_spacing(
        GTK_FLOW_BOX(selected_bar),
        8
    );

    gtk_flow_box_set_max_children_per_line(
        GTK_FLOW_BOX(selected_bar),
        999
    );

    gtk_widget_set_margin_top(
        selected_bar,
        10
    );

    gtk_widget_set_margin_bottom(
        selected_bar,
        4
    );

    gtk_widget_set_margin_start(
        selected_bar,
        10
    );

    gtk_widget_set_margin_end(
        selected_bar,
        10
    );

    gtk_widget_set_hexpand(
        selected_bar,
        TRUE
    );

    gtk_widget_set_halign(
        selected_bar,
        GTK_ALIGN_START
    );

    /*
     * Separator
     */

    separator = gtk_separator_new(
        GTK_ORIENTATION_HORIZONTAL
    );

    /*
     * Scroll container
     */

    scroll = gtk_scrolled_window_new();

    gtk_widget_set_vexpand(
        scroll,
        TRUE
    );

    gtk_widget_set_hexpand(
        scroll,
        TRUE
    );

    /*
     * Gallery
     */

    flowbox = create_gallery(context);

    gtk_scrolled_window_set_child(
        GTK_SCROLLED_WINDOW(scroll),
        flowbox
    );

    /*
     * Bottom action bar
     */

    action_bar = gtk_box_new(
        GTK_ORIENTATION_HORIZONTAL,
        8
    );

    gtk_widget_set_margin_top(
        action_bar,
        6
    );

    gtk_widget_set_margin_bottom(
        action_bar,
        10
    );

    gtk_widget_set_margin_end(
        action_bar,
        10
    );

    gtk_widget_set_halign(
        action_bar,
        GTK_ALIGN_END
    );

    save_button =
        gtk_button_new_with_label(
            "Save"
        );

    g_signal_connect(
        save_button,
        "clicked",
        G_CALLBACK(on_save_clicked),
        context
    );

    close_button =
        gtk_button_new_with_label(
            "Close"
        );

    g_signal_connect(
        close_button,
        "clicked",
        G_CALLBACK(on_close_clicked),
        window
    );

    gtk_box_append(
        GTK_BOX(action_bar),
        save_button
    );

    gtk_box_append(
        GTK_BOX(action_bar),
        close_button
    );

    /*
     * Compose layout
     */

    gtk_box_append(
        GTK_BOX(main_box),
        selected_bar
    );

    gtk_box_append(
        GTK_BOX(main_box),
        separator
    );

    gtk_box_append(
        GTK_BOX(main_box),
        scroll
    );

    gtk_box_append(
        GTK_BOX(main_box),
        action_bar
    );

    gtk_window_present(
        GTK_WINDOW(window)
    );

    return window;
}