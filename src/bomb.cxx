#include "bomb.hxx"

Bomb::Bomb(Position p)
    : bomb_pos {p}
{}

void
Bomb::explosion_upd(Game_config const& config)
{
    close_to_bomb(bomb_pos, config.all_pos_vec());
}

void
Bomb::close_to_bomb(ge211::Posn<int> obj, std::vector<ge211::Posn<int>> area)
{
    explosion_pos.clear();
    for (ge211::Posn<int> pos : area){
        if ((obj == pos) || (obj.up_by(1) == pos) || (obj.right_by(1) == pos)
            || (obj.down_by (1) == pos) || (obj.left_by(1)) == pos){
            explosion_pos.push_back(pos);
        }
    }
}