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

extern char *malloc();
extern char *realloc();
extern char *strcpy();
void imgl_munge_name(char *buf);

char *img_basename(char *filename)
{
  char *newptr, *part;
  int len, dex;

  len = strlen(filename);  dex = len - 1;
  while (dex > -1) {
    if (filename[dex] == '/') {
      break;
    } else {
      dex--;
    }
  }
  dex++;
  part = &(filename[dex]);
  len = strlen(part);
  newptr = (char *) malloc ((unsigned) ((len + 1) * sizeof (char)));
  strcpy(newptr, part);
  return(newptr);
}


IMAGE *img_alloc()
{
  IMAGE *newptr;

  newptr = (IMAGE *) malloc (sizeof (IMAGE));
  if (newptr == NULL) {
    printf("IMGALLOC: Couldn't allocate image structure\n");
    return (NULL);
  }
  newptr->rows = 0;
  newptr->cols = 0;
  newptr->name = NULL;
  newptr->data = NULL;
  return (newptr);
}


IMAGE *img_creat(char *name, int nr, int nc)
{
  int i, j;
  IMAGE *newptr;

  newptr = img_alloc();
  newptr->data = (int *) malloc ((unsigned) (nr * nc * sizeof(int)));
  newptr->name = img_basename(name);
  newptr->rows = nr;
  newptr->cols = nc;
  for (i = 0; i < nr; i++) {
    for (j = 0; j < nc; j++) {
      img_setpixel(newptr, i, j, 0);
    }
  }
  return (newptr);
}


void img_free(IMAGE *img)
{
  if (img->data) free ((char *) img->data);
  if (img->name) free ((char *) img->name);
  free ((char *) img);
}


void img_setpixel(IMAGE *img,int r, int c, int val)
{
  int nc;

  nc = img->cols;
  img->data[(r * nc) + c] = val;
}


int img_getpixel(IMAGE *img,int r, int c)
{
  int nc;

  nc = img->cols;
  return (img->data[(r * nc) + c]);
}


IMAGE *img_open(char *filename)
{
  IMAGE *newptr;
  FILE *pgm;
  char line[512], intbuf[100], ch;
  int type, nc, nr, maxval, i, j, k, found;

  newptr = img_alloc();
  if ((pgm = fopen(filename, "r")) == NULL) {
    printf("IMGOPEN: Couldn't open '%s'\n", filename);
    return(NULL);
  }

  newptr->name = img_basename(filename);

  /*** Scan pnm type information, expecting P5 ***/
  fgets(line, 511, pgm);
  sscanf(line, "P%d", &type);
  if (type != 5 && type != 2) {
    printf("IMGOPEN: Only handles pgm files (type P5 or P2)\n");
    fclose(pgm);
    return(NULL);
  }

  /*** Get dimensions of pgm ***/
  fgets(line, 511, pgm);
  sscanf(line, "%d %d", &nc, &nr);
  newptr->rows = nr;
  newptr->cols = nc;

  /*** Get maxval ***/
  fgets(line, 511, pgm);
  sscanf(line, "%d", &maxval);
  if (maxval > 255) {
    printf("IMGOPEN: Only handles pgm files of 8 bits or less\n");
    fclose(pgm);
    return(NULL);
  }

  newptr->data = (int *) malloc ((unsigned) (nr * nc * sizeof(int)));
  if (newptr->data == NULL) {
    printf("IMGOPEN: Couldn't allocate space for image data\n");
    fclose(pgm);
    return(NULL);
  }

  if (type == 5) {

    for (i = 0; i < nr; i++) {
      for (j = 0; j < nc; j++) {
        img_setpixel(newptr, i, j, fgetc(pgm));
      }
    }

  } else if (type == 2) {

    for (i = 0; i < nr; i++) {
      for (j = 0; j < nc; j++) {

        k = 0;  found = 0;
        while (!found) {
          ch = (char) fgetc(pgm);
          if (ch >= '0' && ch <= '9') {
            intbuf[k] = ch;  k++;
  	  } else {
            if (k != 0) {
              intbuf[k] = '\0';
              found = 1;
	    }
	  }
	}
        img_setpixel(newptr, i, j, atoi(intbuf));

      }
    }

  } else {
    printf("IMGOPEN: Fatal impossible error\n");
    fclose(pgm);
    return (NULL);
  }

  fclose(pgm);
  return (newptr);
}


int img_write(IMAGE *img,char *filename)
{
  int i, j, nr, nc, k, val;
  FILE *iop;

  nr = img->rows;  nc = img->cols;
  iop = fopen(filename, "w");
  fprintf(iop, "P2\n");
  fprintf(iop, "%d %d\n", nc, nr);
  fprintf(iop, "255\n");

  k = 1;
  for (i = 0; i < nr; i++) {
    for (j = 0; j < nc; j++) {
      val = img_getpixel(img, i, j);
      if ((val < 0) || (val > 255)) {
        printf("IMG_WRITE: Found value %d at row %d col %d\n", val, i, j);
        printf("           Setting it to zero\n");
        val = 0;
      }
      if (k % 10) {
        fprintf(iop, "%d ", val);
      } else {
        fprintf(iop, "%d\n", val);
      }
      k++;
    }
  }
  fprintf(iop, "\n");
  fclose(iop);
  return (1);
}


IMAGELIST *imgl_alloc()
{
  IMAGELIST *newptr;

  newptr = (IMAGELIST *) malloc (sizeof (IMAGELIST));
  if (newptr == NULL) {
    printf("IMGL_ALLOC: Couldn't allocate image list\n");
    return(NULL);
  }

  newptr->n = 0;
  newptr->list = NULL;

  return (newptr);
}


void imgl_add(IMAGELIST *il,IMAGE *img)
{
  int n;

  n = il->n;

  if (n == 0) {
    il->list = (IMAGE **) malloc ((unsigned) (sizeof (IMAGE *)));
  } else {
    il->list = (IMAGE **) realloc ((char *) il->list,
      (unsigned) ((n+1) * sizeof (IMAGE *)));
  }

  if (il->list == NULL) {
    printf("IMGL_ADD: Couldn't reallocate image list\n");
  }

  il->list[n] = img;
  il->n = n+1;
}


void imgl_free(IMAGELIST *il)
{
  free((char *) il->list);
  free((char *) il);
}


void imgl_load_images_from_textfile(IMAGELIST *il,char *filename)
{
  IMAGE *iimg;
  FILE *fp;
  char buf[2000];

  if (filename[0] == '\0') {
    printf("IMGL_LOAD_IMAGES_FROM_TEXTFILE: Invalid file '%s'\n", filename);
    return;
  }

  if ((fp = fopen(filename, "r")) == NULL) {
    printf("IMGL_LOAD_IMAGES_FROM_TEXTFILE: Couldn't open '%s'\n", filename);
    return;
  }

  while (fgets(buf, 1999, fp) != NULL) {

    imgl_munge_name(buf);
    printf("Loading '%s'...", buf);  fflush(stdout);
    if ((iimg = img_open(buf)) == 0) {
      printf("Couldn't open '%s'\n", buf);
    } else {
      imgl_add(il, iimg);
      printf("done\n");
    }
    fflush(stdout);
  }

  fclose(fp);
}


void imgl_munge_name(char *buf)
{
  int j;

  j = 0;
  while (buf[j] != '\n') j++;
  buf[j] = '\0';
}
