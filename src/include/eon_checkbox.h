#ifndef _EON_CHECKBOX_H_
#define _EON_CHECKBOX_H_

EAPI Ender_Element * eon_checkbox_new(void);
EAPI void eon_checkbox_selected_set(Ender_Element *e, Eina_Bool selected);
EAPI void eon_checkbox_selected_get(Ender_Element *e, Eina_Bool *selected);

#endif
