
#include <stdio.h>

// image and weights must be given as arrays of dim 1

void Conv2D (int *image, int *new_image,  int stride, int img_dim, int kernel, int *weights, int final_dim, int start)
{
  int i;int k;int j; int l=0;
  int *img = &image[0];
  int s=0;
  for (i=start ; i<final_dim +start; i++)
  {
    new_image[i]=0;
    if(img < &image[img_dim-kernel + 1+ l*img_dim]) {img = &image[(i-start)*stride+(kernel-1)*s];}
    else {img = &image[(i-start)*stride + (kernel-1)*(s+1)];l++;s++;}

    for (k=0; k<kernel; k++)
      {
        for ( j=0; j<kernel; j++)
        {
        // printf("pixel %d weight %d  \n",img[j+(k*img_dim)] , weights[j + (k*kernel)]);
        new_image[i] = new_image[i] + img[j+(k*img_dim)]*weights[j + (k*kernel)] ;
        }
      }
    if(img < &image[img_dim-kernel+ 1 + l*img_dim]) {img = &image[(i-start)*stride+(kernel-1)*s+1];}
    else {img = &image[(i-start)*stride+(kernel-1)*(s+1)+1];}
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

void ZeroPadding(int *image, int *new_image, int img_dim, int padding)
{
 for (int k=0; k<img_dim+2*padding; k++)
        { if (k<padding)
          {
            for (int i=0; i<img_dim+2*padding; i++){new_image[i+k*(img_dim+2*padding)]=0;}
          }
          else
          { if (k<img_dim + padding)
            {
            for(int i=0; i<padding; i++){new_image[k*(img_dim+2*padding)+i]=0;}
            for (int i=padding; i<img_dim+padding; i++){new_image[k*(img_dim+2*padding)+i]=image[i-padding+(k-padding)*img_dim];}
            for(int i=img_dim+padding;i<img_dim+2*padding; i++){new_image[k*(img_dim+2*padding)+i]=0;}
            }
            else
            {
            for (int i=0; i<img_dim+2*padding; i++){new_image[i+k*(img_dim+2*padding)]=0;}
            }
          
          }
        }
}
        
void ConvOneChannel(int *input, int *output, int stride, int img_dim,int nbr_kernels, int * total_weights,int kernel,int final_dim)
{
  int weight[kernel*kernel];

  for (int i=0; i<nbr_kernels; i++)
  {
    for (int k =0; k<(kernel*kernel);k++){weight[k]= total_weights[k+i*(kernel*kernel)];}   // we have the i filter
    int start = i*final_dim;
    Conv2D(input,output,stride,img_dim,kernel,weight,final_dim,start);
  }
}

// ndiro conv ta3 each channel m3a ga3 l filters, apres we add ga3 l arrays to one array fih ga3 l channels 

void TotalConv(int *)
{




}



int main()
{
  int image[25] = {1,2,3,4,5, 6,7,8,9,10, 11,12,13,14,15, 16,17,18,19,20, 21,22,23,24,25};

  // ,26,27,28,29,30, 31,32,33,34,35,36 

  int weights[18] = {0,0,0, 0,1,0, 0,0,0, 0,0,0, 0,2,0, 0,0,0}; // 
  int img_dim = 5;
  int kernel = 3;
  int stride = 1;
  int final_dim_padding, final_dim_pooling;
  int padding = 1;
  int new_image[18];


  final_dim_padding = ((int)(img_dim + 2*padding))*((int)(img_dim + 2*padding));
  final_dim_pooling = ((int)((img_dim - kernel)/stride)+1)*((int)((img_dim - kernel)/stride)+1);

  ConvOneChannel(image, new_image, stride, img_dim,2 ,weights, kernel, final_dim_pooling);
  // Conv2D(image, new_image, stride, img_dim, kernel, weights, final_dim_pooling,9);
  
  for(int i=0; i<18;i++){printf("%d",new_image[i]);}
  return 0;
 
}




