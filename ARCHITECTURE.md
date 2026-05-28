# System Architecture - Stickers Project

## Overview

The Stickers system is a dual-application desktop overlay framework:

```
User Workflow:
┌──────────────────────────────────────────────────────────┐
│                                                          │
│  stickers-editor (interactive)                           │
│  ├─ Gallery: Browse assets                               │
│  ├─ Runtime: Preview stickers on desktop                 │
│  ├─ Interaction: Drag, resize, position                  │
│  └─ Persistence: Save to JSON config                     │
│        ↓                                                 │
│  config/stickers.json (state)                            │
│        ↓                                                 │
│  stickers-render (passive display)                       │
│  ├─ Load JSON                                            │
│  ├─ Render stickers at saved positions/scales            │
│  └─ Display on desktop (no interaction)                  │
│                                                          │
└──────────────────────────────────────────────────────────┘
```

---

## Component Architecture

### 1. Editor Application (`stickers-editor`)

Entry point: `src/editor/main.c`

**Modules:**

#### editor.c - Main Window
- Creates root GTK window
- Manages window lifecycle
- Coordinates all UI components
- Handles save/close buttons

#### gallery.c - Asset Gallery
- Scans `assets/` directory
- Creates thumbnail flow box (4-8 columns)
- Loads image metadata
- Returns GPtrArray of ThumbnailData

#### thumbnail.c - Individual Thumbnail
- Displays small preview (124×124px)
- Handles click selection
- Visual feedback (CSS class toggle)
- Triggers runtime sticker spawn/destroy

#### chip.c - Selected UI Chip
- Shows selected sticker in "selected bar"
- One chip per selected sticker
- Remove button (X) for deselection
- CSS styling for visual separation

#### renderer.c - Runtime Sticker Window
- Creates actual sticker display window
- Uses GTK4 layer-shell for desktop overlay
- Sets background layer (not above other windows)
- Applies CSS transparency
- Returns window widget reference

#### drag.c - Sticker Dragging
- GtkGestureDrag event handler
- Tracks start position and current offset
- Updates layer-shell margins in real-time
- Updates ThumbnailData.x and ThumbnailData.y

#### resize.c - Sticker Scaling
- GtkEventControllerScroll handler
- Normalized scroll delta (±1.0)
- Scale clamping (0.1 to 5.0)
- Updates window/picture sizing
- Smooth single-step increments

#### style.c - CSS Styling
- Editor window CSS class definitions
- Thumbnail selected state styling
- Chip appearance
- Color/spacing definitions

**Data Flow:**
```
main() → activate()
  ↓
create_editor_window()
  ├─ create_gallery() → GPtrArray<ThumbnailData>
  ├─ create_header() → save/close buttons
  ├─ create_selected_bar() → Chip container
  └─ connect signals
       ↓
thumbnail clicked
  ├─ Toggle selected flag
  ├─ create_runtime_sticker_window() → GtkWidget
  ├─ setup_sticker_drag() → Listen to drag
  ├─ setup_sticker_resize() → Listen to scroll
  └─ create_selected_chip() → Add to bar
```

### 2. Renderer Application (`stickers-render`)

Entry point: `src/renderer/main.c`

**Modules:**

#### main.c - Entry Point
- Initializes GtkApplication
- Activates window creation
- Runs application event loop

#### window.c - Sticker Window Creation
- create_sticker_window(app, config)
- Receives StickerConfig (path, x, y, scale)
- Creates window with layer-shell properties
- Sets position and size from config
- Returns window widget
- Simple and stateless

**Data Flow:**
```
main() → activate()
  ↓
load_stickers_config() → GPtrArray<StickerConfig>
  ↓
for each StickerConfig:
  ├─ create_sticker_window()
  │  ├─ gtk_application_window_new()
  │  ├─ gtk_layer_init_for_window()
  │  ├─ gtk_layer_set_layer(BACKGROUND)
  │  ├─ gtk_layer_set_margin() → Position
  │  ├─ gtk_widget_set_size_request() → Size
  │  └─ gtk_window_present()
  └─ Continue loop
  ↓
gtk_window_present() → Display all windows
  ↓
g_application_run() → Event loop (passive)
```

### 3. Common Utilities (`src/common/`)

#### config.c - JSON Configuration
- **save_stickers_config(GPtrArray\*)**: Write to JSON
  - Filters only selected stickers
  - Serializes x, y, scale, path
  - Pretty-prints to file
  
- **load_stickers_config()**: Read from JSON
  - Returns GPtrArray of StickerConfig
  - Returns empty array if file missing
  - Safe deserialization

#### sticker.h - Shared Constants
- BASE_STICKER_SIZE (256px)
- SCALE_STEP (0.1)
- MIN_SCALE (0.1x)
- MAX_SCALE (5.0x)
- CONFIG_PATH ("config/stickers.json")
- ASSETS_DIR ("assets")
- THUMBNAIL_SIZE (124px)

**Why centralized?**
- Single source of truth
- Easy to adjust scaling globally
- Both apps use identical values
- Prevents definition drift

---

## Data Structures

### ThumbnailData (Editor Runtime)

```c
typedef struct ThumbnailData {
    char filepath[512];           // Path to asset
    gboolean selected;            // Is selected?
    int x;                        // Current x position
    int y;                        // Current y position
    double scale;                 // Current scale (1.0 = BASE_SIZE)
    EditorContext *context;       // Parent context
    GtkWidget *chip;              // Selected chip widget (or NULL)
    GtkWidget *widget;            // Thumbnail display widget
    GtkWidget *picture;           // Runtime sticker image
    GtkWidget *sticker_window;    // Runtime overlay window (or NULL)
} ThumbnailData;
```

**Lifecycle:**
- Created: Gallery loads assets
- Selected: Runtime window spawned, chip created
- Dragged: x/y updated, window margins updated
- Resized: scale updated, window sized
- Deselected: Runtime window destroyed, chip removed
- Saved: Persisted to JSON

### StickerConfig (Renderer Static)

```c
typedef struct {
    char path[512];     // Asset path
    int x;              // Position
    int y;              // Position
    double scale;       // Size multiplier
} StickerConfig;
```

**Lifecycle:**
- Created: Loaded from JSON
- Displayed: Window created at position/scale
- Destroyed: App closes

### EditorContext (Global State)

```c
typedef struct {
    GtkWidget *selected_bar;    // Chip container
    GPtrArray *thumbnails;      // All thumbnails
} EditorContext;
```

---

## Layer-Shell Integration

### Window Properties

All sticker windows share these properties:

```c
gtk_layer_init_for_window(window);                    // Enable layer-shell
gtk_layer_set_layer(window, BACKGROUND);              // Render behind windows
gtk_layer_set_anchor(window, TOP, TRUE);              // Anchor to top
gtk_layer_set_anchor(window, LEFT, TRUE);             // Anchor to left
gtk_layer_set_margin(window, TOP, y);                 // Position from top
gtk_layer_set_margin(window, LEFT, x);                // Position from left
gtk_window_set_default_size(window, size, size);      // Window size
gtk_widget_set_size_request(picture, size, size);     // Picture size
```

### Why BACKGROUND Layer?

- **BACKGROUND**: Behind all windows (desktop wallpaper layer)
- **TOP**: Above all windows (unusable - blocks interaction)
- **OVERLAY**: Floats above (not what we want)

---

## Scroll Fix


### Current Fix
```c
scroll_delta = dy < 0 ? 1.0 : -1.0;
scale += SCALE_STEP * scroll_delta;

// dy < 0 (UP) → scroll_delta = 1.0 → scale += 0.1 ✓
// dy > 0 (DOWN) → scroll_delta = -1.0 → scale -= 0.1 ✓
```

### Why Normalize?

GTK4 sends continuous scroll values (-10 to +10), not discrete ±1.
Without normalization, aggressive scrolls would accumulate:
- 5 rapid scroll events × 0.1 = 0.5 scale change (extreme)
- Normalized: 5 events × (single ±1.0) × 0.1 = 0.1 each

---

## Position/Scale Synchronization

### Editor Preview
```c
// Create runtime window
size = BASE_STICKER_SIZE * scale;           // 256 * 1.0 = 256px
gtk_widget_set_size_request(picture, size, size);
gtk_window_set_default_size(window, size, size);

// On scroll
scale = clamp(scale + delta * SCALE_STEP, MIN, MAX);
update_sticker_scale() → recalc and resize

// On drag
gtk_layer_set_margin(window, LEFT, x);
gtk_layer_set_margin(window, TOP, y);

// On save
JSON write: {path, x, y, scale}
```

### Renderer Display
```c
// Load config
StickerConfig config = load from JSON;

// Create window at saved state
size = BASE_STICKER_SIZE * config.scale;
gtk_widget_set_size_request(picture, size, size);
gtk_window_set_default_size(window, size, size);
gtk_layer_set_margin(window, LEFT, config.x);
gtk_layer_set_margin(window, TOP, config.y);
gtk_window_present(window);
```

**Result:** Identical rendering (pixel-perfect match)

---

## File Organization

### Separation of Concerns

**Editor App:**
- `src/editor/main.c` - Startup
- `src/editor/editor.c` - Main UI coordination
- `src/editor/gallery.c` - Asset discovery
- `src/editor/thumbnail.c` - Gallery items
- `src/editor/chip.c` - Selected UI
- `src/editor/renderer.c` - Runtime windows
- `src/editor/drag.c` - Movement interaction
- `src/editor/resize.c` - Scale interaction
- `src/editor/style.c` - CSS styling

**Renderer App:**
- `src/renderer/main.c` - Startup
- `src/renderer/window.c` - Window creation

**Shared:**
- `src/common/config.c` - JSON I/O
- `src/common/sticker.h` - Constants

### Why This Structure?

- **Editor**: Complex, interactive, user-facing
- **Renderer**: Simple, passive, data-driven
- **Common**: Shared utilities, zero duplication
- **Each file**: Single responsibility

---

## Compilation & Linking

### Dependencies Resolved by pkg-config
```bash
pkg-config --cflags gtk4 gtk4-layer-shell-0 json-c
# Provides: -I/usr/include/... (compiler flags)

pkg-config --libs gtk4 gtk4-layer-shell-0 json-c
# Provides: -lgtk-4 -lgtk4-layer-shell -ljson-c (linker flags)
```

### Build Process
```bash
EDITOR_SRC = src/editor/*.c src/common/config.c
RENDERER_SRC = src/renderer/*.c src/common/config.c

$(CC) $(EDITOR_SRC) -o stickers-editor $(CFLAGS) $(LIBS)
$(CC) $(RENDERER_SRC) -o stickers-render $(CFLAGS) $(LIBS)
```

---

## Memory Management

- **GLib Memory**: g_malloc/g_free for objects
- **GTK Objects**: gtk_widget_unref for cleanup (automatic in most cases)
- **String Buffers**: Fixed-size arrays (512 bytes for paths)
- **Arrays**: GPtrArray with g_ptr_array_free on cleanup
- **JSON Objects**: json_object_put() releases references

---

## Error Handling

**Current approach:**
- Silent failures on missing assets
- Empty arrays returned for missing JSON
- GTK warnings on invalid widgets

**Better practices:**
- Check return values from file operations
- Log errors to stderr
- Graceful degradation

---

## Performance Characteristics

- **Startup**: O(n) for n assets (gallery scan)
- **Selection**: O(1) for sticker spawn
- **Drag**: O(1) updates (single window margin)
- **Resize**: O(1) updates (single window size)
- **Save**: O(n) for n selected stickers
- **Render**: O(n) window creation for n stickers

---

## Future Improvements

1. **Multiple Desktops**: Support per-desktop configs
2. **Grouping**: Select/move multiple stickers
3. **Layers**: Depth ordering of stickers
4. **Animation**: Entrance/exit animations
5. **Themes**: User customizable CSS
6. **Hotkeys**: Keyboard shortcuts for common actions
7. **Undo/Redo**: Operation history
8. **Export**: Save as image/screenshot

---

See [BUILD.md](BUILD.md) for build instructions and [docs/API.md](docs/API.md) for detailed function documentation.
