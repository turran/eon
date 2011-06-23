#ifndef _EON_LABEL_H_
#define _EON_LABEL_H_

/**
 * @{
 */

EAPI Enesim_Renderer * eon_label_new(void);
EAPI void eon_label_text_set(Enesim_Renderer *r, const char *text);
EAPI void eon_label_text_get(Enesim_Renderer *r, const char **text);

/**
 * @}
 */

#endif
