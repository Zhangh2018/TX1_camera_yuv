#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include "camera.h"

#define DEV_NAME_LENGTH		50
#define NUM_FRAM			50

int main(int argc, char ** argv) {
    //char *dev_name = "/dev/video0";	//ITU
    //char *dev_name = "/dev/video4";	//UVC
    char *dev_name = NULL;
    FILE * outf = 0;
    unsigned int image_size;

    int camera_type = 0;//0:ITU, 1:UVC	
    int width=1280;
    int height=1080;

    if(argc != 3)
    {
	printf("usage: ./camera /dev/video0 2560x720\n");
	
	return 0;
    }

    	dev_name = (char *)malloc(sizeof(char) * DEV_NAME_LENGTH);
	if(!dev_name)
	{
		printf("malloc mem error\n");
		return -1;
	}

    	memset(dev_name, 0, sizeof(char) * DEV_NAME_LENGTH);
	strcpy(dev_name, argv[1]);
	
	printf("the dev_name is %s\n", dev_name);

	if(!strcmp(dev_name, "/dev/video4"))
	{
		camera_type = 1;
	}
	else
	{
		camera_type = 0;
		printf("camera type is 0\n");
	}


	if(!strcmp(argv[2], "640x480"))
	{
		width = 640;
    		height = 480;
	}
	else if(!strcmp(argv[2], "800x600"))
	{
		width = 800;
                height = 600;
	}
	else if(!strcmp(argv[2], "2560x720"))
	{
		width = 2560;
                height = 720;
	}

    outf = fopen("out.yuv", "wb");
    Camera *camera;
    unsigned char image[width*height*2];

    clock_t starttime, endtime;
    double totaltime;

    camera=new Camera(dev_name,width,height, camera_type);
    if(!camera->OpenDevice()){
        printf("OpenDevice() error\n");	
        return -1;
    }

    printf("camera->OpenDevice()\n");
    
    image_size=camera->getImageSize();
	
    starttime = clock();
    //int frames=50;
    unsigned int writesize=0;
    

    for(int i=0;i<NUM_FRAM;i++){
        if(!camera->GetBuffer(image)){
		printf("camera->GetBuffer error\n");		
            break;
        }
		
        writesize=fwrite(image,1,image_size,outf);
		printf("write yuv filer\n");
        //fflush(outf);
        printf("frame:%d,writesize:%d\n",i,writesize);
    }

	
    endtime = clock();
    totaltime = (double)( (endtime - starttime)/(double)CLOCKS_PER_SEC );
    printf("time :%f, rate :%f\n",totaltime,NUM_FRAM/totaltime);
    camera->CloseDevice();
    fclose(outf);
    return 0;
}

