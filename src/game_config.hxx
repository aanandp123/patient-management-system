// #ifndef GAME_GAME_CONFIG_HXX
// #define GAME_GAME_CONFIG_HXX

#pragma once
// #include "model.hxx"
#include <ge211.hxx>

using namespace ge211;
struct Game_config
{
public:

    /// representing pixel position
    using Pixel_Position = ge211::Posn<int>;

    using Pixel_Dimensions = ge211::Dims<int>;

    using Pixel_Rectangle = ge211::Rect<int>;

    /// representing board position
    using Board_Position = ge211::Posn<int>;

    using Board_Dimensions = ge211::Dims<int>;

    using Board_Rectangle = ge211::Rect<int>;

    /// constructor for the game initial state
    Game_config();

    /// board dimension aligned with scene dim width and scene dim height
    /// leaving the top margin
    Board_Dimensions dims_;

    /// window size for the game
    ge211::Dims<int> scene_dims;

    /// the columns of grids the game has
    int path_cols;

    /// the rows of girds the game has
    int path_rows;

    /// top spacing for health and score
    int top_margin;

    /// total number of wall objects
    int wall_count = 50;

    /// total number of obstacle objects
    int obs_count;

    /// Posn<int> in board position of the player 1's born place
    ge211::Posn<int> player1;

    /// Posn<int> in board position of the player 2's born place
    ge211::Posn<int> player2;

    /// Position vector of the player's board position
    std::vector<ge211::Posn<int>> players = {player1,player2};

    /// spacing for each grid
    int spacing;

    /// grid dimensions
    Pixel_Dimensions grid_dim_;

    /// board's dimension utilizing board positions (not screen position)
    Board_Dimensions dims();

    /// Returns a rectangle containing all the positions of the board. It
    /// can be used to iterate over the board positions
    Board_Rectangle all_positions() const;

    /// Returns a vector of all board positions
    std::vector<Board_Position> all_pos_vec() const;

    /// variable with all board positions
    std::vector<Board_Position> all_pos_ = all_pos_vec();

    /// inputs a board position and a vector of Posn<int> (board positions),
    /// return true if the board position is one grid
    /// horizontally or vertically away from any Posn<int> in the vector
    bool close_to_dest(ge211::Posn<int>, std::vector<ge211::Posn<int>>)
    const;

    /// generate a random vetor of board positions to assign for wall objects
    std::vector<ge211::Posn<int>> random_walls() const;

    /// function to return the value of wall_pos_
    std::vector<ge211::Posn<int>> wall_pos_state() const;

    /// generate a random vetor of board positions to assign for obstacle
    /// objects
    std::vector<ge211::Posn<int>> random_obs();

    /// function to return the value of wall_pos_
    std::vector<ge211::Posn<int>> obs_pos_state() const;

    /// Random Posn<int> board position vector of wall object
    std::vector<ge211::Posn<int>> const wall_pos_ = {Posn<int>(9, 4),
            Posn<int>(11, 4),Posn<int>(4, 7),Posn<int>(8, 13),
            Posn<int>(2, 1),Posn<int>(13, 1),Posn<int>(12, 8),
            Posn<int>(7, 8),Posn<int>(9, 6),Posn<int>(6, 0),
            Posn<int>(9, 8),Posn<int>(5, 0),Posn<int>(6, 6),
            Posn<int>(1, 6),Posn<int>(12, 5),Posn<int>(13, 10),
            Posn<int>(8, 12),Posn<int>(10, 6),Posn<int>(8, 3),
            Posn<int>(6, 12),Posn<int>(5, 1),Posn<int>(10, 8),
            Posn<int>(3, 0), Posn<int>(6, 1),Posn<int>(2, 11),
            Posn<int>(10, 1),Posn<int>(6, 11),Posn<int>(8, 5),
            Posn<int>(0, 8), Posn<int>(6, 11),Posn<int>(13, 5),
            Posn<int>(8,5),Posn<int>(10, 9),Posn<int>(6, 9),
            Posn<int>(14, 7),Posn<int>(0, 9),Posn<int>(6, 13),
            Posn<int>(5, 1),Posn<int>(12, 6),Posn<int>(6, 13),
            Posn<int>(3, 7),Posn<int>(2, 11),Posn<int>(13, 13),
            Posn<int>(7, 3),Posn<int>(1, 2),Posn<int>(14, 3),
            Posn<int>(0, 10),Posn<int>(4, 13),Posn<int>(5, 9),
            Posn<int>(8, 0)};

    /// Random Posn<int> board position vector of obstacles object
    std::vector<ge211::Posn<int>> obs_pos_ = random_obs();



    /// initial state of unmov_objs_
    std::vector<ge211::Posn<int>> init_unmov_objs();

    /// vector with pixel positions of wall_pos_, obs_pos_, and bomb
    std::vector<ge211::Posn<int>> unmov_objs_ = init_unmov_objs();

    /// function that makes all board positions to pixel positions
    Game_config::Pixel_Position
    board_to_screen(Board_Position logical) const;

    /// function that makes all pixel positions to board positions
    Game_config :: Board_Position
    screen_to_board (Pixel_Position physical) const;

};


// #endif GAME_GAME_CONFIG_HXX


