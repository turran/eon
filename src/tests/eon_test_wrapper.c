#include "Eon.h"

int main(int argc, char **argv)
{
	Eon_Window *win;
	Ender_Element *layout;
	Ender_Element *wrapper;
	Ender_Element *rectangle;
	Eon_Backend *backend;
	int i;

	eon_init();
	ecore_init();

	layout = eon_stack_new();
	eon_element_width_set(layout, 320.0);
	eon_element_height_set(layout, 240.0);

	eon_stack_direction_set(layout, EON_STACK_DIRECTION_VERTICAL);

	backend = eon_ecore_sdl_new();
	win = eon_window_new(backend, layout, 320, 240);

	/* create an external ender */
	rectangle = ender_element_new("rectangle");
	ender_element_value_set(rectangle, "width", 100.0, NULL);
	ender_element_value_set(rectangle, "height", 100.0, NULL);

	wrapper = eon_wrapper_new();
	eon_wrapper_wrapped_set(wrapper, rectangle);
	eon_layout_child_add(layout, wrapper);

	ecore_main_loop_begin();
	ecore_shutdown();
	eon_shutdown();

	return 0;
}

