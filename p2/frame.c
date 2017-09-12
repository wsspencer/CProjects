//Header files we'll use functions from
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
//include our header file which define's the frame's color
#include "frame.h"


//MAKE SURE THESE ERROR VALUES ARE USED FOR ALLLLL ERROR CASES BEFORE YOU FINISH
//Global constants
#define ERRFILE 100
#define ERRHEAD 101
#define ERRIMG 102
#define WEIGHT 255
#define PPM "P3"
#define RGBNUM 3

//Global variables (constants once known)
static int X_PLANE;
static int Y_PLANE;
static int MAX_INTSTY;
static double MAX_DIST;
static double FRAME_RADIUS;
static double FRAME_CENTER_X;
static double FRAME_CENTER_Y;

//Void function for checking correct filetype
void checkFileType() {
    
    char fileType[3];
    scanf("%s", fileType);
    if (strcmp(fileType, PPM) != 0) {
        exit(ERRFILE);
    }
    
}

//Void function for shading a pixel using weight as a function of distance
void shade( int color, int borderColor, double dist) {
    //use dist to shade the "color" param with a percentage of "bordercolor" param, then print its value.
    //(this function will be called three times for each pixel, so only print the color value and a space 
    //once it's been calculated)
    
    //add weight to each color component of the pixel.  Add frame color times some weight, w plus
    //the original pixel color times ( 1 - w ).  Round to the nearest int and that will be the new 
    //shaded color for that pixel
    double w = (dist - FRAME_RADIUS) / (MAX_DIST - FRAME_RADIUS);
    int newColor = round(( borderColor * w ) + ( color * ( 1 - w ) ));
    printf("%3d ", newColor);
}

//Main method to be executed
int main() {
    //ensure image is of type .PPM
    checkFileType();
    
    //read the x and y sizes of the image
    scanf("%d", &X_PLANE);
    scanf("%d", &Y_PLANE);
    if (X_PLANE < 2 || Y_PLANE < 2) {
        exit(ERRHEAD);
    }
    
    //Use x and y planes to determine the radius of the frame and its origin
    //(remember the frame is drawn from the center to the edge, so in an even shorter plane the
    //edges of the frame would lay between the furthest pixels, and in an even shorter plane, 
    //the edges of the frame would not touch the furthest pixels.  Thus the -0.5)
    FRAME_CENTER_X = (X_PLANE / 2.0) - 0.5;
    FRAME_CENTER_Y = (Y_PLANE / 2.0) - 0.5;
    if ( X_PLANE > Y_PLANE ) {
        FRAME_RADIUS = FRAME_CENTER_Y;
    }
    else {
        FRAME_RADIUS = FRAME_CENTER_X;
    }
    
    //We can use Euclidean distance to find the Maximum Distance, since we know the max
    //distance is just the distance from the center to any of the corners, such as 0, 0
    //(Though we won't need absolute values since anthing minus 0 is just that value, so
    //in this case it's essentially just Pythagoras' theorem)
    MAX_DIST = sqrt( pow( FRAME_CENTER_Y, 2 ) + pow( FRAME_CENTER_X, 2 ) );
    
    //read in the maximum color intensity for the image, exit with error status if it's not 255
    scanf("%d", &MAX_INTSTY);
    if (MAX_INTSTY != WEIGHT) {
        exit(ERRHEAD);
    }
    
    //Print the PPM identifier, the width and height, and the intensity
    printf("%s\n", PPM);
    printf("%d %d\n", X_PLANE, Y_PLANE);
    printf("%d\n", MAX_INTSTY);
    
    //loop through the input and if a pixel lies outside our circle in ANY dimension (center_x
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
            
            //scan in our RGB values from a pixel in the ppm file
            check = scanf("%d %d %d ", &red, &green, &blue);
            
            if (check != RGBNUM) {
                //body of image error, missing RGB value
                exit(ERRIMG);
            }
            
            
            //if this pixel's coords are inside our frame (outside our circle), we will shade them
            double distance;
            
            //Shade weight will be a function of distance, so determine distance from center 
            //determine pixel distance from center using Euclidean metric and cartesian coords
            //(pow for squaring results.  x and y will represent the x and y coordinate of every 
            //pixel in the image)
            distance = sqrt((FRAME_CENTER_Y - y ) * (FRAME_CENTER_Y - y) + (FRAME_CENTER_X - x) * (FRAME_CENTER_X - x));
            
            //if distance from center > (or equal to?) our circle's radius, pass values to shade.  Otherwise,
            //do not shade the pixel, print it as normal. (call shade 3 times for R, G, and B values)
            //The border/frame color is given in teh frame.h header file, so we will use the 3 constants
            //provided there for our borderColor values, depending on the pixel color we're working with 
            //at the time.
            if (distance > FRAME_RADIUS) {
                shade(red, FRAME_RED, distance);
                shade(green, FRAME_GREEN, distance);
                shade(blue, FRAME_BLUE, distance);
            }
            else {
                printf("%3d %3d %3d ", red, green, blue);
            }
        }
        //print newline for next row of pixels
        printf("\n");
    }
    
    return 0;
}