#ifndef GEN_SYM_H
#define GEN_SYM_H

#include "netting.h"
#include "rules.h"

// Symulacja generacji
net_t sym_gen( net_t net, rules_t rules );

// Ustalanie sąsiedztwa
int cell_neighborhood( net_t *net, int x, int y );

#endif
