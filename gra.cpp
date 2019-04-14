#include "lib.hpp"
#include "scoreboard.hpp"
#include "newgame.hpp"
#include "info.hpp"

const float FPS = 60;
const int SCREEN_W = 800;
const int SCREEN_H = 600;
const int rozmiar = 40;

int main(int argc, char **argv)
{
    ALLEGRO_DISPLAY *display = NULL;
    scores *data = loadscoreboard();

    if(!al_init())
    {
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
    }

    display = al_create_display(SCREEN_W, SCREEN_H);
    al_set_window_position(display, 100, 100);
    al_set_window_title(display, "Super gra na 5");

    if(!display)
    {
      fprintf(stderr, "failed to create display!\n");
      return -1;
    }

    al_install_keyboard();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();

    //ALLEGRO_KEYBOARD_STATE keyboard;
    ALLEGRO_EVENT_QUEUE *event_queue;
    ALLEGRO_EVENT event;
    event_queue = al_create_event_queue();

    ALLEGRO_TIMER *timer = NULL;
    timer = al_create_timer(1.0 / FPS);
    al_start_timer(timer);

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    ALLEGRO_BITMAP *kwadrat = al_create_bitmap(rozmiar, rozmiar);
    al_set_target_bitmap(kwadrat);
    al_clear_to_color(al_map_rgb(255,255,255));
    al_set_target_bitmap(al_get_backbuffer(display));

    ALLEGRO_FONT *font = al_load_font("consola.ttf", 40, NULL);
    al_flip_display();


    int x = 140, y= 100;
    int poz = 0;
    bool end_this_suffering = TRUE;
    while(event.type != ALLEGRO_EVENT_DISPLAY_CLOSE && end_this_suffering)
    {
       // al_get_keyboard_state(&keyboard);
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_bitmap(kwadrat, x, y, 0);
            end_this_suffering = TRUE;
            al_draw_text(font, al_map_rgb(255,255,255), SCREEN_W/2 , 100, ALLEGRO_ALIGN_CENTRE, "New Game");
            al_draw_text(font, al_map_rgb(255,255,255), SCREEN_W/2 , 225, ALLEGRO_ALIGN_CENTRE, "Info");
            al_draw_text(font, al_map_rgb(255,255,255), SCREEN_W/2 , 350, ALLEGRO_ALIGN_CENTRE, "Scoreboard");
            al_draw_text(font, al_map_rgb(255,255,255), SCREEN_W/2 , 475, ALLEGRO_ALIGN_CENTRE, "Exit");
            al_flip_display();
            al_wait_for_event(event_queue, &event);
            if(event.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                if(event.keyboard.keycode == ALLEGRO_KEY_DOWN && poz < 3)
                {
                    y+=125;
                    poz++;
                }
                if(event.keyboard.keycode == ALLEGRO_KEY_UP && poz > 0)
                {
                    y-=125;
                    poz--;
                }
                if(event.keyboard.keycode == ALLEGRO_KEY_ENTER)
                {

                    switch (poz)
                    {
                    case 0:

                        end_this_suffering = game(display, event_queue, timer, data);
                        break;
                    case 1:
                        end_this_suffering = info(display, event_queue, timer);
                        break;
                    case 2:
                        end_this_suffering = scoreboard(display, event_queue, timer, data);
                        break;
                    case 3:
                        end_this_suffering = FALSE;
                        break;

                }

            }
        }

    }
    al_destroy_bitmap(kwadrat);
    al_destroy_display(display);
    al_destroy_font(font);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);


    return 0;
}
