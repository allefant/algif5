#include <allegro5/allegro5.h>
#include "algif.h"
#include <stdio.h>

typedef struct {
    bool key[ALLEGRO_KEY_MAX];
    bool quit;

    double player_x, player_y;
    int dir;

    ALGIF_ANIMATION *skeleton[4];
} Game;

static ALGIF_ANIMATION *load(char const *name) {
    ALLEGRO_PATH *examples = al_create_path_for_directory("examples");
    ALLEGRO_PATH *path = al_create_path(name);
    ALGIF_ANIMATION *gif = algif_load_animation(al_path_cstr(path, '/'));
    if (!gif) {
        al_rebase_path(examples, path);
        gif = algif_load_animation(al_path_cstr(path, '/'));
    }
    al_destroy_path(examples);
    al_destroy_path(path);
    if (!gif) {
        printf("Could not load %s.\n", name);
        exit(1);
    }
    return gif;
}

Game *game_new(void) {
    Game *game = calloc(1, sizeof *game);
    game->skeleton[0] = load("skeleton_left.gif");
    game->skeleton[1] = load("skeleton_right.gif");
    game->skeleton[2] = load("skeleton_up.gif");
    game->skeleton[3] = load("skeleton_down.gif");
    return game;
}

void game_logic(Game *game) {
    if (game->key[ALLEGRO_KEY_ESCAPE]) {
        game->quit = true;
    }
    if (game->key[ALLEGRO_KEY_LEFT]) {
        game->dir = 0;
    }
    if (game->key[ALLEGRO_KEY_RIGHT]) {
        game->dir = 1;
    }
    if (game->key[ALLEGRO_KEY_UP]) {
        game->dir = 2;
    }
    if (game->key[ALLEGRO_KEY_DOWN]) {
        game->dir = 3;
    }

    if (game->dir == 0) {
        game->player_x -= 1;
    }
    if (game->dir == 1) {
        game->player_x += 1;
    }
    if (game->dir == 2) {
        game->player_y -= 1;
    }
    if (game->dir == 3) {
        game->player_y += 1;
    }

    if (game->player_x < 0) {
        game->player_x = 0;
    }
    if (game->player_x > 640 - game->skeleton[0]->width) {
        game->player_x = 640 - game->skeleton[0]->width;
    }
    if (game->player_y < 0) {
        game->player_y = 0;
    }
    if (game->player_y > 480 - game->skeleton[0]->height) {
        game->player_y = 480 - game->skeleton[0]->height;
    }
}

void game_render(Game *game) {
    al_clear_to_color(al_map_rgb_f(0, 0, 0));
    al_draw_bitmap(algif_get_bitmap(game->skeleton[game->dir],
        al_get_time()), game->player_x, game->player_y, 0);
    al_flip_display();
}

int main(void) {
    al_init();
    al_install_keyboard();
    ALLEGRO_DISPLAY *display = al_create_display(640, 480);
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1 / 60.0);

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    Game *game = game_new();

    bool redraw = true;
    al_start_timer(timer);
    while (!game->quit) {
        if (redraw) {
            game_render(game);
            redraw = false;
        }

        while (!al_is_event_queue_empty(event_queue)) {
            ALLEGRO_EVENT event;
            al_wait_for_event(event_queue, &event);

            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                game->quit = true;
            }

            if (event.type == ALLEGRO_EVENT_TIMER) {
                game_logic(game);
                redraw = true;
            }

            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                game->key[event.keyboard.keycode] = true;
            }

            if (event.type == ALLEGRO_EVENT_KEY_UP) {
                game->key[event.keyboard.keycode] = false;
            }
        }
    }
 
    return 0;
}
