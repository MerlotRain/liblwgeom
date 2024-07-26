#ifndef __MATHSE_CANVAS_H__
#define __MATHSE_CANVAS_H__



typedef unsigned int RGB;
const RGB            RGB_MASK = 0x00ffffff;
/* Get the red component of a color */
#define RED(rgb) ((rgb >> 16) & 0xff)
/* Get the green component of a color */
#define GREEN(rgb) ((rgb >> 8) & 0xff)
/* Get the blue component of a color */
#define BLUE(rgb) (rgb & 0xff)
/* Get the alpha component of a color */
#define ALPHA(rgb) (rgb >> 24)
/* Create color with red, green, and blue components */
#define INIT_RGB(r, g, b)                                                      \
    ((0xffu << 24) | ((r & 0xffu) << 16) | ((g & 0xffu) << 8) | (b & 0xffu))
/* Create color with red, green, blue, and alpha components */
#define INIT_RGBA(r, g, b, a)                                                  \
    (((a & 0xffu) << 24) | ((r & 0xffu) << 16) | ((g & 0xffu) << 8) |          \
     (b & 0xffu))

/* https://www.w3schools.com/colors/ A color selection list suitable for HTML.
 * The color value is in standard ARGB format. The specified component of the
 * color can be obtained through RED, GREEN, BLUE, and ALPHA macros. */
typedef enum {
    AliceBlue            = 0xFFF0F8FF,
    AntiqueWhite         = 0xFFFAEBD7,
    Aqua                 = 0xFF00FFFF,
    Aquamarine           = 0xFF7FFFD4,
    Azure                = 0xFFF0FFFF,
    Beige                = 0xFFF5F5DC,
    Bisque               = 0xFFFFE4C4,
    Black                = 0xFF000000,
    BlanchedAlmond       = 0xFFFFEBCD,
    Blue                 = 0xFF0000FF,
    BlueViolet           = 0xFF8A2BE2,
    Brown                = 0xFFA52A2A,
    BurlyWood            = 0xFFDEB887,
    CadetBlue            = 0xFF5F9EA0,
    Chartreuse           = 0xFF7FFF00,
    Chocolate            = 0xFFD2691E,
    Coral                = 0xFFFF7F50,
    CornflowerBlue       = 0xFF6495ED,
    Cornsilk             = 0xFFFFF8DC,
    Crimson              = 0xFFDC143C,
    Cyan                 = 0xFF00FFFF,
    DarkBlue             = 0xFF00008B,
    DarkCyan             = 0xFF008B8B,
    DarkGoldenrod        = 0xFFB8860B,
    DarkGray             = 0xFFA9A9A9,
    DarkGreen            = 0xFF006400,
    DarkKhaki            = 0xFFBDB76B,
    DarkMagenta          = 0xFF8B008B,
    DarkOliveGreen       = 0xFF556B2F,
    DarkOrange           = 0xFFFF8C00,
    DarkOrchid           = 0xFF9932CC,
    DarkRed              = 0xFF8B0000,
    DarkSalmon           = 0xFFE9967A,
    DarkSeaGreen         = 0xFF8FBC8B,
    DarkSlateBlue        = 0xFF483D8B,
    DarkSlateGray        = 0xFF2F4F4F,
    DarkTurquoise        = 0xFF00CED1,
    DarkViolet           = 0xFF9400D3,
    DeepPink             = 0xFFFF1493,
    DeepSkyBlue          = 0xFF00BFFF,
    DimGray              = 0xFF696969,
    DodgerBlue           = 0xFF1E90FF,
    Feldspar             = 0xFFD19275,
    Firebrick            = 0xFFB22222,
    FloralWhite          = 0xFFFFFAF0,
    ForestGreen          = 0xFF228B22,
    Fuchsia              = 0xFFFF00FF,
    Gainsboro            = 0xFFDCDCDC,
    GhostWhite           = 0xFFF8F8FF,
    Gold                 = 0xFFFFD700,
    Goldenrod            = 0xFFDAA520,
    Gray                 = 0xFF808080,
    Green                = 0xFF00FF00,
    GreenYellow          = 0xFFADFF2F,
    Honeydew             = 0xFFF0FFF0,
    HotPink              = 0xFFFF69B4,
    IndianRed            = 0xFFCD5C5C,
    Indigo               = 0xFF4B0082,
    Ivory                = 0xFFFFFFF0,
    Khaki                = 0xFFF0E68C,
    Lavender             = 0xFFE6E6FA,
    LavenderBlush        = 0xFFFFF0F5,
    LawnGreen            = 0xFF7CFC00,
    LemonChiffon         = 0xFFFFFACD,
    LightBlue            = 0xFFADD8E6,
    LightCoral           = 0xFFF08080,
    LightCyan            = 0xFFE0FFFF,
    LightGoldenrodYellow = 0xFFFAFAD2,
    LightGray            = 0xFFD3D3D3,
    LightGrey            = 0xFFD3D3D3,
    LightGreen           = 0xFF90EE90,
    LightPink            = 0xFFFFB6C1,
    LightSalmon          = 0xFFFFA07A,
    LightSeaGreen        = 0xFF20B2AA,
    LightSkyBlue         = 0xFF87CEFA,
    LightSlateBlue       = 0xFF8470FF,
    LightSlateGray       = 0xFF778899,
    LightSteelBlue       = 0xFFB0C4DE,
    LightYellow          = 0xFFFFFFE0,
    Lime                 = 0xFF00FF00,
    LimeGreen            = 0xFF32CD32,
    Linen                = 0xFFFAF0E6,
    Magenta              = 0xFFFF00FF,
    Maroon               = 0xFF800000,
    MediumAquamarine     = 0xFF66CDAA,
    MediumBlue           = 0xFF0000CD,
    MediumOrchid         = 0xFFBA55D3,
    MediumPurple         = 0xFF9370DB,
    MediumSeaGreen       = 0xFF3CB371,
    MediumSlateBlue      = 0xFF7B68EE,
    MediumSpringGreen    = 0xFF00FA9A,
    MediumTurquoise      = 0xFF48D1CC,
    MediumVioletRed      = 0xFFC71585,
    MidnightBlue         = 0xFF191970,
    MintCream            = 0xFFF5FFFA,
    MistyRose            = 0xFFFFE4E1,
    Moccasin             = 0xFFFFE4B5,
    NavajoWhite          = 0xFFFFDEAD,
    Navy                 = 0xFF000080,
    OldLace              = 0xFFFDF5E6,
    Olive                = 0xFF808000,
    OliveDrab            = 0xFF6B8E23,
    Orange               = 0xFFFFA500,
    OrangeRed            = 0xFFFF4500,
    Orchid               = 0xFFDA70D6,
    PaleGoldenrod        = 0xFFEEE8AA,
    PaleGreen            = 0xFF98FB98,
    PaleTurquoise        = 0xFFAFEEEE,
    PaleVioletRed        = 0xFFDB7093,
    PapayaWhip           = 0xFFFFEFD5,
    PeachPuff            = 0xFFFFDAB9,
    Peru                 = 0xFFCD853F,
    Pink                 = 0xFFFFC0CB,
    Plum                 = 0xFFDDA0DD,
    PowderBlue           = 0xFFB0E0E6,
    Purple               = 0xFF800080,
    Red                  = 0xFFFF0000,
    RosyBrown            = 0xFFBC8F8F,
    RoyalBlue            = 0xFF4169E1,
    SaddleBrown          = 0xFF8B4513,
    Salmon               = 0xFFFA8072,
    SandyBrown           = 0xFFF4A460,
    SeaGreen             = 0xFF2E8B57,
    SeaShell             = 0xFFFFF5EE,
    Sienna               = 0xFFA0522D,
    Silver               = 0xFFC0C0C0,
    SkyBlue              = 0xFF87CEEB,
    SlateBlue            = 0xFF6A5ACD,
    SlateGray            = 0xFF708090,
    Snow                 = 0xFFFFFAFA,
    SpringGreen          = 0xFF00FF7F,
    SteelBlue            = 0xFF4682B4,
    Tan                  = 0xFFD2B48C,
    Teal                 = 0xFF008080,
    Thistle              = 0xFFD8BFD8,
    Tomato               = 0xFFFF6347,
    Transparent          = 0x00FFFFFF,
    Turquoise            = 0xFF40E0D0,
    Violet               = 0xFFEE82EE,
    VioletRed            = 0xFFD02090,
    Wheat                = 0xFFF5DEB3,
    White                = 0xFFFFFFFF,
    WhiteSmoke           = 0xFFF5F5F5,
    Yellow               = 0xFFFFFF00,
    YellowGreen          = 0xFF9ACD32,
} COLOR;

/**
 * context_format_t:
 * @FORMAT_INVALID: no such format exist or is supported.
 * @FORMAT_ARGB32: each pixel is a 32-bit quantity, with alpha in the upper 8
 *    bits, red in the next 8 bits, then green, then blue. The 32-bits
 *    quantities are stored native-endian. Pre-multiplied alpha is used.
 * @FORMAT_RGB24: RGB24 each pixel is a 32-bit quantity, with the upper 8 bits
 *    unused. Red, Green, and Blue are stored in the remaining 24 bits int the
 *    order.
 * @FORMAT_A8: A8 each pixel is an 8-bit quantity holding an alpha value.
 *
 */
typedef enum {
    FORMAT_INVALID = -1,
    FORMAT_ARGB32  = 0,
    FORMAT_RGB32   = 1,
    FORMAT_A8      = 2
} ContextFormat;

typedef enum {
    /* A plain line. */
    PEN_STYLE_SOLID_LINE,
    /* Dashes separated by a few pixels. */
    PEN_STYLE_DASH_LINE,
    /* Dots separated by a few pixels. */
    PEN_STYLE_DOT_LINE,
    /* Alternate dots and dashes. */
    PEN_STYLE_DASH_DOT_LINE,
    /* One dash, two dots, one dash, two dots. */
    PEN_STYLE_DASH_DOT_DOT_LINE
} PenStyle;

typedef enum {
    /* a square line end that does not cover the end point of the line. */
    PEN_CAP_FLAT_STYLE,
    /* a square line end that covers the end point and extends beyond it by half
     * the line width. */
    PEN_CAP_ROUND_STYLE,
    /* a rounded line end. */
    PEN_CAP_SQUARE_STYLE
} PenCapStyle;

typedef enum {
    /* A triangular line join. */
    PEN_JOIN_MITER_STYLE,
    /* A triangular line join. */
    PEN_JOIN_BEVEL_STYLE,
    /* A circular line join. */
    PEN_JOIN_ROUND_STYLE
} PenJoinStyle;

typedef enum {
    /* A plain line. */
    BRUSH_SOLID_STYLE,
    /* Horizontal lines. */
    BRUSH_HORIZONTAL_STYLE,
    /* Vertical lines. */
    BRUSH_VERTICAL_STYLE,
    /* Backward diagonal lines. */
    BRUSH_FDIAG_STYLE,
    /* Forward diagonal lines.*/
    BRUSH_BDIAG_STYLE,
    /* Crossing horizontal and vertical lines. */
    BRUSH_CROSS_STYLE,
    /* Crossing diagonal lines. */
    BRUSH_DIAGCROSS_STYLE
} BrushStyle;

void rgb2hsv(RGB rgb, double *h, double *s, double *v);
void hsv2rgb(double h, double s, double v, RGB *rgb);
RGB  random_color();

/* Map drawing context, which is a memory block in png format and supports
 * multi-threaded drawing.*/
typedef struct se_graphics_context graphics_context_t;

/* Graphics display transform structure */
typedef struct se_graphics_display_transform {
    double reference_scale;  /* reference scale */
    double map_extent[4];    /* map extent */
    double device_extent[2]; /* device extent. device is a size struct, only
                                need to set width and height */
    double resolution; /* resolution, The geographic length represented by each
                          pixel*/
    double scale;      /* map scale */
    double rotate_angle; /* map rotate angle */
    float  dpi;          /* device dpi, default value is 96.*/
} graphics_display_transform_t;

/* Point symbol structure */
typedef struct se_point_symbol {
    RGB   color; /* point color */
    float size;  /* Point size. When the symbol is a circular point, it
                    represents  the diameter of the circle; when it is a square
                    point, it  represents the side length of the square. */
    int style;   /* point style, circle or square */

} point_symbol_t;

/* Line symbol structure */
typedef struct se_line_symbol {
    RGB    color;       /* line color */
    float  thickness;   /* line thickness */
    int    pen_cap;     /* pen cap style */
    int    pen_join;    /* pen join style */
    int    pen_style;   /* pen style */
    double miter_limit; /* The miter limit of the pen. The miter limit is only
                           relevant when the join style is miter. */
} line_symbol_t;

/* Fill symbol structure */
typedef struct se_fill_symbol {
    RGB           fill_color;  /* fill color */
    line_symbol_t out_line;    /* fill out line symbol */
    int           brush_style; /* brush style */
} fill_symbol_t;

/* Create graphics context
 * The width and height of the context are in pixels.
 * flags: 0: single thread drawing(default)
 *        1: Multithreaded drawing */
graphics_context_t *create_graphics_context(const int     width,
                                            const int     height,
                                            ContextFormat fmt,
                                            int           flags);
void                destroy_graphics_context(graphics_context_t *context);

#endif//__MATHSE_CANVAS_H__