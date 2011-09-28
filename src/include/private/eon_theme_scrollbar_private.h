#ifndef _EON_THEME_SCROLLBAR_PRIVATE_H_
#define _EON_THEME_SCROLLBAR_PRIVATE_H_

void eon_theme_scrollbar_min_width_get(Enesim_Renderer *r, double *width);
void eon_theme_scrollbar_min_height_get(Enesim_Renderer *r, double *height);
void eon_theme_scrollbar_max_width_get(Enesim_Renderer *r, double *width);
void eon_theme_scrollbar_max_height_get(Enesim_Renderer *r, double *height);
void eon_theme_scrollbar_preferred_width_get(Enesim_Renderer *r, double *width);
void eon_theme_scrollbar_preferred_height_get(Enesim_Renderer *r, double *height);
void eon_theme_scrollbar_orientation_set(Enesim_Renderer *r, Eon_Orientation orientation);
void eon_theme_scrollbar_orientation_get(Enesim_Renderer *r, Eon_Orientation *orientation);
void eon_theme_scrollbar_thumb_min_size_get(Enesim_Renderer *r, double *size);
void eon_theme_scrollbar_thumb_max_size_get(Enesim_Renderer *r, double *size);
void eon_theme_scrollbar_thumb_size_set(Enesim_Renderer *r, double size);
void eon_theme_scrollbar_thumb_size_get(Enesim_Renderer *r, double *size);
void eon_theme_scrollbar_thumb_percent_set(Enesim_Renderer *r, double percent);
void eon_theme_scrollbar_thumb_percent_get(Enesim_Renderer *r, double *percent);
void eon_theme_scrollbar_decrement_arrow_geometry_get(Enesim_Renderer *r, Enesim_Rectangle *geometry);
void eon_theme_scrollbar_increment_arrow_geometry_get(Enesim_Renderer *r, Enesim_Rectangle *geometry);
void eon_theme_scrollbar_thumb_geometry_get(Enesim_Renderer *r, Enesim_Rectangle *geometry);

#endif
