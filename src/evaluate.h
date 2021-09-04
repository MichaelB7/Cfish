#ifndef EVALUATE_H
#define EVALUATE_H

#include "types.h"

#define EvalFileDefaultName "nn-9e3c6298299a.nnue"

#ifdef NNUE
enum { EVAL_HYBRID, EVAL_PURE, EVAL_CLASSICAL };
#ifndef NNUE_PURE
extern int useNNUE;
#else
#define useNNUE EVAL_PURE
#endif
#endif

Value evaluate(const Position *pos);

#endif
