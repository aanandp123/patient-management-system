#pragma once

#include "model.hxx"
#include "view.hxx"

#include <ge211.hxx>

using namespace ge211;

class Controller : public ge211::Abstract_game
{
public:
    Controller(Model& model);

protected:
    void draw(ge211::Sprite_set& set) override;
    ge211::Dims<int> initial_window_dimensions() const override;
    void on_key(ge211::events::Key) override;

   void on_frame(double dt) override;

private:
    Model model_;
    View view_;
    double elapsed_time_p2_;
    double explode_time_p2_;
    double elasped_time_p1_;
    double explode_time_p1_;
};
