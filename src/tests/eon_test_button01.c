#include "Eon.h"
#include "eon_test_main.h"

Ender_Element * eon_test_button(int argc, char **argv)
{
	Ender_Element *button;
	button = eon_button_new_with_label("Enabled");
	return button;
}

EXAMPLE(button)

