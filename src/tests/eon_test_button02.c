#include "Eon.h"
#include "eon_test_main.h"

static void _add_label(Ender_Element *button, int index)
{
	Ender_Element *label;
	char str[PATH_MAX];

	label = eon_label_new();
	sprintf(str, "Button %02d", index);
	eon_label_text_set(label, str);
	eon_bin_child_set(button, label);
}

Ender_Element * eon_test_button(int argc, char **argv)
{
	Ender_Element *container;
	Ender_Element *button;
	int i;

	container = eon_stack_new();
	eon_stack_orientation_set(container, EON_ORIENTATION_VERTICAL);
	eon_stack_homogeneous_set(container, EINA_TRUE);

	/* disabled button */
	button = eon_button_new_with_label("Disabled");
	eon_widget_enabled_set(button, EINA_FALSE);
	eon_container_child_add(container, button);

	/* normal button */
	for (i = 0; i < 5; i++)
	{

		button = eon_button_new();
		_add_label(button, i);
		eon_container_child_add(container, button);
	}

	return container;
}

EXAMPLE(button)
