#include "minneg.h"

#include <stdlib.h>
#include <math.h>

// ----------------------------------------------------------------------------

Column create_column(Number slab_height,
                     Number dim_x, Number dim_y,
                     Number lm_x, Number lm_y,
                     Number li_x, Number li_y,
                     CantileverPosition cx_pos, Number cx_val,
                     CantileverPosition cy_pos, Number cy_val,
                     BorderPosition border_pos)
{
    Column c = malloc(sizeof(TColumn));

    c->aslab.height = slab_height;
    c->geom.dim_x = dim_x;
    c->geom.dim_y = dim_y;
    c->lm.x = lm_x;
    c->lm.y = lm_y;
    c->li.x = li_x;
    c->li.y = li_y;
    c->cantilever_x.pos = cx_pos;
    c->cantilever_x.value = cx_val;
    c->cantilever_y.pos = cy_pos;
    c->cantilever_y.value = cy_val;
    c->border_pos = border_pos;

    return c;
}

// ----------------------------------------------------------------------------

Criteria create_criteria_data(Number fyk,
                              Number increase_factor_loads,
                              Number reduction_factor_concrete,
                              Number reduction_factor_mild_steel)
{
    Criteria c = malloc(sizeof(TCriteria));

    c->fyk = fyk;
    c->increase_factor_loads = increase_factor_loads;
    c->reduction_factor_concrete = reduction_factor_concrete;
    c->reduction_factor_mild_steel = reduction_factor_mild_steel;

    return c;
}

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

bool is_in_border_x(Column column)
{
    return column->border_pos == CCP_BORDER_POSITION_TOP_LEFT ||
           column->border_pos == CCP_BORDER_POSITION_TOP_MIDDLE ||
           column->border_pos == CCP_BORDER_POSITION_TOP_RIGHT ||
           column->border_pos == CCP_BORDER_POSITION_BOTTOM_RIGHT ||
           column->border_pos == CCP_BORDER_POSITION_BOTTOM_MIDDLE ||
           column->border_pos == CCP_BORDER_POSITION_BOTTOM_LEFT;
}

// ----------------------------------------------------------------------------

bool is_in_border_y(Column column)
{
    return column->border_pos == CCP_BORDER_POSITION_TOP_LEFT ||
           column->border_pos == CCP_BORDER_POSITION_TOP_RIGHT ||
           column->border_pos == CCP_BORDER_POSITION_MIDDLE_RIGHT ||
           column->border_pos == CCP_BORDER_POSITION_BOTTOM_RIGHT ||
           column->border_pos == CCP_BORDER_POSITION_BOTTOM_LEFT ||
           column->border_pos == CCP_BORDER_POSITION_MIDDLE_LEFT;
}

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

Reinforcement create_reinforcement(Column column,
                                   Number as_x, Number as_y,
                                   Number as_unit_x, Number as_unit_y,
                                   Number wg_unit_x, Number wg_unit_y,
                                   bool should_adopt_higher_as,
                                   Number tolerance)
{
    Reinforcement r = malloc(sizeof(TReinforcement));

    r->should_adopt_higher_as = should_adopt_higher_as;

    if (should_adopt_higher_as) {
        if(as_x >= as_y) {
            as_y = as_x;
        } else {
            as_x = as_y;
        }
    }

    r->bars_amount_x = calc_bars_amount(as_x, as_unit_x, tolerance);
    r->bars_amount_y = calc_bars_amount(as_y, as_unit_y, tolerance);
    
    const int num_spaces_x = r->bars_amount_x - 1;
    const int num_spaces_y = r->bars_amount_y - 1;

    r->s_x = calc_distreg_y(column) / num_spaces_x;
    r->s_y = calc_distreg_x(column) / num_spaces_y;

    r->bars_length_x = calc_bars_length_x(column);
    r->bars_length_y = calc_bars_length_y(column);

    const Number cm_to_meter_divisor = 100;
    const Number bars_length_x_meters = r->bars_length_x / cm_to_meter_divisor;
    r->bars_weight_x = r->bars_amount_x * bars_length_x_meters * wg_unit_x;
    const Number bars_length_y_meters = r->bars_length_y / cm_to_meter_divisor;
    r->bars_weight_y = r->bars_amount_y * bars_length_y_meters * wg_unit_y;

    return r;
}

// ----------------------------------------------------------------------------

Detail create_detail(Column column, Criteria criteria,
                     Number as_unit_x, Number as_unit_y,
                     Number weight_unit_x, Number weight_unit_y,
                     bool should_adopt_higher_as, Number tolerance)
{
    Detail d = malloc(sizeof(TDetail));

    d->column = column;
    d->criteria = criteria;

    const Number nbr_coef = 0.00075;
    d->as_x = nbr_coef * d->column->aslab.height * d->column->lm.x;
    d->as_y = nbr_coef * d->column->aslab.height * d->column->lm.y;

    d->reinforcement = create_reinforcement(d->column, d->as_x, d->as_y,
                                            as_unit_x, as_unit_y,
                                            weight_unit_x, weight_unit_y,
                                            should_adopt_higher_as, tolerance);

    return d;
}

// ----------------------------------------------------------------------------
