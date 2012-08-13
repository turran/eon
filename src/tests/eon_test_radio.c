#include "Eon.h"
#include "eon_test_main.h"

Ender_Element * eon_test_radio(int argc, char **argv)
{
	Ender_Element *container;
	int i;

	container = eon_stack_new();
	eon_stack_direction_set(container, EON_DIRECTION_VERTICAL);
	eon_stack_homogeneous_set(container, EINA_TRUE);

	for (i = 0; i < 5; i++)
	{
		Ender_Element *radio;
		char str[PATH_MAX];

		sprintf(str, "Radio %d", i);
		radio = eon_radio_new_with_label(str);
		eon_radio_group_name_set(radio, "test");

		eon_container_child_add(container, radio);
	}

	return container;
}

EXAMPLE(radio)

