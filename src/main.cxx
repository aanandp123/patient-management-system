#include "controller.hxx"
#include "player.hxx"
#include <iostream>

int
main()
{
    std::cout<<"??"<<std::endl;
    Game_config config_new = Game_config();
    Model model(config_new);
    std::cout<<"??"<<std::endl;
    Player player1(model.config, model.board_to_screen(model.config.player1));
    Controller(model).run();

    return 0;
}

