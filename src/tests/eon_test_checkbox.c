#include "Eon.h"
#include "eon_test_main.h"

Ender_Element * eon_test_checkbox(int argc, char **argv)
{
	Ender_Element *container;
	int i;

	container = eon_stack_new();
	eon_stack_direction_set(container, EON_DIRECTION_VERTICAL);
	eon_stack_homogeneous_set(container, EINA_TRUE);

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
