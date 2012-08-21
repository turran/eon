#include "Eon.h"
#include "eon_test_main.h"

Ender_Element * eon_test_button(int argc, char **argv)
{
	Ender_Element *container;
	Ender_Element *button;

	container = eon_stack_new();
	eon_stack_orientation_set(container, EON_ORIENTATION_VERTICAL);
	eon_stack_homogeneous_set(container, EINA_FALSE);
	button = eon_button_new_with_label("No weight 01");
	eon_container_child_add(container, button);

	button = eon_button_new_with_label("Weight 1");
	eon_container_child_add(container, button);
	eon_stack_child_weight_set(container, button, 1);

	button = eon_button_new_with_label("Weight 2");
	eon_container_child_add(container, button);
	eon_stack_child_weight_set(container, button, 2);

	button = eon_button_new_with_label("No weight 02");
	eon_container_child_add(container, button);

	return container;
}

EXAMPLE(button)

