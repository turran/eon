#include "Eon.h"
#include "eon_test_main.h"

static Enesim_Color _color[2] = { 0xff0000ff, 0xff00ffff};
static int _color_selected = 0;

static void _wrapper_clicked(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Ender_Element *rectangle = data;

	_color_selected = (_color_selected + 1) % 2;
	ender_element_value_set(rectangle, "fill_color", _color[_color_selected], NULL);
}

Ender_Element * eon_test_wrapper(int argc, char **argv)
{
	Eon_Window *win;
	Enesim_Renderer *r;
	Ender_Element *container;
	Ender_Element *wrapper;
	Ender_Element *rectangle;


	container = eon_stack_new();
	eon_stack_orientation_set(container, EON_ORIENTATION_VERTICAL);

	/* create an external ender */
	rectangle = ender_element_new("rectangle");
	ender_element_value_set(rectangle, "fill_color", _color[0], NULL);
	ender_element_value_set(rectangle, "stroke_color", 0xffff0000, NULL);
	ender_element_value_set(rectangle, "stroke_weight", 2.0, NULL);
	ender_element_value_set(rectangle, "draw_mode", 3, NULL);
	ender_element_value_set(rectangle, "width", 100.0, NULL);
	ender_element_value_set(rectangle, "height", 100.0, NULL);

	wrapper = eon_wrapper_new();
	ender_event_listener_add(wrapper, "MouseClick", _wrapper_clicked, rectangle);
	eon_wrapper_wrapped_set(wrapper, rectangle);
	eon_container_child_add(container, wrapper);
	//eon_canvas_child_x_set(container, wrapper, 50);
	//eon_canvas_child_y_set(container, wrapper, 50);

	return container;
}

EXAMPLE(wrapper)
