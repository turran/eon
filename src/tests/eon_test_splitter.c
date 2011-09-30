#include "Eon.h"

int main(int argc, char **argv)
{
	Eon_Window *win;
	Eon_Backend *backend;
	Ender_Element *layout;
	Ender_Element *splitter;
	Ender_Element *label;
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

	splitter = eon_splitter_new();
	eon_splitter_orientation_set(splitter, EON_ORIENTATION_HORIZONTAL);

	label = eon_label_new_with_text("left");
	eon_container_content_set(splitter, label);

	label = eon_label_new_with_text("right");
	eon_splitter_second_content_set(splitter, label);

	eon_layout_child_add(layout, splitter);

	ecore_main_loop_begin();
	ecore_shutdown();
	eon_shutdown();

	return 0;
}

