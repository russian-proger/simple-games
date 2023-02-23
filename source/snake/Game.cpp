#include "Game.h"

Game::Game() {
    board = NULL;
    update_clock.restart();
    init();
}

Game::~Game() {
    deleteBoard();
}

void Game::update() {
    if (update_clock.getElapsedTime() > game_options.tick) {
        // Make tick

        bool eat = false;
        bool collision = false;
    {
        BoardCell& current_cell = board[snake_head_pos.y][snake_head_pos.x];
        MoveDirection dir = current_cell.direction;

        if (new_direction != MoveDirection::NONE) {
            dir = new_direction;
            new_direction = MoveDirection::NONE;
        }

        // Moving snake head
        sf::Vector2i new_snake_head_pos = {
            snake_head_pos.x + MOVE_DELTA[dir].x,
            snake_head_pos.y + MOVE_DELTA[dir].y
        };

        current_cell.type = CellType::SNAKE_BODY;
        
        BoardCell& next_cell = board[new_snake_head_pos.y][new_snake_head_pos.x];

        eat = (next_cell.type == CellType::FOOD);
        collision = (next_cell.type == CellType::SNAKE_BODY);

        next_cell.type = CellType::SNAKE_HEAD;

        current_cell.direction = dir;
        next_cell.direction = dir;

        snake_head_pos = new_snake_head_pos;
    }

        if (!eat) {
            // Moving snake End
            BoardCell& current_cell = board[snake_end_pos.y][snake_end_pos.x];
            current_cell.type = CellType::EMPTY;
            snake_end_pos = {
                snake_end_pos.x + MOVE_DELTA[current_cell.direction].x,
                snake_end_pos.y + MOVE_DELTA[current_cell.direction].y
            };
        } else {
            food_count--;
        }


        update_clock.restart();
    }

    if (food_count < game_options.max_foods) {
        food_count += generateRandomFood();
    }
}

void Game::render(sf::RenderWindow& window, RenderOptions render_opts) {

    sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().y));
    background.setFillColor(render_opts.background_color);
    window.draw(background);

    // Rendering table of cells

{
    auto wsize = window.getSize();
    int cx = render_opts.cell_size.x;
    int cy = render_opts.cell_size.y;
    int bw = render_opts.border_width;
    int ofx = (wsize.x - board_width * (cx + bw) - bw) / 2;
    int ofy = (wsize.y - board_height* (cy + bw) - bw) / 2;

    sf::RectangleShape background(sf::Vector2f(
        board_width *(cx + bw) + bw,
        board_height*(cy + bw) + bw
    ));

    background.setPosition(ofx, ofy);
    background.setFillColor(render_opts.board_background_color);

    sf::VertexArray board_cells(sf::Quads, 4 * board_width * board_height);
    
    for (int i = 0 ; i < board_height; i++) {
        for (int j = 0 ; j < board_width; j++) {
            int offset = (i*board_width+j) * 4;
            board_cells[offset + 0].position = sf::Vector2f(bw + ofx + j*(cx + bw)     , bw + ofy + i*(cy + bw)     );
            board_cells[offset + 1].position = sf::Vector2f(bw + ofx + j*(cx + bw) + cx, bw + ofy + i*(cx + bw)     );
            board_cells[offset + 2].position = sf::Vector2f(bw + ofx + j*(cx + bw) + cx, bw + ofy + i*(cx + bw) + cx);
            board_cells[offset + 3].position = sf::Vector2f(bw + ofx + j*(cx + bw)     , bw + ofy + i*(cx + bw) + cx);

            for (int k = 0; k < 4; k++) {
                board_cells[offset + k].color = render_opts.board_cell_color[board[i][j].type];
            }
        }
    }

    window.draw(background);
    window.draw(board_cells);
}

}

GameOptions::GameOptions() {
    this->board_height = 30;
    this->board_width  = 30;

    this->max_foods = 5;

    this->tick = sf::milliseconds(100);
}

RenderOptions::RenderOptions() {
    this->border_width = 5;
    this->cell_size = sf::Vector2i(30, 30);

    background_color = sf::Color(0x30, 0x30, 0x30);
    board_background_color = sf::Color(0x40, 0x40, 0x40);
    board_cell_color = {
        { CellType::EMPTY     , sf::Color(0x70, 0x70, 0x70) },
        { CellType::SNAKE_HEAD, sf::Color(0x70, 0x00, 0x00) },
        { CellType::SNAKE_BODY, sf::Color(0x70, 0x70, 0x00) },
        { CellType::FOOD      , sf::Color(0x00, 0x00, 0x70) }
    };
}

void Game::setMoveDirection(MoveDirection move_direction) {
    new_direction = move_direction;
}

void Game::init(GameOptions game_opts) {
    new_direction = MoveDirection::NONE;

    this->game_options = game_opts;

    printf("Game: Board initializing\n");
    this->initBoard(game_opts.board_width, game_opts.board_height);

    // Generate Snake Head
    printf("Game: Snake generating\n");

    snake_head_pos = sf::Vector2i(game_opts.board_width / 2, game_opts.board_height / 2);
    snake_end_pos = sf::Vector2i(snake_head_pos.x, snake_head_pos.y);

    board[snake_head_pos.y][snake_head_pos.x].type = CellType::SNAKE_HEAD;
    board[snake_head_pos.y][snake_head_pos.x].direction = MoveDirection::RIGHT;

    // Generate food on the board
    printf("Game: Food generating\n");

    food_count = 0;
    for (int i = 0; i < 5; i++) {
        food_count += generateRandomFood();
    }
}

bool Game::generateRandomFood() {
    int free_cells_cnt = 0;

    for (int i = 0; i < board_height; i++) {
        for (int j = 0; j < board_width; j++) {
            if (board[i][j].type == CellType::EMPTY) {
                free_cells_cnt++;
            }
        }
    }

    int order = rand() % free_cells_cnt;

    for (int i = 0; i < board_height; i++) {
        for (int j = 0; j < board_width; j++) {
            if (board[i][j].type == CellType::EMPTY) {
                if (order == 0) {
                    board[i][j].type = CellType::FOOD;
                    return true;
                } else {
                    order--;
                }
            }
        }
    }

    return false;
}

void Game::initBoard(int width, int height) {
    if (this->board != NULL) {
        printf("\tGame: deleting board\n");
        deleteBoard();
    }

    printf("\tGame: creating new board\n");
    this->board_width = width;
    this->board_height = height;

    board = new BoardCell* [height];
    for (int i = 0 ; i < height; i++) {
        board[i] = new BoardCell[width];
        for (int j = 0; j < width; j++) {
            board[i][j] = BoardCell();
        }
    }
}

sf::Vector2i Game::getSnakeHeadCoords() {
    for (int i = 0; i < board_height; i++) {
        for (int j = 0; j < board_width; j++) {
            if (board[i][j].type == CellType::SNAKE_HEAD) {
                return sf::Vector2i(i, j);
            }
        }
    }

    return sf::Vector2i(-1, -1);
}

void Game::deleteBoard() {
    for (int i = 0 ; i < board_height ; i++) {
        delete[] board[i];
    }
    delete board;
}