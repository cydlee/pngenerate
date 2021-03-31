# pngenerate
CLI png generator

I haven't tested it very much, so it might be unstable. Please report issues if you find any!

## Build (Ubuntu-based Linux distros)
Requires libpng: `sudo apt install libpng-dev` <br>
Build: `gcc pngenerate.c -lpng -o pngenerate`

## Usage
`./pngenerate --help` <br>
Example: `./pngenerate --width 50 --height 50 output.png` -- Creates a 50x50 png with randomly colored pixels
