# pragma once
#include <ge211.hxx>
#include "game_config.hxx"

struct Bomb
{
    /// Board position of the player
    using Position = ge211::Posn<int>;

    /// Board dimension of the player
    using Dimension = ge211::Dims<int>;

    /// constructor of bomb
    Bomb(Position p);

    /// the bomb location
    Position bomb_pos;

    /// explosion location
    std::vector<Position> explosion_pos;

    /// function to update explosion_pos
    void explosion_upd(Game_config const& config);

    ///
    /// Helper function
    void close_to_bomb(
            ge211::Posn<int> obj, std::vector<ge211::Posn<int>>
    area);
};