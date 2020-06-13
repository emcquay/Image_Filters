#include "helpers.h"
#include <math.h>


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float Grey_tot = 0;
    float Grey_val = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //generates a grey value by taking an average of the R G & B values
            Grey_tot = image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue;
            Grey_val = round(Grey_tot / 3);
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
    RGBTRIPLE temp;
    for (int i = 0, h = height; i < h; i++)
    {
        for (int j = 0, w = width / 2; j < w; j++)
        {
            temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp; 
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
    float tot_red;
    float tot_green;
    float tot_blue;
    RGBTRIPLE temp[height][width];
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
        if (i > 0 && i < h - 1)
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
                for (int l = min_j; l <= max_j; l++)
                {
                    //totals the red green and blue values of each of the adjacent pixels (including origional pixel)
                    tot_red += image[k][l].rgbtRed;
                    tot_green += image[k][l].rgbtGreen;
                    tot_blue += image[k][l].rgbtBlue;
                }
            }
            //replaces each value of rgb with the average value and rounds it to an int
            temp[i][j].rgbtRed = round(tot_red / mult);
            temp[i][j].rgbtGreen = round(tot_green / mult);
            temp[i][j].rgbtBlue = round(tot_blue / mult);

        }
    }
    for (int i = 0, h = height; i < h; i++)
    {
        for (int j = 0, w = width; j < w; j++)
        {
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //creates arrays for Gx values (horizontal direction) that are used to determine the value of adjacent pixels is different enough to be declared an edge
    int Gx[3][3] = 
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    //these will hold the sum of the product of Gx[i][j] and pixel[i][j] for each colour
    int Gx_sumR = 0;
    int Gx_sumG = 0;
    int Gx_sumB = 0;
    //similarly for Gy (vertical direction)
    int Gy[3][3] = 
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };
    int Gy_sumR = 0;
    int Gy_sumG = 0;
    int Gy_sumB = 0;
    //these will hold the sqrt of Gx^2 + Gy^2 (i was having problems with them being ints since the value can be greater than 255)
    double Groot = 0;
    double Rroot = 0;
    double Broot = 0;
    //pixel is the 3x3 array of pixels that surround the pixel being calculated
    RGBTRIPLE pixel[3][3];
    //tempary array that will hold a copy of the image without changing the values of the pixels needed for calculations
    RGBTRIPLE temp[height][width];
    //scan each row and coloumn of pixels
    for (int i = 0, h = height; i < h; i++)
    {
        for (int j = 0, w = width; j < w; j++)
        {
            //create a sub array with the values of rgb values @ [i - 1][j - 1] to [i + 1][j + 1] and store them at pixel[a][b]
            for (int k = i - 1, a = 0; k <= i + 1; k++, a++)
            {
                for (int l = j - 1, b = 0; l <= j + 1; l++, b++)
                {
                    //if the pixel lays on the edge of the ivalue of k or l will lay out side the bounds of the array so for pixel[a][b] we set the rgb values to 0 instead of reading from the image
                    if (k == -1 || l == -1 || k == h || l == w)
                    {
                        pixel[a][b].rgbtRed = 0;
                        pixel[a][b].rgbtGreen = 0;
                        pixel[a][b].rgbtBlue = 0;
                    }
                    //otherwse the rgb values of pixel[a][b] are equal to the values of the image array at k and l
                    else if (0 <= k && k < h && 0 <= l && l < w)
                    {
                        pixel[a][b].rgbtRed = image[k][l].rgbtRed;
                        pixel[a][b].rgbtGreen = image[k][l].rgbtGreen;
                        pixel[a][b].rgbtBlue = image[k][l].rgbtBlue;
                    }
                    // needed another loop to multiply gx[][] and pixel[][] arrays and take the sum of the result
                    for (int m = 0; m < 3; m++)
                    {
                        for (int n = 0; n < 3; n++)
                        {
                            //get sum of Gx values
                            Gx_sumR += (Gx[m][n] * pixel[m][n].rgbtRed);
                            Gx_sumG += (Gx[m][n] * pixel[m][n].rgbtGreen);
                            Gx_sumB += (Gx[m][n] * pixel[m][n].rgbtBlue);
                                
                            //get sum of Gy Values
                            Gy_sumR += (Gy[m][n] * pixel[m][n].rgbtRed);
                            Gy_sumG += (Gy[m][n] * pixel[m][n].rgbtGreen);
                            Gy_sumB += (Gy[m][n] * pixel[m][n].rgbtBlue);
                                
                        }
                    }
                    //this is not elegantly writen but it takes the sqrt of gx_sum^2 + gy_sum^2 and rounds it to the nearest int
                    Rroot = round(sqrt(pow(Gx_sumR, 2) + pow(Gy_sumR, 2)));
                    //however it is possible to exceed 255 this way so we set a cap on rgb values at 255
                    if (Rroot > 255)
                    {
                        Rroot = 255;
                    }
                    temp[i][j].rgbtRed = Rroot;
                    Groot = round(sqrt(pow(Gx_sumG, 2) + pow(Gy_sumG, 2)));
                    if (Groot > 255)
                    {
                        Groot = 255;
                    }
                    temp[i][j].rgbtGreen = Groot;
                    Broot = round(sqrt(pow(Gx_sumB, 2) + pow(Gy_sumB, 2)));
                    if (Broot > 255)
                    {
                        Broot = 255;
                    }
                    temp[i][j].rgbtBlue = Broot;
                    //here i rest the Gy and Gx sums to 0 so that the next pixel can be computed
                    Gy_sumR = 0;
                    Gy_sumG = 0;
                    Gy_sumB = 0;
                    Gx_sumR = 0;
                    Gx_sumG = 0;
                    Gx_sumB = 0;
                    
                    
                }
            }
        }
    }
    //copies the values from the temp image to the final image that will be rendered
    for (int i = 0, h = height; i < h; i++)
    {
        for (int j = 0, w = width; j < w; j++)
        {
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
        }
    }
    return;
}
