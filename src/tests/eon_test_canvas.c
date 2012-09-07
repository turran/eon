#include "Eon.h"
#include "eon_test_main.h"

Ender_Element * eon_test_canvas(int argc, char **argv)
{
	Ender_Element *container;
	int i;

	container = eon_canvas_new();

	for (i = 0; i < 5; i++)
	{
		Ender_Element *label;
		char str[PATH_MAX];

		label = eon_label_new();
		sprintf(str, "My Label %d", i);
		eon_label_text_set(label, str);

		eon_container_child_add(container, label);
		eon_canvas_child_x_set(container, label, 320.0/5 * i);
		eon_canvas_child_y_set(container, label, 240.0/5 * i);
	}
	return container;
}

EXAMPLE(canvas)
