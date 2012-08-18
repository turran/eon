#include "Escen.h"
#include "Ender.h"
#include "Eon.h"

typedef struct _Escen_Viewer
{
	/* command line options */
	int width;
	int height;
	char *file; /* file to open */
	Escen *escen; /* escen loaded */
	Ender_Element *window; /* the main window */
	Ender_Element *enders_states_splitter;
	Ender_Element *states_scene_splitter;
	Ender_Element *enders_stack;
	Ender_Element *enders_scroll;
	Ender_Element *states_stack;
	Ender_Element *states_scroll;
	/* for the area where we should render the escen */
	Ender_Element *scene_area; /* where the escen loaded should be drawn */
	Escen_Ender *current_escen_ender; /* current escen ender */
	Escen_Instance *current_instance; /* current instance displaying */
	Ender_Element *current_ender;
} Escen_Viewer;

static void help(void)
{
	printf("Usage escen_viewer WIDTH HEIGHT ESCEN_FILE\n");
}

static void state_clicked(Ender_Element *e, const char *event_name, void *event_data,
		void *data)
{
	Escen_Viewer *viewer = data;
	Escen_State *escen_state;
	Ender_Element *label;
	char *string;

	eon_container_content_get(e, &label);
	eon_label_text_get(label, &string);
	escen_state = escen_ender_state_get(viewer->current_escen_ender, string);
	escen_instance_state_set(viewer->current_instance, escen_state);
}

static void populate_states(Escen_Viewer *viewer, Ender_Element *container, Escen_Ender *ee)
{
	int num_escen_state;
	int i;

	eon_layout_child_clear(viewer->states_stack);
	/* iterate over the escen_ender and find every state */
	num_escen_state = escen_ender_state_count(ee);
	for (i = 0; i < num_escen_state; i++)
	{
		Ender_Element *radio;
		Ender_Element *label;
		Escen_State *escen_state;
		const char *name;

		escen_state = escen_ender_state_nth_get(ee, i);
		name = escen_state_name_get(escen_state);

		label = eon_label_new();
		eon_label_text_set(label, name);

		radio = eon_radio_new();
		ender_event_listener_add(radio, "MouseClick", state_clicked, viewer);
		eon_radio_group_name_set(radio, "states");
		eon_container_content_set(radio, label);

		eon_layout_child_add(container, radio);
	}
}

static void ender_clicked(Ender_Element *e, const char *event_name, void *event_data,
		void *data)
{
	Escen_Viewer *viewer = data;
	Escen_Ender *new_escen_ender;
	Escen_Instance *new_instance;
	Ender_Element *new_ender;
	Ender_Element *label;
	char *string;

	eon_container_content_get(e, &label);
	if (!label) return;
	eon_label_text_get(label, &string);
	new_escen_ender = escen_ender_get(viewer->escen, string);
	if (!new_escen_ender) return;

	new_instance = escen_instance_new(new_escen_ender);
	new_ender = escen_instance_ender_get(new_instance);
	/* TODO remove the old instance */
	//escen_instance_delete(viewer->current_instance);
	viewer->current_instance = new_instance;
	viewer->current_escen_ender = new_escen_ender;
	viewer->current_ender = new_ender;
	/* just replace what we have as content of the scene area */
	eon_wrapper_wrapped_set(viewer->scene_area, new_ender);
	populate_states(viewer, viewer->states_stack, viewer->current_escen_ender);
}


static void populate_enders(Escen_Viewer *viewer, Ender_Element *container, Escen *escen)
{
	int num_escen_ender;
	int i;

	/* iterate over the escen and find every ender_escen */
	num_escen_ender = escen_ender_count(escen);
	for (i = 0; i < num_escen_ender; i++)
	{
		Ender_Element *radio;
		Ender_Element *label;
		Escen_Ender *escen_ender;
		const char *name;

		escen_ender = escen_ender_nth_get(escen, i);
		name = escen_ender_name_get(escen_ender);

		label = eon_label_new();
		eon_label_text_set(label, name);

		radio = eon_radio_new();
		eon_radio_group_name_set(radio, "enders");
		ender_event_listener_add(radio, "MouseClick", ender_clicked, viewer);
		eon_container_content_set(radio, label);

		eon_layout_child_add(container, radio);
	}
}

static void draw_ui(Escen_Viewer *viewer, Ender_Element *win)
{
	Ender_Element *e;

	/* add a stack with the enders, make it scrollable */
	e = eon_stack_new();
	eon_stack_direction_set(e, EON_STACK_DIRECTION_VERTICAL);
	viewer->enders_stack = e;

	e = eon_scrollview_new();
	eon_container_content_set(e, viewer->enders_stack);
	viewer->enders_scroll = e;

	/* add another stack with the states of each escen, make it scrollable */
	e = eon_stack_new();
	eon_stack_direction_set(e, EON_STACK_DIRECTION_VERTICAL);
	viewer->states_stack = e;

	e = eon_scrollview_new();
	eon_container_content_set(e, viewer->states_stack);
	viewer->states_scroll = e;

	/* the scene area */
	e = eon_wrapper_new();
	eon_element_min_width_set(e, viewer->width);
	eon_element_min_height_set(e, viewer->height);
	//eon_stack_child_vertical_alignment_set(win, e, EON_VERTICAL_ALIGNMENT_CENTER);
	//eon_stack_child_horizontal_alignment_set(win, e, EON_HORIZONTAL_ALIGNMENT_CENTER);
	viewer->scene_area = e;

	/* the second splitter */
	e = eon_splitter_new();
	eon_splitter_orientation_set(e, EON_ORIENTATION_HORIZONTAL);
	eon_container_content_set(e, viewer->states_scroll);
	eon_splitter_second_content_set(e, viewer->scene_area);;
	viewer->states_scene_splitter = e;

	/* the first splitter */
	e = eon_splitter_new();
	eon_splitter_orientation_set(e, EON_ORIENTATION_HORIZONTAL);
	eon_container_content_set(e, viewer->enders_stack);
	eon_splitter_second_content_set(e, viewer->states_scene_splitter);
	viewer->enders_states_splitter = e;

	eon_layout_child_add(win, viewer->enders_states_splitter);

	/* set the default state */
	viewer->current_escen_ender = escen_ender_nth_get(viewer->escen, 0);
	viewer->current_instance = escen_instance_new(viewer->current_escen_ender);
	escen_instance_state_set(viewer->current_instance, escen_ender_state_get(viewer->current_escen_ender, "default"));
	viewer->current_ender = escen_instance_ender_get(viewer->current_instance);
	eon_wrapper_wrapped_set(viewer->scene_area, viewer->current_ender);


	populate_enders(viewer, viewer->enders_stack, viewer->escen);
	populate_states(viewer, viewer->states_stack, viewer->current_escen_ender);
}

int main(int argc, char **argv)
{
	Escen_Viewer *viewer;
	Eon_Window *ee;
	Ender_Element *win;

	if (argc < 4)
	{
		help();
		return 0;
	}

	eon_init();
	ecore_init();
	escen_init();

	/* do the setup */
	viewer = calloc(1, sizeof(Escen_Viewer));
	viewer->width = strtol(argv[1], NULL, 10);
	viewer->height = strtol(argv[2], NULL, 10);
	viewer->file = strdup(argv[3]);
	/* load the escen file */
	viewer->escen = escen_parser_load(viewer->file);
	if (!viewer->escen)
	{
		printf("Can not parse the file %s\n", viewer->file);
		return 1;
	}
	/* create the main window */
	win = eon_stack_new();
	eon_stack_direction_set(win, EON_STACK_DIRECTION_HORIZONTAL);
	eon_stack_last_expand_set(win, EINA_TRUE);
	viewer->window = win;
	ee = eon_window_new(eon_ecore_sdl_new(), win, viewer->width, viewer->height);
	if (!ee)
	{
		printf("Can not create the Eon_Window window\n");
		return 1;
	}
	draw_ui(viewer, win);

	ecore_main_loop_begin();

	escen_shutdown();
	eon_shutdown();
	ecore_shutdown();

	return 0;
}
