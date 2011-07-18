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
		Ender_Element *button;
		Ender_Element *label;
		char str[PATH_MAX];

		label = eon_label_new();
		sprintf(str, "My Button %d", i);
		eon_label_text_set(label, str);

		button = eon_button_new();
		eon_container_content_set(button, label);

		eon_layout_child_add(layout, button);
	}

	ecore_main_loop_begin();
	ecore_shutdown();
	eon_shutdown();

	return 0;
}
