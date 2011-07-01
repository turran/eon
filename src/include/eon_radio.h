#ifndef _EON_RADIO_H_
#define _EON_RADIO_H_

/**
 * @{
 */

EAPI Ender_Element * eon_radio_new(void);
EAPI void eon_radio_group_name_set(Ender_Element *e, const char *group);
EAPI void eon_radio_group_name_get(Ender_Element *e, const char **group);
EAPI void eon_radio_selected_set(Ender_Element *e, Eina_Bool selected);
EAPI void eon_radio_selected_get(Ender_Element *e, Eina_Bool *selected);

/**
 * @}
 */

#endif
