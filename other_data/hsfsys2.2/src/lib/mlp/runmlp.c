/*
# proc: mlphypscons - classifies the given set of feature vectors using
# proc:               the MLP code.
# proc: runmlp - runs the MLP classifier on a given feature vector.
# proc:
*/

#include <stdio.h>
#include <stdlib.h>
#include <mlp/parms.h>
#include <math.h>
#include <mlp/defs.h>



/*************************************************************/
mlphypscons(ninps, nhids, nouts, acfunc_hids, acfunc_outs, weights,
            klts, nklts, hyps_i, cons)
int ninps, nhids, nouts, nklts;
char acfunc_hids, acfunc_outs;
float *weights, *klts, *cons;
int *hyps_i;
{
   int i;
   float *outacts, *kptr;
   void runmlp();

   malloc_flt(&outacts, nouts, "mlphypscons : outacts");

   for(i = 0, kptr = klts; i < nklts; i++, kptr += ninps){
      runmlp(ninps, nhids, nouts, acfunc_hids, acfunc_outs, weights,
             kptr, outacts, &(hyps_i[i]), &(cons[i]));
   }

   free(outacts);
}

/*************************************************************/
/* runmlp: Runs the Multi-Layer Perceptron (MLP) on a feature vector.

Input args:
  ninps, nhids, nouts: Numbers of input, hidden, and output nodes
    of the MLP.
  acfunc_hids_code: Code character specifying the type of activation
    function to be used on the hidden nodes: must be LINEAR,
    SIGMOID, or SINUSOID (defined in parms.h).
  acfunc_outs_code: Code character specifying the type of activation
    function to be used on the output nodes.
  w: The MLP weights.
  featvec: The feature vector that the MLP is to be run on; its first
    ninps elts will be used.

Output args:
  outacs: The output activations.  This buffer must be provided by
    caller, allocated to (at least) nouts floats.
  hypclass: The hypothetical class, as an integer in the range
    0 through nouts - 1.
  confidence: A floating-point value in the range 0. through 1.
    Defined to be outacs[hypclass], i.e. the highest
    output-activation value.
*/

#define MAX_NHIDS 1000 /* Maximum number of hidden nodes */

void
runmlp(ninps, nhids, nouts, acfunc_hids_code, acfunc_outs_code, w,
  featvec, outacs, hypclass, confidence)
int ninps, nhids, nouts, *hypclass;
char acfunc_hids_code, acfunc_outs_code;
float *w, *featvec, *outacs, *confidence;
{
  char str[100];
  static char t = 't';
  static int i1 = 1;
  float *w1, *b1, *w2, *b2, hidacs[MAX_NHIDS], *p, *pe, *maxac_p,
    maxac, ac;
  static float f1 = 1.;
  void (*acfunc_hids)(), (*acfunc_outs)();
  void ac_v_linear(), ac_v_sigmoid(), ac_v_sinusoid();

  if(nhids > MAX_NHIDS) {
    sprintf(str, "nhids, %d, is > MAX_NHIDS, defined as %d in \
runmlp.c", nhids, MAX_NHIDS);
    fatalerr("runmlp", str, NULL);
  }

  /* Resolve the activation function codes to functions. */
  switch(acfunc_hids_code) {
  case LINEAR:
    acfunc_hids = ac_v_linear;
    break;
  case SIGMOID:
    acfunc_hids = ac_v_sigmoid;
    break;
  case SINUSOID:
    acfunc_hids = ac_v_sinusoid;
    break;
  default:
    sprintf(str, "unsupported acfunc_hids_code %d.\n\
Supported codes are LINEAR (%d), SIGMOID (%d), and SINUSOID \
(%d).", (int)acfunc_hids_code, (int)LINEAR, (int)SIGMOID,
      (int)SINUSOID);
    fatalerr("runmlp", str, NULL);
    break;
  }
  switch(acfunc_outs_code) {
  case LINEAR:
    acfunc_outs = ac_v_linear;
    break;
  case SIGMOID:
    acfunc_outs = ac_v_sigmoid;
    break;
  case SINUSOID:
    acfunc_outs = ac_v_sinusoid;
    break;
  default:
    sprintf(str, "unsupported acfunc_outs_code %d.\n\
Supported codes are LINEAR (%d), SIGMOID (%d), and SINUSOID \
(%d).", (int)acfunc_outs_code, (int)LINEAR, (int)SIGMOID,
      (int)SINUSOID);
    fatalerr("runmlp", str, NULL);
    break;
  }

  /* Where the weights and biases of the two layers begin in w. */
  b2 = (w2 = (b1 = (w1 = w) + nhids * ninps) + nhids) + nouts * nhids;

  /* Start hidden activations out as first-layer biases. */
  memcpy((char *)hidacs, (char *)b1, nhids * sizeof(float));

  /* Add product of first-layer weights with feature vector. */
  sgemv_(&t, &ninps, &nhids, &f1, w1, &ninps, featvec, &i1, &f1,
    hidacs, &i1);

  /* Finish each hidden activation by applying activation function. */
  for(pe = (p = (float *)hidacs) + nhids; p < pe; p++)
    acfunc_hids(p);

  /* Same steps again for second layer. */
  memcpy((char *)outacs, (char *)b2, nouts * sizeof(float));
  sgemv_(&t, &nhids, &nouts, &f1, w2, &nhids, hidacs, &i1, &f1,
    outacs, &i1);
  for(pe = (p = outacs) + nouts; p < pe; p++)
    acfunc_outs(p);

  /* Find the hypothetical class -- the class whose output node
  activated most strongly -- and the confidence -- that activation
  value. */
  for(pe = (maxac_p = p = outacs) + nouts, maxac = *p, p++; p < pe;
    p++)
    if((ac = *p) > maxac) {
      maxac = ac;
      maxac_p = p;
    }
  *hypclass = maxac_p - outacs;
  *confidence = maxac;
}
