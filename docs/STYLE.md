# Code Style Guide

## Overview

This project enforces a strict, verbose coding style prioritizing clarity and maintainability over conciseness.

---

## Core Principles

1. **Vertical Formatting**: One statement per line, one parameter per line
2. **Explicit Intent**: No clever tricks or compressed expressions
3. **Readability First**: Future developers should understand code immediately
4. **Consistency**: All files follow identical patterns
5. **Comments**: Document why, not what (code shows what)

---

## Naming Conventions

### Variables
- **snake_case** for all variable names
- **Descriptive names** (no single letters except loop counters)

```c
// Good
int sticker_window_size;
gboolean is_selected;
double current_scale;

// Bad
int sz;
gboolean sel;
double s;
```

### Functions
- **snake_case** for all functions
- **Verb prefix** indicating action

```c
// Good
void create_sticker_window();
gboolean on_scroll();
void update_window_position();
void setup_sticker_drag();

// Bad
void StickerWindow();
gboolean scroll_handler();
void pos_update();
```

### Constants
- **SCREAMING_SNAKE_CASE** for all #define
- **Grouped by purpose** in header files

```c
#define BASE_STICKER_SIZE 256
#define SCALE_STEP 0.1
#define MIN_SCALE 0.1
#define MAX_SCALE 5.0
```

### File Names
- **snake_case.c** and **snake_case.h**
- **One module per file** (thumbnail.c, drag.c, etc.)

```
thumbnail.c     // Good
Thumbnail.c     // Bad
thumbnail_view.c // Good (compound)
```

---

## Formatting Rules

### Include Order

1. Standard library headers
2. GTK/system headers
3. Project headers
4. Blank line between groups

```c
#include <stdio.h>
#include <string.h>

#include <gtk/gtk.h>
#include <json-c/json.h>

#include "thumbnail.h"
#include "renderer.h"
#include "../common/sticker.h"
```

### Function Parameters

**One parameter per line, closing paren on own line:**

```c
// Good
void gtk_window_set_title(
    GTK_WINDOW(window),
    "Sticker Editor"
);

// Bad
void gtk_window_set_title(GTK_WINDOW(window), "Sticker Editor");

// Also bad (parameters on same line)
void gtk_window_set_title(
    GTK_WINDOW(window), "Sticker Editor"
);
```

### Function Arguments

```c
// Good
gtk_widget_set_size_request(
    picture,
    size,
    size
);

// Bad
gtk_widget_set_size_request(picture, size, size);
gtk_widget_set_size_request(
    picture, size, size
);
```

### Conditional Statements

**If condition takes multiple lines, wrap with opening brace:**

```c
// Good
if (
    thumbnail_data->scale < MIN_SCALE ||
    thumbnail_data->scale > MAX_SCALE
)
{
    return;
}

// Bad
if (thumbnail_data->scale < MIN_SCALE || thumbnail_data->scale > MAX_SCALE)
    return;
```

### Loops

```c
// Good
for (
    guint i = 0;
    i < thumbnails->len;
    i++
)
{
    ThumbnailData *thumbnail = g_ptr_array_index(thumbnails, i);
}

// Bad
for (guint i = 0; i < thumbnails->len; i++) {
    ThumbnailData *thumbnail = g_ptr_array_index(thumbnails, i);
}
```

### Variable Declaration

**One declaration per line:**

```c
// Good
int width;
int height;
gboolean is_valid;

// Bad
int width, height;
gboolean is_valid;
```

### Assignment

**One assignment per line, aligned with operation:**

```c
// Good
size = BASE_STICKER_SIZE * thumbnail_data->scale;

thumbnail_data->x = new_x;
thumbnail_data->y = new_y;

// Bad
size = BASE_STICKER_SIZE * thumbnail_data->scale; thumbnail_data->x = new_x;
```

### Comments

**Document why, not what:**

```c
// Good
/*
 * Normalize scroll delta to prevent
 * extreme changes on first scroll
 */
scroll_delta = dy < 0 ? 1.0 : -1.0;

// Also good
// Only applies ONE SCALE_STEP per gesture
thumbnail_data->scale += SCALE_STEP * scroll_delta;

// Bad
// Set scroll_delta
scroll_delta = dy < 0 ? 1.0 : -1.0;

// Bad (obvious)
// Add to scale
thumbnail_data->scale += SCALE_STEP * scroll_delta;
```

### Multi-line Comments

```c
// Good
/*
 * Create runtime sticker window
 *
 * Sets up layer-shell properties and attaches
 * drag/resize handlers. Window is initially
 * positioned at thumbnail_data->x, thumbnail_data->y
 * with scale from thumbnail_data->scale.
 */

// Bad
// Create runtime sticker window
// Sets up layer-shell properties and attaches
// drag/resize handlers. Window is initially
// positioned at thumbnail_data->x, thumbnail_data->y
// with scale from thumbnail_data->scale.
```

---

## Ternary Operators

**Avoid unless simple and necessary:**

```c
// Acceptable (scroll direction normalization)
scroll_delta = dy < 0 ? 1.0 : -1.0;

// Bad (too complex)
size = (thumbnail_data->selected) ? (BASE_STICKER_SIZE * thumbnail_data->scale) : 0;

// Better
if (thumbnail_data->selected)
{
    size = BASE_STICKER_SIZE * thumbnail_data->scale;
}
else
{
    size = 0;
}
```

---

## Pointer Declarations

**Asterisk with variable, not type:**

```c
// Good
GtkWidget *window;
gboolean *flag;

// Bad
GtkWidget* window;
gboolean* flag;
```

---

## Type Casting

**Explicit, no shortcuts:**

```c
// Good
gtk_window_set_default_size(
    GTK_WINDOW(window),
    size,
    size
);

// Bad
gtk_window_set_default_size((GtkWindow*)window, size, size);
```

---

## Control Flow

### If/Else

```c
// Good
if (thumbnail_data->selected)
{
    create_runtime_sticker_window(thumbnail_data);
}
else
{
    destroy_runtime_sticker_window(thumbnail_data);
}

// Bad (no braces)
if (thumbnail_data->selected)
    create_runtime_sticker_window(thumbnail_data);
else
    destroy_runtime_sticker_window(thumbnail_data);

// Bad (compressed)
if (thumbnail_data->selected) { create_runtime_sticker_window(thumbnail_data); }
```

### Return Early

```c
// Good
if (!thumbnail_data || !thumbnail_data->sticker_window)
{
    return;
}

// Continue function...

// Bad (deeply nested)
if (thumbnail_data && thumbnail_data->sticker_window)
{
    // 10 lines of actual work
    // deeply nested
}
```

---

## Struct Initialization

```c
// Good
ThumbnailData *data = g_malloc(sizeof(ThumbnailData));
snprintf(
    data->filepath,
    sizeof(data->filepath),
    "%s",
    filepath
);
data->selected = FALSE;
data->x = 100;
data->y = 100;
data->scale = 1.0;

// Bad
ThumbnailData data = {
    .selected = FALSE,
    .x = 100,
    .y = 100,
    .scale = 1.0
};
```

---

## Error Handling

**Always check return values:**

```c
// Good
GtkWidget *window = gtk_window_new();
if (!window)
{
    return;
}

// Bad (no check)
GtkWidget *window = gtk_window_new();
gtk_window_present(GTK_WINDOW(window));
```

---

## Memory Management

**Explicit allocation and deallocation:**

```c
// Good
ThumbnailData *thumbnail = g_malloc(sizeof(ThumbnailData));
// ... use thumbnail ...
g_free(thumbnail);

// Also good
thumbnail_data->sticker_window = NULL;
gtk_window_destroy(GTK_WINDOW(window));

// Bad (implicit cleanup)
g_ptr_array_free(thumbnails, FALSE);  // Doesn't clean children
```

---

## Macros

**Parenthesize arguments:**

```c
// Good
#define DOUBLE(x) ((x) * 2)

// Bad
#define DOUBLE(x) x * 2  // Fails with DOUBLE(1 + 2)
```

---

## Line Length

**No hard limit, but keep reasonable (< 100 chars for comments):**

```c
// Good (~80 chars)
gtk_picture_set_content_fit(
    GTK_PICTURE(picture),
    GTK_CONTENT_FIT_CONTAIN
);

// Also acceptable (longer for necessity)
gtk_css_provider_load_from_string(provider, "window { background-color: transparent; }");
```

---

## File Organization

### Header Files (.h)

```c
#pragma once

/*
 * Module: thumbnail.c
 * Description: Thumbnail display widget for gallery view
 */

#include <gtk/gtk.h>
#include "editor.h"

typedef struct {
    // Type definitions
} ThumbnailData;

// Function declarations
GtkWidget* create_thumbnail(ThumbnailData *thumbnail_data);
```

### Implementation Files (.c)

```c
#include <gtk/gtk.h>

#include "thumbnail.h"
#include "../common/sticker.h"

/*
 * Static helpers (internal only)
 */
static void update_thumbnail_visual(...) { }

/*
 * Public functions (declared in .h)
 */
GtkWidget* create_thumbnail(...) { }
```

---

## Documentation Comments

### Function Documentation

```c
/*
 * create_runtime_sticker_window
 *
 * Creates a GTK window for displaying a sticker on desktop.
 * Window is undecorated, unfocusable, and positioned at
 * thumbnail_data->x, thumbnail_data->y with scale applied.
 *
 * Parameters:
 *   thumbnail_data - Sticker metadata and state
 *
 * Returns:
 *   GtkWidget* - Sticker window (stored in thumbnail_data->sticker_window)
 *
 * Note:
 *   Sets up drag and resize handlers. Window presented immediately.
 */
GtkWidget* create_runtime_sticker_window(
    ThumbnailData *thumbnail_data
)
```

---

## Common Patterns

### Iterate Array

```c
for (
    guint i = 0;
    i < array->len;
    i++
)
{
    ItemType *item = g_ptr_array_index(array, i);
    // Use item
}
```

### Safe Window Destruction

```c
if (
    !thumbnail_data ||
    !thumbnail_data->sticker_window
)
{
    return;
}

GtkWidget *window = thumbnail_data->sticker_window;
thumbnail_data->sticker_window = NULL;

gtk_window_destroy(GTK_WINDOW(window));
```

### Signal Connection

```c
g_signal_connect(
    button,
    "clicked",
    G_CALLBACK(on_button_clicked),
    user_data
);
```

---

## Checklist Before Commit

- [ ] Code builds: `make clean && make`
- [ ] No compiler warnings
- [ ] Variable names are descriptive
- [ ] Functions have documentation comments
- [ ] One parameter per line in function calls
- [ ] Explicit error handling
- [ ] Memory properly allocated/freed
- [ ] No trailing whitespace
- [ ] Consistent indentation (spaces, not tabs)
- [ ] Tested with both editor and renderer

---

## Violations to Watch For

| Anti-Pattern | Why | Fix |
|---|---|---|
| `widget->property` | Not GTK idiomatic | Use `gtk_widget_get_property()` |
| Inline ternary chains | Hard to read | Use if/else |
| Parameter on same line as function | Breaks style | One per line |
| No error checking | Crashes | Check all returns |
| Mixed types in comparison | Type confusion | Explicit casting |
| Single-letter variables (except i, j) | Unclear purpose | Descriptive names |
| Compressed logic | Hard to maintain | Expand across lines |

---

See [../README.md](../README.md) for project overview and [../ARCHITECTURE.md](../ARCHITECTURE.md) for design.
