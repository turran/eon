#include "Eon.h"

int main(int argc, char **argv)
{
	Eon_Window *win;
	Ender_Element *layout;
	Ender_Element *scrollview;
	Ender_Element *label;
	Ender_Element *button;
	int i;

	eon_init();
	ecore_init();

	layout = eon_stack_new();
	eon_element_width_set(layout, 320.0);
	eon_element_height_set(layout, 240.0);

	eon_stack_direction_set(layout, EON_STACK_DIRECTION_VERTICAL);

	win = eon_window_new(eon_ecore_sdl_new(), layout, 320, 240);
	scrollview = eon_scrollview_new();

	label = eon_label_new();
	eon_label_text_set(label, "very large button");

	button = eon_button_new();
	eon_container_content_set(button, label);
	eon_element_width_set(button, 640.0);
	eon_element_width_set(button, 480.0);

	eon_container_content_set(scrollview, button);
	eon_layout_child_add(layout, scrollview);

	ecore_main_loop_begin();
	ecore_shutdown();
	eon_shutdown();

	return 0;
}
