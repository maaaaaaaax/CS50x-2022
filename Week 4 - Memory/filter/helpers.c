#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE average = (int) round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sepiaRed = (int) round((0.393 * image[i][j].rgbtRed) + (0.769 * image[i][j].rgbtGreen) + (0.189 * image[i][j].rgbtBlue));
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }

            int sepiaGreen = (int) round((0.349 * image[i][j].rgbtRed) + (0.686 * image[i][j].rgbtGreen) + (0.168 * image[i][j].rgbtBlue));
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }

            int sepiaBlue = (int) round((0.272 * image[i][j].rgbtRed) + (0.534 * image[i][j].rgbtGreen) + (0.131 * image[i][j].rgbtBlue));
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        RGBTRIPLE new_row[width];
        int new_row_cursor = 0;
        int current_pixel_width = width;
        while (current_pixel_width > 0)
        {
            new_row[new_row_cursor] = image[i][current_pixel_width - 1];
            new_row_cursor++;
            current_pixel_width--;
        }
        for (int j = 0; j < width; j++)
        {
            image[i][j] = new_row[j];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE blurred_image[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // for each color value, give it a new value by averaging the color values of neighboring pixels.
            float cells_counted = 0.0;
            int redTotal = 0;
            int greenTotal = 0;
            int blueTotal = 0;
            for (int k = -1; k < 2; k++)
            {
                // if this cell is "in bounds" vertically
                if (((i + k) >= 0) && ((i + k) <= (height - 1)))
                {
                    for (int l = -1; l < 2; l++)
                    {
                        // if this cell is "in bounds" horizontally
                        if (((j + l) >= 0) && ((j + l) <= (width - 1)))
                        {
                            redTotal += image[i + k][j + l].rgbtRed;
                            greenTotal += image[i + k][j + l].rgbtGreen;
                            blueTotal += image[i + k][j + l].rgbtBlue;
                            cells_counted += 1.0;
                        }
                    }
                }
            }
            // Compute the averages and save them into the temp blurred_image 2D array
            blurred_image[i][j].rgbtRed = (int) round(redTotal / cells_counted);
            blurred_image[i][j].rgbtGreen = (int) round(greenTotal / cells_counted);
            blurred_image[i][j].rgbtBlue = (int) round(blueTotal / cells_counted);
        }
    }

    // Overwrite the initial image 2D array with temp data
    for (int m = 0; m < height; m++)
    {
        for (int n = 0; n < width; n++)
        {
            image[m][n] = blurred_image[m][n];
        }
    }

    return;
}
