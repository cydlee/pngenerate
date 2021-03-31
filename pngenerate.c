/*************************************************************************************************\
|                                                                                                 |
|  pngenerate - a program that generates solid and random png images                              |
|                                                                                                 |
|  Copyright (c) 2021 Cyrus Lee                                                                   |
|                                                                                                 |
|  Permission is hereby granted, free of charge, to any person obtaining a copy of this           |
|  software and associated documentation files (the “Software”), to deal in the Software without  |
|  restriction, including without limitation the rights to use, copy, modify, merge, publish,     |
|  distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the  |
|  Software is furnished to do so, subject to the following conditions:                           |
|                                                                                                 |
|  The above copyright notice and this permission notice shall be included in all copies or       |
|  substantial portions of the Software.                                                          |
|                                                                                                 |
|  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING  |
|  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND     |
|  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,   |
|  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, |
|  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.        |
|                                                                                                 |
\*************************************************************************************************/

#include <ctype.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <png.h>

#define NAME "pngenerate"
#define VERSION "1.0.1"
#define MAX_BYTES 524288000

unsigned int strtouint (char* str, int base);
int ishex (char ch);

static int use_red;
static int use_green;
static int use_blue;
static int use_alpha;

static int alpha_random_flag;
static int ignore_size_limit_flag;
static int verbose_flag;
static int version_flag;
static int help_flag;

int main (int argc, char *argv[])
{
	// Option parsing
	//---------------------------------------------------------------------------------------------

	unsigned int width;
	unsigned int height;

	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha;

	char* red_str = (char*) malloc (2);
	char* green_str = (char*) malloc (2);
	char* blue_str = (char*) malloc (2);

	int c;
	while (1)
	{
		static struct option long_options[] =
		{
			{"width",             required_argument, 0,                      'w'},
			{"height",            required_argument, 0,                      'h'},
			{"color",             required_argument, 0,                      'c'},
			{"red",               required_argument, 0,                      'r'},
			{"green",             required_argument, 0,                      'g'},
			{"blue",              required_argument, 0,                      'b'},
			{"alpha",             required_argument, 0,                      'a'},
			{"alpha-random",      no_argument,       &alpha_random_flag,      1 },

			{"ignore-size-limit", no_argument,       &ignore_size_limit_flag, 1 },
			{"verbose",           no_argument,       &verbose_flag,           1 },

			{"version",           no_argument,       &version_flag,           1 },
			{"help",              no_argument,       &help_flag,              1 },

			{0, 0, 0, 0}
		};

		int option_index = 0;

		c = getopt_long (argc, argv, "w:h:c:r:g:b:a:Av", long_options, &option_index);

		if (c == -1)
			break;

		switch (c)
		{
			case 0:
				break;
			case 'w':
				width = strtouint (optarg, 10);
				break;
			case 'h':
				height = strtouint (optarg, 10);
				break;
			case 'c':
				red_str[0] = optarg[0];
				red_str[1] = optarg[1];
				green_str[0] = optarg[2];
				green_str[1] = optarg[3];
				blue_str[0] = optarg[4];
				blue_str[1] = optarg[5];
				printf ("%s%s%s\n", red_str, green_str, blue_str);
				red = (unsigned char) strtouint (red_str, 16);
				use_red = 1;
				green = (unsigned char) strtouint (green_str, 16);
				use_green = 1;
				blue = (unsigned char) strtouint (blue_str, 16);
				use_blue = 1;
				break;
			case 'r':
				red = (unsigned char) strtouint (optarg, 10);
				use_red = 1;
				break;
			case 'g':
				green = (unsigned char) strtouint (optarg, 10);
				use_green = 1;
				break;
			case 'b':
				blue = (unsigned char) strtouint (optarg, 10);
				use_blue = 1;
				break;
			case 'a':
				alpha = (unsigned char) strtouint (optarg, 10);
				use_alpha = 1;
				break;
			case 'A':
				alpha_random_flag = 1;
				break;
			case 'v':
				verbose_flag = 1;
				break;
			case '?':
				break;
			default:
				abort ();
		}
	}

	free (red_str);
	free (green_str);
	free (blue_str);

	// Version message
	//---------------------------------------------------------------------------------------------
	if (version_flag)
	{
		printf ("%s %s\n", NAME, VERSION);
		fputs ("\
Copyright (c) 2021 Cyrus Lee\n\
\n\
Permission is hereby granted, free of charge, to any person obtaining a copy of this\n\
software and associated documentation files (the “Software”), to deal in the Software without\n\
restriction, including without limitation the rights to use, copy, modify, merge, publish,\n\
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the\n\
Software is furnished to do so, subject to the following conditions:\n\
\n\
The above copyright notice and this permission notice shall be included in all copies or\n\
substantial portions of the Software.\n\
\n\
THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING\n\
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND\n\
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,\n\
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n\
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n\
", stdout);
		return EXIT_SUCCESS;
	}

	// Help message
	//---------------------------------------------------------------------------------------------
	if (help_flag)
	{
		printf ("Usage: %s [-w <width>] [-h <height>] [OPTION]... [OUTPUT FILE]...\n", NAME);
		// Implement: -w -h --width --height -v 
		fputs ("\
Generates a random png image.\n\
\n\
  -w, --width   Set width in pixels (required)\n\
  -h, --height   Set height in pixels (required)\n\
  -c, --color   Set color using hexadecimal notation (e.g. ffffff)\n\
  -r, --red   Set red value (0-255)\n\
  -g, --green   Set green value (0-255)\n\
  -b, --blue   Set blue value (0-255)\n\
  -a, --alpha   Set alpha value (0-255)\n\
  -A, --alpha-random   Generate random alpha values for each pixel\n\
\n\
      --ignore-size-limit   Ignores 500MB size limit on output files\n\
  -v, --verbose   Explain what is being done\n\
\n\
      --version   Print version and license\n\
      --help   Print this help message\n\
", stdout);
		return EXIT_SUCCESS;
	}
	
	// Get output name from argv
	char* output_name;

	if (optind < argc)
		output_name = argv[optind];
	else
	{
		fprintf (stderr, "ERROR: No output file specified.\n");
		return EXIT_FAILURE;
	}

	// Check width and height
	if (!width)
	{
		fprintf (stderr, "ERROR: Invalid width or none specified.\n");
		return EXIT_FAILURE;
	}
	if (!height)
	{
		fprintf (stderr, "ERROR: Invalid height or none specified.\n");
		return EXIT_FAILURE;
	}

	// Main program
	//---------------------------------------------------------------------------------------------
	srand (time (0));

	if (width * height * 4 > MAX_BYTES && !ignore_size_limit_flag)
	{
		fprintf (stderr, "ERROR: Size of png is larger than 500MB.\n");
		return EXIT_FAILURE;
	}

	FILE* fp = fopen (output_name, "wb");
	if (!fp)
	{
		fprintf (stderr, "ERROR: File %s does not exist or cannot be opened.\n", output_name);
		return EXIT_FAILURE;
	}

	// 1. Create png struct pointer
	png_structp png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		fprintf (stderr, "ERROR: Failed to create png struct pointer.\n");
		return EXIT_FAILURE;
	}
	png_infop info_ptr = png_create_info_struct (png_ptr);
	if (!info_ptr)
	{
		png_destroy_write_struct (&png_ptr, (png_infopp) NULL);
		fprintf (stderr, "ERROR: Failed to create png info pointer.\n");
		return EXIT_FAILURE;
	}

	// 2. Set png info like width, height, bit depth and color type
	png_init_io (png_ptr, fp);
	png_set_IHDR (png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	// 3. Mallocking and setting color values to everything
	if (verbose_flag)
		printf ("Allocating memory to the png...\n");
	png_bytep* row_pointers = (png_bytep*) png_malloc (png_ptr, sizeof (png_bytep) * height);
	for (unsigned int i = 0; i < height; i++) {
		row_pointers[i] = (png_byte*) png_malloc (png_ptr, width * 4);
	}

	if (verbose_flag)
		printf ("Setting color values...\n");
	png_bytep row;
	png_bytep p;
	for (unsigned int h = 0; h < height; h++) {
		row = row_pointers[h];
		for (unsigned int w = 0; w < width; w++) {
			p = &row[w * 4];
			if (use_red)
				p[0] = red;
			else
				p[0] = rand() % 256;
			if (use_green)
				p[1] = green;
			else
				p[1] = rand() % 256;
			if (use_blue)
				p[2] = blue;
			else
				p[2] = rand() % 256;
			if (use_alpha)
				p[3] = alpha;
			else if (alpha_random_flag)
				p[3] = rand() % 256;
			else
				p[3] = 255;

		}
	}

	// 4. Write png file
	if (verbose_flag)
		printf ("Writing png file...\n");
	png_write_info(png_ptr, info_ptr);
	png_write_image(png_ptr, row_pointers);
	png_write_end(png_ptr, NULL);
	png_destroy_write_struct(&png_ptr, &info_ptr);

	return EXIT_SUCCESS;
}

unsigned int strtouint (char* str, int base)
{
	char* x;
	for (x = str; *x; x++)
	{
		if (base == 10 && !isdigit (*x))
			return 0L;
		else if (base == 16 && !isdigit (*x) && !ishex (*x))
			return 0L;
	}
	return (strtoul (str, 0L, base));
}

int ishex (char ch)
{
	char* hex;
	for (hex = "abcdefABCDEF"; *hex; hex++)
	{
		if (ch == *hex)
			return 1;
	}
	return 0;
}
