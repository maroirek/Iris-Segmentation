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
layer 1 : conv : 96 out chan, 11x11 kernel, stride 4, input dim 128, output dim 30
layer 2 : maxpool : 3x3 kernel, stride 2, input dim 30, output dim 14

layer 3 : conv :96 input chan, 16 out chan, 5x5 kernel, stride 1, input dim 14, output dim 14, padding 2
layer 5 : maxpool : input chan 16, output chan 16, kernel 3x3, stride 2, input dim 14, output dim 6

layer 6 : conv : inchan 16, outchan 8, kernel 3x3, stride 1, pad 1, in out dim 6
layer 8 : maxpool : inchan 8, kernel 3x3, stride 2, out dim 3

layer 9 : dense: input 3x3x8, output dim 4096

// paddings are counted as layers tan
*/

// Inputs : 
int image_input[128*128];
int weights_l1[96*11*11];
int weights_l2[16*3*3];
int weights_l3[8*5*5];
int weights_dense[8*3*3*4096];

// Intermediate Outputs
int output_l1[30*30*96]={0};
int output_l2[15*15*96]={0};
int output_l3[(15+2*2)*(15+2*2)*96]={0};
int output_l4[17*17*16]={0};
int output_l5[8*8*16]={0};
int output_l6[(8+2)*(8+2)*16]={0};
int output_l7[6*6*8]={0};
int output_l8[3*3*8]={0};

// Final Output
int output_dense[4096]={0};

// version one of the architecture, then I put g3 les calculs bch l fpga mathsebch 128*128 ... additional useless computation 
ConvOneChannel(image_input, output_l1, 4, 128,96, weights_l1, 11,30*30);
MaxPoolManyChannel(output_l1, output_l2, 2,2, 30,96,15*15);

ZeroPadding(output_l2, output_l3, 15, 2,96,(15+2*2)*(15+2*2));
ConvManyChannel(output_l3, output_l4, 17, 1, 15+2*2,16, weights_l2,3,17*17, 96);
MaxPoolManyChannel(output_l4, output_l5, 2,2, 17,16,8*8);

ZeroPadding(output_l5, output_l6, 8, 1,16,(8+2)*(8+2));
ConvManyChannel(output_l6, output_l7, 6, 1, 8+2,8, weights_l3,5,6*6, 16);
MaxPoolManyChannel(output_l7, output_l8, 2,2, 6,8,3*3);

Dense(output_l8, weights_dense, output_dense, 8*3*3, 4096);

// optimisation : we ommit s or l ; in conv many chan we ommit final dim and replace it in the code by output dim * output dim

return 0;
}