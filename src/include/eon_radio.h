#ifndef _EON_RADIO_H_
#define _EON_RADIO_H_

/**
 * @{
 */

EAPI Enesim_Renderer * eon_radio_new(void);
EAPI void eon_radio_group_name_set(Enesim_Renderer *r, const char *group);
EAPI void eon_radio_group_name_get(Enesim_Renderer *r, const char **group);
EAPI void eon_radio_selected_set(Enesim_Renderer *r, Eina_Bool selected);
EAPI void eon_radio_selected_get(Enesim_Renderer *r, Eina_Bool *selected);

/**
 * @}
 */

#endif
