#ifndef CCP_MINNEG_H_
#define CCP_MINNEG_H_

#include <stdbool.h>

#ifdef _WIN32
    #ifdef BUILD_DLL
        #define _APICCP __declspec(dllexport)
    #else
        #define _APICCP __declspec(dllimport)
    #endif
#else
    #define _APICCP
#endif

#ifdef __cplusplus
extern "C" {
#endif

// ----------------------------------------------------------------------------

typedef float Number;

typedef struct slab_t {
    Number height;
} Slab;

typedef struct rect_t {
    Number dim_x;
    Number dim_y;
} Rect;

typedef struct span_t {
    Number x;
    Number y;
} Span;

typedef enum cantilever_position_t {
    CCP_CANTILEVER_NONE,
    CCP_CANTILEVER_TOP_LEFT,
    CCP_CANTILEVER_BOTTOM_RIGHT
} CantileverPosition;

typedef struct cantilever_t {
    CantileverPosition pos;
    Number value;
} Cantilever;

typedef enum border_position_t {
    CCP_BORDER_POSITION_NONE,
    CCP_BORDER_POSITION_TOP_LEFT,
    CCP_BORDER_POSITION_TOP_MIDDLE,
    CCP_BORDER_POSITION_TOP_RIGHT,
    CCP_BORDER_POSITION_MIDDLE_RIGHT,
    CCP_BORDER_POSITION_BOTTOM_RIGHT,
    CCP_BORDER_POSITION_BOTTOM_MIDDLE,
    CCP_BORDER_POSITION_BOTTOM_LEFT,
    CCP_BORDER_POSITION_MIDDLE_LEFT
} BorderPosition;

typedef struct column_t {
    Slab aslab;
    Rect geom;
    Span lm;
    Span li;
    Cantilever cantilever_x;
    Cantilever cantilever_y;
    BorderPosition border_pos;
} TColumn;
typedef TColumn* Column;

typedef struct criteria_t {
    Number fyk;
    Number increase_factor_loads;
    Number reduction_factor_concrete;
    Number reduction_factor_mild_steel;
} TCriteria;
typedef TCriteria* Criteria;

typedef struct reinforcement_t {
    bool should_adopt_higher_as;

    Number s_x;
    Number s_y;

    Number bars_amount_x;
    Number bars_amount_y;

    Number bars_length_x;
    Number bars_length_y;

    Number bars_weight_x;
    Number bars_weight_y;
} TReinforcement;
typedef TReinforcement* Reinforcement;

typedef struct detail_t {
    Column column;
    Criteria criteria;

    Number as_x;
    Number as_y;

    Reinforcement reinforcement;
} TDetail;
typedef TDetail* Detail;

// ----------------------------------------------------------------------------

Column _APICCP create_column(Number slab_height,
                             Number dim_x, Number dim_y,
                             Number lm_x, Number lm_y,
                             Number li_x, Number li_y,
                             CantileverPosition cx_pos, Number cx_val,
                             CantileverPosition cy_pos, Number cy_val,
                             BorderPosition border_pos);

Criteria _APICCP create_criteria_data(Number fyk,
                                      Number increase_factor_loads,
                                      Number reduction_factor_concrete,
                                      Number reduction_factor_mild_steel);

Reinforcement _APICCP create_reinforcement(Column column,
                                           Number as_x, Number as_y,
                                           Number as_unit_x, Number as_unit_y,
                                           Number wg_unit_x, Number wg_unit_y,
                                           bool should_adopt_higher_as,
                                           Number tolerance);

Detail _APICCP create_detail(Column column, Criteria criteria,
                             Number as_unit_x, Number as_unit_y,
                             Number weight_unit_x, Number weight_unit_y,
                             bool should_adopt_higher_as, Number tolerance);

// ----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif // CCP_MINNEG_H_
