#ifndef _EON_IMAGE_H_
#define _EON_IMAGE_H_

/**
 * @{
 */

EAPI Ender_Element * eon_image_new(void);
EAPI void eon_image_file_set(Ender_Element *e, const char *file);
EAPI void eon_image_file_get(Ender_Element *e, const char **file);

/**
 * @}
 */

#endif

