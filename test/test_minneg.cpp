#include <gtest/gtest.h>

#include <iostream>
#include <filesystem>

#include <minneg.h>

// ----------------------------------------------------------------------------

// Redefining EXPECT_FLOAT_EQ, gtest's version is too precise for our fits
#undef  EXPECT_FLOAT_EQ
#define EXPECT_FLOAT_EQ(a, b) EXPECT_NEAR(a, b, 0.01)

// ----------------------------------------------------------------------------

const Number fyk = 500;
const Number increase_factor_loads = 1.4;
const Number reduction_factor_concrete = 1.4;
const Number reduction_factor_mild_steel = 1.15;

const Number slab_height = 35;
const Number dim_x = 20;
const Number dim_y = 30;
const Number lm_x = 534;
const Number lm_y = 487;
const Number li_x = 559;
const Number li_y = 517;
const CantileverPosition cantilever_x_pos = CCP_CANTILEVER_TOP_LEFT;
const CantileverPosition cantilever_y_pos = CCP_CANTILEVER_TOP_LEFT;
const Number cantilever_y_val = 55;
const Number cantilever_x_val = 50;
const BorderPosition border_position = CCP_BORDER_POSITION_NONE;

const Number as_x = 2.010619298;
const Number as_y = 2.010619298;
const Number wg_x = 1.578;
const Number wg_y = 1.578;

const bool should_adopt_higher_as = false;
const Number tolerance = 0.02;

static constexpr const char* rel_fp = "build/test/test-det.bin";

// ----------------------------------------------------------------------------

TEST(test_minneg, should_be_able_to_create_new_column)
{
    Column c = create_column(slab_height, dim_x, dim_y,
                             lm_x, lm_y, li_x, li_y,
                             cantilever_x_pos, cantilever_x_val,
                             cantilever_y_pos, cantilever_y_val,
                             border_position);
    
    EXPECT_EQ(c->aslab.height, slab_height);
    EXPECT_EQ(c->geom.dim_x, dim_x);
    EXPECT_EQ(c->geom.dim_y, dim_y);
    EXPECT_EQ(c->lm.x, lm_x);
    EXPECT_EQ(c->lm.y, lm_y);
    EXPECT_EQ(c->li.x, li_x);
    EXPECT_EQ(c->li.y, li_y);
    EXPECT_EQ(c->border_pos, CCP_BORDER_POSITION_NONE);
}

// ----------------------------------------------------------------------------

TEST(test_minneg, should_be_able_to_create_criteria_data)
{
    Criteria c = create_criteria_data(fyk,
                                      increase_factor_loads,
                                      reduction_factor_concrete,
                                      reduction_factor_mild_steel);
    
    EXPECT_EQ(c->fyk, fyk);
    EXPECT_EQ(c->increase_factor_loads, increase_factor_loads);
    EXPECT_EQ(c->reduction_factor_concrete, reduction_factor_concrete);
    EXPECT_EQ(c->reduction_factor_mild_steel, reduction_factor_mild_steel);
}

// ----------------------------------------------------------------------------

TEST(test_minneg, should_be_able_to_create_new_reinforcement)
{
    Column c = create_column(slab_height, dim_x, dim_y,
                             lm_x, lm_y, li_x, li_y,
                             cantilever_x_pos, cantilever_x_val,
                             cantilever_y_pos, cantilever_y_val,
                             border_position);

    const Number as_calc_x = 14.0175;
    const Number as_calc_y = 12.7837;
    Reinforcement r = create_reinforcement(c, as_calc_x, as_calc_y,
                                           as_x, as_y, wg_x, wg_y,
                                           should_adopt_higher_as, tolerance);

    EXPECT_EQ(r->bars_amount_x, 7);
    EXPECT_EQ(r->bars_amount_y, 7);
    EXPECT_FLOAT_EQ(r->s_x, 22.5000);
    EXPECT_FLOAT_EQ(r->s_y, 20.4170);
    EXPECT_FLOAT_EQ(r->bars_length_x, 163.1670);
    EXPECT_FLOAT_EQ(r->bars_length_y, 171.1670);
    EXPECT_FLOAT_EQ(r->bars_weight_x, 18.0234);
    EXPECT_FLOAT_EQ(r->bars_weight_y, 18.9071);
}

// ----------------------------------------------------------------------------

TEST(test_minneg, should_be_able_to_create_new_detail)
{
    Column column = create_column(slab_height, dim_x, dim_y,
                                  lm_x, lm_y, li_x, li_y,
                                  cantilever_x_pos, cantilever_x_val,
                                  cantilever_y_pos, cantilever_y_val,
                                  border_position);

    Criteria criteria = create_criteria_data(fyk,
                                             increase_factor_loads,
                                             reduction_factor_concrete,
                                             reduction_factor_mild_steel);
    
    Detail det = create_detail(column, criteria, as_x, as_y, wg_x, wg_y,
                               should_adopt_higher_as, tolerance);

    EXPECT_FLOAT_EQ(det->as_x, 14.0175);
    EXPECT_FLOAT_EQ(det->as_y, 12.7837);
    EXPECT_EQ(det->reinforcement->bars_amount_x, 7);
    EXPECT_EQ(det->reinforcement->bars_amount_y, 7);
    EXPECT_FLOAT_EQ(det->reinforcement->s_x, 22.5000);
    EXPECT_FLOAT_EQ(det->reinforcement->s_y, 20.4170);
    EXPECT_FLOAT_EQ(det->reinforcement->bars_length_x, 163.1670);
    EXPECT_FLOAT_EQ(det->reinforcement->bars_length_y, 171.1670);
    EXPECT_FLOAT_EQ(det->reinforcement->bars_weight_x, 18.0234);
    EXPECT_FLOAT_EQ(det->reinforcement->bars_weight_y, 18.9071);
}

// ----------------------------------------------------------------------------

TEST(test_minneg, should_be_able_to_save_detail_to_file)
{
    Column column = create_column(slab_height, dim_x, dim_y,
                                  lm_x, lm_y, li_x, li_y,
                                  cantilever_x_pos, cantilever_x_val,
                                  cantilever_y_pos, cantilever_y_val,
                                  border_position);

    Criteria criteria = create_criteria_data(fyk,
                                             increase_factor_loads,
                                             reduction_factor_concrete,
                                             reduction_factor_mild_steel);
    
    Detail det = create_detail(column, criteria, as_x, as_y, wg_x, wg_y,
                               should_adopt_higher_as, tolerance);

    std::string path = std::filesystem::current_path().append(rel_fp).string();
    save_detail_to_file(path.c_str(), det);
}

// ----------------------------------------------------------------------------

TEST(test_minneg, should_be_able_to_load_detail_from_file)
{
    std::string path = std::filesystem::current_path().append(rel_fp).string();
    Detail det = load_detail_from_file(path.c_str());

    EXPECT_FLOAT_EQ(det->column->aslab.height, slab_height);
    EXPECT_FLOAT_EQ(det->column->geom.dim_x, dim_x);
    EXPECT_FLOAT_EQ(det->column->geom.dim_y, dim_y);
    EXPECT_FLOAT_EQ(det->column->lm.x, lm_x);
    EXPECT_FLOAT_EQ(det->column->lm.y, lm_y);
    EXPECT_FLOAT_EQ(det->column->li.x, li_x);
    EXPECT_FLOAT_EQ(det->column->li.y, li_y);
    EXPECT_EQ(det->column->cantilever_x.pos, cantilever_x_pos);
    EXPECT_FLOAT_EQ(det->column->cantilever_x.value, cantilever_x_val);
    EXPECT_EQ(det->column->cantilever_y.pos, cantilever_y_pos);
    EXPECT_FLOAT_EQ(det->column->cantilever_y.value, cantilever_y_val);
    EXPECT_EQ(det->column->border_pos, border_position);

    EXPECT_FLOAT_EQ(det->criteria->fyk, fyk);
    EXPECT_FLOAT_EQ(det->criteria->increase_factor_loads,
                    increase_factor_loads);
    EXPECT_FLOAT_EQ(det->criteria->reduction_factor_concrete,
                    reduction_factor_concrete);
    EXPECT_FLOAT_EQ(det->criteria->reduction_factor_mild_steel,
                    reduction_factor_mild_steel);

    EXPECT_FLOAT_EQ(det->as_x, 14.0175);
    EXPECT_FLOAT_EQ(det->as_y, 12.7837);
    EXPECT_EQ(det->reinforcement->bars_amount_x, 7);
    EXPECT_EQ(det->reinforcement->bars_amount_y, 7);
    EXPECT_FLOAT_EQ(det->reinforcement->s_x, 22.5000);
    EXPECT_FLOAT_EQ(det->reinforcement->s_y, 20.4170);
    EXPECT_FLOAT_EQ(det->reinforcement->bars_length_x, 163.1670);
    EXPECT_FLOAT_EQ(det->reinforcement->bars_length_y, 171.1670);
    EXPECT_FLOAT_EQ(det->reinforcement->bars_weight_x, 18.0234);
    EXPECT_FLOAT_EQ(det->reinforcement->bars_weight_y, 18.9071);
}

// ----------------------------------------------------------------------------
