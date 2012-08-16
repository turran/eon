#include "Eon.h"

int main(int argc, char **argv)
{
	Eon_Window *win;
	Ender_Element *layout;
	Eon_Backend *backend;
	Ender_Element *e;
	int i;

	eon_init();
	ecore_init();

	//backend = eon_ecore_remote_new();
	backend = eon_ecore_sdl_new();

	layout = eon_stack_new();

	win = eon_window_new(backend, layout, 320, 240);
	eon_stack_orientation_set(layout, EON_STACK_ORIENTATION_VERTICAL);

	e = eon_color_new();
	eon_layout_child_add(layout, e);
	e = eon_color_new();
	eon_layout_child_add(layout, e);

	ecore_main_loop_begin();
	ecore_shutdown();
	eon_shutdown();

	return 0;
}

