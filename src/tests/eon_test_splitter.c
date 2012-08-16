#include "Eon.h"

int main(int argc, char **argv)
{
	Eon_Window *win;
	Eon_Backend *backend;
	Ender_Element *layout;
	Ender_Element *splitter;
	Ender_Element *button;
	int i;

	eon_init();
	ecore_init();

	//backend = eon_ecore_remote_new();
	backend = eon_ecore_sdl_new();

	layout = eon_stack_new();

	win = eon_window_new(backend, layout, 320, 240);
	eon_stack_orientation_set(layout, EON_STACK_ORIENTATION_VERTICAL);

	splitter = eon_splitter_new();
	eon_splitter_orientation_set(splitter, EON_ORIENTATION_HORIZONTAL);
	eon_splitter_position_set(splitter, 1);

	button = eon_button_new_with_label("left");
	eon_container_content_set(splitter, button);

	button = eon_button_new_with_label("right");
	eon_splitter_second_content_set(splitter, button);

	eon_layout_child_add(layout, splitter);

	ecore_main_loop_begin();
	ecore_shutdown();
	eon_shutdown();

	return 0;
}

