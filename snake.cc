#include "snake.h"
#include "snake_decls.h"
#include "vector"
#include "list"

using namespace std;

struct snake {
    int col, row, dir, status;
    long unsigned int len;
    vector<char> board;
    list<pair<int, int>> player;
    snake(int c, int r, int l)
    : board{vector<char> (c*r, ' ')} , col{c},
    row{r}, len{l}, status{SNAKE_NEW} {}
};

typedef struct snake snake;

pair<int, int> create_new_piece(pair<int, int> & head, int dir) {
    pair<int, int> res = head;
    if (dir == SNAKE_UP) 
        res.second-=1;
    else if (dir == SNAKE_DOWN) 
        res.second+=1;
    else if (dir == SNAKE_RIGHT) 
        res.first+=1;
    else
        res.first-=1;
    return res;
}

bool check_out_of_bounds(pair<int, int> & head, int row, int col) {
    return head.first < 0 || head.first >= col || head.second < 0 || head.second >= row;
}

bool check_hits (pair<int, int> & head, list<pair<int, int>> & player) {
    for (auto el : player) {
        if (el == head)
            return true;
    }
    return false;
}


bool checkDir(int el) {
    return el != SNAKE_DOWN && el != SNAKE_LEFT 
        && el != SNAKE_RIGHT && el != SNAKE_UP;
}

int get_coordinate(int row, int colums, int c) {
    return row*c + colums;
}

struct snake * snake_new (int c, int r, int l) {
    if (c < 0 || r < 0 || l < 0) return nullptr;
    return new snake(c, r, l);
}

void snake_destroy (struct snake * game) {
    delete game;
}

int snake_start (struct snake * game, int c, int r, int d) {
    list<pair<int,int>> & player = game->player;
    if (c < 0 || r < 0 || r >= game->row || c >= game->col || checkDir(d)){
        game->status = SNAKE_FAIL;
        return SNAKE_FAIL;
    } 
    player.push_back({c, r}), game->dir = d, game->status = SNAKE_OKAY;
    return SNAKE_OKAY;
}

int snake_change_direction (struct snake * game, int dir) {
    int & player = game->dir;
    if ((player == SNAKE_DOWN && dir == SNAKE_UP) || (player == SNAKE_RIGHT && dir == SNAKE_LEFT)
    || (player == SNAKE_UP && dir == SNAKE_DOWN )|| (player == SNAKE_LEFT && dir == SNAKE_RIGHT)) {
        game->status = SNAKE_FAIL;
        return SNAKE_FAIL;
    }
    player = dir;
    return SNAKE_OKAY;
}

int snake_step (struct snake * game) {
    if (game->status != SNAKE_OKAY) 
        return SNAKE_FAIL;
    pair<int, int> new_piece = create_new_piece(game->player.front(), game->dir);
    if (check_out_of_bounds(new_piece, game->row, game->col) ||
        check_hits(new_piece, game->player)) {
            game->status = SNAKE_FAIL;
            return SNAKE_FAIL;
        }
    game->player.push_front(new_piece);
    if (game->player.size() > game->len)
        game->player.pop_back();
    return SNAKE_OKAY;
}
const char * snake_get_field (const struct snake * game) {
    vector<char> board = game->board;
    if (game->player.size() == 0) return &board[0];
    int cur = 0;
    for (auto el : game->player) {
        char & tmp = board[get_coordinate(el.first, el.second, game->col)];
        if (cur==0) 
            tmp = '@';
        else if (cur < game->player.size()-1) 
            tmp = '#';
        else 
            tmp = '+';
    }
    return &board[0];
}

int snake_get_status (const struct snake * game) {
    return game->status;
} 