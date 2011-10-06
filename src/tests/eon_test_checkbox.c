#include "Eon.h"

int main(int argc, char **argv)
{
	Eon_Window *win;
	Eon_Backend *backend;
	Ender_Element *layout;
	int i;

	eon_init();
	ecore_init();

	backend = eon_ecore_sdl_new();

	layout = eon_stack_new();

	eon_stack_direction_set(layout, EON_STACK_DIRECTION_VERTICAL);
	win = eon_window_new(backend, layout, 320, 240);

	for (i = 0; i < 5; i++)
	{
		Ender_Element *checkbox;
		Ender_Element *label;
		char str[PATH_MAX];

		label = eon_label_new();
		sprintf(str, "Option %d", i);
		eon_label_text_set(label, str);
		checkbox = eon_checkbox_new();
		eon_container_content_set(checkbox, label);

		eon_layout_child_add(layout, checkbox);
	}


	ecore_main_loop_begin();
	ecore_shutdown();
	eon_shutdown();

	return 0;
}

