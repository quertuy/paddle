#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include "constants.h"
#include <SDL_ttf.h>

int score = 0;
int game_is_running = false;
float ball_speed = 300.0;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* font = NULL;
SDL_Texture* text_texture = NULL;
int last_frame_time = 0;


struct pongEvent
{
    int timestamp;
    int ball_x;
    int ball_y;
    int paddle_x;
    int paddle_y;
    int score;
};

struct Node
{
    struct pongEvent event;
    struct Node* next;
};
struct Node* event_list_head = NULL;
struct game_object
{
    float x;
    float y;
    float width;
    float height;
    float vel_x;
    float vel_y;
} ball, paddle;

void save_event_history(struct Node* event_list_head, const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error al abrir el archivo para escribir el historial de eventos.\n");
        return;
    }

    struct Node* current = event_list_head;
    while (current != NULL) {
        struct pongEvent event = current->event;
        fprintf(file, "%d|%d|%d|%d|%d|%d|%d\n",
            event.timestamp,
            event.ball_x,
            event.ball_y,
            event.paddle_x,
            event.paddle_y,
            event.score);

        current = current->next;
    }

    fclose(file);
}

void add_event(struct Node** head, struct pongEvent new_event)
{
    // Crea un nuevo nodo
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    if (new_node == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para el nodo.\n");
        return;
    }

    new_node->event = new_event;


    new_node->next = *head;
    *head = new_node;
}


int initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS
    );
    if (!window)
    {
        fprintf(stderr, "Error creating SDL Window.\n");
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        fprintf(stderr, "Error creating SDL Renderer.\n");
        return false;
    }
    if (TTF_Init() != 0)
    {
        fprintf(stderr, "Error inicializando SDL_ttf");
        return false;
    }
    return true;
}


void process_input(void)
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        game_is_running = false;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
            game_is_running = false;
        if (event.key.keysym.sym == SDLK_LEFT)
            paddle.vel_x = -400;
        if (event.key.keysym.sym == SDLK_RIGHT)
            paddle.vel_x = +400;
        break;
    case SDL_KEYUP:
        if (event.key.keysym.sym == SDLK_LEFT)
            paddle.vel_x = 0;
        if (event.key.keysym.sym == SDLK_RIGHT)
            paddle.vel_x = 0;
        break;
    }
}
SDL_Texture* render_text(const char* text, TTF_Font* font, SDL_Color color)
{
    SDL_Surface* text_surface = TTF_RenderText_Solid(font, text, color);
    if (text_surface == NULL)
    {
        fprintf(stderr, "Error rendering text: %s\n", TTF_GetError());
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    if (texture == NULL)
    {
        fprintf(stderr, "Error creating texture from surface: %s\n", SDL_GetError());
    }

    SDL_FreeSurface(text_surface);

    return texture;
}

void update_score_text()
{
    char score_text[32];
    snprintf(score_text, sizeof(score_text), "Score: %d", score);

    SDL_Color color = { 255, 255, 255, 255 };
    if (text_texture != NULL)
    {
        SDL_DestroyTexture(text_texture);
    }
    text_texture = render_text(score_text, font, color);
}
void setup(void)
{
    ball.width = 15;
    ball.height = 15;
    ball.x = 20;
    ball.y = 20;
    ball.vel_x = ball_speed;
    ball.vel_y = ball_speed;

    paddle.width = 150;
    paddle.height = 25;
    paddle.x = (WINDOW_WIDTH / 2) - (paddle.width / 2);
    paddle.y = WINDOW_HEIGHT - 40;
    paddle.vel_x = 0;
    paddle.vel_y = 0;

    font = TTF_OpenFont("Eloque-BlackItalic.ttf", 27);
    if (font == NULL)
    {
        fprintf(stderr, "Error loading font: %s\n", TTF_GetError());
    }

    update_score_text();
}

void update(void)
{

    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
        SDL_Delay(time_to_wait);

    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0;

    last_frame_time = SDL_GetTicks();

    ball.x += ball.vel_x * delta_time;
    ball.y += ball.vel_y * delta_time;
    paddle.x += paddle.vel_x * delta_time;
    paddle.y += paddle.vel_y * delta_time;

    if (ball.x <= 0 || ball.x + ball.width >= WINDOW_WIDTH)
        ball.vel_x = -ball.vel_x;
    if (ball.y < 0)
        ball.vel_y = -ball.vel_y;


    if (ball.y + ball.height >= paddle.y &&
        ball.x + ball.width >= paddle.x &&
        ball.x <= paddle.x + paddle.width)
    {
        ball.vel_y = -ball.vel_y;


        score += 15;
        update_score_text();
    }

    if (paddle.x <= 0)
        paddle.x = 0;
    if (paddle.x >= WINDOW_WIDTH - paddle.width)
        paddle.x = WINDOW_WIDTH - paddle.width;

    if (ball.y + ball.height > WINDOW_HEIGHT)
    {
        ball.x = WINDOW_WIDTH / 2;
        ball.y = 0;
        score = 0;
    }
}

void render(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect ball_rect =
    {
        (int)ball.x,
        (int)ball.y,
        (int)ball.width,
        (int)ball.height
    };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &ball_rect);


    SDL_Rect paddle_rect =
    {
        (int)paddle.x,
        (int)paddle.y,
        (int)paddle.width,
        (int)paddle.height
    };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &paddle_rect);

    if (text_texture != NULL)
    {
        SDL_Rect text_rect = { 10, 10, 100, 30 };
        SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
    }

    SDL_RenderPresent(renderer);
}

void destroy_window(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}


int main(int argc, char* args[])
{
    game_is_running = initialize_window();

    setup();

    struct pongEvent ball_movement_event;
    ball_movement_event.timestamp = SDL_GetTicks();
    ball_movement_event.ball_x = ball.x;
    ball_movement_event.ball_y = ball.y;
    ball_movement_event.paddle_x = paddle.x;
    ball_movement_event.paddle_y = paddle.y;
    ball_movement_event.score = score;

    add_event(&event_list_head, ball_movement_event);



    while (game_is_running)
    {
        process_input();
        update();
        render();
    }

    save_event_history(event_list_head, "save.txt");

    destroy_window();

    return 0;
}
