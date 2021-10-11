#include "utils.h"

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
