
#include <stdio.h>
#include <math.h>

// image and weights must be given as arrays of dim 1

void conv2D (int *image, int *new_image,  int stride, int img_dim, int kernel, int *weights, int final_dim )
{
    
    int i;int k;int j; int l=0;
    int *img = &image[0];
    int s=0;
    for (i=0 ; i<final_dim ; i++)
    {
      new_image[i]=0;
      
      if(img < &image[img_dim-kernel + 1+ l*img_dim]) {img = &image[i*stride+(kernel-1)*s];}
      else {img = &image[i*stride + (kernel-1)*(s+1)];l++;s++;}

      for (k=0; k<kernel; k++)
        {
          for ( j=0; j<kernel; j++)
          {
          // printf("pixel %d weight %d  \n",img[j+(k*img_dim)] , weights[j + (k*kernel)]);
          new_image[i] = new_image[i] + img[j+(k*img_dim)]*weights[j + (k*kernel)] ;
          }
        }
      if(img < &image[img_dim-kernel+ 1 + l*img_dim]) {img = &image[i*stride+(kernel-1)*s+1];}
      else {img = &image[i*stride+(kernel-1)*(s+1)+1];}
      
    }
}

void MaxPool2D (int *image, int *new_image,  int stride, int img_dim, int kernel, int final_dim )
{
    
    int i;int k;int j; int l=0;
    int *img = &image[0];
    int s=0;
    for (i=0 ; i<final_dim ; i++)
    {
      new_image[i]=0;
      
      if(img < &image[img_dim-kernel + 1+ l*img_dim]) {img = &image[i*stride+(kernel-1)*s];}
      else {img = &image[i*stride + (kernel-1)*(s+1)];l++;s++;}

      for (k=0; k<kernel; k++)
        {
          for ( j=0; j<kernel; j++)
          {
          if(img[j+(k*img_dim)]>new_image[i]){new_image[i] = img[j+(k*img_dim)];}
          }
        }
      if(img < &image[img_dim-kernel+ 1 + l*img_dim]) {img = &image[i*stride+(kernel-1)*s+1];}
      else {img = &image[i*stride+(kernel-1)*(s+1)+1];}
      
    }
}



int main()
{
  int image[36] = {1,2,3,4,5,6, 7,8,9,10,11,12, 13,14,15,16,17,18, 19,20,21,22,23,24, 25,26,27,28,29,30, 31,32,33,34,35,36 };
  int weights[9] = {0,0,0, 0,1,0, 0,0,0};
  int img_dim = 6;
  int kernel = 2;
  int stride = 1;
  int final_dim;

  final_dim = ((int)((img_dim - kernel)/stride)+1)*((int)((img_dim - kernel)/stride)+1);

  printf("image final dim is %d \n", final_dim);

  int new_img[final_dim];


  // test(image);
  MaxPool2D(image, new_img, stride, img_dim, kernel,final_dim) ;
  int i;
  for(i=0; i<final_dim; i++)
  {
   printf("%d \n ",new_img[i]);
 
  }
  
  return 0;
}




