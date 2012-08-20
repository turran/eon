#include "Eon.h"
#include "eon_test_main.h"

Ender_Element * eon_test_scrollview(int argc, char **argv)
{
	Ender_Element *container;
	Ender_Element *scrollview;
	Ender_Element *label;
	Ender_Element *button;

	container = eon_stack_new();
	eon_stack_orientation_set(container, EON_ORIENTATION_VERTICAL);
	eon_stack_homogeneous_set(container, EINA_TRUE);

	scrollview = eon_scrollview_new();

	label = eon_label_new();
	eon_label_text_set(label, "very large button");

	button = eon_button_new();
	eon_bin_child_set(button, label);
	eon_element_width_set(button, 640.0);
	//eon_element_height_set(button, 480.0);

	eon_bin_child_set(scrollview, button);
	eon_container_child_add(container, scrollview);

	return container;
}

EXAMPLE(scrollview)
