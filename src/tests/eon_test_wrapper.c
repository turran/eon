#include "Eon.h"

static void _wrapper_clicked(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Ender_Element *rectangle = data;

	ender_element_value_set(rectangle, "fill_color", 0xff0000ff, NULL);
}

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

	//layout = eon_canvas_new();
	layout = eon_stack_new();

	eon_stack_direction_set(layout, EON_STACK_DIRECTION_VERTICAL);

	backend = eon_ecore_sdl_new();
	win = eon_window_new(backend, layout, 320, 240);

	/* create an external ender */
	rectangle = ender_element_new("rectangle");
	ender_element_value_set(rectangle, "fill_color", 0xff00ffff, NULL);
	ender_element_value_set(rectangle, "stroke_color", 0xffff0000, NULL);
	ender_element_value_set(rectangle, "stroke_weight", 2.0, NULL);
	ender_element_value_set(rectangle, "draw_mode", 2, NULL);
	ender_element_value_set(rectangle, "width", 100.0, NULL);
	ender_element_value_set(rectangle, "height", 100.0, NULL);

	wrapper = eon_wrapper_new();
	ender_event_listener_add(wrapper, "MouseClick", _wrapper_clicked, rectangle);
	eon_wrapper_wrapped_set(wrapper, rectangle);
	eon_layout_child_add(layout, wrapper);
	//eon_canvas_child_x_set(layout, wrapper, 50);
	//eon_canvas_child_y_set(layout, wrapper, 50);

	ecore_main_loop_begin();
	ecore_shutdown();
	eon_shutdown();

	return 0;
}

