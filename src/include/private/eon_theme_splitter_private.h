#ifndef _EON_THEME_SPLITTER_PRIVATE_H_
#define _EON_THEME_SPLITTER_PRIVATE_H_

void eon_theme_splitter_thickness_get(Enesim_Renderer *r, double *thickness);
void eon_theme_splitter_min_length_get(Enesim_Renderer *r, double *length);
void eon_theme_splitter_orientation_set(Enesim_Renderer *r, Eon_Orientation orientation);
void eon_theme_splitter_second_content_set(Enesim_Renderer *r, Enesim_Renderer *second_content);
void eon_theme_splitter_position_set(Enesim_Renderer *r, double position);

#endif

