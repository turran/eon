#include "Eon.h"

/* this functions should be called whenever the value of the scrollbar has changed */
static void _scroll_updated(Ender_Element *e, const char *event_name, void *event_data, void *data)
{

}

int main(int argc, char **argv)
{
	Eon_Window *win;
	Ender_Element *layout;
	Ender_Element *scrollbar;
	Eon_Backend *backend;
	int i;

	eon_init();
	ecore_init();

	//backend = eon_ecore_remote_new();
	backend = eon_ecore_sdl_new();

	layout = eon_stack_new();
	eon_element_width_set(layout, 320.0);
	eon_element_height_set(layout, 240.0);

	win = eon_window_new(backend, layout, 320, 240);
	eon_stack_direction_set(layout, EON_STACK_DIRECTION_VERTICAL);

	scrollbar = eon_scrollbar_new();
	eon_scrollbar_min_set(scrollbar, 0);
	eon_scrollbar_max_set(scrollbar, 255);
	eon_scrollbar_step_increment_set(scrollbar, 1);
	eon_scrollbar_page_increment_set(scrollbar, 2);
	//ender_event_listener_add(scrollbar, "ScrollAdjustment", _scroll_updated, NULL);

	eon_layout_child_add(layout, scrollbar);

	ecore_main_loop_begin();
	ecore_shutdown();
	eon_shutdown();

	return 0;
}

