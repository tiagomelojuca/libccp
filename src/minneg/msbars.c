#include "msbars.h"

#include <math.h>

#include "utils.h"

// ----------------------------------------------------------------------------

int calc_bars_amount(Number as, Number as_unit, Number tol)
{
    const Number needed_bars = as / as_unit;

    const int first_shoot_amount = floor(needed_bars);
    const Number first_shoot_as = first_shoot_amount * as_unit;
    const Number found_tol = 1 - (first_shoot_as / as);

    return found_tol < tol ? first_shoot_amount : first_shoot_amount + 1;
}

// ----------------------------------------------------------------------------

Number calc_nbr_length_x(Column column)
{
    const Number nbr_divisor = 6.0;
    return column->li.x / nbr_divisor;
}

// ----------------------------------------------------------------------------

Number calc_nbr_length_y(Column column)
{
    const Number nbr_divisor = 6.0;
    return column->li.y / nbr_divisor;
}

// ----------------------------------------------------------------------------

Number calc_var_length_x(Column column)
{
    if (is_in_border_y(column)) {
        return 0.0;
    }

    const bool has_cant_x = column->cantilever_x.pos != CCP_CANTILEVER_NONE;
    if (has_cant_x) {
        return column->cantilever_x.value;
    }

    return calc_nbr_length_x(column);
}

// ----------------------------------------------------------------------------

Number calc_var_length_y(Column column)
{
    if (is_in_border_x(column)) {
        return 0.0;
    }

    const bool has_cant_y = column->cantilever_y.pos != CCP_CANTILEVER_NONE;
    if (has_cant_y) {
        return column->cantilever_y.value;
    }

    return calc_nbr_length_y(column);
}

// ----------------------------------------------------------------------------

Number calc_bars_length_x(Column column)
{
    return column->geom.dim_x + calc_nbr_length_x(column)
                              + calc_var_length_x(column);
}

// ----------------------------------------------------------------------------

Number calc_bars_length_y(Column column)
{
    return column->geom.dim_y + calc_nbr_length_y(column)
                              + calc_var_length_y(column);
}

// ----------------------------------------------------------------------------
