#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <defs.h>
#include <debug.h>
#include <param.h>

#ifndef VERSION
#define VERSION "0.0.1-cvs"
#endif

#ifndef no_argument
#define no_argument 0
#endif

#ifndef required_argument
#define required_argument 1
#endif

#ifndef optional_argument
#define optional_argument 2
#endif

#define PLUS_OR_MINUS(c)  ((c) == '+' || (c) == '-')
#define IMAGINARY_UNIT(x) ((x) == 'i' || (x) == 'j')
#define MINIMUM(x, y) ((x) <= (y) ? (x) : (y))
#define MAXIMUM(x, y) ((x) >= (y) ? (x) : (y))
#define SIGN(c) ((c) == '-' ? -1.0 : +1.0)


static void do_plot(void);
extern void generic_plot(param_t *);
extern void mips32_plot(param_t *);

/*
 * Parametros globales.
 */

int x_res = 640;		/* Ancho de imagen por defecto. */
int y_res = 480;		/* Alto de imagen, por defecto. */
float upper_left_re = -1.0f;	/* Extremo superior izquierzo (re). */
float upper_left_im = +1.0f;	/* Extremo superior izquierzo (im). */
float lower_right_re = +1.0f;	/* Extremo inferior derecho (re). */
float lower_right_im = -1.0f;	/* Extremo inferior derecho (im). */
float seed_re = -0.72689535f;   /* Semilla (re). */
float seed_im = +0.18888713f;   /* Semilla (im). */
void (*plot)(param_t *) = NULL;
FILE *output = NULL;

static void parse_cmdline(int, char * const []);
static void do_usage(const char *, int);
static void do_version(const char *);
static void do_resolution(const char *, const char *);
static void do_seed(const char *, const char *);
static void do_center(const char *, const char *);
static void do_width(const char *, const char *);
static void do_height(const char *, const char *);
static void do_method(const char *, const char *);
static void do_output(const char *, const char *);

int
main(int argc, char * const argv[], char * const envp[])
{
	parse_cmdline(argc, argv);
	do_plot();

	return 0;
}

static void
parse_cmdline(int argc, char * const argv[])
{
	int ch;
	int index = 0;

	struct option options[] = {
		{"help", no_argument, NULL, 'h'},
		{"version", no_argument, NULL, 'V'},
		{"seed", required_argument, NULL, 's'},
		{"resolution", required_argument, NULL, 'r'},
		{"center", required_argument, NULL, 'c'},
		{"width", required_argument, NULL, 'w'},
		{"height", required_argument, NULL, 'H'},
		{"output", required_argument, NULL, 'o'},
	};

	while ((ch = getopt_long(argc, argv, 
	                         "hc:H:m:o:r:w:s:V", options, &index)) != -1) {
		switch (ch) {
		case 'h':
			do_usage(argv[0], 0);
			break;
		case 'V':
			do_version(argv[0]);
			break;
		case 's':
			do_seed(argv[0], optarg);
			break;
		case 'r':
			do_resolution(argv[0], optarg);
			break;
		case 'c':
			do_center(argv[0], optarg);
			break;
		case 'w':
			do_width(argv[0], optarg);
			break;
		case 'H':
			do_height(argv[0], optarg);
			break;
		case 'm':
			do_method(argv[0], optarg);
			break;
		case 'o':
			do_output(argv[0], optarg);
			break;
		default:
			do_usage(argv[0], 1);
		}
	}

	if (plot == NULL)
		plot = &generic_plot;

	if (output == NULL)
		output = stdout;
}

static void
do_usage(const char *name, int status)
{
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "  %s -h\n", name);
	fprintf(stderr, "  %s -V\n", name);
	fprintf(stderr, "  %s [options]\n", name);
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "  -r, --resolution "
	                " Set bitmap resolution to WxH pixels.\n");
	fprintf(stderr, "  -c, --center     "
	                " Set coordinates for the center of the image.\n");
	fprintf(stderr, "  -s, --seed       "
	                " Set seed of the Julia set.\n");
	fprintf(stderr, "  -w, --width      "
	                " Change the width of the spanned region.\n");
	fprintf(stderr, "  -H, --height     "
	                " Change the height of the spanned region.\n");
	fprintf(stderr, "  -o, --output     "
	                " Path to output file.\n");
	fprintf(stderr, "Examples:\n");
	fprintf(stderr, "  %s -o output.pgm\n", name);
	fprintf(stderr, "  %s -r 1600x1200 -o output.pgm\n", name);
	fprintf(stderr, "  %s -c +0.282+0.01i -o output.pgm\n", name);
	exit(status);
}

static void
do_version(const char *name)
{
	fprintf(stderr, "%s\n", VERSION);
	exit(0);
}

static void
do_resolution(const char *name, const char *spec)
{
	int x;
	int y;
	char c;
	char d;

	if (sscanf(spec, "%d%c%d %c", &x, &c, &y, &d) != 3
	    || x <= 0
	    || c != 'x'
	    || y <= 0)
		do_usage(name, 1);

	/* Set new resolution. */
	x_res = x;
	y_res = y;
}

static void
do_seed(const char *name, const char *spec)
{
	double re, im;
	char ii;
	char sg;
	char ch;

	if (sscanf(spec, 
	           "%lf %c %lf %c %c", 
	           &re,
	           &sg,
	           &im,
	           &ii,
	           &ch) != 4
	    || !PLUS_OR_MINUS(sg)
	    || !IMAGINARY_UNIT(ii)) {
		fprintf(stderr, "invalid seed specification.\n");
		exit(1);
	}

	im *= SIGN(sg);
	seed_re = re;
	seed_im = im;
}

static void
do_center(const char *name, const char *spec)
{
	double width;
	double height;
	double re, im;
	char ii;
	char sg;
	char ch;

	if (sscanf(spec, 
	           "%lf %c %lf %c %c", 
	           &re,
	           &sg,
	           &im,
	           &ii,
	           &ch) != 4
	    || !PLUS_OR_MINUS(sg)
	    || !IMAGINARY_UNIT(ii)) {
		fprintf(stderr, "invalid center specification.\n");
		exit(1);
	}

	im *= SIGN(sg);
	width = fabs(upper_left_re - lower_right_re);
	height = fabs(upper_left_im - lower_right_im);

	upper_left_re = re - width / 2;
	upper_left_im = im + height / 2;
	lower_right_re = re + width / 2;
	lower_right_im = im - height / 2;
}

static void
do_height(const char *name, const char *spec)
{
	double width;
	double height;
	double re, im;
	char ch;

	if (sscanf(spec, 
	           "%lf %c", 
	           &height,
	           &ch) != 1
	    || height <= 0.0) {
		fprintf(stderr, "invalid height specification.\n");
		exit(1);
	}

	re = (upper_left_re + lower_right_re) / 2;
	im = (upper_left_im + lower_right_im) / 2;
	width = fabs(upper_left_re - lower_right_re);

	upper_left_re = re - width / 2;
	upper_left_im = im + height / 2;
	lower_right_re = re + width / 2;
	lower_right_im = im - height / 2;
}

static void
do_width(const char *name, const char *spec)
{
	double width;
	double height;
	double re, im;
	char ch;

	if (sscanf(spec, 
	           "%lf %c", 
	           &width,
	           &ch) != 1
	    || width <= 0.0) {
		fprintf(stderr, "invalid width specification.\n");
		exit(1);
	}

	re = (upper_left_re + lower_right_re) / 2;
	im = (upper_left_im + lower_right_im) / 2;
	height = fabs(upper_left_im - lower_right_im);

	upper_left_re = re - width / 2;
	upper_left_im = im + height / 2;
	lower_right_re = re + width / 2;
	lower_right_im = im - height / 2;
}

static void
do_method(const char *name, const char *spec)
{
	if (strcmp (spec, "generic") == 0) {
		plot = &generic_plot;
	} else if (strcmp (spec, "mips32") == 0) {
		plot = &mips32_plot;
	} else {
		fprintf(stderr, "invalid method\n");
		exit(1);
	}
}

static void
do_output(const char *name, const char *spec)
{
	if (output != NULL) {
		fprintf(stderr, "multiple do output files.");
		exit(1);
	}

	if (strcmp(spec, "-") == 0) {
		output = stdout;
	} else {
		if (!(output = fopen(spec, "w"))) {
			fprintf(stderr, "cannot open output file.\n");
			exit(1);
		}
	}
}

static void
do_plot(void)
{
	param_t parms;

	memset(&parms, 0, sizeof(parms));
	parms.UL_re = upper_left_re;
	parms.UL_im = upper_left_im;
	parms.LR_re = lower_right_re;
	parms.LR_im = lower_right_im;
	parms.d_re = (lower_right_re - upper_left_re) / x_res;
	parms.d_im = (upper_left_im - lower_right_im) / y_res;
	parms.s_re = seed_re;
	parms.s_im = seed_im;
	parms.x_res = x_res;
	parms.y_res = y_res;
	parms.shades = 256;
	parms.fp = output;

	plot(&parms);
}
