#include "Eon.h"

int main(int argc, char **argv)
{
	Eon_Window *win;
	Ender_Element *layout;
	int i;

	eon_init();
	ecore_init();

	layout = eon_stack_new();
	eon_element_width_set(layout, 320.0);
	eon_element_height_set(layout, 240.0);

	eon_stack_direction_set(layout, EON_STACK_DIRECTION_VERTICAL);

	win = eon_window_new(eon_ecore_sdl_new(), layout, 320, 240);

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
