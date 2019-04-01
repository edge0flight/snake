#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include <stdlib.h>

#define HEIGHT 800
#define WIDTH 800
#define SIZE 40
#define MAX 1000

/*SNAKE game developed by Franck Lazar
 *
 *        -snake can be elongated
 *        -game over if head hurts wall or snake
 *
/* * * * */

enum {LEFT, RIGHT, UP, DOWN};
enum {FAILURE, SUCCESS};

int move(int, SDL_Rect*, Uint32, Uint32*, SDL_Rect *snake, int count);
void add_random_point(SDL_Rect*);
void draw_snake(SDL_Rect snake[], int count, SDL_Renderer *r);
void print_buttons(SDL_Renderer*);
int test_move(SDL_Rect [ ], int count);
void test_addon(SDL_Rect snake[ ], int *count, SDL_Rect *add_on, SDL_Rect pos);
void main_menu(SDL_Renderer *, SDL_Texture *, SDL_Texture*, SDL_Rect, SDL_Rect);
void display(SDL_Renderer* r, SDL_Rect snake[ ], int count, SDL_Rect add_on);

SDL_Texture *LoadFont(SDL_Renderer * r, TTF_Font *font, char *text, SDL_Color color, SDL_Rect *);

int main(int argc, char *argv[ ])
{
	int i=0;
	int k=1;
	int game_active = 0;
	int count = 1;
	int direction;
	Uint32 timer =0, timer0 = 0;
	srand(time(NULL));
	SDL_Color white={255, 255, 255, 255};
	
	SDL_Rect snake[MAX];
	
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init( );
	
	TTF_Font *calibri = TTF_OpenFont("calibri.ttf", 80);
	
	SDL_Window *window=SDL_CreateWindow("snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	
	SDL_Renderer *r=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
	SDL_Rect pos_play={0.4*WIDTH, 0.35*HEIGHT, 0, 0}, pos_quit= {0.4*WIDTH, 0.55*HEIGHT, 0, 0};
	
	SDL_Texture *play = LoadFont(r, calibri, "1.Play", white, &pos_play);
	SDL_Texture *quit = LoadFont(r, calibri,"2.Quit", white, &pos_quit);
	
	SDL_Rect pos={0, 0, SIZE, SIZE};
	direction = RIGHT;
	snake[count]=pos;
	
	SDL_Event e;
	
	timer0=SDL_GetTicks( );
	
	SDL_Rect add_on;
	add_random_point(&add_on);
	
	while(k)
	{
		while(SDL_PollEvent(&e))
		{
			if (e.type==SDL_QUIT) k=0;
			else if(e.type == SDL_MOUSEBUTTONDOWN && game_active)
			{
				if(e.button.x > 0.2*WIDTH && e.button.x < 0.4*WIDTH && e.button.y < 0.8*HEIGHT && e.button.y > 0.7*HEIGHT)
				{
					direction=LEFT;
				}
				else if(e.button.x > 0.6*WIDTH && e.button.x < 0.8*WIDTH && e.button.y < 0.8*HEIGHT && e.button.y > 0.7*HEIGHT)
				{
					direction=RIGHT;
				}
				if(e.button.y < 0.7*HEIGHT && e.button.y > 0.6*HEIGHT && e.button.x < 0.6*WIDTH && e.button.x > 0.4*WIDTH)
				{
					direction=UP;
				}
				if(e.button.y > 0.8*HEIGHT && e.button.y < 0.9*HEIGHT && e.button.x < 0.6*WIDTH && e.button.x > 0.4*WIDTH)
				{
					direction = DOWN;
				}
			}
			
			else if (e.type == SDL_MOUSEBUTTONDOWN && !game_active)
			{
				if(e.button.x >0.4*WIDTH && e.button.x <0.6*WIDTH && e.button.y <0.45*HEIGHT && e.button.y >0.35*HEIGHT) 
				        game_active = 1;
				else if(e.button.x >0.4*WIDTH && e.button.x <0.6*WIDTH && e.button.y <0.65*HEIGHT && e.button.y >0.55*HEIGHT) 
				        k = 0;
			}
		}
		
		if (!game_active) main_menu(r, play, quit, pos_play, pos_quit);
		else
		{
			timer=SDL_GetTicks( ) - timer0;
			if (move(direction, &pos, timer, &timer0, snake, count) == FAILURE) 
			{
				game_active=0;
				count=1;
				pos.x=0;
				pos.y=0;
				direction=RIGHT;
			}
			else
			{
				test_addon(snake, &count, &add_on, pos);
				display(r, snake, count, add_on);
			}
		}
	}
	
	SDL_DestroyRenderer(r);
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(play);
	SDL_DestroyTexture(quit);
	TTF_CloseFont(calibri);
	
	SDL_Quit();
	TTF_Quit();

	return 0;
}

void main_menu(SDL_Renderer *r, SDL_Texture *text1, SDL_Texture *text2, SDL_Rect pos1, SDL_Rect pos2)
{
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderClear(r);
	SDL_RenderCopy(r, text1, NULL, &pos1);
	SDL_RenderCopy(r, text2, NULL, &pos2);
	SDL_RenderPresent(r);
}

SDL_Texture *LoadFont(SDL_Renderer * r, TTF_Font *font, char *text, SDL_Color color, SDL_Rect *pos)
{
	SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(r, surface);
	SDL_FreeSurface(surface);
	TTF_SizeText(font, text, &pos->w, &pos->h);
	return texture;
}

int move (int direction, SDL_Rect *pos, Uint32 timer, Uint32 *timer0, SDL_Rect snake[ ], int count)
{
	if(timer>100)
	{
		
		if(direction==RIGHT)
		{
			pos->x+=SIZE;
			*timer0 = SDL_GetTicks( );
		}
		
		else if(direction==LEFT)
		{
			pos->x-=SIZE;
			*timer0 = SDL_GetTicks( );
		}
		
		else if(direction==UP)
		{
			pos->y-=SIZE;
			*timer0 = SDL_GetTicks( );
		}
		
		else if(direction==DOWN)
		{
			pos->y+=SIZE;
			*timer0 = SDL_GetTicks( );
		}
		for(int i=count; i>0; i--)
		{
			snake[i]=snake[i-1];
		}
		
		snake[0]=*pos;
		if(test_move(snake, count) == FAILURE)
		    return FAILURE;
	
	}
	return SUCCESS;
}

void display(SDL_Renderer* r, SDL_Rect snake[ ], int count, SDL_Rect add_on)
{
		SDL_SetRenderDrawColor(r, 0, 70, 20, 255);
		SDL_RenderClear(r);
		
		SDL_SetRenderDrawColor(r, 0, 200, 20, 255);
		draw_snake(snake, count, r);
		
		SDL_SetRenderDrawColor(r, 210, 255, 210, 255);
		SDL_RenderFillRect(r, &add_on);
		
		print_buttons(r);
		
		SDL_RenderPresent(r);
}

void print_buttons(SDL_Renderer *r)
{
	SDL_Rect pos[4]=
	{{0.2*WIDTH, 0.7*HEIGHT, 0.2*WIDTH, 0.1*HEIGHT},
	{0.6*WIDTH, 0.7*HEIGHT, 0.2*WIDTH, 0.1*HEIGHT},
	{0.4*WIDTH, 0.6*HEIGHT, 0.2*WIDTH, 0.1*HEIGHT},
	{0.4*WIDTH, 0.8*HEIGHT, 0.2*WIDTH, 0.1*HEIGHT}};
	
	SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(r, 220, 200, 200, 140);
	for(int i=0; i<4; i++)
	{
		SDL_RenderFillRect(r, &pos[i]);
	}
}

void add_random_point(SDL_Rect *position)
{
	position->x = rand( )%(WIDTH-SIZE);
	position->y = rand( )%(HEIGHT-SIZE);
	position->w=SIZE;
	position->h=SIZE;
}
	
void draw_snake(SDL_Rect snake[ ], int count, SDL_Renderer *r)
{
	for(int i=0; i<count; i++)
	{
		SDL_RenderFillRect(r, &snake[i]);
	}
}

int test_move(SDL_Rect snake[ ], int count)
{
	if(snake[0].x >WIDTH - SIZE || snake[0].x < 0 || snake[0].y > HEIGHT - SIZE || snake[0].y < 0)
	    return FAILURE;
	for(int i=1; i<count; i++)
	{
		if(snake[0].x > snake[i].x-snake[i].w && snake[0].x < snake[i].x + snake[i].w && snake[0].y > snake[i].y - snake[i].h && snake[0].y < snake[i].y + snake[i].h)
		          return FAILURE;
	}
	return SUCCESS;
}

void test_addon(SDL_Rect snake[ ], int *count, SDL_Rect *add_on, SDL_Rect  pos)
{
	if(snake[0].x > add_on->x-add_on->w && snake[0].x < add_on->x + add_on->w && snake[0].y > add_on->y - add_on->h && snake[0].y < add_on->y + add_on->h)
	{
		add_random_point(add_on);
		(*count)++;
		for(int i=*count;i>0;i--)
		{
		    snake[i] = snake[i-1];
		}
		snake[0] = pos;
	}
}
	