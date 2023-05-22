#include <stdio.h>

void Relu(int x)
{if(x<0){x=0;}}

void Conv2D (int *image, int *new_image,  int stride, int img_dim, int kernel, int *weights, int final_dim, int start)
{
  //final_dim of only one output image
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

void MaxPool2D (int *image, int *new_image,  int stride, int img_dim, int kernel, int final_dim , int start)
{
  int l=0;
  int *img = &image[0];
  int s=0;
  for (int i=start ; i<final_dim +start; i++)
  {
    new_image[i]=0;
    
    if(img < &image[img_dim-kernel + 1+ l*img_dim]) {img = &image[(i-start)*stride+(kernel-1)*s];}
    else {img = &image[(i-start)*stride + (kernel-1)*(s+1)];l++;s++;}

    for (int k=0; k<kernel; k++)
      {
        for (int j=0; j<kernel; j++)
        {
        if(img[j+(k*img_dim)]>new_image[i]){new_image[i] = img[j+(k*img_dim)];}
        }
      }
    if(img < &image[img_dim-kernel+ 1 + l*img_dim]) {img = &image[(i-start)*stride+(kernel-1)*s+1];}
    else {img = &image[(i-start)*stride+(kernel-1)*(s+1)+1];} 
  }
}

void ZeroPadding2D(int *image, int *new_image, int img_dim, int padding)
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
  //final_dim also ta3 image whda
  int weight[kernel*kernel];

  for (int i=0; i<nbr_kernels; i++)
  {
    for (int k =0; k<(kernel*kernel);k++){weight[k]= total_weights[k+i*(kernel*kernel)];}   // we have the i filter
    int start = i*final_dim;
    Conv2D(input,output,stride,img_dim,kernel,weight,final_dim,start);
  }
}

void ConvManyChannel(int *input, int *output, int output_dim, int stride, int img_dim,int nbr_kernels, int * total_weights,int kernel,int final_dim, int nbr_input_chan)
{  // input fih concatenation ta3 bzf channels
  // output fih concatenation ta3 bzf images => output_dim = final_dim x nbr of kernels (output kernels)

  int input_interm[img_dim*img_dim];  // to iterate the diff input channels 
  int inter[output_dim];   // calcul de le conv d'une image avec tous les filtres to be able to do the sum

  for (int i=0; i<nbr_input_chan; i++)   // for each input chan
  { 
    for (int k =0; k<(img_dim*img_dim);k++){input_interm[k]= input[k+i*(img_dim*img_dim)];}  // one channel as input
    ConvOneChannel(input_interm, inter, stride, img_dim, nbr_kernels, total_weights, kernel, final_dim); // conv of one chan with all filters

    // result of one image with all filters
    for (int j=0; j<output_dim; j++){output[j]=output[j]+inter[j];}  // output must be initilized with zeros out of the function
    
  }

  for (int j=0; j<output_dim; j++) {Relu(output[j]);}  
  // need to be optimized hadi but i think placetha hna 3ala koli 7al

}

void MaxPoolManyChannel(int *input, int *output, int stride,int kernel, int img_dim,int nbr_input_chan,int final_dim)
{ 
  // final dim of pooling of one image
  int input_interm[img_dim*img_dim]; 
  for (int i=0; i<nbr_input_chan; i++)
  {  // we have the i filter
    for (int k =0; k<(img_dim*img_dim);k++){input_interm[k]= input[k+i*(img_dim*img_dim)];} 
    int start = i*final_dim;
    MaxPool2D(input_interm,output,stride,img_dim,kernel,final_dim,start);
  }
}

void ZeroPadding(int *input, int *output, int img_dim, int padding,int nbr_input_chan,int final_dim)
{
  // final dim of padding of one image
  int input_interm[img_dim*img_dim];
  int output_inter[final_dim];
  for (int i=0; i<nbr_input_chan; i++)
  {  // we have the i filter
    for (int k =0; k<img_dim*img_dim;k++){input_interm[k]= input[k+i*img_dim*img_dim];} 
    ZeroPadding2D(input_interm,output_inter,img_dim,padding);
    for (int j =0; j<final_dim;j++){output[j+i*final_dim]= output_inter[j];} 
  }  
}

void Dense(int *input, int * weights, int *output, int input_dim, int output_dim ) // input dim since weights are in an array not in a matrix
{
  for (int i=0; i<output_dim ; i++)
  { 
    output[i]=0;
    for(int j=0; j<input_dim; j++){output[i]= output[i]+ input[j]*weights[j+i*input_dim];}
  }
}


int main()
{
/* Proposed architecture :
layer 1 : conv : 4 out chan, 3x3 kernel, stride 1, input dim 5x5, output dim 3   done
layer 2 : maxpool : 2x2 kernel, stride 1, input dim 3, output dim 2  

layer 3 : conv :4 input chan, 6 out chan, 2x2 kernel, stride 1, input dim 2, output dim 5, padding 2
layer 5 : maxpool : input chan 6, output chan 6, kernel 2x2, stride 1, input dim 5, output dim 4

layer 6 : conv : inchan 6, outchan 3, kernel 2x2, stride 1, pad 1, in out dim 6
layer 8 : maxpool : inchan 3, kernel 2x2, stride 1, out dim 5

layer 9 : dense: input 5x5x3, output dim 10

// paddings are counted as layers tan
*/

int test[10]={[0 ... 9] = 1};

// Inputs : 
int image_input[5*5]= {1,1,1,1,1, 2,2,2,2,2, 3,3,3,3,3, 4,4,4,4,4, 5,5,5,5,5};   // input image done
int weights_l1[4*3*3]={[0 ... 35] = 1};   // conv 1 done 
int weights_l2[6*2*2]={[0 ... 23] = 1};   // conv 2 done 
int weights_l3[3*2*2]={[0 ... 11] = 1};   // conv 3
int weights_dense[3*4*4*10]={[0 ... 479] = 1};  // dense 

// Intermediate Outputs
int output_l1[3*3*4]={0}; // conv1
int output_l2[2*2*4]={0};  // maxpool 1
int output_l3[(2+2*2)*(2+2*2)*4]={0};  // padding 2
int output_l4[5*5*6]={0};   // conv 2
int output_l5[4*4*6]={0};   // maxpool 2
int output_l6[(4+2)*(4+2)*6]={0};  // padding 3
int output_l7[5*5*3]={0};  // conv 3
int output_l8[4*4*3]={0};  // maxpool 3

// Final Output
int output_dense[10]={0};  // dense 

// version one of the architecture, then I put g3 les calculs bch l fpga mathsebch 128*128 ... additional useless computation 

ConvOneChannel(image_input, output_l1, 1, 5,4, weights_l1, 3,3*3);   // it works
MaxPoolManyChannel(output_l1, output_l2, 1,2, 3,4,2*2);   // iiiiit works

ZeroPadding(output_l2, output_l3, 2, 2,4,(2+2*2)*(2+2*2));     // it wooorks
ConvManyChannel(output_l3, output_l4, 5*5*6, 1, 6,6, weights_l2,2,5*5, 4); // it wooooooorks ha ha ha
MaxPoolManyChannel(output_l4, output_l5, 1,2, 5 ,6,4*4);   // iiiit woooorks tan nnniceeeee

ZeroPadding(output_l5, output_l6, 4, 1,6,(4+2)*(4+2));    // doooooone working
ConvManyChannel(output_l6, output_l7, 5*5*3, 1, 4+2,3, weights_l3,2,5*5,6); // done
MaxPoolManyChannel(output_l7, output_l8, 1,2, 5,3,4*4);  // dooooone

Dense(output_l8, weights_dense, output_dense, 3*4*4, 10);  // wwwwoooooorkiiiiiiing yey



for (int i=0; i<10; i++)
{
printf("%d \n", output_dense[i]);
}

return 0;
}