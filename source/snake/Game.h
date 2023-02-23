#include <SFML/Graphics.hpp>
#include <map>
#include <chrono>

enum CellType {
    EMPTY,
    SNAKE_HEAD,
    SNAKE_BODY,
    FOOD
};

enum MoveDirection {
    UP,
    RIGHT,
    DOWN,
    LEFT,
    NONE
};

struct BoardCell {
    CellType type = CellType::EMPTY;

    // For "snake" object
    MoveDirection direction;
};

enum GameState {
    STOP,
    PLAY
};

struct GameOptions {
    int board_height;
    int board_width;

    int max_foods;

    sf::Time tick;

    GameOptions();
};

struct RenderOptions {
    // Layout
    sf::Vector2i cell_size;
    int border_width;

    // Color Scheme
    sf::Color background_color;
    sf::Color board_background_color;
    std::map<CellType, sf::Color> board_cell_color;

    RenderOptions();
};

const sf::Vector2i MOVE_DELTA[4] = {
    {0, -1},
    {1, 0},
    {0, 1},
    {-1, 0}
};

class Game {
private:
    int board_height;
    int board_width;
    BoardCell** board;

    sf::Vector2i snake_head_pos, snake_end_pos;

    GameState game_state;

    int food_count;

    sf::Clock update_clock;

    /** @deprecated */
    sf::Vector2i getSnakeHeadCoords();


    void initBoard(int width, int height);
    bool generateRandomFood();

    void deleteBoard();

    GameOptions game_options;

    MoveDirection new_direction;

public:
    Game();
    ~Game();
    void setMoveDirection(MoveDirection);
    void step();

    void init(GameOptions game_opts = GameOptions());
    void update();

    void render(sf::RenderWindow& window, RenderOptions render_opts = RenderOptions());
};