# WaySticker - Desktop Overlay Sticker System

<div align="center">

![WaySticker Icon](app_icon/ws.png)

A lightweight, native dual-application Linux desktop sticker system built with GTK4 and Wayland layer-shell protocol.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Platform: Linux Wayland](https://img.shields.io/badge/Platform-Linux%20Wayland-blue.svg)](https://wayland.freedesktop.org/)
[![Language: C](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))

[Installation](#installation) • [Usage](#usage) • [Build](#build--development) • [Advanced Topics](#advanced-topics) • [Contributing](#contributing)

</div>

---

## Screenshots

| Editor Interface | Desktop Overlay |
|---|---|
| ![Editor App](screenshots/app.png) | ![Desktop Stickers](screenshots/screenshot.png) |

---

## Overview

WaySticker provides two complementary applications:

- **stickers-editor** (40 KB): Interactive editor for composing and arranging stickers on your desktop
- **stickers-render** (24 KB): Lightweight renderer that displays saved sticker configurations

The system uses GTK4 and the Wayland layer-shell protocol to render stickers in the BACKGROUND layer, keeping them behind all application windows while maintaining full visual fidelity.

## Key Features

- **Wayland Native**: Full gtk4-layer-shell integration without X11 fallback
- **Minimal Footprint**: Combined binary size of 64 KB with typical memory usage under 30 MB
- **Interactive Editor**: Real-time drag-and-drop positioning with scroll wheel resizing
- **Persistent Configuration**: JSON-based sticker positions and scales
- **Compositor Compatible**: Tested with Hyprland; works with any Wayland compositor supporting layer-shell
- **Negative Coordinates**: Place stickers off-screen for flexible desktop layouts
- **Scale Control**: Adjustable scale range from 0.1x to 5.0x (base size 256 × 256 pixels)

---

## Core Components

### Editor Application (stickers-editor)

The editor provides an interactive interface for creating and arranging desktop stickers:

- **Asset Gallery**: Browse all image files in the `assets/` directory (PNG, JPG, WebP)
- **Real-time Preview**: Click thumbnails to place stickers on the desktop immediately
- **Position Control**: Click and drag stickers to adjust their location
- **Scale Control**: Scroll wheel to resize stickers (0.1x to 5.0x)
- **Visual Selection**: Selected stickers appear in the bottom bar for management
- **Persistence**: Save button writes all sticker positions, sizes, and paths to `config/stickers.json`

### Renderer Application (stickers-render)

The renderer is a minimal display engine that loads and displays saved sticker configurations:

- **Stateless Design**: Reads configuration once and renders without modification
- **Fast Startup**: Loads and displays all stickers in under 100ms
- **Background Layer**: Renders stickers in the Wayland BACKGROUND layer, keeping them behind all windows
- **Read-Only**: No user interaction; purely for display

---

## Installation

### Prerequisites

- Linux Wayland environment (verified on Hyprland; compatible with other Wayland compositors)
- GCC compiler
- Development libraries: GTK4, gtk4-layer-shell, json-c, pkg-config

### Arch Linux

```bash
sudo pacman -S gcc gtk4 gtk4-layer-shell json-c pkgconf make
cd WaySticker
make
```

### Ubuntu/Debian

```bash
sudo apt-get install build-essential libgtk-4-dev libgtk4-layer-shell0 libjson-c-dev pkg-config
cd WaySticker
make
```

### Fedora

```bash
sudo dnf install gcc gtk4-devel gtk4-layer-shell-devel json-c-devel pkgconfig
cd WaySticker
make
```

### Verify Installation

```bash
# Check that binaries were created
ls -lh stickers-editor stickers-render

# Verify Wayland session is active
echo $WAYLAND_DISPLAY  # Should output something like "wayland-0"
```

## Usage

### Editor Workflow

1. **Launch the editor**
   ```bash
   ./stickers-editor
   ```

2. **Browse available stickers**
   - The left panel displays all PNG, JPG, and WebP images from the `assets/` directory
   - Scroll to view all thumbnails

3. **Add a sticker to the desktop**
   - Click any thumbnail to place that sticker on your desktop
   - The sticker window appears immediately for preview
   - The sticker appears in the "Selected" bar at the bottom

4. **Position stickers**
   - Click and drag any sticker window to move it to the desired location
   - Negative coordinates are supported for off-screen placement
   - Position updates appear in real-time

5. **Resize stickers**
   - Hover the mouse over a sticker window
   - Scroll UP to increase size (increments of 0.1x, maximum 5.0x)
   - Scroll DOWN to decrease size (minimum 0.1x)

6. **Remove stickers**
   - Click the remove button on the sticker's chip in the "Selected" bar
   - Or click the thumbnail removal option
   - The sticker is removed from both the desktop preview and selection list

7. **Save your configuration**
   - Click the SAVE button in the editor
   - All sticker positions and scales are written to `config/stickers.json`
   - Configuration is now ready for the renderer to display

### Renderer Workflow

1. **Ensure configuration is saved**
   - Use the editor to arrange and save stickers (see Editor Workflow above)

2. **Launch the renderer**
   ```bash
   ./stickers-render
   ```

3. **View stickers on desktop**
   - All saved stickers appear in the BACKGROUND layer
   - They remain behind all application windows
   - Stickers remain visible while you work on other applications

4. **Close the renderer**
   - Press Ctrl+Q or close the window
   - Stickers disappear from the desktop

### Configuration File Format

Sticker configurations are saved to `config/stickers.json`:

```json
{
  "stickers": [
    {
      "path": "assets/Abby.png",
      "x": 255,
      "y": -55,
      "scale": 2.4
    },
    {
      "path": "assets/Amora.png",
      "x": 100,
      "y": 200,
      "scale": 1.5
    }
  ]
}
```

**Configuration Fields:**

| Field | Type | Purpose | Example |
|-------|------|---------|---------|
| `path` | String | Relative path to image file | `assets/Abby.png` |
| `x` | Integer | Horizontal position in pixels (negative values place stickers off-screen) | `255` |
| `y` | Integer | Vertical position in pixels (negative values place stickers off-screen) | `-55` |
| `scale` | Float | Size multiplier relative to base size (1.0 = 256 × 256 pixels) | `2.4` |

---

## Project Structure

```
WaySticker/
├── README.md                    # Project documentation
├── ARCHITECTURE.md              # Detailed system design and components
├── BUILD.md                     # Build instructions and troubleshooting
├── LICENSE                      # MIT License
├── Makefile                     # Build configuration
│
├── app_icon/
│   └── ws.png                   # Application icon
│
├── screenshots/
│   ├── app.png                  # Editor interface screenshot
│   └── screenshot.png           # Desktop overlay screenshot
│
├── assets/
│   ├── Abby.png                 # Example sticker image
│   ├── Amora.png                # Example sticker image
│   └── [user stickers]          # Add PNG, JPG, or WebP files here
│
├── config/
│   └── stickers.json            # Saved sticker configuration (auto-generated)
│
├── docs/
│   ├── API.md                   # Function reference and API documentation
│   └── STYLE.md                 # Code style guidelines
│
└── src/
    ├── common/
    │   ├── config.c             # JSON serialization utilities
    │   └── sticker.h            # Shared constants and type definitions
    │
    ├── editor/
    │   ├── main.c               # Editor entry point
    │   ├── editor.c             # Main editor window and UI coordination
    │   ├── gallery.c            # Asset gallery and thumbnail display
    │   ├── thumbnail.c          # Individual thumbnail UI component
    │   ├── chip.c               # Selected sticker badge UI
    │   ├── renderer.c           # Runtime sticker window creation
    │   ├── drag.c               # Drag gesture event handling
    │   ├── resize.c             # Scroll wheel resize event handling
    │   └── style.c              # CSS styling definitions
    │
    └── renderer/
        ├── main.c               # Renderer entry point
        └── window.c             # Sticker window creation and configuration
```
> Interested in the internal implementation?
> See [ARCHITECTURE.md](ARCHITECTURE.md) for system design and application structure.

---

## Build & Development

### Build Commands

```bash
make              # Build both editor and renderer
make editor       # Build editor only
make renderer     # Build renderer only
make clean        # Remove binaries
make run-editor   # Build and run editor immediately
make run-renderer # Build and run renderer immediately
```

### Build Process

The build system uses GNU Make with pkg-config for dependency resolution:

```bash
# Compile flags are automatically generated
pkg-config --cflags gtk4 gtk4-layer-shell-0 json-c

# Link flags are automatically generated
pkg-config --libs gtk4 gtk4-layer-shell-0 json-c
```

### Development Workflow

```bash
# After making code changes
make clean && make

# Test the editor
./stickers-editor

# In another terminal, test the renderer
./stickers-render
```

### Performance Characteristics

| Metric | Value |
|--------|-------|
| Editor Binary | 40 KB |
| Renderer Binary | 24 KB |
| Startup Time | <100 ms |
| Editor Memory | 15-20 MB |
| Renderer Memory | 8-12 MB per displayed set |
| Supported Stickers | Unlimited (tested with 50+) |
| Scale Range | 0.1x to 5.0x |
| Base Asset Size | 256 × 256 pixels |

---

## Troubleshooting

### Build Issues

| Problem | Solution |
|---------|----------|
| `pkg-config: command not found` | Install `pkgconf` (Arch) or `pkg-config` (Ubuntu/Fedora) |
| `gtk/gtk.h: No such file` | Install GTK4 development package |
| `gtk4-layer-shell.h: No such file` | Install `gtk4-layer-shell` package |
| `json.h: No such file` | Install `json-c` development package |
| `undefined reference` linker errors | Run `make clean && make` to rebuild all objects |

### Runtime Issues

| Problem | Solution |
|---------|----------|
| Stickers don't appear | Verify `config/stickers.json` exists and contains valid paths |
| Assets not visible in gallery | Ensure images are in `assets/` directory with correct file extensions |
| Wayland session error | Check `echo $WAYLAND_DISPLAY` returns a value; ensure using Wayland session |
| Scroll sensitivity is wrong | Check `src/common/sticker.h` constants; recompile with `make clean && make` |

### Diagnostic Commands

```bash
# Check Wayland is active
echo $XDG_SESSION_TYPE      # Should output "wayland"
echo $WAYLAND_DISPLAY       # Should output something like "wayland-0"

# Verify configuration file format
cat config/stickers.json    # Should contain valid JSON

# Check asset directory
ls -la assets/              # Should show image files

# Verify library installation
pkg-config --libs gtk4      # Should output library flags
pkg-config --cflags gtk4    # Should output compiler flags

# Clean rebuild
make clean && make          # Remove all binaries and rebuild from scratch
```

> For detailed build instructions and dependencies, see [BUILD.md](BUILD.md)

---

## Customization

### Adding Custom Stickers

1. Prepare image files in PNG, JPG, or WebP format
2. Copy them to the `assets/` directory
3. Relaunch the editor; new stickers appear in the gallery
4. Select, arrange, and save as usual

Example:
```bash
cp my_sticker.png assets/
./stickers-editor
```

### Adjusting Configuration Constants

Edit `src/common/sticker.h` to modify system behavior:

```c
#define BASE_STICKER_SIZE 256    // Base size in pixels (default: 256)
#define SCALE_STEP 0.1           // Scale increment per scroll event (default: 0.1)
#define MIN_SCALE 0.1            // Minimum allowed scale (default: 0.1x)
#define MAX_SCALE 5.0            // Maximum allowed scale (default: 5.0x)
#define THUMBNAIL_SIZE 124       // Thumbnail display size (default: 124px)
```

After modifying, rebuild:
```bash
make clean && make
```

### Custom Styling

The editor's visual appearance is defined in `src/editor/style.c` using GTK CSS:

```c
const char *CSS_STRING =
    "window { background-color: #2a2a2a; }"
    ".thumbnail { min-width: 124px; min-height: 124px; }"
    /* Add additional CSS rules here */
```

Modify colors, spacing, or other visual properties and rebuild to apply changes.

---

---

## Advanced Topics

### System Architecture Overview

WaySticker employs a separation-of-concerns design with two specialized applications:

**Editor Application (stickers-editor)**
- Interactive interface for composing and arranging desktop stickers
- Handles user input: clicking, dragging, scrolling
- Displays real-time preview of stickers on desktop during editing
- Maintains selection state of active stickers
- Serializes final configuration to JSON file

**Renderer Application (stickers-render)**
- Minimal, stateless display engine for saved configurations
- Loads sticker data from JSON at startup
- Creates windows at exact saved positions and scales
- No user interaction capability; purely display-focused
- Exits cleanly when the window is closed

**Shared Components**
- `src/common/config.c`: JSON serialization and deserialization utilities
- `src/common/sticker.h`: Centralized constants and type definitions used by both applications

### Wayland Layer-Shell Protocol

Both applications use the gtk4-layer-shell library to integrate with Wayland compositors. Stickers render in the BACKGROUND layer, ensuring they appear behind all regular application windows:

```c
gtk_layer_init_for_window(window);           // Enable layer-shell protocol
gtk_layer_set_layer(window, BACKGROUND);     // Render in background layer
gtk_layer_set_anchor(window, TOP, TRUE);     // Anchor to top-left
gtk_layer_set_anchor(window, LEFT, TRUE);
gtk_layer_set_margin(window, TOP, y);        // Absolute pixel positioning
gtk_layer_set_margin(window, LEFT, x);
```

This approach provides:
- Stickers render behind all application windows
- No interference with panels, docks, or taskbars
- Persistent, wallpaper-like visual appearance
- Seamless integration with any Wayland compositor supporting layer-shell

### Configuration Data Flow

1. **Editor Session**: User selects, arranges, and saves stickers via the editor UI
2. **Serialization**: Editor writes selected stickers' properties to `config/stickers.json`
3. **Renderer Launch**: User starts the renderer application
4. **Deserialization**: Renderer reads JSON configuration into memory
5. **Window Creation**: Renderer creates windows at saved positions and scales
6. **Session End**: Renderer closes; all sticker windows disappear

## Documentation References

| Document | Purpose |
|----------|---------|
| [README.md](README.md) | Overview and quick start (this file) |
| [ARCHITECTURE.md](ARCHITECTURE.md) | Deep dive into system design and components |
| [BUILD.md](BUILD.md) | Installation, compilation, and debugging |
| [docs/API.md](docs/API.md) | Function signatures and detailed API reference |
| [docs/STYLE.md](docs/STYLE.md) | Code style guidelines and conventions |

---

## Contributing

Contributions are welcome. Please follow these guidelines:

1. **Code Style**: Follow [docs/STYLE.md](docs/STYLE.md)
   - Vertical formatting (one argument per line)
   - Descriptive variable names
   - Comprehensive comments

2. **Testing**: Before submitting
   ```bash
   make clean && make
   ./stickers-editor        # Test editor
   ./stickers-render        # Test renderer
   ```

3. **Verification**
   - Positions and scales sync correctly between editor and renderer
   - No memory leaks or segmentation faults
   - Works on Wayland (tested with Hyprland)

4. **Submission**: Create a pull request with a clear description of changes

---

## Known Limitations

| Limitation | Workaround | Future |
|------------|-----------|--------|
| **X11 Not Supported** | Use Wayland only | – |
| **No Animations** | Save intermediate positions | Animation system |
| **No Grouping** | Arrange individually | Multi-select grouping |
| **No Undo/Redo** | Keep backups of JSON | History system |

---

## Comparison with Alternatives

| Feature | WaySticker | Conky | xwinwrap | Electron App |
|---------|-----------|-------|----------|--------------|
| Memory | ~20 MB | ~50 MB | ~30 MB | ~200 MB |
| Binary Size | 40 KB | 500 KB | 100 KB | 100+ MB |
| Wayland Native | Yes | No | No | Yes* |
| Easy Setup | Yes | No | No | Yes |
| Interactive Editor | Yes | No | No | Yes |
| JSON Config | Yes | No | No | No |
| Pure C | Yes | No | Yes | No |
| GTK4 | Yes | No | No | No |

*Most Electron apps require X11 fallback for Wayland compatibility

---

## Tips & Tricks

### Pro Tips

```bash
# Quick edit and test
make run-editor              # One command to build and launch

# Save before testing renderer
# (Editor SAVE button is essential)

# Multiple terminals
# Terminal 1: ./stickers-render
# Terminal 2: ./stickers-editor
# Keep renderer running while editing

# Arrange off-screen
# Use negative X/Y coordinates for stickers hidden off-screen
# Position them as needed without taking up space

# Batch operations
# Edit JSON directly for bulk position changes
# Then reload with renderer
```

---

## Support & Issues

### Getting Help

1. **Check Troubleshooting** (above)
2. **Review Logs**:
   ```bash
   make clean && make 2>&1 | tee build.log
   ./stickers-editor 2>&1 | tee runtime.log
   ```
3. **Check Dependencies**: `pkg-config --cflags gtk4`
4. **Report Issue**: Include build.log, runtime.log, and output of:
   ```bash
   uname -a
   echo $WAYLAND_DISPLAY
   pkg-config --modversion gtk4
   ```

## License

WaySticker is licensed under the MIT License.

Permissions:
- Use commercially
- Modify and redistribute
- Use privately

Requirement:
- Include a copy of the license

See [LICENSE](LICENSE) file for full details.

---

## Author & Credits

**Karthikeyan K** - GTK4/Wayland Desktop Sticker System

**Technology Stack:**
- Language: C (C99)
- GUI Framework: GTK4
- Wayland Protocol: gtk4-layer-shell
- Configuration Format: JSON (json-c library)
- Build System: GNU Make

---

## Project Roadmap

### Current Version (v1.0)
- Editor application with real-time sticker preview
- Renderer application for desktop display
- Drag-and-drop positioning
- Scroll wheel resizing
- JSON configuration persistence
- Hyprland compatibility

### Planned Features (v2.0)
- Multi-monitor support
- Sticker grouping and multi-select
- Keyboard shortcuts for common actions
- Animation effects for entrance/exit
- Export and screenshot functionality
- Customizable themes


---

<div align="center">

## Getting Started

```bash
# Clone the repository
git clone https://github.com/karthi-keyank/WaySticker.git
cd WaySticker

# Build both applications
make

# Launch the editor
./stickers-editor
```

For more details, see [ARCHITECTURE.md](ARCHITECTURE.md) or [BUILD.md](BUILD.md).

For the Wayland desktop community ♥️.

</div>
