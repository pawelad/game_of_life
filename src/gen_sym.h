#ifndef GEN_SYM_H
#define GEN_SYM_H

#include "netting.h"
#include "rules.h"

// Generation symulation
net_t *sym_gen( net_t *n, rules_t *r );

// Next state - alive, dead - of the cell based on rules 
int next_state( net_t *n, int cell_num, rules_t *r );

// Number of cells alive neighbours
int cell_neighborhood( net_t *n, int cell_num );

#endif
