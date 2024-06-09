#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <memory>
#include <random>


std::random_device rd;
std::mt19937 gen(rd());

int randint(int min, int max)
{
    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Minecrap - 0.1", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);


    sf::SoundBuffer pop, gameover_buffer;
    if (!pop.loadFromFile("assets/sound/pop.wav") || (!gameover_buffer.loadFromFile("assets/sound/game-over-sound.wav"))) {
        std::cerr << "Failed to load the sounds.\n";
        return EXIT_FAILURE;
    }
    sf::Sound pop_sound(pop), gameover_sound(gameover_buffer);

    sf::Music music;
    if (!music.openFromFile("assets/sound/soundtrack.wav")) {
        std::cerr << "Failed to load the sounds.\n";
        return EXIT_FAILURE;
    }


    /*Ponteiros*/
    //sf::RectangleShape* object = new sf::RectangleShape();
    //object->setSize(sf::Vector2f(100.f, 100.f));
    //object->setFillColor(sf::Color(0, 255, 0));
    //object->setPosition(sf::Vector2f(10.f, 10.f));
    /*Ponteiro Inteligente Lib memory*/
    //auto object = std::make_shared<sf::RectangleShape>();

    sf::Font font;
    if (!font.loadFromFile("assets/fonts/Minecraft.ttf")){
        std::cerr << "Failed to read the font.\n";
        return EXIT_FAILURE;
    }
    sf::Text score, life, gameover_text,restart, paused_text;

    score.setFont(font);
    score.setString("Score: 0");
    score.setFillColor(sf::Color::White);
    score.setPosition(5.f, 5.f);

    life.setFont(font);
    life.setString("Life: 5");
    life.setFillColor(sf::Color::White);
    life.setPosition(1130.f, 5.f);
    
    restart.setFont(font);
    restart.setString("To restart press F1");
    restart.setFillColor(sf::Color::White);
    restart.setPosition(960.f, 5.f);

    sf::FloatRect textRect = gameover_text.getLocalBounds();
    gameover_text.setFont(font);
    gameover_text.setFillColor(sf::Color::Red);
    gameover_text.setString("Game Over");
    gameover_text.setPosition(350,300);
    gameover_text.setCharacterSize(100);

    paused_text.setFont(font);
    paused_text.setString("                  Paused\nPress Esc to return to the game.");
    paused_text.setFillColor(sf::Color::White);
    paused_text.setPosition(350, 300);
    paused_text.setCharacterSize(40);


    sf::Texture texture, background, hammer, pause;
    if (!background.loadFromFile("./assets/img/fundo.jpg") || !texture.loadFromFile("./assets/img/minecrap.png") 
        || !hammer.loadFromFile("./assets/img/hammer.png") || !pause.loadFromFile("./assets/img/paused.jpg")) {
        std::cerr << "Failed to read the images.\n";
        return EXIT_FAILURE;
    }

    sf::Sprite object(texture), bg(background), ham(hammer), psd(pause);

    float x = static_cast<float>(randint(10, int(window.getSize().x - texture.getSize().x - 10)));
    object.setPosition(sf::Vector2f(x, 10.f));

    sf::Vector2i pos_mouse_win;//Mouse position window
    sf::Vector2f pos_mouse_coord;//Store maped coord

    bool pressed = false, gameover = false, paused = false;
    int points = 0;
    int health = 5;
    float speed = 2.f;

    //BLOCOS
    std::vector<sf::Sprite> objs;
    size_t max_objs = 5;
    float obj_speed_max = 10.f;
    float obj_speed = obj_speed_max;

    while (window.isOpen())
    {
        sf::Event event;
        if (music.getStatus() == sf::Music::Stopped || music.getStatus() == sf::Music::Paused) {
            music.play();
        }
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::Resized) {
                sf::FloatRect visible_area(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visible_area));
            }

            if (event.type == event.MouseButtonPressed) {
                pressed = false;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
                paused = true;
            }

            pos_mouse_win = sf::Mouse::getPosition(window);
            pos_mouse_coord = window.mapPixelToCoords(pos_mouse_win);
            ham.setPosition(((sf::Vector2f)pos_mouse_win));
        }

        if (gameover) {
            music.stop();
            window.setMouseCursorVisible(true);
            window.clear(sf::Color::Black);
            window.draw(score);
            window.draw(restart);
            window.draw(gameover_text);
            window.display();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {
                objs.clear();
                health = 5;
                life.setString("Life: " + std::to_string(health));
                points = 0;
                score.setString("Score: " + std::to_string(points));
                gameover = false;
                window.setMouseCursorVisible(false);
                speed = 2.f;
            }
        }
        else {
            if (paused) {
                music.pause();
                window.clear();
                window.draw(psd);
                window.draw(paused_text);
                window.display();
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                    paused = false;
                }
            }
            else {
                //Game Code
                //Add objects with delay
                if (objs.size() < max_objs) {
                    if (obj_speed >= obj_speed_max) {
                        x = static_cast<float>(randint(10, int(window.getSize().x - texture.getSize().x)));
                        object.setPosition(x, 10.f);
                        objs.push_back(object);
                        obj_speed = 0.f;
                    }
                    else {
                        obj_speed += 1.f;
                    }
                }
                //Move and delete
                for (int i{}; i < objs.size(); ++i) {
                    bool del = false;
                    objs[i].move(0.f, speed);
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !pressed) {
                        if (objs[i].getGlobalBounds().contains(pos_mouse_coord)) {
                            pop_sound.play();
                            del = true;
                            points += 10.f;
                            score.setString("Score: " + std::to_string(points));
                            pressed = true;
                            if (points % 100 == 0) {
                                speed += 0.1f;
                            }
                            if (points % 1000 == 0) {
                                ++health;
                                life.setString("Life: " + std::to_string(health));
                            }
                        }
                    }
                    if (objs[i].getPosition().y > window.getSize().y) {
                        --health;
                        life.setString("Life: " + std::to_string(health));
                        del = true;
                        if (health <= 0) {
                            gameover_sound.play();
                            gameover = true;
                            //window.close();
                        }
                    }
                    if (del) {
                        objs.erase(objs.begin() + i);
                    }
                }
                window.clear();
                window.draw(bg);
                window.draw(score);
                window.draw(life);
                for (auto& e : objs) {
                    window.draw(e);
                }
                window.draw(ham);
                window.display();
            }

        }
    }

    /*Liberar Ponteiros*/
    //delete object;
    //object = nullptr;

    return 0;
}
