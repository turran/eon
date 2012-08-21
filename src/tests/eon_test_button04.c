#include "Eon.h"
#include "eon_test_main.h"

Ender_Element * eon_test_button(int argc, char **argv)
{
	Ender_Element *container;
	int i;

	container = eon_grid_new();
	eon_grid_num_columns_set(container, 5);
	for (i = 0; i < 10; i++)
	{
		Ender_Element *button;
		char name[PATH_MAX];

		snprintf(name, PATH_MAX, "Button %02d", i);
		button = eon_button_new_with_label(name);
		eon_container_child_add(container, button);
	}

	return container;
}

EXAMPLE(button)

