#pragma once

#include "model.hxx"
#include <iostream>

using namespace ge211;


class View
{
public:

    /// View::Position is pixel(screen) position of the window
    using Position = ge211::Posn<int>;

    /// View::Dimension is pixel(screen) dimension of the window
    using Dimension = ge211::Dims<int>;

    /// Constructs a view that knows about the given model
    explicit View(Model const& model);

    /// draw designated sprites
    void draw(ge211::Sprite_set& set);

    /// gives the initial window size
    ge211::Dims<int> initial_window_dimensions() const;

    /// converts board position to pixel(screen) position
    View::Position board_to_screen(Model::Position logical) const;

    /// converts pixel(screen) position to board position
    Model::Position screen_to_board(View::Position physical) const;

    /// check if enter key is pressed
    bool enter_key;

    /// check if space key is pressed
    bool space_key;

    /// check the time has passed for 3s of player 2's bomb
    bool over_three_p2;

    /// check the time has passed for 3s of player 1's bomb
    bool over_three_p1;

    /// check if player2 is bombed
    bool player2_bombed_;

    /// check if player1 is bombed
    bool player1_bombed_;

private:
    Model const& model_;
    /// every grid's pixel dimensions
    View::Dimension grid_dim_;
    ge211::Rectangle_sprite const grid_sprite;
    ge211::Rectangle_sprite const obs_sprite;
    ge211::Rectangle_sprite const wall_sprite;
    ge211::Rectangle_sprite const player1_sprite;
    ge211::Rectangle_sprite const player2_sprite;
    ge211::Circle_sprite const bomb_sprite;
    ge211::Rectangle_sprite const explosion_sprite;
    ge211::Circle_sprite const burn_sprite_2;
    ge211::Circle_sprite const burn_sprite_1;
    ge211::Font sans15_{"sans.ttf", 15};
    ge211::Font sans50_{"sans.ttf", 50};
    ge211::Text_sprite player1_text_sprite_;
    ge211::Text_sprite player2_text_sprite_;
    ge211::Text_sprite score1_sprite_;
    ge211::Text_sprite score2_sprite_;
    ge211::Rectangle_sprite const black_sprite;
    ge211::Text_sprite gameover_sprite_;
};

