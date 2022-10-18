#include <X11/Xlib.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define DIM 512

/******************************************************************/
/* This structure contains the coordinates of a box drawn with    */
/* the left mouse button on the image window.                     */
/* roi.x , roi.y  - left upper corner's coordinates               */
/* roi.width , roi.height - width and height of the box           */
/******************************************************************/
extern XRectangle roi;

void generateSubImage(startX, startY, width, height, image, ret) int startX;
int startY;
int width;
int height;
unsigned char image[DIM][DIM];
int ret[width][height];
{
  int x = 0, y = 0, xLimit = floor(width / 2), yLimit = floor(height / 2);
  for (x = startX - xLimit; ((x + xLimit) - startX) < width; x++) {
    for (y = startY - yLimit; ((y + yLimit) - startY) < height; y++) {
      if ((x >= 0 && x < DIM) && (y >= 0 && y < DIM)) {
        ret[((x + xLimit) - startX)][((y + yLimit) - startY)] = image[x][y];
      } else {
        ret[((x + xLimit) - startX)][((y + yLimit) - startY)] = 0;
      }
    }
  }
}

void generateTemplate(startX, startY, width, height, image, ret) int startX;
int startY;
int width;
int height;
unsigned char image[DIM][DIM];
int ret[width][height];
{
  int i = 0, k = 0, goalX = startX + width, goalY = startY + height;
  for (i = startX; i < goalX; i++) {
    for (k = startY; k < goalY; k++) {
      ret[i - startX][k - startY] = image[i][k];
    }
  }
}

double getStd(x, y, mean, image) int x, y;
double mean;
int image[x][y];
{
  int i = 0, k = 0;
  double std = 0;
  // Find the std dev
  for (i = 0; i < x; i++) {
    for (k = 0; k < y; k++) {
      std += pow((image[i][k] - mean), 2);
    }
  }
  return sqrt(std / (x * y));
}

double mean(x, y, arr) int x, y;
int arr[x][y];
{
  int mean = 0, i = 0, k = 0;
  for (i = 0; i < x; i++) {
    for (k = 0; k < y; k++) {
      mean += arr[i][k];
    }
  }
  return (mean / (x * y));
}

int normalizeSection(width, height, mean, std, tempMean, tempStd, subImage,
                     template) int width,
    height;
double mean, std, tempMean, tempStd;
int subImage[width][height], template[width][height];
{
  int x = 0, y = 0, sum = 0;
  for (x = 0; x < width; x++) {
    for (y = 0; y < height; y++) {
      sum += (subImage[x][y] - mean) * (template[x][y] - tempMean);
    }
  }
  return round(sum / (std * tempStd));
}

/******************************************************************/
/* Main processing routine. This is called upon pressing the      */
/* Process button of the interface.                               */
/* image  - the original greyscale image                          */
/* size   - the actual size of the image                          */
/* proc_image - the image representation resulting from the       */
/*              processing. This will be displayed upon return    */
/*              from this function.                               */
/******************************************************************/
void process_image(image, size, proc_img) unsigned char image[DIM][DIM];
int size[2];
unsigned char proc_img[DIM][DIM];
{
  printf("processing image:\n");
  int template[roi.width][roi.height];
  generateTemplate(roi.x, roi.y, roi.width, roi.height, image, template);
  double templateMean = 0, imageMean = 0, imageStd = 0;
  printf("template = %hu, %hu\n", roi.width, roi.height);
  // Find template mean.
  for (int i = 0; i < roi.width; i++) {
    for (int k = 0; k < roi.height; k++) {
      templateMean += template[i][k];
    }
  }
  templateMean /= (roi.width * roi.height);
  double templateStd = getStd(roi.width, roi.height, templateMean, template);
  // Normalize template.
  double sum = 0, max = 0;
  for (int i = 0; i < roi.width; i++) {
    for (int k = 0; k < roi.height; k++) {
      template[i][k] = (template[i][k] - templateMean) / templateStd;
      if (max < template[i][k])
        max = template[i][k];
    }
  }
  // Normalize template between 255
  for (int i = 0; i < roi.width; i++) {
    for (int k = 0; k < roi.height; k++) {
      template[i][k] = fabs(round(template[i][k] * (255 / max)));
      sum += template[i][k];
    }
  }
  // Find new mean and std
  templateMean = sum / (roi.width * roi.height);
  templateStd = getStd(roi.width, roi.height, templateMean, template);
  max = 0;
  // Normalize Image in sections:
  int subImage[roi.width][roi.height];
  int tempImg[DIM][DIM];
  max = 0;
  for (int i = 0; i < size[0]; i++) {
    for (int k = 0; k < size[1]; k++) {
      // Calculate mean and standard at each iteration.
      // Generate Sub image:
      generateSubImage(i, k, roi.width, roi.height, image, subImage);
      imageMean = mean(roi.width, roi.height, subImage);
      imageStd = getStd(roi.width, roi.height, imageMean, subImage);
      // Normalize the image using the new mean and standard and template.
      if (imageStd > 0.00) {
        tempImg[i][k] = abs(normalizeSection(roi.width, roi.height, imageMean,
                                             imageStd, templateMean,
                                             templateStd, subImage, template));
      } else {
        tempImg[i][k] = 0;
      }
      if (max < tempImg[i][k]) {
        max = tempImg[i][k];
      }
    }
  }
  // Convultion of template over image:
  // Load proc_img:
  for (int i = 0; i < DIM; i++) {
    for (int k = 0; k < DIM; k++) {
      proc_img[i][k] = round(tempImg[i][k] * (255 / max));
    }
  }
}
