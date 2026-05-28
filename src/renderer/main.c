#include <gtk/gtk.h>

#include "../common/config.h"
#include "window.h"

static void on_activate(
    GtkApplication *app,
    gpointer data
)
{
    GPtrArray *stickers;

    (void)data;

    stickers =
        load_stickers_config();

    for (
        guint i = 0;
        i < stickers->len;
        i++
    )
    {
        StickerConfig *config;

        config =
            g_ptr_array_index(
                stickers,
                i
            );

        create_sticker_window(
            app,
            config
        );
    }
}

int main(
    int argc,
    char **argv
)
{
    GtkApplication *app;

    int status;

    app =
        gtk_application_new(
            "com.karthi.stickers.renderer",
            G_APPLICATION_DEFAULT_FLAGS
        );

    g_signal_connect(
        app,
        "activate",
        G_CALLBACK(on_activate),
        NULL
    );

    status =
        g_application_run(
            G_APPLICATION(app),
            argc,
            argv
        );

    g_object_unref(app);

    return status;
}