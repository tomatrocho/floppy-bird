#incude "tracking.h"


void initTrackedObject(TrackedObject* obj, int hue, int sat, int val, IplImage* img, CvRect* trackZone, int shape, int width, int height)
{
	obj->computeTracking = true;
	obj->h = hue;
	obj->s = sat;
	obj->v = val;
	obj->img = img;
	obj->trackingZone = trackZone;
	obj->shape = shape;
	obj->origin = cvPoint(-1,-1);
	obj->width = width;
	obj->height = height;
}

void enableTracking(TrackedObject* obj)
{
	obj->computeTracking = true;
}

void disableTracking(TrackedObject* obj)
{
	obj->computeTracking = false;
}

void updateTracking(TrackedObject* obj)
{
	if(obj->computeTracking){
		
	}
}


/*
 * Transform the image into a two colored image, one color for the color we want to track, another color for the others colors
 * From this image, we get two datas : the number of pixel detected, and the center of gravity of these pixel
 */
CvPoint binarisation(IplImage* image, int *nbPixels, char* window) {
 
    int x, y;
    IplImage *hsv, *mask;
    IplConvKernel *kernel;
    int sommeX = 0, sommeY = 0;
    *nbPixels = 0;
	
	int zoneWidth = 50;		// The zone width in wich the colour will be tracked
	CvRect roi = cvRect(((image->roi->width/3) - (zoneWidth/2)),0,zoneWidth,image->roi->height);
	
    mask = cvCreateImage(cvGetSize(image), image->depth, 1);	/* Create the mask &initialize it to white (no color detected) */
	
    // Create the hsv image
    hsv = cvCloneImage(image);
    cvCvtColor(image, hsv, CV_BGR2HSV);
 
	
	
    // We create the mask
    cvInRangeS(hsv, cvScalar(h - Htolerance -1, s - Stolerance, 0,0), cvScalar(h + Htolerance -1, s + Stolerance, 255,0), mask);
	
    // Create kernels for the morphological operation
    kernel = cvCreateStructuringElementEx(5, 5, 2, 2, CV_SHAPE_RECT,NULL);
 
    // Morphological opening (inverse because we have white pixels on black background)
    cvDilate(mask, mask, kernel, 2);
    cvErode(mask, mask, kernel, 2);  
    cvSetImageROI(mask,roi);
	
    // We go through the mask to look for the tracked object and get its gravity center
    for(x = mask->roi->xOffset; x < mask->roi->width + mask->roi->xOffset; x++) {
        for(y = mask->roi->yOffset; y < mask->roi->height + mask->roi->yOffset ; y++) { 
 
            // If its a tracked pixel, count it to the center of gravity's calcul
            if((mask->imageData[x+ y*mask->widthStep]) == 255) {
                sommeX += x;
                sommeY += y;
                (*nbPixels)++;
            }
        }
    }
    
    
    
      for(x = mask->roi->xOffset; x < mask->roi->width - mask->roi->xOffset; x++) {
        for(y = mask->roi->yOffset; y < mask->roi->height - mask->roi->yOffset ; y++) { 
 
            // If its a tracked pixel, count it to the center of gravity's calcul
            if(((uchar *)(mask->imageData + y*mask->widthStep))[x] == 255) {
                sommeX += x;
                sommeY += y;
                (*nbPixels)++;
            }
        }
    }
	
	cvResetImageROI(mask);
    // Show the result of the mask image
    if(window != NULL) 
		cvShowImage(window, mask);
   
	cvRectangleR(image,roi,cvScalar(0,0,255,0),1,8,0);
    // We release the memory of kernels
    cvReleaseStructuringElement(&kernel);
 
    // We release the memory of the mask
    cvReleaseImage(&mask);
    // We release the memory of the hsv image
    cvReleaseImage(&hsv);
 
    // If there is no pixel, we return a center outside the image, else we return the center of gravity
    if(*nbPixels > 0)
        return cvPoint((int)(sommeX / (*nbPixels)), (int)(sommeY / (*nbPixels)));
    else
        return cvPoint(-1, -1);
}


/*
 * Get the color of the pixel where the mouse has clicked
 * We put this color as model color (the color we want to tracked)
 */
void getObjectColor(int event, int x, int y, int flags, void *param) {
 
    struct TrackedObject* obj = (struct TrackedObject*)param;	/* tracked object is passed through param */
    CvScalar pixel;
    IplImage *hsv;
 
    if(event == CV_EVENT_LBUTTONUP) {
 
        /* Get the hsv image */
        hsv = cvCloneImage(obj->img);
        cvCvtColor(image, hsv, CV_BGR2HSV);
 
        /* Get the selected pixel */
        pixel = cvGet2D(hsv, y, x);
 
        /* Change the value of the tracked color with the color of the selected pixel */
        obj->h = (int)pixel.val[0];
        obj->s = (int)pixel.val[1];
        obj->v = (int)pixel.val[2];
 
        /* Release the memory of the hsv image */
        cvReleaseImage(&hsv);
 
    }
 
}

void getCurrentPointCoordinates(int event, int x, int y, int flags, void *param){
	struct VolatileRect * workingSpace = (struct VolatileRect *)param;
	if(workingSpace->originDefined == 1 && event == CV_EVENT_MOUSEMOVE){
		CvPoint origin = cvPoint(workingSpace->rect.x,workingSpace->rect.y);
		workingSpace->rect = cvRect(min(x,origin.x),min(y,origin.y),abs(x-origin.x),abs(y-origin.y));
	}
	
	if(event == CV_EVENT_LBUTTONUP){
		printf("click at x=%d \ty=%d\n",x,y);
		if(workingSpace->originDefined){
			CvPoint origin = cvPoint(workingSpace->rect.x,workingSpace->rect.y);
			workingSpace->rect = cvRect(min(x,origin.x),min(y,origin.y),abs(x-origin.x),abs(y-origin.y));
			workingSpace->rectDefined = 1;
		} else {
			workingSpace->rect.x = x;
			workingSpace->rect.y = y;
			workingSpace->originDefined = 1;
		}
	}
}

CvRect initWorkSpace(RaspiCamCvCapture * capture, char* window){
	struct VolatileRect workingSpace;
	struct ImageBroadcast flux;
	
	workingSpace.originDefined = 0;
	workingSpace.rectDefined = 0;
	initImageBroadcast(&flux, NULL,window,NULL);
	
	cvSetMouseCallback(window, getCurrentPointCoordinates, &workingSpace);
	printf("Definition of the working space \n");
	while(workingSpace.rectDefined == 0) {			/* wait for the definition of the workspace */
		updateImageFromCapture(&flux,capture);
		if(workingSpace.originDefined) {
			cvRectangleR(flux.img,workingSpace.rect,cvScalar(0,0,255,0),1,8,0);
		}
			showImage(&flux);		
		char keyPressed = cvWaitKey(30);
		switch (keyPressed){
			case 27:				/* ESC to reset the rectangle origin */
				workingSpace.originDefined = 0;
				break;
		}
	}
	printf("Working space defined\n");
	cvDestroyWindow(window);
	return workingSpace.rect;
}
