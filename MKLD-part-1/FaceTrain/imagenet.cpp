/*
 ******************************************************************
 * HISTORY
 * 15-Oct-94  Jeff Shufelt (js), Carnegie Mellon University
 *      Prepared for 15-681, Fall 1994.
 *
 ******************************************************************
 */

#include "stdafx.h"

#include <stdio.h>
#include "pgmimage.h"
#include "backprop.h"

extern void exit();

#define TARGET_HIGH 0.9
#define TARGET_LOW 0.1


/*** This is the target output encoding for a network with one output unit.
     It scans the image name, and if it's an image of me (js) then
     it sets the target unit to HIGH; otherwise it sets it to LOW.
     Remember, units are indexed starting at 1, so target unit 1
     is the one to change....  ***/

void load_target(IMAGE *img,BPNN *net)
{
  int scale;
  char userid[40], head[40], expression[40], eyes[40], photo[40];

  userid[0] = head[0] = expression[0] = eyes[0] = photo[0] = '\0';

  /*** scan in the image features ***/
  sscanf(NAME(img), "%[^_]_%[^_]_%[^_]_%[^_]_%d.%[^_]",
    userid, head, expression, eyes, &scale, photo);

  char *p=NULL;
  p = strrchr(eyes,'.');
  if(p!=NULL)
  {
	    userid[0] = head[0] = expression[0] = eyes[0] = photo[0] = '\0';

	    sscanf(NAME(img), "%[^_]_%[^_]_%[^_]_%[^.].%[^_]",
    userid, head, expression, eyes, photo);
		scale = 1;
  }

  p = strrchr(userid,'\\');
  if(p!=NULL)
	  p++;


  //if (!strcmp(p, "glickman")) {
  //对下面的内容进行了修改
  //if (!strcmp(eyes, "sunglasses")) {
  //  net->target[1] = TARGET_HIGH;  /* it's me, set target to HIGH */
  //} else {
  //  net->target[1] = TARGET_LOW;   /* not me, set it to LOW */
  //}
  //memset(net->target, 0, sizeof(net->target)*(net->output_n+1));
  for(int i=1; i<=net->output_n; i++) {
	net->target[i] = 0;
  }
  if (!strcmp(head, "straight")) {
    net->target[1] = 1;  /* it's me, set target to HIGH */
  } else if (!strcmp(head, "left")) {
    net->target[2] = 1;  /* it's me, set target to HIGH */
  } else if (!strcmp(head, "right")) {
    net->target[3] = 1;  /* it's me, set target to HIGH */
  } else if (!strcmp(head, "up")) {
    net->target[4] = 1;  /* it's me, set target to HIGH */
  }
}


/***********************************************************************/
/********* You shouldn't need to change any of the code below.   *******/
/***********************************************************************/

void load_input_with_image(IMAGE *img,BPNN *net)
{
  double *units;
  int nr, nc, imgsize, i, j, k;

  nr = ROWS(img);
  nc = COLS(img);
  imgsize = nr * nc;;
  if (imgsize != net->input_n) {
    printf("LOAD_INPUT_WITH_IMAGE: This image has %d pixels,\n", imgsize);
    printf("   but your net has %d input units.  I give up.\n", net->input_n);
    exit (-1);
  }

  units = net->input_units;
  k = 1;
  for (i = 0; i < nr; i++) {
    for (j = 0; j < nc; j++) {
      units[k] = ((double) img_getpixel(img, i, j)) / 255.0;
      k++;
    }
  }
}
