#pragma once

/*
 * Sticker Constants
 *
 * Shared definitions for sticker sizing, scaling, and positioning
 * Used by both editor runtime preview and renderer output
 */

/*
 * Base sticker size in pixels
 * When scale = 1.0, sticker renders at this size
 */

#define BASE_STICKER_SIZE 256

/*
 * Scale modification step per scroll event
 * Each scroll gesture changes scale by this amount
 */

#define SCALE_STEP 0.1

/*
 * Minimum allowed sticker scale
 * Prevents stickers from becoming too small
 */

#define MIN_SCALE 0.1

/*
 * Maximum allowed sticker scale
 * Prevents stickers from becoming too large
 */

#define MAX_SCALE 5.0

/*
 * JSON configuration file path
 * Relative to project root
 */

#define CONFIG_PATH "config/stickers.json"

/*
 * Assets directory path
 * Contains sticker image files
 */

#define ASSETS_DIR "assets"

/*
 * Thumbnail grid display size in pixels
 * Used in gallery view
 */

#define THUMBNAIL_SIZE 124
