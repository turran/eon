#ifndef _EON_THEME_ENTRY_BASE_PRIVATE_H_
#define _EON_THEME_ENTRY_BASE_PRIVATE_H_

void eon_theme_entry_min_width_get(Enesim_Renderer *r, double *width);
void eon_theme_entry_min_height_get(Enesim_Renderer *r, double *height);
void eon_theme_entry_buffer_has_changed(Enesim_Renderer *r);

Etex_Buffer * eon_theme_entry_buffer_get(Enesim_Renderer *r);

#endif


