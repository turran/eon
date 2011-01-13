#ifndef _EON_H
#define _EON_H

#include "Enesim.h"
#include "Ender.h"

EAPI Enesim_Renderer * eon_canvas_new(void);
EAPI void eon_canvas_width_set(Enesim_Renderer *r, unsigned int width);
EAPI void eon_canvas_height_set(Enesim_Renderer *r, unsigned int height);
EAPI void eon_canvas_child_add(Enesim_Renderer *r, Ender *child);
EAPI void eon_canvas_child_x_set(Enesim_Renderer *r, Ender *child, double x);
EAPI void eon_canvas_child_y_set(Enesim_Renderer *r, Ender *child, double y);

#endif
