#include "Eon.h"

static void _add_label(Ender_Element *button, int index)
{
	Ender_Element *label;
	char str[PATH_MAX];

	label = eon_label_new();
	sprintf(str, "My Button %d", index);
	eon_label_text_set(label, str);
	eon_container_content_set(button, label);
}

int main(int argc, char **argv)
{
	Eon_Window *win;
	Ender_Element *layout;
	Eon_Backend *backend;
	Ender_Element *button;
	int i;

	eon_init();
	ecore_init();

	//backend = eon_ecore_remote_new();
	backend = eon_ecore_sdl_new();

	layout = eon_stack_new();
	eon_element_width_set(layout, 321.0);
	eon_element_height_set(layout, 240.0);

	win = eon_window_new(backend, layout, 321, 240);
	eon_stack_direction_set(layout, EON_STACK_DIRECTION_VERTICAL);

	/* normal button */
	/* something more than a label */
	/* disabled button */
	button = eon_button_new_with_label("Disabled");
	eon_button_base_enabled_set(button, EINA_FALSE);
	eon_layout_child_add(layout, button);

	button = eon_button_new_with_label("Button 01");
	eon_layout_child_add(layout, button);

	for (i = 0; i < 5; i++)
	{

		button = eon_button_new();
		_add_label(button, i);
		eon_layout_child_add(layout, button);
	}

	ecore_main_loop_begin();
	ecore_shutdown();
	eon_shutdown();

	return 0;
}
