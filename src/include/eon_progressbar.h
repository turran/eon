#ifndef _EON_PROGRESSBAR_H_
#define _EON_PROGRESSBAR_H_

EAPI Ender_Element * eon_progressbar_new(void);
EAPI void eon_progressbar_progression_get(Ender_Element *e, double *progression);
EAPI void eon_progressbar_progression_set(Ender_Element *e, double progression);

#endif
