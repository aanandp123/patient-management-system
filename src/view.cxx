#include "view.hxx"
#include <iostream>
static ge211::Color const grid_color {100, 100, 100};
static ge211::Color const obs_color {100, 0, 0};
static ge211::Color const wall_color {0, 100, 0};
static ge211::Color const player1_color {0, 0, 100};
static ge211::Color const player2_color {255, 255, 0};
static ge211::Color const explosion_color {255, 39, 27};
static ge211::Color const burn_color_2 {255,255 ,0 };
static ge211::Color const burn_color_1 {0,0 ,100 };
View::View(Model const& model)
        : enter_key(false),
          space_key(false),
          over_three_p2(false),
          over_three_p1(false),
          player2_bombed_(false),
          player1_bombed_(false),
          model_(model),
          grid_dim_((model_.config.scene_dims.width - (model_.config
          .path_cols * model_.config.spacing))/model_.config.path_cols,
                    (model_.config.scene_dims.height - model_.config.top_margin-
                    (model_.config.path_cols *model_.config.spacing))/model_.config.path_rows),
          grid_sprite(grid_dim_, grid_color),
          obs_sprite(grid_dim_, obs_color),
          wall_sprite(grid_dim_, wall_color),
          player1_sprite(grid_dim_, player1_color),
          player2_sprite(grid_dim_, player2_color),
          bomb_sprite(grid_dim_.height/2,wall_color),
          explosion_sprite(grid_dim_,explosion_color),
          burn_sprite_2(grid_dim_.height/2,burn_color_2),
          burn_sprite_1(grid_dim_.height/2,burn_color_1),
          black_sprite(grid_dim_,{0,0,0})
{ }

void
View::draw(ge211::Sprite_set& set)
{
    // std::cout<<model_.get_player1().player_pos_;
    // print out player 1 and health in window
    ge211::Text_sprite::Builder player1_builder(sans15_);
    player1_builder << "Player 1: " << model_.player1.player_health_;
    player1_text_sprite_.reconfigure(player1_builder);
    set.add_sprite(player1_text_sprite_, {0,0});
    // displaying score of player 1
    ge211::Text_sprite::Builder score1_builder(sans15_);
    score1_builder << "Score p1: " << model_.player1.destroy_obs;
    score1_sprite_.reconfigure(score1_builder);
    set.add_sprite(score1_sprite_, {0,40});
    // ge211::Text_sprite::Builder score1_builder(sans28_);
    // score1_builder << "Score p1: ";
    // score1_sprite_.reconfigure(score1_builder);
    // set.add_sprite(score1_sprite_, {0, 60});

    // print out player 2 and health in window
    ge211::Text_sprite::Builder player2_builder(sans15_);
    player2_builder << "Player 2: " << model_.player2.player_health_;
    player2_text_sprite_.reconfigure(player2_builder);
    set.add_sprite(player2_text_sprite_, {(2*model_.config.scene_dims.width)/3,
                                       0});
    // displaying socre of player 2
    ge211::Text_sprite::Builder score2_builder(sans15_);
    score2_builder << "Score p2: " << model_.player2.destroy_obs;
    score2_sprite_.reconfigure(score2_builder);
    set.add_sprite(score2_sprite_, {(2*model_.config.scene_dims.width)/3,40});

    if (model_.get_game_over() == true){
        for (auto pos: model_.config.all_pos_vec()){
            set.add_sprite(black_sprite, board_to_screen(pos),6);
        }
        ge211::Text_sprite::Builder gameover_builder(sans50_);
        gameover_builder << "Game Over";
        gameover_sprite_.reconfigure(gameover_builder);
        set.add_sprite(gameover_sprite_, {(model_.config.scene_dims.width/4 ),
                                          (model_.config.scene_dims.height/2)
                                          },7);
    }
    // draw grey for all grids
    for (auto pos: model_.config.all_pos_vec()){
        set.add_sprite(grid_sprite, board_to_screen(pos),0);
    }

    // draw all wall objects
    for (auto pos: model_.config.wall_pos_state()){
            set.add_sprite(wall_sprite, board_to_screen(pos),1);
    }

    // draw obstacles
    for (auto pos: model_.obs_pos_state()){
        set.add_sprite(obs_sprite, board_to_screen(pos),1);
    }

    // draw player 1
    set.add_sprite(player1_sprite, board_to_screen(model_.get_player1().player_pos_), 2);

    // draw player 2
    set.add_sprite(player2_sprite, board_to_screen(model_.get_player2().player_pos_), 2);

    // check if player 2 places bomb and consecutive actions needed
    if (enter_key == true){
        set.add_sprite(bomb_sprite, board_to_screen(model_.player2.bomb
        .bomb_pos),3);
    }
    if (over_three_p2 == true){
        for(size_t i = 0; i < model_.get_player2().bomb.explosion_pos.size();
            i++){
            set.add_sprite(explosion_sprite, board_to_screen(model_
                                                                .get_player2()
                                                                .bomb.explosion_pos[i]),
                           3);
        }
    }
    if (player2_bombed_ == true){
        set.add_sprite(burn_sprite_2, board_to_screen(model_.get_player2()
        .player_pos_),4);
    }

    // check if player 2 places bomb and consecutive actions needed
    if (space_key == true){
        set.add_sprite(bomb_sprite, board_to_screen(model_.player1.bomb
                                                          .bomb_pos),3);
    }
    if (over_three_p1 == true){
        for(size_t i = 0; i < model_.get_player1().bomb.explosion_pos.size();
            i++){
            set.add_sprite(explosion_sprite, board_to_screen(model_
                                                                     .get_player1()
                                                                     .bomb.explosion_pos[i]),
                           3);
        }
    }
    if (player1_bombed_ == true){
        set.add_sprite(burn_sprite_1, board_to_screen(model_.get_player1()
                                                            .player_pos_),4);
    }
}

ge211::Dims<int>
View::initial_window_dimensions() const
{
    return model_.config.scene_dims;
}

View::Position
View::board_to_screen(Model::Position logical) const{
    Position physical = {0,0};
    physical.x = logical.x * ((grid_dim_.width) + model_.config.spacing);
    physical.y = model_.config.top_margin + logical.y * ((grid_dim_.height) +
            model_.config.spacing);
    return physical;
}

Model :: Position
View :: screen_to_board (View :: Position physical) const{
    Position logical = {0,0};
    logical.x = physical.x / ((grid_dim_.width) + model_.config.spacing);
    logical.y = (physical.y - model_.config.top_margin) / ((grid_dim_.height)
            + model_.config.spacing);
    return logical;
}
