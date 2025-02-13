> Tene Victor-Gabriel, 315CA

# Command Line Image Editor

A simple, yet efficient command line image editor implemented entirely in C.
This provides a couple of image manipulation features, such as cropping,
rotating, equalization, filters and an intensity histogram, described in more 
detail in the [Features](#features) section.

## Table of Contents
- [Introduction](#introduction)
- [Installation](#installation)
- [Features](#features)
- [Commands and Examples](#commands-and-examples)
- [Contributions](#contributions)

## Introduction
This project is a command line image editor implemented entirely in C. It
enables users to perform image manipulation tasks, such as cropping,
rotating, equalization, filters and an intensity histogram directly from the
terminal in an easy to use command line interface. Whether you are a developer
or a terminal enthusiast, the ***Command Line Image Editor*** is the ideal
image manipulation tool for you.

## Installation

### Prerequisites
- C compiler (e.g.: `gcc`, `clang`)
- `Make` utility

### Steps
1. **Clone the repository**
```sh
git clone <repository-url>
cd <repository-url>
```

2. **Build the project**
```sh
make build
```

3. **Run the executable**
```sh
./image_editor
```

### Notes
- Make sure you have the latest version of the C compiler and the Make utility
installed.
- Ensure that the executable has the necessary permissions to run properly.

## Features

- **Load Image**: Load an image from a file
	1. Clears and deallocates any memory used for the previous loaded image
	(if existed).
	2. Assures that the provided file exists.
	3. Loads the image header (contains the image metadata).
	4. Loads the image body (contains the data for each pixel of the image).
	5. Sets the current selection area to the whole image.

- **Select Area**: Select a rectangular area of the image
	1. Determines the coordinates for the new selection area.
	2. Assures that these coordinates are valid.
	3. Changes the current selection area to the new one.

- **Rotate Image**: Rotate the selected area by a specified angle
	1. Obtains the rotation angle and checks its validity. The valid angles are:
		- *0˚*
		- *±90˚*
		- *±180˚*
		- *±270˚*
		- *±360˚*
	2. Checks whether the selected area is valid for rotation:
		- a square submatrix of the image
		- the whole image
	3. Determines the number of 90˚ rotations that need to be performed in
	order to complete the specified rotation.
	4. Rotates the image body matrix accordingly.

- **Crop Image**: Crop the selected area of the image
	1. Makes a copy of the current image.
	2. Tries to resize the image to its new dimensions (after cropping).
	3. Fills the newly resized image with the pixel data from the selection 
	area of the image copy.
	4. Resets the current selection area to the whole cropped image.
	5. Clears and deallocates the memory used for the image copy.

- **Intensity Histogram**: Create the intensity histogram of an image
*(only PGM images supported)*
	1. Assures the image format is PGM.
	2. Checks the validity of the provided histogram parameters:
		- the maximum height of a bar
		- the number of bins (a power of 2 between 2 and 256)
	3. Calculates the histogram data.
	4. Prints the histogram.

- **Equalize Image**: Equalize an image *(only PGM images supported)*
	1. Assures the image format is PGM.
	2. Calculates the intensity histogram of the image.
	3. Applies the equalization process to each pixel of the image.

- **Apply Filter**: Apply various filters to the selected area of the image
*(only PPM images supported)*
	1. Assures the image format is PPM.
	2. Checks the validity of the provided filter. The valid filters are:
		- *edge detection (EDGE)*
		- *sharpen (SHARPEN)*
		- *box blur - normalized (BLUR)*
		- *gaussian blur 3x3 - approximation (GAUSSIAN_BLUR)*
	3. Defines the image kernels and determines the corresponding kernel type to
	to the provided filter.
	4. Makes a copy of the current image.
	5. Overwrites the selection area of the image copy with the new pixels
	(calculated after applying the specified filter).
	6. Modifies the original image accordingly.
	7. Clears and deallocates the memory used for the image copy.

- **Save Image**: Save the image to a file *(using ASCII/binary format)*
	1. Determines the file name and the saving format (ASCII/binary).
	2. Save the image header.
	3. Save the image body.

- **Exit**: Quit the program
	1. Clears and deallocates any memory used during runtime.
	2. Stops the input loop.

## Commands and Examples

- `LOAD <filename>`: Load an image from a file
```sh
LOAD example.pgm
```
- `SELECT <parameters>`
	- `SELECT <x1> <y1> <x2> <y2>`: Select a rectangular area of the image
  	- `SELECT ALL`: Select the whole image
```sh
SELECT 0 0 100 100
```
```sh
SELECT ALL
```
- `ROTATE <angle>`: Rotate the selected area by a specified angle
```sh
ROTATE 270
```
- `CROP`: Crop the selected area of the image
```sh
CROP
```
- `HISTOGRAM <bar_height> <bin_count>`: Create the intensity histogram of an image
```sh
HISTOGRAM 40 32
```
- `EQUALIZE`: Equalize an image
```sh
EQUALIZE
```
- `APPLY <filter>`: Apply various filters to the selected area of the image
```sh
APPLY EDGE
```
- `SAVE <filename> [ascii]`: Save the image to a file (the `ascii` flag determines the saving format)
```sh
SAVE output.ppm ascii
```
```sh
SAVE output.ppm
```
- `EXIT`: Quit the program
```sh
EXIT
```

## Notes
- The ***Command Line Image Editor*** supports the following image formats:
	- portable bitmap format (**PBM**)
	- portable graymap format (**PGM**)
	- portable pixmap format (**PPM**)
- Any other image format needs a prior conversion to one of the supported
formats before use.
- All commands are available after running the `image_editor` executable.

## Contributions
Any contributions to the project are welcomed and appreciated. Whether you're
fixing bugs, enhancing the documentation or implementing new features that
could improve this project, your efforts and passion will be held in high
regard.
