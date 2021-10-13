#include "minneg.h"

#include <stdio.h>
#include <stdlib.h>

#include "distreg.h"
#include "msbars.h"

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

int save_detail_to_file(const char* filepath, Detail d)
{
    FILE* f = fopen(filepath, "wb");

    if (f == NULL) {
        return 1;
    }

    fwrite(d->column, sizeof(TColumn), 1, f);
    fwrite(d->criteria, sizeof(TCriteria), 1, f);
    fwrite(&d->as_x, sizeof(Number), 1, f);
    fwrite(&d->as_y, sizeof(Number), 1, f);
    fwrite(d->reinforcement, sizeof(TReinforcement), 1, f);

    fclose(f);
    return 0;
}

// ----------------------------------------------------------------------------

Detail load_detail_from_file(const char* filepath)
{
    FILE* f = fopen(filepath, "rb");

    if (f == NULL) {
        return NULL;
    }

    Detail d = malloc(sizeof(TDetail));
    d->column = malloc(sizeof(TColumn));
    d->criteria = malloc(sizeof(TCriteria));
    d->reinforcement = malloc(sizeof(TReinforcement));

    fread(d->column, sizeof(TColumn), 1, f);
    fread(d->criteria, sizeof(TCriteria), 1, f);
    fread(&d->as_x, sizeof(Number), 1, f);
    fread(&d->as_y, sizeof(Number), 1, f);
    fread(d->reinforcement, sizeof(TReinforcement), 1, f);

    fclose(f);
    return d;
}

// ----------------------------------------------------------------------------
