#include "controller.hxx"


Controller::Controller(Model& model)
        : model_(model),
          view_(model_),
          elapsed_time_p2_ {0},
          explode_time_p2_ {0},
          elasped_time_p1_ {0},
          explode_time_p1_ {0}
{ }

void
Controller::draw(ge211::Sprite_set& set)
{
    view_.draw(set);
}

ge211::Dims<int>
Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}

// think of the keys
void
Controller::on_key(ge211::events::Key key)
{
    if (key == ge211::Key::right()){
        if (view_.player2_bombed_ == false){
            model_.next_right(model_.config, model_.player2);
        }
        // model_.next_right(model_.config, model_.player2);
        // model_.player2.throw_bomb(model_.player2.bomb,model_.player2);
        // std::cout<<model_.player2.bomb.bomb_pos;
    }
    else if (key == ge211::Key::up()){
        if (view_.player2_bombed_ == false){
            model_.next_up(model_.config, model_.player2);
        }
        // model_.next_up(model_.config, model_.player2);
        // model_.player2.throw_bomb(model_.player2.bomb,model_.player2);
    }
    else if (key == ge211::Key::left()){
        if (view_.player2_bombed_ == false){
            model_.next_left(model_.config, model_.player2);
        }
        // model_.next_left(model_.config, model_.player2);
        // model_.player2.throw_bomb(model_.player2.bomb,model_.player2);
    }
    else if (key == ge211::Key::down()){
        if (view_.player2_bombed_ == false){
            model_.next_down(model_.config, model_.player2);
        }
        // model_.next_down(model_.config, model_.player2);
        // model_.player2.throw_bomb(model_.player2.bomb,model_.player2);
    }
    if (key == ge211::Key::code('\r')){
        if (elapsed_time_p2_ > 3.0 || elapsed_time_p2_ == 0){
            model_.player2.throw_bomb(model_.player2.bomb,model_.player2);
            // std::cout<<model_.player2.bomb.bomb_pos;
        }
        // model_.player2.throw_bomb(model_.player2.bomb,model_.player2);
        // std::cout<<model_.player2.bomb.bomb_pos;
        view_.enter_key = true;
        // if (view_.over_three == true){
        //     model_.player2.bomb.explosion_upd(model_.config);
        //     model_.obs_pos_upd();
        // }
        // std::cout<<model_.player2.bomb.explosion_pos[1];
    }
    // for player 1
    if (key == ge211::Key::code('a')){
        if (view_.player1_bombed_ == false){
            model_.next_left(model_.config, model_.player1);
        }
    }
    else if (key == ge211::Key::code('s')){
        if (view_.player1_bombed_ == false){
            model_.next_down(model_.config, model_.player1);
        }
    }
    else if (key == ge211::Key::code('d')){
        if (view_.player1_bombed_ == false){
            model_.next_right(model_.config, model_.player1);
        }
    }
    else if (key == ge211::Key::code('w')){
        if (view_.player1_bombed_ == false) {
            model_.next_up(model_.config, model_.player1);
        }
    }
    if (key == ge211::Key::code(' ')) {
        if (elasped_time_p1_ > 3.0 || elasped_time_p1_ == 0) {
            model_.player1.throw_bomb(model_.player1.bomb, model_.player1);
            // std::cout << model_.player1.bomb.bomb_pos;
        }
        view_.space_key = true;
    }
}

void
Controller::on_frame(double dt)
{
    // view_.player2_bombed_ = model_.player_in_explode(model_.player2);
    // view_.player1_bombed_ = model_.player_in_explode(model_.player1);
    // starting at hitting the enter key

    if (view_.enter_key == true){
        elapsed_time_p2_ += dt;
        if (elapsed_time_p2_ < 2.0){
            view_.enter_key = true;
        }
        else if (elapsed_time_p2_ >= 2.0){
            view_.enter_key = false;
            view_.over_three_p2 = true;
            model_.player2.bomb.explosion_upd(model_.config);
            model_.count_destory_obs(model_.player2);
            model_.obs_pos_upd();
            elapsed_time_p2_ = 0;
            view_.player2_bombed_ = model_.player_in_explode(model_.player2);
            // also checks for player 1
            view_.player1_bombed_ = model_.player_in_explode(model_.player1);
            // view_.enter_key = false;
        }
    }
    if (view_.over_three_p2 == true){
        for (size_t i = 0; i < model_.all_explosion_pos_state().size(); i++){
            std::cout<<"All explosion positions: "<< model_
            .all_explosion_pos_state()[i];
        }
        std::cout<<'\n' << "Player 2 location: "<<model_.player2.player_pos_;
        std::cout<<'\n' << "Player 1 location: "<<model_.player1.player_pos_<<'\n';
        view_.player2_bombed_ = model_.player_in_explode(model_.player2);
        // also checks for player 1
        view_.player1_bombed_ = model_.player_in_explode(model_.player1);
        // check if player2 is in the explosion area then deduct health
        // no need to check when calling the first time because player's
        // position will still be inside
        explode_time_p2_ += dt;
        if (view_.player2_bombed_ == true){
            model_.health_detect(model_.player2, model_.player1);
        }
        // also checks for player 1
        if (view_.player1_bombed_ == true){
            model_.health_detect(model_.player1, model_.player2);
        }

        if (explode_time_p2_ > 0.5){
            // std::cout<<"Time elapse" << model_.player2.bomb.bomb_pos;
            // model_.player2.bomb.explosion_upd(model_.config);
            // model_.obs_pos_upd();
            //model_.player2.throw_bomb(model_.player2.bomb,model_.player2);
            //std::cout<<"Time elapse" << model_.player2.bomb.bomb_pos;
            view_.over_three_p2 = false;
            view_.player2_bombed_ = false;
            view_.player1_bombed_ = false;
            model_.player2.bomb.explosion_pos.clear();
            explode_time_p2_ = 0;
        }
    }

    // // starting at hitting space key
    if (view_.space_key == true){
        elasped_time_p1_ += dt;
        if (elasped_time_p1_ < 2.0){
            view_.space_key = true;
        }
        else if (elasped_time_p1_ >= 2.0){
            view_.space_key = false;
            view_.over_three_p1= true;
            model_.player1.bomb.explosion_upd(model_.config);
            model_.count_destory_obs(model_.player1);
            model_.obs_pos_upd();
            // for (size_t i = 0; i < model_.obs_pos_state().size();i++ ){
            //     std::cout<<"obs left: "<<model_.obs_pos_state()[i]<<'\n';
            // }
            view_.player1_bombed_ = model_.player_in_explode(model_.player1);
            // checks for player 2
            view_.player2_bombed_ = model_.player_in_explode(model_.player2);
            elasped_time_p1_ = 0;
            // view_.player1_bombed_ = false;
            // view_.enter_key = false;
        }
    }
    if (view_.over_three_p1 == true){
        view_.player1_bombed_ = model_.player_in_explode(model_.player1);
        // checks for player 2 if the bomb is from player 1
        view_.player2_bombed_ = model_.player_in_explode(model_.player2);
        // if (view_.player1_bombed_ == true && explode_time_p1_ == 0.0){
        //     model_.health_detect(model_.player1);
        // }
        explode_time_p1_ += dt;
        if (view_.player1_bombed_ == true){
            model_.health_detect(model_.player1, model_.player2);
        }
        // check if player 2 got bombed from player 1's bomb
        if (view_.player2_bombed_ == true){
            model_.health_detect(model_.player2, model_.player1);
        }
        if (explode_time_p1_ > 0.5){
            // std::cout<<"Time elapse" << model_.player1.bomb.bomb_pos;
            // model_.player2.bomb.explosion_upd(model_.config);
            // model_.obs_pos_upd();
            //model_.player2.throw_bomb(model_.player2.bomb,model_.player2);
            //std::cout<<"Time elapse" << model_.player2.bomb.bomb_pos;
            view_.over_three_p1 = false;
            view_.player1_bombed_ = false;
            view_.player2_bombed_ = false;
            // clear the bomb explosion position after it take effect
            model_.player1.bomb.explosion_pos.clear();
            explode_time_p1_ = 0;
        }
    }
}