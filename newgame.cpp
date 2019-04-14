#include "lib.hpp"
#include "scoreboard.hpp"
#include <queue>
#include <ctime>
#include <allegro5/allegro_image.h>
using namespace std;
struct player
{
    int x, y, width, life, score;
};
struct enemy
{
    int x, y, width , alive, shoottimer, shootdelay;
};
struct projectile
{
    int x, y, width, height, speed;
};
bool collision(int x1, int y1, int x2, int y2, int w1, int w2, int h1, int h2)
{
    if( (x1 + w1 >= x2   && x1 - w2 <= x2 ) )
    {
        if(y1 >= y2 - h1 && y1 <= y2 + h2 )
        {
            return true;
        }

    }
    return false;
}
void reset(enemy enemytab[])
{
    for(int i = 0; i < 8; i++)
    {
    enemytab[i].alive = 1;
    enemytab[i + 8].alive = 1;
    enemytab[i + 16].alive = 1;

    enemytab[i].x = 75 + 60*i;
    enemytab[i + 8].x = 75 + 60*i;
    enemytab[i + 16].x = 75 + 60*i;
    enemytab[i].y = 60;
    enemytab[i + 8].y = 120;
    enemytab[i + 16].y = 180;

    enemytab[i].shoottimer = rand()%12000;
    enemytab[i + 8].shoottimer = rand()%12000;
    enemytab[i + 16].shoottimer = rand()%12000;

    enemytab[i].shootdelay = rand()%120 + 1;
    enemytab[i + 8].shootdelay = rand()%120 + 1;
    enemytab[i + 16].shootdelay = rand()%120 + 1;
    }
}


enum MYKEYS {
   KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

bool game(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer, scores *data)
{
    srand(time(0));
    int SCREEN_W = 800;
    int SCREEN_H = 600;
    bool key[4] = { false, false, false, false };
    al_init_image_addon();
    queue <projectile> playerqueue;
    projectile pbullet,ebullet;

    pbullet.height = 10;
    pbullet.width = 4;
    pbullet.speed = -6;

    ebullet.height = 10;
    ebullet.width = 4;
    ebullet.speed = 7;

    enemy enemytab[24];
    for(int i = 0; i < 8; i++)
    {
        enemytab[i].width = 30;
        enemytab[i + 8].width = 30;
        enemytab[i + 16].width = 30;
    }
    reset(enemytab);
    ALLEGRO_FONT *font = al_load_font("consola.ttf", 24, NULL);
    ALLEGRO_COLOR black = al_map_rgb(0,0,0);
    ALLEGRO_COLOR red = al_map_rgb(255,0,0);
    ALLEGRO_COLOR green = al_map_rgb(0,255,0);
    ALLEGRO_COLOR blue = al_map_rgb(0,0,255);
    ALLEGRO_COLOR white = al_map_rgb(255,255,255);
    player player;
    player.life = 3;
    player.score = 0;
    player.width = 30;
    player.x = (SCREEN_W * 3/4)/2 + 1/2 * player.width;
    player.y = SCREEN_H - player.width ;

    ALLEGRO_BITMAP *playersquare = al_load_bitmap("player.png");

    ALLEGRO_BITMAP *playerbullet = al_create_bitmap(pbullet.width, pbullet.height);
    al_set_target_bitmap(playerbullet);
    al_clear_to_color(blue);
    al_set_target_bitmap(al_get_backbuffer(display));

    ALLEGRO_BITMAP *enemysquare = al_load_bitmap("enemy.png");
    char temp[11];
    int level = 1;
    int numberofenemies;
    bool used;
    int movement = level;
    int p;
    int counter = 13;
    while(player.life)
    {
        al_clear_to_color(black);
        al_draw_line(int(SCREEN_W * 3/4), 0, int(SCREEN_W * 3/4), SCREEN_H + 1, white, 1);
        al_draw_text(font, white, ((SCREEN_W * 3/4) + (SCREEN_W * 1/4)/2) ,50 ,ALLEGRO_ALIGN_CENTER , "Lifes");
        sprintf(temp,"%d" ,player.life );
        al_draw_text(font, white, ((SCREEN_W * 3/4) + (SCREEN_W * 1/4)/2) ,100 ,ALLEGRO_ALIGN_CENTER , temp);

        al_draw_text(font, white, ((SCREEN_W * 3/4) + (SCREEN_W * 1/4)/2) ,150 ,ALLEGRO_ALIGN_CENTER , "Level");
        sprintf(temp,"%d" ,level );
        al_draw_text(font, white, ((SCREEN_W * 3/4) + (SCREEN_W * 1/4)/2) ,200 ,ALLEGRO_ALIGN_CENTER , temp);

        al_draw_text(font, white, ((SCREEN_W * 3/4) + (SCREEN_W * 1/4)/2) ,250 ,ALLEGRO_ALIGN_CENTER , "Score");
        sprintf(temp,"%d" ,player.score );
        al_draw_text(font, white, ((SCREEN_W * 3/4) + (SCREEN_W * 1/4)/2) ,300 ,ALLEGRO_ALIGN_CENTER , temp);

        al_draw_bitmap(playersquare, player.x, player.y, 0);
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            al_destroy_font(font);
            al_destroy_bitmap(playersquare);
            al_destroy_bitmap(enemysquare);
            return FALSE;
        }
        if(ev.type == ALLEGRO_EVENT_TIMER)
        {
             if(key[KEY_UP] && player.y >= SCREEN_H * 3 / 4 + 4)
             {
                player.y -= 4.0;
             }

             if(key[KEY_DOWN] && player.y <= SCREEN_H - player.width - 4 )
            {
                player.y += 4.0;
            }

             if(key[KEY_LEFT] && player.x >= 0 + 4)
            {
                player.x -= 4.0;
            }

             if(key[KEY_RIGHT] && player.x <= (SCREEN_W * 3 / 4) - player.width - 4 )
            {
                player.x += 4.0;
            }

            p = playerqueue.size();
            for(int i = 0 ;i < p; i++)
            {
                pbullet = playerqueue.front();
                pbullet.y += pbullet.speed;
                al_draw_bitmap(playerbullet, pbullet.x, pbullet.y, 0);
                playerqueue.pop();
                used = 1;
                if(pbullet.y > 0)
                {
                    if(pbullet.speed < 0)
                    {
                      for ( int j=0; j<24; j++ )
                        {
                            if(enemytab[j].alive && collision(pbullet.x, pbullet.y, enemytab[j].x, enemytab[j].y, pbullet.width, enemytab[j].width, pbullet.height, enemytab[j].width))
                            {
                                enemytab[j].alive = 0;
                                player.score += 100;
                                used = 0;
                                break;
                            }
                        }
                    }
                    else
                    {
                        if(collision(pbullet.x, pbullet.y, player.x, player.y, pbullet.width, player.width, pbullet.height, player.width))
                        {
                            player.life--;
                            player.x = (SCREEN_W * 3/4)/2 + 1/2 * player.width;
                            player.y = 600 - player.width;
                            for(int j = 0 ; j < 24 ; j++ )
                            {
                                enemytab[j].shoottimer = rand()%12000;
                            }
                            while(!playerqueue.empty())
                            {
                                playerqueue.pop();
                            }
                            break;
                        }
                        else
                        {
                            if(pbullet.y < SCREEN_H)
                            {
                                playerqueue.push(pbullet);
                            }
                        }
                        used = 0;
                    }
                    if(used)
                    {
                        playerqueue.push(pbullet);
                    }
                }
            }
            for(int i = 0 ; i < 8; i++)
            {
                enemytab[i].x += movement;
                enemytab[i + 8].x -= movement;
                enemytab[i + 16].x += movement;

            }
            if(counter)
            {
                counter--;
            }
            else
            {
                counter = 26;
                movement = -movement;
            }
            numberofenemies = 0 ;
            for(int i = 0 ; i < 24; i++)
            {
                if(enemytab[i].alive)
                {
                    numberofenemies++;
                    enemytab[i].shoottimer += enemytab[i].shootdelay;
                    al_draw_bitmap(enemysquare, enemytab[i].x, enemytab[i].y, 0);
                    if(enemytab[i].shoottimer > 15000 - 1000 * level)
                    {
                        enemytab[i].shoottimer = 0;
                        ebullet.x = enemytab[i].x + (enemytab[i].width / 2 - 1);
                        ebullet.y = enemytab[i].y + enemytab[i].width;
                        playerqueue.push(ebullet);
                    }
                }

            }
            al_flip_display();
            if(!numberofenemies)
            {
                al_draw_text(font, white, SCREEN_W*3 / 8, SCREEN_H/2, ALLEGRO_ALIGN_CENTER, "Next Level");
                if(level < 8)
                {
                    level++;
                }

                al_flip_display();
                al_rest(2);
                al_flush_event_queue(event_queue);
                if(level < 3)
                {
                   movement = level;
                }
                reset(enemytab);
                while(!playerqueue.empty())
                {
                    playerqueue.pop();
                }
                counter = 13;
                player.x = (SCREEN_W * 3/4)/2 + 1/2 * player.width;
                player.y = SCREEN_H - player.width ;
            }

        }
          else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
            {
             switch(ev.keyboard.keycode)
             {
                case ALLEGRO_KEY_UP:
                   key[KEY_UP] = true;
                   break;

                case ALLEGRO_KEY_DOWN:
                   key[KEY_DOWN] = true;
                   break;

                case ALLEGRO_KEY_LEFT:
                   key[KEY_LEFT] = true;
                   break;

                case ALLEGRO_KEY_RIGHT:
                   key[KEY_RIGHT] = true;
                   break;
             }
          }
          else if(ev.type == ALLEGRO_EVENT_KEY_UP)
            {
             switch(ev.keyboard.keycode)
             {
                case ALLEGRO_KEY_UP:
                    key[KEY_UP] = false;
                    break;

                case ALLEGRO_KEY_DOWN:
                    key[KEY_DOWN] = false;
                    break;

                case ALLEGRO_KEY_LEFT:
                    key[KEY_LEFT] = false;
                    break;

                case ALLEGRO_KEY_RIGHT:
                    key[KEY_RIGHT] = false;
                    break;

                case ALLEGRO_KEY_ESCAPE:
                    player.life--;
                    break;
                case ALLEGRO_KEY_SPACE:
                    pbullet.x = player.x + (player.width / 2) - 1;
                    pbullet.y = player.y;
                    pbullet.speed = -6;
                    playerqueue.push(pbullet);
                    break;
             }
            }

    }
        al_clear_to_color(black);
        al_draw_line(int(SCREEN_W * 3/4), 0, int(SCREEN_W * 3/4), SCREEN_H + 1, white, 1);
        al_draw_text(font, white, ((SCREEN_W * 3/4) + (SCREEN_W * 1/4)/2) ,50 ,ALLEGRO_ALIGN_CENTER , "Lifes");
        sprintf(temp,"%d" ,player.life );
        al_draw_text(font, white, ((SCREEN_W * 3/4) + (SCREEN_W * 1/4)/2) ,100 ,ALLEGRO_ALIGN_CENTER , temp);

        al_draw_text(font, white, ((SCREEN_W * 3/4) + (SCREEN_W * 1/4)/2) ,150 ,ALLEGRO_ALIGN_CENTER , "Level");
        sprintf(temp,"%d" ,level );
        al_draw_text(font, white, ((SCREEN_W * 3/4) + (SCREEN_W * 1/4)/2) ,200 ,ALLEGRO_ALIGN_CENTER , temp);

        al_draw_text(font, white, ((SCREEN_W * 3/4) + (SCREEN_W * 1/4)/2) ,250 ,ALLEGRO_ALIGN_CENTER , "Score");
        sprintf(temp,"%d" ,player.score );
        al_draw_text(font, white, ((SCREEN_W * 3/4) + (SCREEN_W * 1/4)/2) ,300 ,ALLEGRO_ALIGN_CENTER , temp);
        al_destroy_bitmap(playersquare);
        al_destroy_bitmap(enemysquare);
        al_draw_text(font, white, SCREEN_W * 3  / 8, SCREEN_H/2, ALLEGRO_ALIGN_CENTER, "Game Over");
        al_flip_display();
        al_rest(2);
        if(player.score > (data[9]).score)
        {
            return newhighscore(display, event_queue, timer, data, player.score);
        }


    return true;
}
