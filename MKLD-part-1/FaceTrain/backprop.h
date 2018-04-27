/*
 ******************************************************************
 * HISTORY
 * 15-Oct-94  Jeff Shufelt (js), Carnegie Mellon University
 *      Prepared for 15-681, Fall 1994.
 *
 ******************************************************************
 */

#ifndef _BACKPROP_H_

#define _BACKPROP_H_

#define BIGRND 0x7fffffff

/*** The neural network data structure.  The network is assumed to
     be a fully-connected feedforward three-layer network.
     Unit 0 in each layer of units is the threshold unit; this means
     that the remaining units are indexed from 1 to n, inclusive.
 ***/

typedef struct {
  int input_n;                  /* number of input units */
  int hidden_n;                 /* number of hidden units */
  int output_n;                 /* number of output units */

  double *input_units;          /* the input units */
  double *hidden_units;         /* the hidden units */
  double *output_units;         /* the output units */

  double *hidden_delta;         /* storage for hidden unit error */
  double *output_delta;         /* storage for output unit error */

  double *target;               /* storage for target vector */

  double **input_weights;       /* weights from input to hidden layer */
  double **hidden_weights;      /* weights from hidden to output layer */

                                /*** The next two are for momentum ***/
  double **input_prev_weights;  /* previous change on input to hidden wgt */
  double **hidden_prev_weights; /* previous change on hidden to output wgt */
} BPNN;


/*** User-level functions ***/

void bpnn_initialize(unsigned int seed);

BPNN *bpnn_create(int n_in, int n_hidden, int n_out);
void bpnn_free(BPNN *net);

void bpnn_train(BPNN *net,double eta, double dmomentum, double *eo, double *eh);
void bpnn_feedforward(BPNN *net);

void bpnn_save(BPNN *net,char *filename);
BPNN *bpnn_read(char *filename);


#endif
