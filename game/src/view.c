/*!
* \file view.c
* \brief File containing the functions relative to the SDL, that display elements on the screen
*/
#include "view.h"
#include "camera.h"

/*!
* \brief Color a rectangle
* \param[out] renderer the drawing target
* \param[in] camera the view of the scene
* \param[in] x the abscissa of the rectangle
* \param[in] y the ordinate of the rectangle
* \param[in] w the width of the rectangle
* \param[in] h the height of the rectangle
* \param[in] r the red component (integer between 0 and 255)
* \param[in] g the green component (integer between 0 and 255)
* \param[in] b the blue component (integer between 0 and 255)
*/
void drawRectangle(SDL_Renderer * renderer, Camera * camera, int x, int y, int w, int h, int r, int g, int b)
{
    SDL_Rect rect = {x - camera->x, y, w, h};
    SDL_SetRenderDrawColor( renderer, r, g, b, 255 );
    SDL_RenderFillRect(renderer, &rect);
}

/*!
* \brief Color the bird
* \param[out] renderer the drawing target
* \param[in] bird the bird to color
* \param[in] camera the view of the scene
*/
void drawBird(SDL_Renderer * renderer, Bird * bird, Camera * camera)
{
    drawRectangle(renderer, camera, bird->x - BIRD_SIZE/2, bird->y - BIRD_SIZE/2, bird->w, bird->h, 255, 0, 0);
}

/*!
* \brief Color the two pipes of an obstacle
* \param[out] renderer the drawing target
* \param[in] obstacle the two pipes to color
* \param[in] camera the view of the scene
*/
void drawObstacle(SDL_Renderer * renderer, Obstacle * obstacle, Camera * camera)
{
    Pipe obslow = obstacle->lower;
    Pipe obsup = obstacle->upper;
    drawRectangle(renderer, camera, obslow.x, obslow.y, obslow.w, obslow.h, 0, 0, 255);
    drawRectangle(renderer, camera, obsup.x, obsup.y, obsup.w, obsup.h, 0, 255, 0);
}


/*!
* \brief Draw two squares with the color of upper and lower pipes and the size of the bird at the center of the screen
* \param[out] renderer the drawing target
* \param[in] camera the view of the scene
*/
void drawForTI(SDL_Renderer * renderer, Camera * camera)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    int x1 = SCREEN_WIDTH/2 - 3*BIRD_SIZE/2;
    int x2 = SCREEN_WIDTH/2 + BIRD_SIZE/2;
    int y = SCREEN_HEIGHT/2 - BIRD_SIZE/2;
    drawRectangle(renderer, camera, x1, y, BIRD_SIZE,BIRD_SIZE, 0, 255, 0);
    drawRectangle(renderer, camera, x2, y, BIRD_SIZE,BIRD_SIZE, 0, 0, 255);
    SDL_RenderPresent(renderer);
}
/*!
* \brief Display the items of the game
* \param[out] renderer the drawing target
* \param[in] bird the bird to display
* \param[in] l the list of obstacle
* \param[in] camera the view of the scene
* \param[in] score the current score
* \param[in] font the font used to write text
*/
void displayGame(SDL_Renderer * renderer, Bird * bird, List * l, Camera * camera, int score, TTF_Font * font)
{
    int i = 0;
    setOnFirst(l);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    drawBird(renderer, bird, camera);
    while (i < OBSTACLE_NUMBER)
    {
        if (l->current->lower.x != 0){
            drawObstacle(renderer, l->current, camera);
            next(l);
            ++i;
        }
    }
    displayScore(renderer, score, font);
    drawRectangle(renderer, camera, SCREEN_WIDTH-50+camera->x, 0, 50, 50, 100, 100, 100);
    SDL_RenderPresent(renderer);
}

/*!
* \brief Quit the SDL and destroy renderer and window
* \param[out] window the window to destroy
* \param[out] renderer the renderer to destroy
*/
void quitGame(SDL_Window * window, SDL_Renderer * renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
	Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
}

/*!
* \brief Display the current score on screen
* \param[out] renderer the drawing target
* \param[in] score the current score to be displayed
* \param[in] font the font used to display score
*/
int displayScore(SDL_Renderer * renderer, int score, TTF_Font * font)
{
	char scoreString[10];
	sprintf(scoreString, "%d", score);
    SDL_Color color = {0, 0, 0};
	SDL_Surface * scoreSurface = TTF_RenderText_Blended(font, scoreString, color);
	SDL_Texture * scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
	SDL_Rect dest = {30, 30, scoreSurface->w, scoreSurface->h};
	SDL_RenderCopy(renderer, scoreTexture, NULL, &dest);
	SDL_FreeSurface(scoreSurface);
	return 1;
}

/*!
* \brief Display the best score at the death of the bird
* \param[out] renderer the drawing target
* \param[in] font the font used to write score
* \param[in] score_file the file where the best score is saved
*/
int displayBestScore(SDL_Renderer * renderer, TTF_Font * font, FILE * score_file)
{
    int best_score;
    best_score = readBestScore(score_file);
    if (best_score < 0)
    {
        fprintf(stderr, "Impossible to display best score");
        return 0;
    }
    char score_string[10];
	sprintf(score_string, "%d", best_score);
	SDL_Color color = {0, 0, 0};
    SDL_Surface * score_surface = TTF_RenderText_Blended(font, score_string, color);
	SDL_Texture * score_texture = SDL_CreateTextureFromSurface(renderer, score_surface);
    SDL_Surface * text_surface = TTF_RenderText_Blended(font, "Best score :", color);
	SDL_Texture * text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_Rect score_dest = {SCREEN_WIDTH/2 - score_surface->w/2, SCREEN_HEIGHT/2 - score_surface->h/2, score_surface->w, score_surface->h};
	SDL_Rect text_dest = {SCREEN_WIDTH/2 - text_surface->w/2, SCREEN_HEIGHT/2 - score_surface->h/2 - text_surface->h, text_surface->w, text_surface->h};
	SDL_RenderCopy(renderer, score_texture, NULL, &score_dest);
	SDL_RenderCopy(renderer, text_texture, NULL, &text_dest);
	SDL_FreeSurface(score_surface);
	SDL_FreeSurface(text_surface);
	return 1;
}
