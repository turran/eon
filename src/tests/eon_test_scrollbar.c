#include "Eon.h"
#include "eon_test_main.h"

/* this functions should be called whenever the value of the scrollbar has changed */
static void _scroll_updated(Ender_Element *e, const char *event_name, void *event_data, void *data)
{

}

Ender_Element * eon_test_scrollbar(int argc, char **argv)
{
	Ender_Element *container;
	Ender_Element *scrollbar;

	container = eon_stack_new();

	scrollbar = eon_scrollbar_new();
	eon_scrollbar_min_set(scrollbar, 0);
	eon_scrollbar_max_set(scrollbar, 255);
	eon_scrollbar_step_increment_set(scrollbar, 1);
	eon_scrollbar_page_increment_set(scrollbar, 2);
	//ender_event_listener_add(scrollbar, "ScrollAdjustment", _scroll_updated, NULL);
	eon_container_child_add(container, scrollbar);

	return container;
}

EXAMPLE(scrollbar)

