#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //generates a grey value by taking an average of the R G & B values
            int Grey_val = (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue)/3;
            image[i][j].rgbtBlue = Grey_val;
            image[i][j].rgbtRed = Grey_val;
            image[i][j].rgbtGreen = Grey_val;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0, h = height; i < h; i++)
    {
        for (int j = 0, w = width/2; j < w; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - (1+j)];
            image[i][width-j] = temp; 
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // these four variables are used to deal with values that would be out of bounds when taking average of adjacent cells
    int max_i;
    int min_i;
    int max_j;
    int min_j;
    // these 2 variables are used to determine the denominator (mult) when calculating averages. They are either 2 or 3
    int mult_i;
    int mult_j;
    //multi is multi_i * multi_j meaning values are either 4 (corner case), 6 (edge case), or 9 (all other cases)
    int mult;
    //the sum of all the red, green and blue values of all adjacent pixels respectively
    int tot_red;
    int tot_green;
    int tot_blue;
    //scans first row of pixles
    for (int i = 0, h = height; i < h; i++)
    {
        //sets values of max, min and multi_i
        //corner case
        if (i == 0)
        {
            max_i = i + 1;
            min_i = i;
            mult_i = 2;
        }
        //edge case
        if (i == h - 1)
        {
            max_i = i;
            min_i = i - 1;
            mult_i = 2;
        }
        //all other cases (not sure why an else statment doest work here)
        if (i > 0 && i < h -1)
        {
            max_i = i + 1;
            min_i = i - 1;
            mult_i = 3;
        }
        //similarly for each column of pixels
        for (int j = 0, w = width; j < w; j++)
        {
            tot_red = 0;
            tot_green = 0;
            tot_blue = 0;
            if (j == 0)
            {
                max_j = j + 1;
                min_j = j;
                mult_j = 2;
            }
            if (j == w - 1)
            {
                max_j = j;
                min_j = j - 1;
                mult_j = 2;
            }
            if (j > 0 && j < w - 1)
            {
                max_j = j + 1;
                min_j = j - 1;
                mult_j = 3;
            }
            //determines the number of pixels used to blur according to the assignment outline
            mult = mult_j * mult_i;
            //this scans the appropriate adjacent pixels omiting the values that fall out of bounds
            for (int k = min_i; k <= max_i; k++)
            {
                for (int l = min_j; l <= max_j; l++ )
                {
                    //totals the red green and blue values of each of the adjacent pixels (including origional pixel)
                    tot_red += image[k][l].rgbtRed;
                    tot_green += image[k][l].rgbtGreen;
                    tot_blue += image[k][l].rgbtBlue;
                }
            }
            //replaces each value of rgb with the average value and rounds it to an int
            image[i][j].rgbtRed = round(tot_red/mult);
            image[i][j].rgbtGreen = round(tot_green/mult);
            image[i][j].rgbtBlue = round(tot_blue/mult);

        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}