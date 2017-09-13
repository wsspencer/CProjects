/**
    @file frame.c
    @author Scott Spencer (wsspence)

    This program takes a .ppm image and "frames" it with a 1970s style color gradient.  This
    frame will not encroach on a circle in the center of the image with a radius set to about
    half the distance of the shortest plane (it will bisect the furthest pixels on a plane of
    an odd number of pixels, and barely miss the furthest pixels on a plane of even pixels).
    The frame's color is determined by a header file called frame.h and the weight of that
    frame's color applied to a pixel will be determined by it's percentage of distance between
    the edge of the frame and the furthest pixel from the center of the image (any corner).
*/


/** Header file containing standard input/output functions we will use. */
#include <stdio.h>
/** Header file containing standard library functions we will use. */
#include <stdlib.h>
/** Header file containing math functions we will use. */
#include <math.h>
/** Header file containing string functions we will use. */
#include <string.h>
/** Header file containing the frame's color definition. */
#include "frame.h"


//Global constants.
/** Constant for exit status when there is a file type error. */
#define ERRFILE 100
/** Constant for exit status when there is another header error. */
#define ERRHEAD 101
/** Constant for exit status when there is an image error. */
#define ERRIMG 102
/** Constant for checking the max intensity. */
#define WEIGHT 255
/** Constant for the number of color definitions in a ppm pixel. */
#define RGBNUM 3
/** Constant for bisecting the X or Y coordinate plane. */
#define INHALF 2.0
/** Constant for the value of one-half pixel distance. */
#define HALFPIXEL 0.5
/** Constant for the proper first line of a .ppm file. */
#define PPM "P3"

//Global variables (constants once known).
/** Variable for the distance of the X coordinate plane. */
static int X_PLANE;
/** Variable for the distance of the Y coordinate plane. */
static int Y_PLANE;
/** Variable for the maximum intensity of the input file. */
static int MAX_INTSTY;
/** Variable for the maximum distance of a pixel from the center of the input image. */
static double MAX_DIST;
/** Variable for the radius of our frame's "window" (inner circle). */
static double FRAME_RADIUS;
/** Variable for the X coordinate of the image's center point. */
static double FRAME_CENTER_X;
/** Variable for the Y coordinate of the image's center point. */
static double FRAME_CENTER_Y;


/**
    This function is meant to check our input file to ensure it's the correct file type.
    This is known by whether or not the file begins with "P3."

    @return void
*/
void checkFileType() {
    //If the first two chars of input are not p3, it is not a ppm image.
    char fileType = getchar();
    if (fileType != 'P') {
        exit(ERRFILE);
    }
    else {
        fileType = getchar();
        if (fileType != '3') {
            exit(ERRFILE);
        }
    }
}

/**
    This function shades the parameterized color by altering its value based on the percentage
    of distance its pixel is from the edge of the frame.  Towards the edges the pixels in the frame
    become entirely the frame color, but closer to the window they are mostly their original color
    with a small percentage of the frame color added.  We accomplish this by altering the RGB
    values of each color of each pixel according to the R, G, or B value of the border color and
    the pixel's distance.

    @param color int representing the RGB color that will have weight applied to it.
    @param borderColor int representing the RGB color of our frame.
    @param dist double representing the distance from the edge of the frame this pixel is.
    @return void
*/
void shade( int color, int borderColor, double dist) {
    //Add weight to each color component of the pixel.  Add frame color times some weight, w plus
    //the original pixel color times ( 1 - w ).  Round to the nearest int and that will be the new
    //shaded color for that pixel.
    double w = (dist - FRAME_RADIUS) / (MAX_DIST - FRAME_RADIUS);
    int newColor = round(( borderColor * w ) + ( color * ( 1 - w ) ));
    printf("%3d ", newColor);
}

/**
    This is our main function which executes at run.  It will read our user's input, delegate
    params to our helper functions as needed, and ultimately print the output .ppm file.

    @return int for the exit status (either failure or success).
*/
int main() {
    //Ensure image is of type .PPM.
    checkFileType();

    //Read the x and y sizes of the image.
    //If they don't parse as ints or are less than 2, exit with header error.
    int check;
    check = scanf("%d %d", &X_PLANE, &Y_PLANE);
    if (check != 2 || X_PLANE < 2 || Y_PLANE < 2) {
        exit(ERRHEAD);
    }

    //Use x and y planes to determine the radius of the frame and its origin
    //(Remember the frame is drawn from the center to the edge, so in an even shorter plane the
    //edges of the frame would lay between the furthest pixels, and in an even shorter plane,
    //the edges of the frame would not touch the furthest pixels.  Thus the -0.5).
    FRAME_CENTER_X = (X_PLANE / INHALF) - HALFPIXEL;
    FRAME_CENTER_Y = (Y_PLANE / INHALF) - HALFPIXEL;
    if ( X_PLANE > Y_PLANE ) {
        FRAME_RADIUS = FRAME_CENTER_Y;
    }
    else {
        FRAME_RADIUS = FRAME_CENTER_X;
    }

    //We can use Euclidean distance to find the Maximum Distance, since we know the max
    //distance is just the distance from the center to any of the corners, such as 0, 0
    //(Though we won't need absolute values since anthing minus 0 is just that value, so
    //in this case it's essentially just Pythagoras' theorem).
    MAX_DIST = sqrt( pow( FRAME_CENTER_Y, 2 ) + pow( FRAME_CENTER_X, 2 ) );

    //Read in the maximum color intensity for the image, exit with error status if it's not 255.
    scanf("%d", &MAX_INTSTY);
    if (MAX_INTSTY != WEIGHT) {
        exit(ERRHEAD);
    }
    
    //Print the PPM identifier, the width and height, and the intensity
    printf("%s\n", PPM);
    printf("%d %d\n", X_PLANE, Y_PLANE);
    printf("%d\n", MAX_INTSTY);

    //Loop through the input and if a pixel lies outside our circle in ANY dimension (center_x
    //+ radius OR center_y + radius) we will alter its color weight by the % distance it is from
    //the center to the maximum distance. (The corners will be the 4 maximum distances).  If it
    //lay within the circle, we will not alter its coloring.
    for (int y = 0; y < Y_PLANE; y++) {
        for (int x = 0; x < X_PLANE; x++) {
            //scan in the pixel's three values (y and x are its coordinates)
            int red;
            int green;
            int blue;
            int check;

            //Scan in our RGB values from a pixel in the ppm file.
            check = scanf("%d %d %d", &red, &green, &blue);

            if (check != RGBNUM) {
                //Body of image error, missing RGB value.
                exit(ERRIMG);
            }

            //If this pixel's coords are inside our frame (outside our circle), we will shade them.
            double distance;

            //Shade weight will be a function of distance, so determine distance from center
            //determine pixel distance from center using Euclidean metric and cartesian coords
            //(pow for squaring results.  x and y will represent the x and y coordinate of every
            //pixel in the image).
            distance = sqrt((FRAME_CENTER_Y - y ) * (FRAME_CENTER_Y - y) + (FRAME_CENTER_X - x) *
                                (FRAME_CENTER_X - x));

            //If distance from center > (or equal to?) our circle's radius, pass values to shade.
            //Otherwise, do not shade the pixel, print it as normal. (call shade 3 times for R, G,
            //and B values) The border/frame color is given in teh frame.h header file, so we will
            //use the 3 constants provided there for our borderColor values, depending on the pixel
            //color we're working with at the time.
            if (distance > FRAME_RADIUS) {
                shade(red, FRAME_RED, distance);
                shade(green, FRAME_GREEN, distance);
                shade(blue, FRAME_BLUE, distance);
            }
            else {
                printf("%3d %3d %3d ", red, green, blue);
            }
        }
        //Print newline for next row of pixels.
        printf("\n");
    }
    return 0;
}
