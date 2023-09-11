#include "player.hxx"

/// takes in game_config and board position of the player
Player::Player(Game_config const& config, Position board_pos)
    : player_pos_ {board_pos},
      bomb(board_pos),
      live(true),
      player_health_ {100},
      destroy_obs(0),
      player_velocity(1000,800)
{}
    // std::cout<<"This is player"<<std::endl;


// void
// Player::calc_obj(std)
// {
//     all_obj_.clear();
//     for (auto pos:config.wall_pos_){
//
//     }
// }

// Player
// Player::next_right(Game_config const& config)
// {
//     Player result(config,Player::player_pos_);
//     std::cout<<result.player_pos_;
//     // std::cout<<result.hit_obj(config, result.player_pos_,
//     //                config.unmov_objs_);
//     result.player_pos_.x += config.grid_dim_.width + config.spacing;
//     if (result.hit_obj(config, result.player_pos_,
//                                          config.unmov_objs_) == true){
//         Player::player_pos_.x += config.grid_dim_.width + config.spacing;
//     }
//     std::cout<<Player::player_pos_;
//     return
// }


bool
Player::hit_obj(Game_config const& config, Game_config::Board_Position posn,
                std::vector<Position> v)
{
    for (auto p : v) {
        if (posn == p){
            return true;
        }
    }
    return false;
    // ge211::Posn<int> right_bound = {posn.x + config.grid_dim_.width, posn.y};
    // ge211::Posn<int> left_bound = posn;
    // ge211::Posn<int> up_bound = posn;
    // ge211::Posn<int> bottom_bound = {posn.x, posn.y + config.grid_dim_
    //                                  .height};
    // for(auto p: v){
    //     if ((p.x <= right_bound.x) && p.x + config.grid_dim_.width + config
    //             .spacing >= left_bound.x && p.y <= bottom_bound.y && (p.y +
    //             config.grid_dim_.height + config.spacing) >= up_bound
    //             .y){
    //         return true;
    //     }
        // else if ((p.x + config.grid_dim_.width + config
        //         .spacing) >= left_bound.x){
        //     return true;
        // }
        // else if (p.y <= bottom_bound.y) {
        //     return true;
        // }
        // else if ((p.y + config.grid_dim_.height + config.spacing) >= up_bound
        // .y){
        //     return true;
        // }
    // }
    // return false;
}

bool
Player::hit_border(const Game_config& config, Player::Position posn)
{
    if (posn.x < 0){
        return true;
    }
    else if (posn.x >= config.path_cols){
        return true;
    }
    else if (posn.y < 0){
        return true;
    }
    else if (posn.y >= config.path_rows){
        return true;
    }
    else{
        return false;
    }
}

void
Player::throw_bomb(Bomb& b, Player& player)
{
    b.bomb_pos = player.player_pos_;
}

// int
// Player::player_hp_state()
// {
//     return Player::player_health_;
// }