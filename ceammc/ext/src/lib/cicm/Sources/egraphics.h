/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#ifndef DEF_EGRAPHICS
#define DEF_EGRAPHICS

#include "ecommon.h"

#include <vector>

/*! @addtogroup groupgraphics The Graphics Part
 * @brief The t_elayer part.
 * @details This part refers to the methods and structures used to paint.
 *  @{
 */

/*!
 * \fn          void egraphics_set_line_width(t_elayer *g, float width)
 * \brief       Sets the line width that will be used by the t_elayer.
 * \param g     The t_elayer pointer.
 * \param width The line width in pixel.
 */
void egraphics_set_line_width(t_elayer* g, float width);

/*!
 * \fn          void egraphics_set_line_capstyle(t_elayer *g, t_capstyle style)
 * \brief       Sets the line capstyle that will be used by the t_elayer.
 * \param g     The t_elayer pointer.
 * \param style The t_capstyle value.
 */
void egraphics_set_line_capstyle(t_elayer* g, t_capstyle style);

/*!
 * \fn          void egraphics_set_line_dashstyle(t_elayer *g, t_dashstyle style)
 * \brief       Sets the line dashstyle that will be used by the t_elayer.
 * \param g     The t_elayer pointer.
 * \param style The t_dashstyle value.
 */
void egraphics_set_line_dashstyle(t_elayer* g, t_dashstyle style);

/*!
 * \fn          void egraphics_set_line_smooth(t_elayer *g, t_smooth smooth)
 * \brief       Sets the line smooth that will be used by the t_elayer.
 * \param g     The t_elayer pointer.
 * \param smooth The t_dashstyle value.
 */
void egraphics_set_line_smooth(t_elayer* g, t_smooth smooth);

/*!
 * \fn          void egraphics_set_color_rgba(t_elayer *g, const t_rgba *rgba)
 * \brief       Sets the color in rgba that will be used by the t_elayer.
 * \param g     The t_elayer pointer.
 * \param rgba  The t_rgba color.
 */
void egraphics_set_color_rgba(t_elayer* g, const t_rgba* rgba);

/*!
 * \fn          void egraphics_set_color_rgb(t_elayer *g, const t_rgb *rgb)
 * \brief       Sets the color in rgb that will be used by the t_elayer.
 * \param g     The t_elayer pointer.
 * \param rgb   The t_rgb color.
 */
void egraphics_set_color_rgb(t_elayer* g, const t_rgb* rgb);

/*!
 * \fn          void egraphics_set_color_hsla(t_elayer *g, const t_hsla *hsla)
 * \brief       Sets the color in hsla that will be used by the t_elayer.
 * \param g     The t_elayer pointer.
 * \param hsla  The hsla color.
 */
void egraphics_set_color_hsla(t_elayer* g, const t_hsla* hsla);

/*!
 * \fn          void egraphics_set_color_hsl(t_elayer *g, const t_hsl *hsl)
 * \brief       Sets the color in hsl that will be used by the t_elayer.
 * \param g     The t_elayer pointer.
 * \param hsl   The hsla color.
 */
void egraphics_set_color_hsl(t_elayer* g, const t_hsl* hsl);

/*!
 * \fn          void egraphics_set_color_hex(t_elayer *g, uint32_t c)
 * \brief       Sets the color in hex that will be used by the t_elayer.
 * \param g     The t_elayer pointer.
 * \param hex   The hex color.
 */
void egraphics_set_color_hex(t_elayer* g, uint32_t c);

/*!
 * \fn          void egraphics_matrix_init(t_matrix *x, float xx, float yx, float xy, float yy, float x0, float y0)
 * \brief       Initializes a matrix.
 * \param x     The t_matrix pointer.
 * \param xx    The xx coefficient.
 * \param yx    The yx coefficient.
 * \param xy    The xy coefficient.
 * \param yy    The yy coefficient.
 * \param x0    The abscissa offset.
 * \param y0    The ordiante offset.
 */
void egraphics_matrix_init(t_matrix* x, float xx, float yx, float xy, float yy, float x0, float y0);

/*!
 * \fn          void egraphics_set_matrix(t_elayer *g, const t_matrix *matrix)
 * \brief       Initializes the t_matrix of a t_elayer.
 * \param g     The t_elayer pointer.
 * \param matrix The t_matrix pointer.
 */
void egraphics_set_matrix(t_elayer* g, const t_matrix* matrix);

/*!
 * \fn          void egraphics_rotate(t_elayer *g, float angle)
 * \brief       Adds a rotation to the t_matrix of a t_elayer.
 * \param g     The t_elayer pointer.
 * \param angle  The offset of rotation in radian.
 */
void egraphics_rotate(t_elayer* g, float angle);

/*!
 * \fn          void egraphics_move_to(t_elayer *g, float x, float y)
 * \brief       Starts a new path in a t_elayer.
 * \details     All paths should start will the function.
 * \param g     The t_elayer pointer.
 * \param x     The abscissa of the new path.
 * \param y     The ordiante of the new path.
 */
void egraphics_move_to(t_elayer* g, const float x, const float y);

/**
 * Reserves space for new drawing points
 * Use for performance optimisation
 */
void egraphics_preallocate_points(t_elayer* g, size_t npoints);

/**
 * Reserves space for new drawing objects
 * Use for performance optimisation
 */
void egraphics_preallocate_objects(t_elayer* g, size_t nobj);

/*!
 * \fn          void egraphics_line_to(t_elayer *g, float x, float y)
 * \brief       Adds a line in the current path of a t_elayer.
 * \details     The start point is the previous point in the path.
 * \param g     The t_elayer pointer.
 * \param x     The abscissa of the end point.
 * \param y     The ordiante of the end point.
 */
void egraphics_line_to(t_elayer* g, const float x, const float y);

/*!
 * \fn          void egraphics_curve_to(t_elayer *g, float ctrl1x, float ctrl1y, float ctrl2x, float ctrl2y, float endx, float endy)
 * \brief       Adds a bezier curve in the current path of a t_elayer. 
 * \details     The first point of the curve is the previous point in the path.
 * \param g     The t_elayer pointer.
 * \param ctrl1x     The abscissa of the first control point.
 * \param ctrl1y     The ordiante of the first control point.
 * \param ctrl2x     The abscissa of the second control point.
 * \param ctrl2y     The ordiante of the second control point.
 * \param endx     The abscissa of the end point.
 * \param endy     The ordiante of the end point.
 */
void egraphics_curve_to(t_elayer* g, float ctrl1x, float ctrl1y, float ctrl2x, float ctrl2y, float endx, float endy);

/*!
 * \fn          void egraphics_arc_to(t_elayer *g, float cx, float cy, float extend)
 * \brief       Adds an arc to the current path of a t_elayer.
 * \details     The start point of the arc is the previous point in the path.
 * \param g     The t_elayer pointer.
 * \param cx     The abscissa of the center of the arc.
 * \param cy     The ordiante of the center of the arc.
 * \param extend The extend angle in radian.
 */
void egraphics_arc_to(t_elayer* g, float cx, float cy, float extend);

/*!
 * \fn          void egraphics_arc_oval_to(t_elayer *g, float cx, float cy, float radius, float extend)
 * \brief       Adds an oval arc to the current path of a t_elayer.
 * \details     The start point of the oval arc is the previous point in the path and the first radius of the oval is compute with the center of the oval arc.
 * \param g     The t_elayer pointer.
 * \param cx     The abscissa of the center of the oval arc.
 * \param cy     The ordiante of the center of the oval arc.
 * \param radius The second radius of the oval arc.
 * \param extend The extend angle in radian.
 */
void egraphics_arc_oval_to(t_elayer* g, float cx, float cy, float radius, float extend);

/*!
 * \fn          void egraphics_line(t_elayer *g, float x0, float y0,  float x1, float y1)
 * \brief       Adds a new path at specific point and link it linearly to another.
 * \details     This function just call egraphics_move_to and egraphics_line_to.
 * \param g     The t_elayer pointer.
 * \param x0     The abscissa of the first point.
 * \param y0     The ordiante of the first point.
 * \param x1     The abscissa of the second point.
 * \param y1     The ordiante of the second point.
 */
void egraphics_line(t_elayer* g, const float x0, const float y0, const float x1, const float y1);

/*!
 * \fn          void egraphics_curve(t_elayer *g, float startx, float starty, float ctrl1x, float ctrl1y, float ctrl2x, float ctrl2y, float endx, float endy)
 * \brief       Adds a new path at specific point and continue the path with a curve.
 * \details     This function just call egraphics_move_to and egraphics_curve_to.
 * \param g     The t_elayer pointer.
 * \param startx     The abscissa of the first point.
 * \param starty     The ordiante of the first point.
 * \param ctrl1x     The abscissa of the first control point.
 * \param ctrl1y     The ordiante of the first control point.
 * \param ctrl2x     The abscissa of the second control point.
 * \param ctrl2y     The ordiante of the second control point.
 * \param endx     The abscissa of the end point.
 * \param endy     The ordiante of the end point.
 */
void egraphics_curve(t_elayer* g, float startx, float starty, float ctrl1x, float ctrl1y, float ctrl2x, float ctrl2y, float endx, float endy);

/*!
 * \fn          void egraphics_line_fast(t_elayer *g, float x0, float y0, float x1, float y1)
 * \brief       Adds a new path at specific point and link it linearly to another then draw directly the line.
 * \details     This function just call egraphics_move_to, egraphics_line_to and egraphics_stroke.
 * \param g     The t_elayer pointer.
 * \param x0     The abscissa of the first point.
 * \param y0     The ordiante of the first point.
 * \param x1     The abscissa of the second point.
 * \param y1     The ordiante of the second point.
 */
void egraphics_line_fast(t_elayer* g, float x0, float y0, float x1, float y1);

/*!
 * \fn          void egraphics_close_path(t_elayer *g)
 * \brief       Marks the current path as closed.
 * \param g     The t_elayer pointer.
 */
void egraphics_close_path(t_elayer* g);

/*!
 * \fn          void egraphics_rectangle(t_elayer *g, float x, float y, float width, float height)
 * \brief       Adds a rectangle in the t_elayer.
 * \param g     The t_elayer pointer.
 * \param x     The abscissa of the rectangle.
 * \param y     The ordiante of the rectangle.
 * \param width     The width of the rectangle.
 * \param height    The height of the rectangle.
 */
void egraphics_rectangle(t_elayer* g, float x, float y, float width, float height);

/*!
 * \fn          void egraphics_circle(t_elayer *g, float xc, float yc, float radius)
 * \brief       Adds a circle in the t_elayer.
 * \param g     The t_elayer pointer.
 * \param xc     The abscissa of the center of the circle.
 * \param yc     The ordiante of the center of the circle.
 * \param radius The radius of the circle.
 */
void egraphics_circle(t_elayer* g, float xc, float yc, float radius);

/*!
 * \fn          void egraphics_oval(t_elayer *g, float xc, float yc, float radiusx, float radiusy)
 * \brief       Adds a oval in the t_elayer.
 * \param g     The t_elayer pointer.
 * \param xc     The abscissa of the center of the oval.
 * \param yc     The ordiante of the center of the oval.
 * \param radiusx The horizontal radius of the circle.
 * \param radiusy The vertical radius of the circle.
 */
void egraphics_oval(t_elayer* g, float xc, float yc, float radiusx, float radiusy);

/*!
 * \fn          void egraphics_arc(t_elayer *g, float xc, float yc, float radius, float angle1, float angle2)
 * \brief       Adds an arc in the t_elayer.
 * \param g     The t_elayer pointer.
 * \param xc     The abscissa of the center of the arc.
 * \param yc     The ordiante of the center of the arc.
 * \param radius The radius of the arc.
 * \param angle1 The first angle of the arc in radian.
 * \param angle2 The second angle of the arc in radian.
 */
void egraphics_arc(t_elayer* g, float xc, float yc, float radius, float angle1, float angle2);

/*!
 * \fn          void egraphics_arc_oval(t_elayer *g, float xc, float yc, float radiusx, float radiusy, float angle1, float angle2)
 * \brief       Adds an oval arc in the t_elayer.
 * \param g     The t_elayer pointer.
 * \param xc     The abscissa of the center of the oval arc.
 * \param yc     The ordiante of the center of the oval arc.
 * \param radiusx The horizontal radius of the oval arc.
 * \param radiusy The vertical radius of the oval arc.
 * \param angle1 The first angle of the oval arc in radian.
 * \param angle2 The second angle of the oval arc in radian.
 */
void egraphics_arc_oval(t_elayer* g, float xc, float yc, float radiusx, float radiusy, float angle1, float angle2);

void egraphics_image(t_elayer* g, float xc, float yc, t_eimage* image);
void egraphics_poly(t_elayer* g, const std::vector<t_pt>& points);
void egraphics_raise(t_elayer* over, t_elayer* l);

/*!
 * \fn          t_etext* etext_layout_create(void)
 * \brief       Allocates a new t_etext layout.
 * \return      The new t_etext pointer.
 */
t_etext* etext_layout_create(void);

/*!
 * \fn          void etext_layout_destroy(t_etext* textlayout)
 * \brief       Frees a t_etext layout.
 * \param textlayout The t_etext pointer.
 */
void etext_layout_destroy(t_etext* textlayout);

/*!
 * \fn          void etext_layout_set(t_etext* textlayout, const char* text, t_efont *jfont,  float x, float y, float width,  float height, etextanchor_flags anchor, etextjustify_flags justify, etextwrap_flags wrap)
 * \brief       Initializes a t_etext.
 * \param textlayout The t_etext pointer.
 * \param text The text to draw.
 * \param font The t_efont to use.
 * \param x The absicssa position of the t_etext.
 * \param y The ordinate position of the t_etext.
 * \param width The width of the t_etext.
 * \param height The height of the t_etext.
 * \param anchor The anchor of the text.
 * \param justify The justification of the text.
 * \param wrap if the text should be wrap or not.
 */
void etext_layout_set(t_etext* textlayout, const char* text, t_efont* font, float x, float y, float width, float height, etextanchor_flags anchor, etextjustify_flags justify, etextwrap_flags wrap);

/*!
 * \fn          void etext_layout_settextcolor(t_etext* textlayout, t_rgba* color)
 * \brief       Sets the color of a t_etext layout.
 * \param textlayout The t_etext pointer.
 * \param color The t_rgba color pointer.
 */
void etext_layout_settextcolor(t_etext* textlayout, t_rgba* color);

/*!
 * \fn          t_efont* efont_create(t_symbol* family, t_symbol* slant, t_symbol* weight, float size)
 * \brief       Allocates a new t_efont.
 * \param family The font family.
 * \param slant The font slant.
 * \param weight The font weight.
 * \param size The font size.
 * \return      The new t_efont pointer.
 */
t_efont* efont_create(t_symbol* family, t_symbol* slant, t_symbol* weight, float size);

/*!
 * \fn          void efont_destroy(t_efont* font)
 * \brief       Frees a t_efont.
 * \param font The t_efont pointer.
 */
void efont_destroy(t_efont* font);

/*!
 * \fn          void egraphics_fill(t_elayer *g)
 * \brief       Fills the t_elayer with the current path.
 * \param g     The t_elayer pointer.
 */
void egraphics_fill(t_elayer* g);

/*!
 * \fn          void egraphics_fill_preserve(t_elayer *g)
 * \brief       Fills the t_elayer with the current path and preserves the current path.
 * \param g     The t_elayer pointer.
 */
void egraphics_fill_preserve(t_elayer* g);

/*!
 * \fn          void egraphics_stroke(t_elayer *g)
 * \brief       Strokes the t_elayer with the current path.
 * \param g     The t_elayer pointer.
 */
void egraphics_stroke(t_elayer* g);

/*!
 * \fn          void egraphics_stroke_preserve(t_elayer *g)
 * \brief       Strokes the t_elayer with the current path and preserves the current path.
 * \param g     The t_elayer pointer.
 */
void egraphics_stroke_preserve(t_elayer* g);

/*!
 * \fn          void etext_layout_draw(t_etext* textlayout, t_elayer *g)
 * \brief       Draws the t_etext in a t_elayer.
 * \param textlayout The t_etext pointer.
 * \param g     The t_elayer pointer.
 */
void etext_layout_draw(t_etext* textlayout, t_elayer* g);

/*!
 * \fn          void rgba_set(t_rgba *color, float red, float green, float blue, float alpha)
 * \brief       Intialize a t_rgba color.
 * \param color The t_rgba color.
 * \param red The red component.
 * \param green The green component.
 * \param blue The blue component.
 * \param alpha The alpha component.
 */
void rgba_set(t_rgba* color, float red, float green, float blue, float alpha);

/*!
 * \fn          void rgb_set(t_rgb *color, float red, float green, float blue)
 * \brief       Intialize a t_rgb color.
 * \param color The t_rgb color.
 * \param red The red component.
 * \param green The green component.
 * \param blue The blue component.
 */
void rgb_set(t_rgb* color, float red, float green, float blue);

/*!
 * \fn          void hsla_set(t_hsla *color, float hue, float saturation, float luminosity, float alpha)
 * \brief       Intialize a t_hsla color.
 * \param color The t_hsla color.
 * \param hue The hue component.
 * \param saturation The saturation component.
 * \param lightness The luminosity component.
 * \param alpha The alpha component.
 */
void hsla_set(t_hsla* color, float hue, float saturation, float lightness, float alpha);

/*!
 * \fn          void hsl_set(t_hsl *color, float hue, float saturation, float luminosity)
 * \brief       Intialize a t_hsl color.
 * \param color The t_hsl color.
 * \param hue The hue component.
 * \param saturation The saturation component.
 * \param lightness The luminosity component.
 */
void hsl_set(t_hsl* color, float hue, float saturation, float lightness);

/*!
 * \fn          char* rgba_to_hex(t_rgba color)
 * \brief       Converts a t_rgba color to an hexadecimal color.
 * \param color The t_rgba color.
 * \return The hexadecimal color.
 */
char* rgba_to_hex(t_rgba color);

/*!
 * \fn          char* rgba_to_hex(t_rgba color)
 * \brief       Converts a t_rgba color to an hexadecimal color.
 * \param color The t_rgba color.
 * \return The hexadecimal color.
 */
int rgba_to_hex_int(const t_rgba& color);

/*!
 * \fn          char* rgba_to_hex(t_rgba color)
 * \brief       Converts a t_rgba color to an hexadecimal color.
 * \param color The t_rgba color.
 * \return The hexadecimal color.
 */
int rgb_to_hex_int(const t_rgb& color);

/*!
 * \fn          t_hsla rgba_to_hsla(t_rgba color)
 * \brief       Converts a t_rgba color to a t_hsla color.
 * \param color The t_rgba color.
 * \return The t_hsla color.
 */
t_hsla rgba_to_hsla(t_rgba color);

/*!
 * \fn          t_hsl rgb_to_hsl(t_rgb color)
 * \brief       Converts a t_rgb color to a t_hsl color.
 * \param color The t_rgb color.
 * \return The t_hsl color.
 */
t_hsl rgb_to_hsl(t_rgb color);

/*!
 * \fn          t_rgba hex_to_rgba(char* color)
 * \brief       Converts an hexadecimal color to a t_rgba color.
 * \param color The hexadecimal color.
 * \return The t_rgba color.
 */
t_rgba hex_to_rgba(const char* color);

/*!
 * \fn          t_rgb hex_to_rgb(char* color)
 * \brief       Converts an hexadecimal color to a t_rgb color.
 * \param color The hexadecimal color.
 * \return The t_rgb color.
 */
t_rgb hex_to_rgb(const char* color);

/*!
 * \fn          t_rgba hsla_to_rgba(t_hsla color)
 * \brief       Converts a t_hsla color to a t_rgba color.
 * \param color The t_hsla color.
 * \return The t_rgba color.
 */
t_rgba hsla_to_rgba(t_hsla color);

/*!
 * \fn          t_rgb hsl_to_rgb(t_hsl color)
 * \brief       Converts a t_hsl color to a t_rgb color.
 * \param color The t_hsl color.
 * \return The t_rgb color.
 */
t_rgb hsl_to_rgb(t_hsl color);

/*!
 * \fn          t_rgba rgba_addContrast(t_rgba color, float contrast)
 * \brief       Adds some constrast to a t_rgba color.
 * \details     Adds the constrast value to all the component of the color.
 * \param color The t_rgba color.
 * \param contrast The contrast value.
 * \return The new t_rgba color.
 */
t_rgba rgba_addContrast(t_rgba color, float contrast);

/*!
 * \fn          t_rgb rgb_addContrast(t_rgb color, float contrast)
 * \brief       Adds some constrast to a t_rgb color.
 * \details     Adds the constrast value to all the component of the color.
 * \param color The t_rgb color.
 * \param contrast The contrast value.
 * \return The new t_rgb color.
 */
t_rgb rgb_addContrast(t_rgb color, float contrast);

t_rgba rgba_color_sum(const t_rgba* c1, const t_rgba* c2, float alpha);

t_eimage* eimage_create(const char* data, int width, int height, etextanchor_flags anchor);
void eimage_destroy(t_eimage* img);
void eimage_copy(t_eimage* dest, const t_eimage* img1);
void eimage_set_base64_data(t_eimage* img, const char* base64);

//! @cond
t_float pd_clip_min(t_float aValue, t_float aMinimum);
t_float pd_clip_max(t_float aValue, t_float aMaximum);
t_float pd_clip_minmax(t_float aValue, t_float aMinimum, t_float aMaximum);
t_float pd_ordinate(t_float radius, t_float angle);
t_float pd_abscissa(t_float radius, t_float angle);
t_float pd_radius(t_float x, t_float y);
t_float pd_angle(t_float x, t_float y);
//! @endcond

/** @} */

#endif
