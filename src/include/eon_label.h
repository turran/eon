#ifndef _EON_LABEL_H_
#define _EON_LABEL_H_

/**
 * @{
 */

EAPI Ender_Element * eon_label_new(void);
EAPI void eon_label_text_set(Ender_Element *e, const char *text);
EAPI void eon_label_text_get(Ender_Element *e, const char **text);

/**
 * @}
 */

#endif
