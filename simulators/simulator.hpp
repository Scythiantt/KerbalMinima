#pragma once

#include <vector>
#include <iostream>
#include <thread>

#include <SFML/Graphics.hpp>

#include "model/planet.hpp"
#include "model/spacecraft.hpp"

namespace minima {

struct Binding {

};

struct TextOutput {
    std::thread thread;

    void print() {

    }
};

struct GraphicsOutput {
    std::thread thread;

    sf::RenderWindow window;
    sf::View view;

    std::vector<Binding> bindings;

    GraphicsOutput(int width, int height, const char* title) {
        window.create(sf::VideoMode(width, height), title);
        view = window.getView();

        view.setCenter(0.0f, 0.0f);
        window.setView(view);

        thread = std::thread(&display);
    }

    void display() {
        while (window.isOpen()) {
            sf::Event event;
            window.pollEvent(event);
            if (event.type == sf::Event::Closed) window.close();

            window.clear();
            window.display();
        }        
    }
};

struct Simulator {
    GraphicsOutput& graphics_output;
    TextOutput& text_output;

    float step_size;

    Simulator(GraphicsOutput& graphics_output, TextOutput& text_output) : graphics_output(graphics_output), text_output(text_output) {

    }

    void step() {


    }
};

}

