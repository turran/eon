#include <Eon.h>
/*----------------------------------------------------------------------------*
 *                                 Helpers                                    *
 *----------------------------------------------------------------------------*/
static void help(void)
{
	printf("Usage: eon_loader FILE.eon FILE.png [width height]\n");
	printf("Where width and height defines the size of the destination image\n");
}

int main(int argc, char *argv[])
{
	Eon_Size size = { 640, 480 };
	Egueb_Dom_Node *doc = NULL;
	Egueb_Dom_Node *eon = NULL;
	Enesim_Surface *s;
	Enesim_Buffer *b;
	Enesim_Log *err = NULL;
	Enesim_Stream *stream;

	if (argc < 3)
	{
		help();
		return -1;
	}

	if (argc > 3)
	{
		size.width = atoi(argv[3]);
 		if (argc < 5)
		{
			help();
			return -1;
		}
		size.height = atoi(argv[4]);
	}

	if (!eon_init())
		return -1;

	stream = enesim_stream_file_new(argv[1], "r+");
	if (!stream)
	{
		printf("Fail to parse %s\n", argv[1]);
		goto shutdown_eon;
	}
	/* create the document */
	doc = eon_document_new();

	egueb_dom_parser_parse(stream, doc);
	enesim_stream_unref(stream);

	eon = egueb_dom_document_element_get(doc);
	if (!eon)
	{
		printf("The parsed element is not a topmost EON element\n");
		goto shutdown_eon;
	}

	/* set the final image size as the container size */
	eon_element_eon_size_set(eon, &size);
	/* now process the document */
	egueb_dom_document_process(doc);
	/* finally draw */
	s = enesim_surface_new(ENESIM_FORMAT_ARGB8888, size.width, size.height);
	eon_element_eon_draw(eon, s, ENESIM_ROP_FILL, NULL, 0, 0, &err);
	if (err)
	{
		enesim_log_dump(err);
		enesim_log_delete(err);
	}
	b = enesim_surface_buffer_get(s);
	enesim_image_file_save(argv[2], b, NULL);
	enesim_buffer_unref(b);
	enesim_surface_unref(s);

shutdown_eon:
	if (doc)
		egueb_dom_node_unref(doc);
	if (eon)
		egueb_dom_node_unref(eon);
	eon_shutdown();

	return 0;
}
