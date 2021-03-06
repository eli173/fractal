//some comment...

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <OpenCL/opencl.h>
#include <SDL/sdl.h>

#define COMPUTE_KERNEL_FILE ("mandelbrot_kernel.cl")
#define COMPUTE_KERNEL_METHOD ("Mandelbrot")

//other functions begin
Uint8 *k_set() {
  SDL_PumpEvents();
  return SDL_GetKeyState(NULL);
}

void k_quit(Uint8 *keys) {
  if (keys[SDLK_ESCAPE]||keys[SDLK_q])
    exit(0);
}
//others end

//cl funcs begin
//cl funcs end

int main (int argc, char **argv) {
  //vars and stuff
  Uint8 *keys;
  //SDL Init block
  if(SDL_Init(SDL_INIT_VIDEO)!=0){
    printf("Error: %s\n",SDL_GetError());
    return -1;
  }
  SDL_Surface *screen;
  screen = SDL_SetVideoMode(1024,768,8,SDL_RESIZABLE);
  if (screen == NULL) {
    printf("Error: %s\n",SDL_GetError());
    return -1;
  }

  int err;
  int xpixels = screen->w;
  int ypixels = screen->h;
  unsigned char *mandelbrot_set;
  mandelbrot_set = malloc(xpixels*ypixels*sizeof(unsigned char));
  
  //NEED TO CHANGE THIS TO ACTUALLY GET THE RANGE A DECENT WAY...
  //GETS MINs,MAXES
  double xmin = -2;
  double xmax = 2;
  double ymin = -1.5;
  double ymax = 1.5;
  //END THAT

  //??
  size_t local;
  size_t global;

  //CL_BEGIN
  //sets up cl source file
  FILE *fp;
  fp = fopen(COMPUTE_KERNEL_FILE,"rb");
  fseek(fp,0,SEEK_END);
  long pos = ftell(fp);
  fseek(fp,0,SEEK_SET);
  /*char *source = malloc(pos);
  fclose(fp);
  fp = fopen(COMPUTE_KERNEL_FILE,"r");
  int flag=0;
  for(long l=0;l<pos;l++) {
    if(!feof(fp)||!flag)
      source[l]=fgetc(fp);
    else {
      flag=1;
      source[l]=' ';
    }
  }*/
  char *source = malloc(pos-1);
  fread(source,pos-1,1,fp);
  fclose(fp);
  //test if file ok, debug
  //printf("File:\n%s\n",source);
  //file's cool
  cl_device_id device_id;
  cl_context context;
  cl_command_queue commands;
  cl_program program;
  cl_kernel kernel;

  cl_mem mset;
  //connect to a compute device
  int gpu = 1;//gpu or cpu flag?
  err = clGetDeviceIDs(NULL,gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU,1,&device_id,NULL);
  if(err!=CL_SUCCESS) {
    printf("Error: Failed to create a device group\n");
    return EXIT_FAILURE;
  }
  //create a compute context
  context = clCreateContext(0,1,&device_id,NULL,NULL,&err);
  if(!context) {
    printf("Error: failed to create a context\n");
    return EXIT_FAILURE;
  }
  //create a command commands
  commands = clCreateCommandQueue(context,device_id,0,&err);
  if(!commands) {
    printf("Error: Failed to create a compute context\n");
    return EXIT_FAILURE;
  }
  //create prog from source
  program = clCreateProgramWithSource(context,1,(const char **) & source,NULL,&err);
  if(!program) {
    printf("Error: failed to create prog\n");
    return EXIT_FAILURE;
  }
  //build the exec
  err = clBuildProgram(program,0,NULL,NULL,NULL,NULL);
  if(err != CL_SUCCESS) {
    size_t len;
    char buffer[2048]; 
    printf("Error: Failed to build program executable!\n");
    clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
    printf("%s\n", buffer);
    exit(1);
  }
  //create the kernel
  kernel = clCreateKernel(program,COMPUTE_KERNEL_METHOD,&err);
  if(!kernel || err != CL_SUCCESS) {
    printf("Error: kernel blah blah blah\n");
    exit(1);
  }
  mset = clCreateBuffer(context, CL_MEM_WRITE_ONLY,sizeof(unsigned char)*ypixels*xpixels,NULL,NULL);
  //set the args to the kernel
  err = 0;
  err=clSetKernelArg(kernel,0,sizeof(cl_mem),&mset);
  err=clSetKernelArg(kernel,1,sizeof(int),&xpixels);
  err=clSetKernelArg(kernel,2,sizeof(int),&ypixels);
  err=clSetKernelArg(kernel,3,sizeof(double),&xmin);
  err=clSetKernelArg(kernel,4,sizeof(double),&xmax);
  err=clSetKernelArg(kernel,5,sizeof(double),&ymin);
  err=clSetKernelArg(kernel,6,sizeof(double),&ymax);
  if (err != CL_SUCCESS) {
    printf("Error:kernel args fail set:%d\n",err);
    exit(1);
  }
  //I dunno anymore, something about a work group?
  err=clGetKernelWorkGroupInfo(kernel,device_id,CL_KERNEL_WORK_GROUP_SIZE,sizeof(local),&local,NULL);
  if(err!=CL_SUCCESS) {
    printf("Error:failed work group\n");
    exit(1);
  }
  //Execute the kernel!!!
  global=xpixels*ypixels;
  err = clEnqueueNDRangeKernel(commands,kernel,1,NULL,&global,&local,0,NULL,NULL);
  if(err) {
    printf("Error:execution of kernal failed :(\n");
    return EXIT_FAILURE;
  }
  //wait for finish
  clFinish(commands);
  //read back the data
  err=clEnqueueReadBuffer(commands,mset,CL_TRUE,0,sizeof(unsigned char)*xpixels*ypixels,mandelbrot_set,0,NULL,NULL);
  if(err!=CL_SUCCESS) {
    printf("Error:output array error:%d\n",err);
    exit(1);
  }
  //CL_END

  //check
  /*for(int m=0;m<ypixels;m++) {
    for(int n=0;n<xpixels;n++) {
      printf("%d\t",mandelbrot_set[n*xpixels+m]);
    }  
    printf("\n");
    }*/
  
  //sets the picture 
  SDL_LockSurface(screen);
  for(int i=0;i<xpixels;i++) {
    for(int j=0;j<ypixels;j++) {
      Uint8 *p = (Uint8 *)screen->pixels +j *screen->pitch +i;
      if(mandelbrot_set[(xpixels*j+i)]==0xff)
	*p = SDL_MapRGB(screen->format,0x00,0x00,0x00);
      else
	*p = SDL_MapRGB(screen->format,0xffff-2*mandelbrot_set[(xpixels*j+i)],0xffff-16*mandelbrot_set[(xpixels*j+i)],0xffff-16*mandelbrot_set[(xpixels*j+i)]);
    }
  }
  SDL_UnlockSurface(screen);
  SDL_UpdateRect(screen,0,0,0,0);

  //display loop
  while(1) {
    SDL_Delay(3);
    keys = k_set();
    k_quit(keys);
  }
  free(source);
  return 0;
}
