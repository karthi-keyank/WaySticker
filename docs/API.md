# API Documentation

## Common API (`src/common/`)

### config.c - JSON Configuration

#### `save_stickers_config(GPtrArray *thumbnails)`

Serializes selected stickers to JSON configuration file.

**Parameters:**
- `thumbnails`: GPtrArray of ThumbnailData*

**Return:** void

**Behavior:**
1. Creates JSON root object
2. Creates "stickers" array
3. Iterates through thumbnails
4. For each selected sticker:
   - Creates sticker object
   - Adds path, x, y, scale fields
   - Appends to array
5. Writes to `config/stickers.json` with pretty formatting

**Example:**
```c
// Inside editor, when Save button clicked
save_stickers_config(context->thumbnails);
// Creates: config/stickers.json with all selected stickers
```

**Notes:**
- Only saves thumbnails with `selected == TRUE`
- Overwrites existing config
- Creates file if not exists

---

#### `load_stickers_config()`

Deserializes sticker configuration from JSON file.

**Parameters:** None

**Return:** `GPtrArray*` of StickerConfig*

**Behavior:**
1. Attempts to open `config/stickers.json`
2. If file missing, returns empty array
3. Parses JSON
4. Iterates through "stickers" array
5. For each sticker:
   - Creates StickerConfig struct
   - Extracts path, x, y, scale
   - Adds to array
6. Returns populated array

**Example:**
```c
// Inside renderer main()
GPtrArray *stickers = load_stickers_config();
for (guint i = 0; i < stickers->len; i++) {
    StickerConfig *config = g_ptr_array_index(stickers, i);
    create_sticker_window(app, config);
}
```

**Notes:**
- Safe: returns empty array if file not found
- Creates StickerConfig with g_malloc
- Caller must g_ptr_array_free() when done
- Maximum path length: 512 characters

---

### sticker.h - Shared Constants

#### `BASE_STICKER_SIZE`
```c
#define BASE_STICKER_SIZE 256
```
Base pixel size when scale = 1.0

#### `SCALE_STEP`
```c
#define SCALE_STEP 0.1
```
Per-scroll increment for size changes

#### `MIN_SCALE` / `MAX_SCALE`
```c
#define MIN_SCALE 0.1
#define MAX_SCALE 5.0
```
Scale bounds for clamping

#### `CONFIG_PATH`
```c
#define CONFIG_PATH "config/stickers.json"
```
Sticker configuration file location

#### `ASSETS_DIR`
```c
#define ASSETS_DIR "assets"
```
Sticker image file directory

#### `THUMBNAIL_SIZE`
```c
#define THUMBNAIL_SIZE 124
```
Gallery thumbnail display size

---

## Editor API (`src/editor/`)

### renderer.c - Runtime Sticker Windows

#### `create_runtime_sticker_window(ThumbnailData *thumbnail_data)`

Creates a GTK window for displaying a sticker on the desktop.

**Parameters:**
- `thumbnail_data`: Pointer to sticker metadata

**Return:** `GtkWidget*` window widget

**Behavior:**
1. Creates undecorated GTK window
2. Initializes layer-shell for window
3. Sets to BACKGROUND layer
4. Anchors to top-left corner
5. Sets position from `thumbnail_data->x`, `thumbnail_data->y`
6. Creates picture widget from `thumbnail_data->filepath`
7. Sizes picture based on `thumbnail_data->scale`
8. Sets window as child of picture
9. Sets up drag and resize handlers
10. Presents window on desktop
11. Stores window reference in `thumbnail_data->sticker_window`

**Example:**
```c
// When user clicks thumbnail
create_runtime_sticker_window(thumbnail_data);
// Window appears on desktop at (x, y) with current scale
```

**Window Properties:**
- No decorations (title bar, borders)
- Not focusable (click-through)
- BACKGROUND layer (behind all windows)
- Transparent background

---

#### `destroy_runtime_sticker_window(ThumbnailData *thumbnail_data)`

Destroys a sticker window and cleans up references.

**Parameters:**
- `thumbnail_data`: Pointer to sticker metadata

**Return:** void

**Behavior:**
1. Checks if sticker_window is valid
2. Stores window pointer
3. Sets sticker_window to NULL (prevent double destroy)
4. Calls gtk_window_destroy()
5. Cleans up gesture listeners

**Example:**
```c
// When user clicks remove button on chip
destroy_runtime_sticker_window(thumbnail_data);
// Window disappears, references cleared
```

---

### drag.c - Sticker Movement

#### `setup_sticker_drag(ThumbnailData *thumbnail_data)`

Adds drag gesture handler to sticker window.

**Parameters:**
- `thumbnail_data`: Pointer to sticker metadata

**Return:** void

**Behavior:**
1. Creates GtkGestureDrag
2. Connects "drag-begin" signal
   - Records start position
3. Connects "drag-update" signal
   - Calculates offset
   - Updates x, y in thumbnail_data
   - Updates window margins
   - Provides smooth real-time feedback
4. Attaches gesture to sticker window

**Drag Event Flow:**
```
on_drag_begin() - Record (start_x, start_y)
on_drag_update() - Update offset, recalc (x, y), move window
```

---

### resize.c - Sticker Scaling

#### `setup_sticker_resize(ThumbnailData *thumbnail_data)`

Adds scroll handler for size changes.

**Parameters:**
- `thumbnail_data`: Pointer to sticker metadata

**Return:** void

**Behavior:**
1. Creates GtkEventControllerScroll (vertical)
2. Connects "scroll" signal handler
3. Handler normalizes scroll delta to ±1.0
4. Applies SCALE_STEP (0.1) per scroll
5. Clamps scale between MIN_SCALE and MAX_SCALE
6. Updates window sizing
7. Attaches controller to sticker window

**Scroll Behavior:**
- Scroll UP (dy < 0) → scale increase (grow)
- Scroll DOWN (dy > 0) → scale decrease (shrink)
- Each gesture = one 0.1 increment
- Range: 0.1x to 5.0x

---

### gallery.c - Asset Discovery

#### `create_gallery(EditorContext *context)`

Creates asset gallery UI with thumbnails.

**Parameters:**
- `context`: Editor context (stores results)

**Return:** `GtkWidget*` flow box widget

**Behavior:**
1. Creates GTK flow box (4-8 column grid)
2. Scans ASSETS_DIR for image files
3. For each valid image:
   - Creates ThumbnailData struct
   - Initializes at position (100, 100)
   - Creates thumbnail widget
   - Adds to gallery
   - Stores in context->thumbnails
4. Returns flow box

**Supported Formats:** .png, .jpg, .jpeg, .webp

**Example:**
```c
gallery_widget = create_gallery(context);
// Returns flow box with all available stickers
// Populates context->thumbnails
```

---

### thumbnail.c - Individual Thumbnail

#### `create_thumbnail(ThumbnailData *thumbnail_data)`

Creates a single thumbnail display widget.

**Parameters:**
- `thumbnail_data`: Metadata (updated with widget reference)

**Return:** `GtkWidget*` thumbnail box

**Behavior:**
1. Creates vertical box (image + label)
2. Creates picture from filepath
3. Creates label with filename
4. Sets size to THUMBNAIL_SIZE (124px)
5. Adds click gesture handler
6. On click:
   - Toggles selected flag
   - Spawns/destroys runtime window
   - Updates visual styling
   - Updates chip in selected bar
7. Stores widget in thumbnail_data->widget

---

### chip.c - Selected UI

#### `create_selected_chip(ThumbnailData *thumbnail_data)`

Creates UI representation for selected sticker.

**Parameters:**
- `thumbnail_data`: Metadata for sticker

**Return:** `GtkWidget*` chip widget

**Behavior:**
1. Creates horizontal box
2. Adds filename label
3. Adds remove button (✕)
4. Connects remove button to deselection
5. Applies CSS styling

**Location:** Appears in "selected bar" at top of editor

---

## Renderer API (`src/renderer/`)

### window.c - Sticker Rendering

#### `create_sticker_window(GtkApplication *app, StickerConfig *config)`

Creates window to display single sticker.

**Parameters:**
- `app`: GtkApplication instance
- `config`: Sticker configuration (path, x, y, scale)

**Return:** `GtkWidget*` window widget

**Behavior:**
1. Creates application window
2. Initializes layer-shell
3. Sets BACKGROUND layer
4. Anchors to top-left
5. Sets position from config.x, config.y
6. Sets size from config.scale
7. Loads image from config.path
8. Creates picture widget
9. Sets picture sizing
10. Attaches picture to window
11. Presents window

**Example:**
```c
StickerConfig config = {
    .path = "assets/Abby.png",
    .x = 255,
    .y = -55,
    .scale = 2.4
};
create_sticker_window(app, &config);
// Window appears at (255, -55) with scale 2.4x
```

**Window Properties:**
- Undecorated
- Not focusable
- BACKGROUND layer
- Transparent
- No user interaction

---

## Data Structures

### ThumbnailData

```c
typedef struct ThumbnailData {
    char filepath[512];              // Asset file path
    gboolean selected;               // Is currently selected?
    int x;                           // X position (pixels)
    int y;                           // Y position (pixels)
    double scale;                    // Size multiplier
    EditorContext *context;          // Parent context
    GtkWidget *chip;                 // UI chip widget (or NULL)
    GtkWidget *widget;               // Thumbnail display
    GtkWidget *picture;              // Runtime sticker image
    GtkWidget *sticker_window;       // Runtime window (or NULL)
} ThumbnailData;
```

**Initialization:**
```c
ThumbnailData *data = g_malloc(sizeof(ThumbnailData));
data->filepath = "assets/Abby.png";
data->selected = FALSE;
data->x = 100;
data->y = 100;
data->scale = 1.0;
data->sticker_window = NULL;
```

### StickerConfig

```c
typedef struct {
    char path[512];      // Asset file path
    int x;               // X position
    int y;               // Y position
    double scale;        // Size multiplier
} StickerConfig;
```

### EditorContext

```c
typedef struct {
    GtkWidget *selected_bar;    // Container for chips
    GPtrArray *thumbnails;      // All ThumbnailData*
} EditorContext;
```

---

## Constants Reference

| Constant | Value | Purpose |
|----------|-------|---------|
| BASE_STICKER_SIZE | 256 | Base size in pixels |
| SCALE_STEP | 0.1 | Per-scroll change |
| MIN_SCALE | 0.1 | Minimum scale |
| MAX_SCALE | 5.0 | Maximum scale |
| THUMBNAIL_SIZE | 124 | Gallery thumbnail size |
| CONFIG_PATH | "config/stickers.json" | Config file |
| ASSETS_DIR | "assets" | Image directory |

---

## JSON Schema

Configuration file: `config/stickers.json`

```json
{
  "stickers": [
    {
      "path": "assets/filename.png",
      "x": 0,
      "y": 0,
      "scale": 1.0
    }
  ]
}
```

**Fields:**
- `path`: String (relative path to image)
- `x`: Integer (pixel position from left)
- `y`: Integer (pixel position from top)
- `scale`: Float (size multiplier, >= 0.1, <= 5.0)

**Notes:**
- Negative coordinates allowed (off-screen)
- No duplicates (one entry per sticker)
- Only selected stickers included

---

## Error Handling

### Config Loading
```c
GPtrArray *stickers = load_stickers_config();
if (!stickers || stickers->len == 0) {
    // No stickers configured
}
```

### Window Creation
```c
GtkWidget *window = create_sticker_window(app, config);
if (!window) {
    // Creation failed (check path validity)
}
```

### File Operations
```c
// Load returns empty array if file not found
// Save creates file if not exists
// No explicit error handling (silent fail)
```

---

## Thread Safety

Current implementation is **NOT thread-safe**:
- All GTK operations in main thread
- No locks or mutexes
- Single application instance

For multi-threaded use, wrap all GTK calls with `gdk_threads_add_idle()`.

---

## Memory Leaks

Common sources:

1. **ThumbnailData**: g_malloc'd, must g_free when done
2. **GPtrArray**: Must g_ptr_array_free() when done
3. **GtkWidgets**: Automatically freed by parent
4. **JSON Objects**: json_object_put() when done
5. **Strings**: Fixed buffers (no heap strings)

---

See [ARCHITECTURE.md](../ARCHITECTURE.md) for system design and [../README.md](../README.md) for usage.
