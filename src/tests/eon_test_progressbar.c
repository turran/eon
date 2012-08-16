#include "Eon.h"

int main(int argc, char **argv)
{
	Eon_Window *win;
	Eon_Backend *backend;
	Ender_Element *layout;
	int i;

	eon_init();
	ecore_init();

	layout = eon_stack_new();

	eon_stack_orientation_set(layout, EON_STACK_ORIENTATION_VERTICAL);
	backend = eon_ecore_sdl_new();
	win = eon_window_new(backend, layout, 320, 240);

	for (i = 0; i < 5; i++)
	{
		Ender_Element *pb;

		pb = eon_progressbar_new();
		eon_progressbar_progression_set(pb, i * 0.25);
		eon_layout_child_add(layout, pb);
	}

	ecore_main_loop_begin();
	ecore_shutdown();
	eon_shutdown();

	return 0;
}
