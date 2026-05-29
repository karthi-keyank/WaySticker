---

name: Bug report
about: Report crashes, rendering problems, scaling issues, or unexpected behavior
title: "[BUG] "
labels: bug
assignees: ''

---

## Bug Description

Describe the issue clearly and concisely.

Examples:

* Sticker scale not syncing
* Renderer crashes on startup
* Dragging feels jittery
* Stickers appear above windows
* Blur/transparency issues

---

## Steps To Reproduce

Steps to reproduce the behavior:

1. Launch `stickers-editor`
2. Add a sticker
3. Resize or move it
4. Save configuration
5. Launch `stickers-render`
6. Observe the issue

---

## Expected Behavior

Describe what you expected to happen.

Example:

* Sticker should keep correct scale
* Renderer should match editor preview
* Stickers should stay behind normal windows

---

## Actual Behavior

Describe what actually happened.

Example:

* Scale resets to default
* Sticker appears oversized
* Window becomes interactive unexpectedly

---

## Screenshots / Videos

If applicable, add screenshots or recordings.

This is especially useful for:

* Rendering issues
* Scaling bugs
* Layer-shell behavior
* Visual glitches
* Blur/compositor issues

---

## Environment

### System Information

* OS:
* Distribution:
* Kernel Version:
* Desktop Environment / WM:
* Wayland or X11:
* GPU:
* GPU Driver:

Example:

* Arch Linux
* Hyprland
* Wayland
* AMD RX 6600
* Mesa 25.x

---

## Build Information

* GTK4 version:
* gtk4-layer-shell version:
* json-c version:
* GCC version:

Output of:

```bash
gcc --version
pkg-config --modversion gtk4
pkg-config --modversion gtk4-layer-shell-0
pkg-config --modversion json-c
```

---

## Configuration File

If related to rendering or scaling, attach your:

```text
config/stickers.json
```

---

## Logs / Terminal Output

Paste any warnings, GTK criticals, or crashes here.

Example:

```text
Gtk-CRITICAL **: gtk_window_destroy assertion failed
```

---

## Additional Context

Add any additional information that may help reproduce or debug the issue.

Examples:

* Multi-monitor setup
* Fractional scaling enabled
* Hyprland blur enabled
* Running through XWayland
* Large sticker counts
