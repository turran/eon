#ifndef _EON_CANVAS_H_
#define _EON_CANVAS_H_

/**
 * @{
 */

EAPI Enesim_Renderer * eon_canvas_new(void);
EAPI void eon_canvas_width_set(Enesim_Renderer *r, unsigned int width);
EAPI void eon_canvas_height_set(Enesim_Renderer *r, unsigned int height);
EAPI void eon_canvas_child_x_set(Enesim_Renderer *r, Ender_Element *child, double x);
EAPI void eon_canvas_child_y_set(Enesim_Renderer *r, Ender_Element *child, double y);

/**
 * @}
 */

#endif
