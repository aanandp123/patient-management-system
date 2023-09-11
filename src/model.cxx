#include "model.hxx"
#include <algorithm>
#include <iostream>
// create all initial states in game_config but copy all variables to the
// model file adn mutate the objects within model file
// two additional objects : bomb & explosion
// bomb & explosioin should have their own files and player includes bomb
// .hxxto acess bomb's method and model gets player thus having bomb stuff
// when calling player's function


Model::Model(Game_config const& config)
    : config(config),
      player1(config, config.player1),
      player2(config, config.player2),
      game_over(false)
{
    std::cout<<"THis is model"<<std::endl;
}


/*Model::Rectangle
Model::all_positions() const
{
    return Rectangle::from_top_left(the_origin, dims_) ;
}

Model::Dimensions
Model::dims()
{
    return dims_;
}

std::vector<Model::Position>
Model::all_pos_vec() const
{
    std::vector<Model::Position> all_pos;
    for(auto pos: all_positions()){
        all_pos.push_back(pos);
    }
    return all_pos;
}

std::vector<Model::Position>
Model::random_walls() const
{
    //
    std::vector<Model::Position> walls;
    // random num for posn.x of wall
    while (walls.size() != Model::config.wall_count){
        ge211::Random_source<int> wall_posn_x(0,config.path_cols - 1);
        // random num for posn.y of wall
        ge211::Random_source<int> wall_posn_y(0,config.path_rows - 1);
        int x = wall_posn_x.next();
        int y = wall_posn_y.next();
        // check if the board position is one grid away from the players
        if (Model::close_to_dest({x,y},Model::config.players) == false){
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
*/
Model::Dimensions
Model::dims_state()
{
    return Model::dims_;
}

Player
Model::get_player1() const
{
    return Model::player1;
}

Player
Model::get_player2() const
{
    return Model::player2;
}

std::vector<Model::Position>
Model::all_pos_state() const
{
    return Model::all_pos_;
}

std::vector<Model::Position>
Model::wall_pos_state() const
{
    return wall_pos_;
}

/*std::vector<Model::Position>
Model::random_obs() const
{
    std::vector<Model::Position> obs;
    // random num for posn.x of wall
    while (obs.size() != Model::config.obs_count){
        ge211::Random_source<int> obs_posn_x(0,config.path_cols - 1);
        // random num for posn.y of wall
        ge211::Random_source<int> obs_posn_y(0,config.path_rows - 1);
        int x = obs_posn_x.next();
        int y = obs_posn_y.next();
        // check if the board position is one grid away from the players
        if (Model::close_to_dest({x,y},Model::config.players) == false &&
        Model::close_to_dest({x,y},wall_pos_) == false ){
            obs.push_back({x,y});
        }
    }
    return obs;
}*/


void
Model::next_right(Game_config const& config, Player& player)
{
    Player result(config,player.player_pos_);
    result.player_pos_.x += 1;
    if (result.hit_obj(config, result.player_pos_,
                       unmov_objs_) == false && result.hit_border(config,result
                       .player_pos_) == false){
        player.player_pos_.x += 1;
    }
}

void Model::next_left(Game_config const& config, Player& player)
{
    Player result(config,player.player_pos_);
    result.player_pos_.x -= 1;
    if (result.hit_obj(config, result.player_pos_,
                       unmov_objs_) == false && result.hit_border(config,result
            .player_pos_) == false){
        player.player_pos_.x -= 1;
    }
}

void Model::next_up(Game_config const& config, Player& player)
{
    Player result(config,player.player_pos_);
    result.player_pos_.y -= 1;
    if (result.hit_obj(config, result.player_pos_,
                       unmov_objs_) == false && result.hit_border(config,result
            .player_pos_) == false){
        player.player_pos_.y -= 1;
    }
}

void Model::next_down(Game_config const& config, Player& player)
{
    Player result(config,player.player_pos_);
    result.player_pos_.y += 1;
    if (result.hit_obj(config, result.player_pos_,
                       unmov_objs_) == false && result.hit_border(config,result
            .player_pos_) == false){
        player.player_pos_.y += 1;
    }
}

std::vector<Model::Position>
Model::obs_pos_state() const
{
    return obs_pos_;
}

// void
// Model::calc_valid_pos()
// {
//     for (auto pos : Model::all_pos_state()){
//
//     }
// }

std::vector<Player::Position>
Model::unmov_objs_state()
{
    return Model::unmov_objs_;
}

void
Model::all_explosion_pos_upd()
{
    all_explosion_pos_.clear();
    for(auto p :player1.bomb.explosion_pos){
        all_explosion_pos_.push_back(p);
    }
    for(auto q:player2.bomb.explosion_pos){
        all_explosion_pos_.push_back(q);
    }
}

void
Model::count_destory_obs(Player& player)
{
    all_explosion_pos_upd();
    for (auto q:all_explosion_pos_){
        for (auto p : obs_pos_){
            if (p == q){
                player.destroy_obs += 100;
            }
        }
    }
}

void
Model::obs_pos_upd()
{
    all_explosion_pos_upd();
    // std::vector<Position> new_obs;
    // size_t j = 0;
    for (auto q:all_explosion_pos_){
        size_t i = 0;
        for (auto p : obs_pos_){
            if (p == q){
                obs_pos_.erase(obs_pos_.begin() + i);
                // std::cout<<"size: "<< obs_pos_.size() <<'\n';
                // std::vector<Position>::iterator new_end;
                //
                // remove(obs_pos_.begin(),obs_pos_.end() + 1,p);
                // std::cout<<"size: "<< obs_pos_.size() <<'\n';
                break;
            }
            i += 1;
            // if (p != q){
            //     new_obs.push_back(p);
            // }
        }
    }
    // obs_pos_.clear();
    // obs_pos_ = new_obs;
    unmov_objs_update();
}

void
Model::unmov_objs_update()
{
    unmov_objs_.clear();
    for(auto p : wall_pos_){
        unmov_objs_.push_back(p);
    }
    for(auto q:obs_pos_){
        unmov_objs_.push_back(q);
    }
}

std::vector<Model::Position>
Model::all_explosion_pos_state()
{
    return Model::all_explosion_pos_;
}

bool
Model::player_in_explode(Player& player)
{
    for (auto p: Model::all_explosion_pos_state()){
        if(player.player_pos_ == p){
            return true;
        }
    }
    return false;
}

void
Model::health_detect(Player& player, Player& stealer)
{
    if (player_in_explode(player)){
        player.player_health_ -= 1;
        stealer.destroy_obs += 100;
    }
    if (player.player_health_ <= 0 ){
        player.player_health_ = 0;
        game_over = true;
    }
}

bool
Model::get_game_over() const
{
    return Model::game_over;
}
// void
// Model::on_frame(double dt)
// {
//
// }



///
/// Helper functions
///
/*bool
Model::close_to_dest(Model::Position obj, std::vector<Model::Position>
        hit_obj) const
{
    for (Model::Position pos : hit_obj){
        if ((obj == pos) || (obj.up_by(1) == pos) || (obj.right_by(1) == pos)
        || (obj.down_by (1) == pos) || (obj.left_by(1)) == pos){
            return true;
        }

    }
    return false;
}*/

Player::Position
Model::board_to_screen(Model::Position logical) const{
    Position physical = {0,0};
    physical.x = logical.x * ((config.grid_dim_.width) + config
            .spacing);
    physical.y = config.top_margin + logical.y * ((config.grid_dim_.height) +
                                                         config.spacing);
    return physical;
}

Model :: Position
Model :: screen_to_board (Player :: Position physical) const{
    Position logical = {0,0};
    logical.x = physical.x / ((config.grid_dim_.width) + config.spacing);
    logical.y = (physical.y - config.top_margin) / ((config.grid_dim_
            .height) + config.spacing);
    return logical;
}



