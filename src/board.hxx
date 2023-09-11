#pragma once
#include "game_config.hxx"
#include <ge211.hxx>

class Board
{
public:
    /// Board dimensions will use `int` coordinates.
    using Dimensions = ge211::Dims<int>;

    /// Board positions will use `int` coordinates.
    using Position = ge211::Posn<int>;

    /// Board rectangles will use `int` coordinates.
    using Rectangle = ge211::Rect<int>;

    /// constructor for Board
    explicit Board(Game_config const& config = Game_config());

    /// Dimensions of the board
    Dimensions dimensions();

    /// all positions on board
    Rectangle all_positions() const;

private:
    Game_config const& config;

    /// board dimension aligned with scene dim width and scene dim height
    /// leaving the top margin
    Dimensions dims_ = {config.path_cols, config.path_rows};
};