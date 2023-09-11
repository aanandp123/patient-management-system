#include "model.hxx"
#include "game_config.hxx"
#include <catch.hxx>
#include <iostream>

struct Test_access
{
    Model& model;
    using Position = ge211::Posn<int>;

    // Constructs a `Test_access` with a reference to the Model under test.
    explicit Test_access(Model&);

    // inputs a board position and a vector of Posn<int> (board positions),
    // return a bool indicating if the board position is one grid
    // horizontally or vertically away from any Posn<int> in the vector
    bool close_to_dest(Model::Position obj, std::vector<Model::Position>
            hit_obj);

    std::vector<Model::Position> random_walls();

    Game_config::Pixel_Position board_to_screen(Game_config::Board_Position
    p) const;

    Game_config::Pixel_Position screen_to_board(Game_config::Pixel_Position)
    const;
};

//constructor
Test_access::Test_access(Model& mod)
    :model(mod)
{}

TEST_CASE("Spec 1: Players initial position and grid size"){
    Game_config config_new = Game_config();
    Model model(config_new);
    Test_access t(model);

    //checking if the initial positions of the two players are different
    CHECK(t.model.player1.player_pos_ != t.model.player2.player_pos_);
    //check that we are working with a 15 by 14 grid. Additional verification
    // can be seen by playing the game
    CHECK(t.model.config.path_cols == 15);
    CHECK(t.model.config.path_rows == 14);

}

TEST_CASE("Model test 1- testing that player's cannot move beyond the grid"){
    Game_config config_new = Game_config();
    Model model(config_new);
    Test_access t(model);

    //checking that a player can move right initially
    int original_position_x = t.model.player1.player_pos_.x;
    t.model.next_right(config_new, t.model.player1);
    CHECK(t.model.player1.player_pos_.x == original_position_x + 1);

    //checking that if the player is placed at the right-hand side
    // border,the player cannot move further right as before
    t.model.player1.player_pos_.x= t.model.config.path_cols;
    t.model.next_right(config_new, t.model.player1);
    CHECK(t.model.player1.player_pos_.x == t.model.config.path_cols);

    //checking that if the player is placed at the top-side border, the
    // player cannot move further up
    t.model.player1.player_pos_.y= 0;
    t.model.next_up(config_new, t.model.player1);
    CHECK(t.model.player1.player_pos_.y == 0);

}

TEST_CASE("Model test 2- testing that player's cannot move at positions "
          "where there are obstacles"){
    Game_config config_new = Game_config();
    Model model(config_new);
    Test_access t(model);

    //testing that a player cannot occupy the same position as an
    // indestructible wall

    //assign the player to right of an indestructible wall
    Posn<int> original_position = {t.model.config.wall_pos_[0].x+1,
                                   t.model.config.wall_pos_[0].y};
    t.model.player1.player_pos_= original_position;
    //trying to move the player to the left
    t.model.next_left(config_new, t.model.player1);
    //Because of the obstacle, the player cannot move left and is at the same
    // original position
    CHECK(t.model.player1.player_pos_ == original_position);

    //testing that a player cannot occupy the same position as an
    // destructible obstacle

    //assign the player below of a destructible obstacle
    Posn<int> original_position2 = {t.model.config.obs_pos_[0].x,
                                   t.model.config.obs_pos_[0].y+1};
    t.model.player1.player_pos_= original_position2;
    //trying to move the player to up
    t.model.next_up(config_new, t.model.player1);
    //Because of the obstacle, the player cannot move up and is at the same
    // original position
    CHECK(t.model.player1.player_pos_ == original_position2);
}

TEST_CASE("Model test 3- testing that explosions remove destructible "
          "obstacles"){
    Game_config config_new = Game_config();
    Model model(config_new);
    Test_access t(model);

    //Take a destructible obstacle for this example
    Posn<int> destructible_posn = t.model.config.obs_pos_[0];
    //simulate an explosion hitting a destructible obstacle
    Posn<int> explosion_posn = {destructible_posn.x,
                                destructible_posn.y};
    //add this simulated explosion in player1's explosion vector
    t.model.player1.bomb.explosion_pos.push_back(explosion_posn);
    //updating obstacles according to the simulated explosion
    t.model.obs_pos_upd();
    //check if this destructible obstacle is in the list of
    // destructible obstacles anymore
    bool exist = false;
    for(auto p : t.model.obs_pos_state()){
       if(p==destructible_posn)
           exist = true;
    }
    CHECK(exist == false);

    //now lets see if we can move the player to this newly available position
    //assign the player below of a destructible obstacle

    //assign the player to a position just below this newly available position
    Posn<int> original_position = {destructible_posn.x,
                                    destructible_posn.y+1};
    t.model.player1.player_pos_= original_position;
    //trying to move the player up
    t.model.next_up(config_new, t.model.player1);
    //Because the obstacle has been destroyed, the player can move up and is
    // no longer at the same original position
    CHECK(t.model.player1.player_pos_ != original_position);

}

TEST_CASE("Model test 4- testing that explosions increase a player's score"){
    Game_config config_new = Game_config();
    Model model(config_new);
    Test_access t(model);
    //simulate an explosion hitting a player 1 because of player 2
    Posn<int> explosion_posn = t.model.player1.player_pos_;
    t.model.player1.bomb.explosion_pos.push_back(explosion_posn);
    //model needs to keep track of player2's new explosion
    t.model.all_explosion_pos_upd();
    //check that player 1 is in the explosion
    CHECK(t.model.player_in_explode(t.model.player1));
    //save player 2's original score to check later
    int p2_original_score = t.model.player2.destroy_obs;
    //increases player2's score
    t.model.health_detect(t.model.player1, t.model.player2);
    //check if player2's score has actually increased
    CHECK(t.model.player2.destroy_obs>p2_original_score);
}

TEST_CASE("Model test 5- self harm and forcing game over "){
    Game_config config_new = Game_config();
    Model model(config_new);
    Test_access t(model);


    t.model.player1.player_health_ = 1;
    //simulate an explosion hitting a player 1
    Posn<int> explosion_posn = t.model.player1.player_pos_;
    //because its self harm, the explosion must be added to the player1's
    // list of explosion
    t.model.player1.bomb.explosion_pos.push_back(explosion_posn);
    //model needs to keep track of player 1's new explosion
    t.model.all_explosion_pos_upd();
    //check that player 1 is in the explosion
    CHECK(t.model.player_in_explode(t.model.player1));
    //save player 2's original score to check later
    int p2_original_score = t.model.player1.destroy_obs;
    int p1_original_health = t.model.player1.player_health_;
    //increases player2's score because it is self harm
    t.model.health_detect(t.model.player1, t.model.player2);
    //check if player2's score has actually increased
    CHECK(t.model.player2.destroy_obs>p2_original_score);
    //check if player2's health has actually increased
    CHECK(t.model.player1.player_health_<p1_original_health);
    //check that game should be over
    CHECK(t.model.get_game_over());

}

 //TEST_CASE("close to dest functionality")
 //{
     //Game_config config_new = Game_config();
     //Model model(config_new);

     // // intialize a vector
     //  std::vector<ge211::Posn<int>> v = {{2,1},{5,4},{6,5}};
//      check one below
//     CHECK(t.close_to_dest({3,1},v) == true);
//     // check one above
//     CHECK(t.close_to_dest({1,1},v) == true);
//     // check one to the right
//     CHECK(t.close_to_dest({7,5},v) == true);
//     // check one to the left
//     CHECK(t.close_to_dest({4,4},v) == true);
//     // check the exact position in v
//     CHECK(t.close_to_dest({5,4},v) == true);
//     // check a position far away
//     CHECK(t.close_to_dest({7,1},v) == false);
//     std::cout << t.model.config.players[0] << t.model.config.players[1];
 //}

// TEST_CASE("random generated walls behavior")
// {
//     Model model;
//     Test_access t(model);
//     std::vector<Model::Position> v;
//     v = t.random_walls();
//     // check no made object of v is around {3,3} or {12,11} which are
//     // players' born location
//     CHECK(t.close_to_dest(ge211::Posn<int> {3,3},v) == false);
//     CHECK(t.close_to_dest(ge211::Posn<int> {12,11},v) == false);
// }

// TEST_CASE("behavior of all_explosion_pos_upd"){
//
// }
//
// TEST_CASE("behavior of obs_obj_upd"){
//
// }
//
// TEST_CASE("board to screen and screen to board ")
// {
//     Model model;
//     Test_access t(model);
//     Game_config::Pixel_Position p = t.board_to_screen({3,1});
//     CHECK(p == ge211::Posn<int> {120,100});
//     Game_config::Pixel_Position q = t.screen_to_board({120,100});
//     CHECK(q == ge211::Posn<int> {3,1});
// }

///
/// Member function definitions for Test_access
///
//
// Test_access::Test_access(Model& model)
//     :model(model)
// {}
//
// bool
// Test_access::close_to_dest(
//         Model::Position obj,
//         std::vector<Model::Position> hit_obj)
// {
//     return model.config.close_to_dest(obj, hit_obj);
// }
//
// std::vector<Model::Position>
// Test_access::random_walls()
// {
//     return model.config.random_walls();
// }
//
// Game_config::Pixel_Position
// Test_access::board_to_screen(Game_config::Board_Position p) const
// {
//     return model.config.board_to_screen(p);
// }
//
// Game_config::Board_Position
// Test_access::screen_to_board(Game_config::Board_Position p) const
// {
//     return model.config.screen_to_board(p);
// }


