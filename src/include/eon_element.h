#ifndef _EON_ELEMENT_H_
#define _EON_ELEMENT_H_

/**
 * @{
 */

EAPI Eina_Bool eon_is_element(Enesim_Renderer *r);
EAPI void eon_element_actual_height_get(Ender_Element *e, double *height);
EAPI void eon_element_actual_width_get(Ender_Element *e, double *width);
EAPI void eon_element_height_get(Ender_Element *e, double *height);
EAPI void eon_element_height_set(Ender_Element *e, double height);
EAPI void eon_element_width_get(Ender_Element *e, double *width);
EAPI void eon_element_width_set(Ender_Element *e, double width);
EAPI void eon_element_min_width_get(Ender_Element *e, double *width);
EAPI void eon_element_min_width_set(Ender_Element *e, double width);
EAPI void eon_element_min_height_set(Ender_Element *e, double height);
EAPI void eon_element_min_height_get(Ender_Element *e, double *height);
EAPI void eon_element_max_width_get(Ender_Element *e, double *width);
EAPI void eon_element_max_width_set(Ender_Element *e, double width);
EAPI void eon_element_max_height_set(Ender_Element *e, double height);
EAPI void eon_element_max_height_get(Ender_Element *e, double *height);

/**
 * @}
 */

#endif
