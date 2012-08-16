#include "Eon.h"
#include "eon_test_main.h"

Ender_Element * eon_test_button(int argc, char **argv)
{
	Ender_Element *container;
	Ender_Element *button;
	int i;

	container = eon_stack_new();
	eon_stack_orientation_set(container, EON_ORIENTATION_VERTICAL);
	eon_stack_homogeneous_set(container, EINA_FALSE);
	button = eon_button_new_with_label("No gravity 01");
	eon_container_child_add(container, button);

	button = eon_button_new_with_label("Gravity 1");
	eon_container_child_add(container, button);
	eon_stack_child_gravity_set(container, button, 1);

	button = eon_button_new_with_label("Gravity 2");
	eon_container_child_add(container, button);
	eon_stack_child_gravity_set(container, button, 2);

	button = eon_button_new_with_label("No gravity 02");
	eon_container_child_add(container, button);

	return container;
}

EXAMPLE(button)

