#include "Eon.h"
#include "eon_test_main.h"

Ender_Element * eon_test_checkbox(int argc, char **argv)
{
	Ender_Element *container;
	int i;

	container = eon_list_new();
	eon_list_orientation_set(container, EON_ORIENTATION_VERTICAL);

	for (i = 0; i < 5; i++)
	{
		Ender_Element *checkbox;
		char str[PATH_MAX];

		sprintf(str, "Checkbox %d", i);
		checkbox = eon_checkbox_new_with_label(str);
		eon_container_child_add(container, checkbox);
	}

	return container;
}

EXAMPLE(checkbox)
