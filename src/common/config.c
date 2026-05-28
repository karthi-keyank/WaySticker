#include <stdio.h>

#include <json-c/json.h>

#include "config.h"
#include "../editor/thumbnail.h"

#define CONFIG_PATH "config/stickers.json"

void save_stickers_config(
    GPtrArray *thumbnails
)
{
    json_object *root;
    json_object *stickers_array;

    root = json_object_new_object();

    stickers_array =
        json_object_new_array();

    for (
        guint i = 0;
        i < thumbnails->len;
        i++
    )
    {

        ThumbnailData *thumbnail =
            g_ptr_array_index(
                thumbnails,
                i
            );

        if (!thumbnail->selected)
        {
            continue;
        }

        json_object *sticker_object;

        sticker_object =
            json_object_new_object();

        json_object_object_add(
            sticker_object,
            "path",
            json_object_new_string(
                thumbnail->filepath
            )
        );

        json_object_object_add(
            sticker_object,
            "x",
            json_object_new_int(thumbnail->x)
        );

        json_object_object_add(
            sticker_object,
            "y",
            json_object_new_int(thumbnail->y)
        );

        json_object_object_add(
            sticker_object,
            "scale",
            json_object_new_double(thumbnail->scale)
        );

        json_object_array_add(
            stickers_array,
            sticker_object
        );
    }

    json_object_object_add(
        root,
        "stickers",
        stickers_array
    );

    json_object_to_file_ext(
        CONFIG_PATH,
        root,
        JSON_C_TO_STRING_PRETTY
    );

    json_object_put(root);

    printf(
        "Saved config: %s\n",
        CONFIG_PATH
    );
}


GPtrArray* load_stickers_config(void)
{
    GPtrArray *stickers;

    json_object *root;
    json_object *stickers_array;

    stickers =
        g_ptr_array_new();

    root =
        json_object_from_file(
            CONFIG_PATH
        );

    if (!root)
    {
        return stickers;
    }

    if (
        !json_object_object_get_ex(
            root,
            "stickers",
            &stickers_array
        )
    )
    {
        json_object_put(root);

        return stickers;
    }

    int count;

    count =
        json_object_array_length(
            stickers_array
        );

    for (int i = 0; i < count; i++)
    {
        json_object *item;

        StickerConfig *sticker;

        item =
            json_object_array_get_idx(
                stickers_array,
                i
            );

        sticker =
            g_new0(StickerConfig, 1);

        json_object *path_obj;
        json_object *x_obj;
        json_object *y_obj;
        json_object *scale_obj;

        json_object_object_get_ex(
            item,
            "path",
            &path_obj
        );

        json_object_object_get_ex(
            item,
            "x",
            &x_obj
        );

        json_object_object_get_ex(
            item,
            "y",
            &y_obj
        );

        json_object_object_get_ex(
            item,
            "scale",
            &scale_obj
        );

        snprintf(
            sticker->path,
            sizeof(sticker->path),
            "%s",
            json_object_get_string(
                path_obj
            )
        );

        sticker->x =
            json_object_get_int(
                x_obj
            );

        sticker->y =
            json_object_get_int(
                y_obj
            );

        sticker->scale =
            json_object_get_double(
                scale_obj
            );

        g_ptr_array_add(
            stickers,
            sticker
        );
    }

    json_object_put(root);

    return stickers;
}