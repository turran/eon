#include "Eon.h"

int main(int argc, char **argv)
{
	Eon_Window *win;
	Ender_Element *layout;
	Ender_Element *frame;
	Ender_Element *label;
	Eon_Backend *backend;
	int i;

	eon_init();
	ecore_init();

	//backend = eon_ecore_remote_new();
	backend = eon_ecore_sdl_new();

	layout = eon_stack_new();

	win = eon_window_new(backend, layout, 320, 240);
	eon_stack_direction_set(layout, EON_STACK_DIRECTION_VERTICAL);

	frame = eon_frame_new();

	eon_frame_description_set(frame, "My Frame");

	label = eon_label_new();
	eon_label_text_set(label, "My Frame Content");
	eon_container_content_set(frame, label);

	eon_layout_child_add(layout, frame);

	ecore_main_loop_begin();
	ecore_shutdown();
	eon_shutdown();

	return 0;
}
