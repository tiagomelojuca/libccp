#ifndef CCP_MSBARS_H_
#define CCP_MSBARS_H_

#include "minneg.h"

int calc_bars_amount(Number as, Number as_unit, Number tol);
Number calc_nbr_length_x(Column column);
Number calc_nbr_length_y(Column column);
Number calc_var_length_x(Column column);
Number calc_var_length_y(Column column);
Number calc_bars_length_x(Column column);
Number calc_bars_length_y(Column column);

#endif // CCP_MSBARS_H_
