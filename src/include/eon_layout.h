#ifndef _EON_LAYOUT_H_
#define _EON_LAYOUT_H_

/**
 * @{
 */

Eina_Bool eon_layout_is_topmost(Enesim_Renderer *r);
EAPI void eon_layout_child_remove(Enesim_Renderer *r, Ender_Element *child);
EAPI void eon_layout_child_add(Enesim_Renderer *r, Ender_Element *child);
EAPI void eon_layout_redraw_get(Enesim_Renderer *r, Eina_List **redraws);
EAPI Ender_Element * eon_layout_child_get_at_coord(Enesim_Renderer *r, double x, double y);
EAPI Ender_Element * eon_layout_child_get_at_destination_coord(Enesim_Renderer *r, unsigned int x, unsigned int y);

/**
 * @}
 */

#endif
