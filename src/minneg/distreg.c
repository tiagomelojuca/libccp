#include "distreg.h"

#include "utils.h"

// ----------------------------------------------------------------------------

Number calc_nbr_offset(Column column)
{
    const Number nbr_coef = 1.5;
    Number offset = nbr_coef * column->aslab.height;

    return offset;
}

// ----------------------------------------------------------------------------

Number calc_reduced_offset_x(Column column)
{
    Number offset = calc_nbr_offset(column);

    if (is_in_border_y(column)) {
        return 0.0;
    }

    const bool has_cant_x = column->cantilever_x.pos != CCP_CANTILEVER_NONE;
    const bool has_space_for_offset = column->cantilever_x.value > offset;
    if (has_cant_x && !has_space_for_offset) {
        return column->cantilever_x.value;
    }

    return offset;
}

// ----------------------------------------------------------------------------

Number calc_reduced_offset_y(Column column)
{
    Number offset = calc_nbr_offset(column);

    if (is_in_border_x(column)) {
        return 0.0;
    }

    const bool has_cant_y = column->cantilever_y.pos != CCP_CANTILEVER_NONE;
    const bool has_space_for_offset = column->cantilever_y.value > offset;
    if (has_cant_y && !has_space_for_offset) {
        return column->cantilever_y.value;
    }

    return offset;
}

// ----------------------------------------------------------------------------

Number calc_distreg_x(Column column)
{
    Number offset = calc_nbr_offset(column);
    Number reducedOffset = calc_reduced_offset_x(column);

    return column->geom.dim_x + offset + reducedOffset;
}

// ----------------------------------------------------------------------------

Number calc_distreg_y(Column column)
{
    Number offset = calc_nbr_offset(column);
    Number reducedOffset = calc_reduced_offset_y(column);

    return column->geom.dim_y + offset + reducedOffset;
}

// ----------------------------------------------------------------------------
