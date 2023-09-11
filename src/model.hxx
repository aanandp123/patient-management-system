#pragma once

#include <ge211.hxx>
#include "game_config.hxx"
#include "board.hxx"
#include "player.hxx"

class Model
{
public:
    explicit Model(Game_config const& config = Game_config());
    // Model(Game_config const& config);
    /// Model::Dimension gives Board dimensions
    using Dimensions = ge211::Dims<int>;

    /// Model::Position gives Board positions
    using Position = ge211::Posn<int>;

    /// Model::Rectangle gives Board rectangles
    using Rectangle = ge211::Rect<int>;

    /// board's dimension utilizing board positions (not screen position)
    Dimensions dims_state();

    /// Returns a vector of all board positions
    std::vector<Position> all_pos_state() const;

    /// create game_config object
    Game_config config;

/*
    bool close_to_dest(Model::Position obj, std::vector<Model::Position>
            hit_obj) const;

    /// generate a random vetor of board positions to assign for wall objects
    std::vector<Position> random_walls() const;
*/
    /// function to return the value of wall_pos_
    std::vector<Position> wall_pos_state() const;
/*
    /// generate a random vetor of board positions to assign for obstacle
    /// objects
    std::vector<Position> random_obs() const;
*/
    /// function to return the value of obs_pos_
    std::vector<Position> obs_pos_state() const;

    /// function to return the vector of unmov_objs_
    std::vector<Player::Position> unmov_objs_state();

    /// function that makes all board positions to pixel positions
    Player::Position
    board_to_screen(Model::Position logical) const;

    /// function that makes all pixel positions to board positions
    Model :: Position
    screen_to_board (Player :: Position physical) const;

    /// for external functions to access player1
    Player
    get_player1() const;

    /// for external functions to access player2
    Player
    get_player2() const;

    /// update vector unmov_objs_ with board positions of wall_pos_, obs_pos_,
    /// and bomb
    void unmov_objs_update();

    void on_frame(double dt);

    /// moves the player one grid to the right (updating player_pos_ from the
    /// player object)
    void next_right(Game_config const& config, Player& player );

    /// moves the player one grid to the left (updating player_pos_ from the
    /// player object)
    void next_left(Game_config const& config, Player& player );

    /// moves the player one grid to the up (updating player_pos_ from the
    /// player object)
    void next_up(Game_config const& config, Player& player );

    /// moves the player one grid to the down (updating player_pos_ from the
    /// player object)
    void next_down(Game_config const& config, Player& player );

    /// function to update obs_pos after explosion_pos has coordinates
    /// identical to one of the coordinates in obs_pos, and also updates
    /// unmov_objects
    void obs_pos_upd();

    /// updates all_explosion_pos from both players' explosion_pos
    void all_explosion_pos_upd();

    /// gets the state of all_explosion_pos_
    std::vector<Position> all_explosion_pos_state();

    /// check if player is at the explosion area
    bool player_in_explode(Player&);

    /// function that deducts player_health_ health if in the bomb area
    void health_detect(Player&, Player&);

    /// function to access if game is over
    bool get_game_over() const;

    /// Construct player1
    Player player1;

    /// Construct player2
    Player player2;

    /// update destroy_obs of each player
    void count_destory_obs(Player&);


    /// a function that keeps track of the player's health

    friend struct Test_access;
    ///
    /// Member Variables
    ///
private:

    /// board dimension aligned with scene dim width and scene dim height
    /// leaving the top margin
    Dimensions dims_ = config.dims_;

    /// Vector of all board position
    std::vector<Position> all_pos_ = config.all_pos_;

    /// Random Posn<int> board position vector of wall object
    std::vector<Position> const wall_pos_ = config.wall_pos_;

    /// Random Posn<int> board position vector of obstacles object
    std::vector<Position> obs_pos_ = config.obs_pos_;

    /// vector with board positions of wall_pos_, obs_pos_, and bomb
    std::vector<Player::Position> unmov_objs_ = config.unmov_objs_;


    std::vector<Position> all_explosion_pos_;

    /// check if game is over
    bool game_over;

};
