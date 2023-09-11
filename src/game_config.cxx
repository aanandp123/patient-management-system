
// create initial states

#include "game_config.hxx"
#include <iostream>
#include <unordered_set>
#include <iterator>
using namespace ge211;

// Constructor of Game_config
Game_config::Game_config()
    : dims_ {15,14},
      scene_dims {600,500},
      path_cols{15},
      path_rows {14},
      top_margin {70},
      obs_count {60},
      player1 {3,3},
      player2 {12,11},
      spacing {3},
      grid_dim_((scene_dims.width - (path_cols * spacing))/path_cols,
                (scene_dims.height - top_margin-(path_cols * spacing))/path_rows)
{
//     Board_Dimensions dims_ = {path_cols,path_rows};
}

Game_config::Board_Rectangle
Game_config::all_positions() const
{
    return Board_Rectangle::from_top_left(the_origin, dims_) ;
}

Game_config::Board_Dimensions
Game_config::dims()
{
    return dims_;
}

std::vector<Game_config::Board_Position>
Game_config::all_pos_vec() const
{
    std::vector<Game_config::Board_Position> all_pos;
    for(auto pos: all_positions()){
        all_pos.push_back(pos);
    }
    // std::cout<<"??"<<std::endl;
    return all_pos;
}

std::vector<ge211::Posn<int>>
Game_config::random_walls() const
{
    //
    std::vector<ge211::Posn<int>> walls;
    // random num for posn.x of wall
    while ((int)walls.size() != wall_count){
        ge211::Random_source<int> wall_posn_x(0,path_cols - 1);
        // random num for posn.y of wall
        ge211::Random_source<int> wall_posn_y(0,path_rows - 1);
        int x = wall_posn_x.next();
        int y = wall_posn_y.next();
        // check if the board position is one grid away from the players
        if (close_to_dest({x,y},players) == false){
            // if (walls.size() != 0) {
            //     // for(Model::Position pos : walls) {
            //     //     if(ge211::Posn<int> {x,y} == pos){
            //     //         break;
            //     //     }
            //     // }
            //     // if ((std::count(walls.begin(),
            //     //                walls.end(),
            //     //                ge211::Posn<int> {x, y}))== false){
            //     //     walls.push_back({x,y});
            //     // }
            //
            //
            //     walls.push_back({x, y});
            //
            // }
            walls.push_back({x,y});
        }
    }
    return walls;
}

std::vector<ge211::Posn<int>>
Game_config::wall_pos_state() const
{
    return wall_pos_;
}

std::vector<ge211::Posn<int>>
Game_config::random_obs()
{
    std::vector<ge211::Posn<int>> obs;
    std::unordered_set<ge211::Posn<int>> set;
    // random num for posn.x of wall
    while ((int)obs.size() != obs_count){
        ge211::Random_source<int> obs_posn_x(0,path_cols - 1);
        // random num for posn.y of wall
        ge211::Random_source<int> obs_posn_y(0,path_rows - 1);
        int x = obs_posn_x.next();
        int y = obs_posn_y.next();
        // check if the board position is one grid away from the players
        if (close_to_dest({x,y},players) == false &&
            close_to_dest({x,y},wall_pos_) == false ){
            obs.push_back({x,y});
        }
    }
    for (auto pos: obs){
        set.insert(pos);
    }
    obs.assign(set.begin(),set.end());
    return obs;
}

std::vector<ge211::Posn<int>>
Game_config::obs_pos_state() const
{
    return obs_pos_;
}

///
/// Helper functions
///
bool
Game_config::close_to_dest(ge211::Posn<int> obj, std::vector<ge211::Posn<int>>
hit_obj) const
{
    for (ge211::Posn<int> pos : hit_obj){
        if ((obj == pos) || (obj.up_by(1) == pos) || (obj.right_by(1) == pos)
            || (obj.down_by (1) == pos) || (obj.left_by(1)) == pos){
            return true;
        }

    }
    return false;
}

std::vector<ge211::Posn<int>>
Game_config::init_unmov_objs()
{
    std::vector<ge211::Posn<int>> v;
    for(auto pos: wall_pos_){
        v.push_back(pos);
    }
    for(auto pos:obs_pos_){
        v.push_back(pos);
    }
    return v;
}

///
/// Helper functions
///

Game_config::Pixel_Position
Game_config::board_to_screen(Board_Position logical) const{
    Pixel_Position physical = {0,0};
    physical.x = logical.x * ((grid_dim_.width) + spacing);
    physical.y = top_margin + logical.y * ((grid_dim_.height) + spacing);
    return physical;
}

Game_config:: Board_Position
Game_config:: screen_to_board (Pixel_Position physical) const{
    Board_Position logical = {0,0};
    logical.x = physical.x / ((grid_dim_.width) + spacing);
    logical.y = (physical.y - top_margin) / ((grid_dim_.height) + spacing);
    return logical;
}


