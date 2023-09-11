#pragma once
#include "game_config.hxx"
#include <ge211.hxx>
#include "bomb.hxx"
#include <iostream>

struct Player
{
    /// Board position of the player
    using Position = ge211::Posn<int>;

    /// Board dimension of the player
    using Dimension = ge211::Dims<int>;

    Player(Game_config const&, Position);

    /// keeps track of all the objects existing in the map and assign to
    // all_obj_
    void calc_obj();

    /// check if player hits border
    bool hit_border(Game_config const& config, Position posn);

    /// returns true if the player hits another object take in board
    /// positions for both the player and the vector of objects
    bool hit_obj(Game_config const& config, Position posn,
                 std::vector<Position> v);

    /// throw bombs at the location the player is at
    void throw_bomb(Bomb& b ,Player& player);

    ///
    /// Member variables
    ///


    /// the board position of the player
    Position player_pos_;

    /// bomb that player one has
    Bomb bomb;

    // /// bomb that player 2 has
    // Bomb bomb_p2;

    /// player is active or not
    bool live;

    // /// function to get player's health
    // int player_hp_state();

    /// player's health
    int player_health_;

    /// nums of obstacles player destroyed as points
    int destroy_obs;

private:
    ge211::Dims<int> player_velocity;

    // ///
    // std::vector<Game_config::Position> all_obj_;


};
