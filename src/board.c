#include "defs.h"

struct square square[8][8];

/*
We define our chess as below :
   0 1 2 3 4 5 6 7
   _ _ _ _ _ _ _ _
0 |_|_|_|_|_|_|_|_|
1 |_|_|_|_|_|_|_|_|
2 |_|_|_|_|_|_|_|_|
3 |_|_|_|_|_|_|_|_|
4 |_|_|_|_|_|_|_|_|
5 |_|_|X|_|_|_|_|_|
6 |_|_|_|_|_|_|_|_|
7 |_|_|_|_|_|_|_|_|

Example : X = square[2][5]

And alternative square numbers (used by some functions) :
   __ __ __ __ __ __ __ __
0 |01|02|03|04|05|06|07|08|
1 |09|10|11|12|13|14|15|16|
2 |17|18|19|20|21|22|23|24|
3 |25|26|27|28|29|30|31|32|
4 |33|34|35|36|37|38|39|40|
5 |41|42|43|44|45|46|47|48|
6 |49|50|51|52|53|54|55|56|
7 |57|58|59|60|61|62|63|64|
*/

/* Fonction permettant d'afficher la base se trouvant sous l'�chiquier*/
void render_base(SDL_Renderer *renderer)
{
    SDL_Texture *bg = NULL;
    SDL_Rect bg_chess = {((WINDOW_WIDTH - SQUARE_SIZE * CHESS_NB_SQUARE) / 2) - SPACING, ((WINDOW_HEIGHT - SQUARE_SIZE * CHESS_NB_SQUARE) / 2) - SPACING, SPACING + (SQUARE_SIZE + SPACING) * CHESS_NB_SQUARE, SPACING + (SQUARE_SIZE + SPACING) * CHESS_NB_SQUARE};
    SDL_Rect square_rect = {0, 0, SQUARE_SIZE, SQUARE_SIZE};
    bg = loadIMG("sprites/wood.jpg", renderer);
    for(int i = 0; i <= (WINDOW_WIDTH / 512); i++)
    {
        for(int j = 0; j <= (WINDOW_HEIGHT / 512); j++)
        {
            RendTex(bg, renderer, 512 * i, 512 * j);
        }
    }
    SDL_SetRenderDrawColor(renderer, 133, 44, 16, 255);
    SDL_RenderFillRect(renderer, &bg_chess);
    SDL_RenderPresent(renderer);
    for(int i = 0; i <= 7; i++)
    {
        for(int j = 0; j <= 7; j++)
        {
            square_rect.y = (WINDOW_HEIGHT - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * j;
            square_rect.x = (WINDOW_WIDTH - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * i;
            if(((j + i) % 2 == 0))
            {
                SDL_SetRenderDrawColor(renderer, 209, 139, 71, 255);
                SDL_RenderFillRect(renderer, &square_rect);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 255, 206, 158, 255);
                SDL_RenderFillRect(renderer, &square_rect);
            }
        }
    }
    SDL_RenderPresent(renderer);
}
/*Fonction transposant le num�ro d'une case en coordon�es (x)
int numcase_to_coord_x(int numcase)
{
    return (WINDOW_WIDTH - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * numcase;
}
/* Fonction transposant le num�ro d'une case en coordon�es (y)
int numcase_to_coord_y(int numcase)
{
    return (WINDOW_HEIGHT - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * numcase;
}
//********************************************************************************************************************
int posx(int numcase)
{
    return (WINDOW_WIDTH - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * numcase + (SQUARE_SIZE - PAWN_SIZE) / 2;
}
//********************************************************************************************************************
int posy(int numcase)
{
    return (WINDOW_HEIGHT - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * numcase + (SQUARE_SIZE - PAWN_SIZE) / 2;
}
/* Fonction initialisant la position de d�part des pions ainsi que leurs couleurs
void init_pawn(pion pion[])
{
    int k = 0;
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j <= 1; j++)
        {
            pion[k].isBlack = 1;
            pion[k].x = i;
            pion[k].y = j;
            pion[k].isInInitPos = 1;
            pion[k].isSelected = 0;
            k++;
        }
    }
    for(int i = 0; i <= 7; i++)
    {
        for(int j = 6; j <= 7; j++)
        {
            pion[k].isBlack = 0;
            pion[k].x = i;
            pion[k].y = j;
            pion[k].isInInitPos = 1;
            pion[k].isSelected = 0;
            k++;
        }
    }
}
//********************************************************************************************************************
void render_pawn(SDL_Renderer *renderer, pion pion[32])
{
    SDL_Texture *pion_noir = NULL, *pion_blanc = NULL;
    pion_noir = loadIMG("sprites/blackpawn.png", renderer);
    pion_blanc = loadIMG("sprites/whitepawn.png", renderer);
    for(int i = 0; i <= 32; i++)
    {
        if(pion[i].isBlack == 0)
        {
            RendTex(pion_blanc, renderer, posx(pion[i].x), posy(pion[i].y));
        }
        if(pion[i].isBlack == 1)
        {
            RendTex(pion_noir, renderer, posx(pion[i].x), posy(pion[i].y));
        }
    }
    SDL_RenderPresent(renderer);
}
//********************************************************************************************************************
int highlight_square(SDL_Rect clickedSquare, SDL_Renderer *renderer, pion pion[32])
{
    int rendersquare = 1;
    int isMovesShown=0;
    SDL_Texture *dot = NULL;
    if((clickedSquare.x!=0)&&(clickedSquare.y!=0))
    {
        for(int k = 1; k <= 32; k++)
        {
            if((clickedSquare.x == numcase_to_coord_x(pion[k].x)) && (clickedSquare.y == numcase_to_coord_y(pion[k].y)))
            {
                printf("Un pion a ete detecte sur la case, selection de la case.\n");
                SDL_SetRenderDrawColor(renderer, 231, 252, 212, 255);
                SDL_RenderFillRect(renderer, &clickedSquare);
                if(pion[k].isInInitPos == 1)   //Si le pion est en positon initiale = 2 cases
                {
                    for(int b = 1; b <= 32; b++)   //on v�rifie qu'il n'y ait pas de pion devant le pion
                    {
                        if(((numcase_to_coord_x(pion[k].y + 1) == numcase_to_coord_x(pion[b].y))&&(pion[k].isBlack == 1))
                                ||((numcase_to_coord_x(pion[k].y - 1) == numcase_to_coord_x(pion[b].y)) && (pion[k].isBlack == 0)))
                        {
                            printf("Pion detecte devant, aucun mouvement � afficher !\n");
                            rendersquare = 0;
                        }
                    }
                    if(rendersquare == 1)
                    {
                        for(int c = 1; c <= 2; c++)
                        {
                            dot = loadIMG("sprites/dot.png", renderer);
                            RendTex(dot, renderer, posx(pion[k].x), posy(pion[k].y-c+3*pion[k].isBlack));//=+2 si blanc(isBlack=0)/=-2 si noir (isBlack=1)
                            SDL_RenderPresent(renderer);
                        }
                    }

                }
            }
        }

    }
    SDL_RenderPresent(renderer);
    return rendersquare;
}*/
/* Fonction permettant de renvoyer la case sur laquelle on a cliqu�*/
SDL_Rect get_clicked_square(int x, int y)
{
    SDL_Rect square = {0, 0, SQUARE_SIZE, SQUARE_SIZE};
    for(int i=0; i<=7; i++)
    {
        for(int j=0; j<=7; j++)
        {
            if((x > (WINDOW_WIDTH - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING)*i)
                    && (x < (WINDOW_WIDTH - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING)*i + SQUARE_SIZE)
                    && (y > (WINDOW_HEIGHT - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING)*j)
                    && (y < (WINDOW_HEIGHT - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING)*j + SQUARE_SIZE))
            {
                square.x=(WINDOW_WIDTH - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * i;
                square.y=(WINDOW_HEIGHT - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * j;
                printf("Position de la case cliquee acquise : x=%d y=%d !\n",square.x,square.y);
            }
        }
    }
    return square;
}
//********************************************************************************************************************
void game(SDL_Window *window, SDL_Renderer *renderer)
{   /*int isMovesShown;
    pion pion[32];*/
    int stop=0;
    SDL_Event event;
    SDL_Rect clickedSquare;
    SDL_RenderClear(renderer);
    render_base(renderer);
    initialize_pawns_pos();
    render_pawn(renderer);
    while(stop!=1)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            stop=1;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                stop=1;
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(event.button.button == SDL_BUTTON_LEFT)   // Bouton souris gauche
            {
                clickedSquare=get_clicked_square(event.button.x,event.button.y);
                SDL_RenderClear(renderer);
                render_base(renderer);
                //isMovesShown=highlight_square(clickedSquare, renderer, pion);
                //render_pawn(renderer, pion);
                /*if(isMovesShown==1)
                {
                    movement(clickedSquare, pion);
                    render_pawn(renderer, pion);
                }*/
            }
            break;
        default:
            break;
        }
    }
}

void initialize_pawns_pos(void)
{
    for(int i=0; i<=7; i++)
    {
        for(int j=0; j<=7; j++)
        {
            if(j<2)
            {
               square[i][j].pawn=1;
            }
            else if(j>5)
            {
               square[i][j].pawn=2;
            }
            else
            {
               square[i][j].pawn=0;
            }
        }
    }
}

void render_pawn(SDL_Renderer *renderer)
{
    SDL_Texture *pion_noir = NULL, *pion_blanc = NULL;
    pion_noir = loadIMG("sprites/blackpawn.png", renderer);
    pion_blanc = loadIMG("sprites/whitepawn.png", renderer);
    for(int i = 0; i<=7; i++)
    {
        for(int j=0; j<=7; j++)
        {
            if(square[i][j].pawn==2)
            {
                RendTex(pion_blanc, renderer, posx(i), posy(j));
            }
            else if(square[i][j].pawn==1)
            {
                RendTex(pion_noir, renderer, posx(i), posy(j));
            }
        }
    }
    SDL_RenderPresent(renderer);
}

//Fonction transposant le num�ro d'une case en coordon�es (x)
int numcase_to_coord_x(int numcase)
{
    return (WINDOW_WIDTH - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * numcase;
}
// Fonction transposant le num�ro d'une case en coordon�es (y)
int numcase_to_coord_y(int numcase)
{
    return (WINDOW_HEIGHT - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * numcase;
}
//Fonction transposant le num�ro d'une case en coordon�es pour le pion (x)
int posx(int numcase)
{
    return (WINDOW_WIDTH - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * numcase + (SQUARE_SIZE - PAWN_SIZE) / 2;
}
//********************************************************************************************************************
int posy(int numcase)
{
    return (WINDOW_HEIGHT - SQUARE_SIZE * CHESS_NB_SQUARE) / 2 + (SQUARE_SIZE + SPACING) * numcase + (SQUARE_SIZE - PAWN_SIZE) / 2;
}
