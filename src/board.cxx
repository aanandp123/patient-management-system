
# include <ge211.hxx>
#include "board.hxx"


Board::Board(Game_config const& config)
    : config(config)
{}

Board::Dimensions
Board::dimensions()
{
    return dims_;
}

Board::Rectangle
Board::all_positions() const
{
    return Rectangle::from_top_left(the_origin, dims_) ;
}
