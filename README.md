# Stickers - Desktop Overlay Sticker System

A dual-application Linux desktop sticker system built with GTK4 and Wayland layer-shell protocol.

**Status:** ✓ Production ready  
**Platform:** Linux (Wayland)  
**Desktop Tested:** Hyprland

**Not Tested:** Other distros (Ubuntu, Fedora, etc...) and Other WMs (KDE, Sway, i3, etc....)

---

## Features

### Editor App (`stickers-editor`)
- Browse sticker assets from gallery
- Spawn runtime preview stickers on desktop
- Drag stickers to reposition
- Scroll-wheel resize with smooth increments
- Save sticker configuration to JSON
- Real-time visual feedback

### Renderer App (`stickers-render`)
- Load and display saved sticker configuration
- Render stickers as desktop overlays
- No GUI interaction (display-only)
- Persistent sticker positioning
- Perfect replication of editor preview

### Core Features
- Transparent sticker windows
- Hyprland layer-shell integration
- Negative coordinate support (off-screen)
- Scale range: 0.1x to 5.0x
- JSON-based configuration
- Pixel-perfect position sync

### Advantages

* Native Wayland support using GTK4 + gtk4-layer-shell
* Lightweight renderer written in pure C
* Separate editor and renderer architecture for better performance
* Real-time drag and resize sticker editing
* JSON-based portable configuration system
* Desktop-background layer rendering without interfering with normal apps
* No scripting engines, Electron, or heavy runtimes
* Minimal dependencies and low memory usage
* Modern Linux compositor-friendly design (Hyprland/Wayland focused)
* Easier and cleaner workflow compared to Conky, xwinwrap, and similar desktop overlay tools

---

## Quick Start

### Build

```bash
cd <PROJECT_FOLDER>
make
```

Produces:
- `stickers-editor` - Interactive sticker editor
- `stickers-render` - Sticker renderer

### Run

**Editor Mode:**
```bash
./stickers-editor
```

**Display Mode:**
```bash
./stickers-render
```

---

## Usage

### Editor Workflow

1. Launch editor: `./stickers-editor`
2. Click sticker thumbnails to spawn on desktop
3. Drag stickers to reposition
4. Scroll UP to grow sticker
5. Scroll DOWN to shrink sticker
6. Click thumbnail remove button to delete sticker
7. Click SAVE button to persist configuration

### Renderer Workflow

1. Edit stickers using `stickers-editor`
2. Save configuration (creates/updates `config/stickers.json`)
3. Launch renderer: `./stickers-render`
4. Stickers display as desktop overlays
5. No interaction needed (stateless display)

---

## Directory Structure

```
stickers/
├── assets/              # Sticker image files (.png, .jpg, .webp)
├── config/
│   └── stickers.json    # Saved sticker configuration
├── src/
│   ├── common/          # Shared utilities and constants
│   │   ├── config.c     # JSON save/load
│   │   └── sticker.h    # Shared constants
│   ├── editor/          # Editor application
│   │   ├── main.c       # Entry point
│   │   ├── editor.c     # Main window
│   │   ├── gallery.c    # Thumbnail grid
│   │   ├── thumbnail.c  # Individual thumbnail
│   │   ├── chip.c       # Selected sticker UI
│   │   ├── renderer.c   # Runtime sticker window
│   │   ├── drag.c       # Drag interaction
│   │   └── resize.c     # Scroll resize
│   └── renderer/        # Renderer application
│       ├── main.c       # Entry point
│       └── window.c     # Sticker window creation
├── Makefile             # Build configuration
└── README.md            # This file
```

---

## Configuration Format

Sticker configuration is stored in JSON at `config/stickers.json`:

```json
{
  "stickers": [
    {
      "path": "assets/Abby.png",
      "x": 255,
      "y": -55,
      "scale": 2.4
    }
  ]
}
```

### Fields
- **path**: Relative path to sticker image
- **x**: Horizontal position (pixels from left)
- **y**: Vertical position (pixels from top)
- **scale**: Size multiplier (BASE_SIZE * scale)

### Notes
- Negative coordinates are supported (off-screen placement)
- Scale range: 0.1 (minimum) to 5.0 (maximum)
- BASE_SIZE = 256 pixels

---

## Technical Details

### Dependencies

**Runtime:**
- GTK 4
- gtk4-layer-shell (Wayland layer-shell bindings)
- json-c (JSON parsing/serialization)

**Build:**
- GCC compiler
- pkg-config

**Arch Linux:**
```bash
sudo pacman -S gcc gtk4 gtk4-layer-shell json-c pkgconf
```

### Architecture

See [ARCHITECTURE.md](ARCHITECTURE.md) for detailed system design.

Key concepts:
- **Editor Runtime**: Preview windows before saving
- **Renderer Runtime**: Stateless display from saved config
- **Layer-Shell**: Desktop background layer (BACKGROUND, not TOP)
- **Configuration**: Centralized JSON for state persistence

### Constants

All sizing and scaling constants defined in `src/common/sticker.h`:

```c
BASE_STICKER_SIZE = 256px       # Base size when scale = 1.0
SCALE_STEP        = 0.1          # Per-scroll increment
MIN_SCALE          = 0.1x         # Minimum scale
MAX_SCALE          = 5.0x         # Maximum scale
THUMBNAIL_SIZE    = 124px        # Gallery thumbnail
```

---

## Build System

**Makefile Targets:**

```bash
make            # Build both apps
make clean      # Remove binaries
make renderer   # Build renderer only
make editor     # Build editor only
make run-editor # Build and run editor
make run-renderer # Build and run renderer
```

---

## Recent Fixes

### Position/Scale Sync
- Fixed mismatch between editor preview and renderer output
- Both apps now use identical window sizing
- Result: Pixel-perfect replication

### Scroll Behavior
- Fixed inverted scroll direction
- Scroll UP now grows stickers
- Scroll DOWN now shrinks stickers
- Normalized scroll delta prevents extreme jumps

### Consistency
- Unified constants in `src/common/sticker.h`
- Removed duplicate definitions
- Cleaner codebase organization

---

## Coding Style

The project follows strict formatting conventions:

- Verbose vertical formatting (one argument per line)
- Explicit GTK API calls (no method chaining)
- Clear variable names
- Comprehensive comments
- No inline ternary operators (except scroll normalization)

Example:
```c
gtk_window_set_title(
    GTK_WINDOW(window),
    "Sticker Editor"
);
```

See [docs/STYLE.md](docs/STYLE.md) for full guidelines.

---

## Known Behavior

- **Hyprland Blur**: Transparent layer-shell windows interact with Hyprland's blur,
  creating a frosted-glass effect. This is compositor-specific, not a bug.

- **Layer Ordering**: Stickers render in BACKGROUND layer, always behind normal windows

- **Coordinate System**: Origin (0,0) is top-left. Negative values place stickers off-screen.

---

## API Documentation

See [docs/API.md](docs/API.md) for function signatures and detailed documentation.

---

## Troubleshooting

### Build Fails
- Verify dependencies: `pkg-config --cflags gtk4`
- Check Arch packages: `pacman -Q | grep gtk`
- Try clean rebuild: `make clean && make`

### Stickers Don't Appear
- Verify `config/stickers.json` exists
- Check asset paths in JSON are correct
- Ensure images are in `assets/` directory

### Scroll Direction Wrong
- Use scroll normalization (latest version)
- Scroll UP should grow, DOWN should shrink
- If inverted, recompile: `make clean && make`

---

## Contributing

All code follows strict style guidelines (see [docs/STYLE.md](docs/STYLE.md)).

Before submitting:
1. Ensure code builds: `make clean && make`
2. Follow vertical formatting style
3. Add documentation comments
4. Test with both editor and renderer
5. Verify positions/scales sync correctly

---

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## Author

Karthi - GTK/Wayland Sticker System  
Built for Hyprland desktop environment

---

## See Also

- [ARCHITECTURE.md](ARCHITECTURE.md) - System design overview
- [BUILD.md](BUILD.md) - Build instructions
- [docs/API.md](docs/API.md) - API documentation
- [docs/STYLE.md](docs/STYLE.md) - Code style guide
