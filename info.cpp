#include "lib.hpp"
bool info(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer)
{
    int SCREEN_W = 800;
    int SCREEN_H = 600;
    ALLEGRO_COLOR white = al_map_rgb(255,255,255);
    ALLEGRO_COLOR black = al_map_rgb(0,0,0);
    al_clear_to_color(black);
    ALLEGRO_FONT *font = al_load_font("consola.ttf", 40, NULL);
    al_draw_text(font, white, SCREEN_W/2, 100, ALLEGRO_ALIGN_CENTER, "You're a hero");
    al_draw_text(font, white, SCREEN_W/2, 160, ALLEGRO_ALIGN_CENTER, "Kill enemies");
    al_draw_text(font, white, SCREEN_W/2, 220, ALLEGRO_ALIGN_CENTER, "Earn points");
    al_draw_text(font, white, SCREEN_W/2, 340, ALLEGRO_ALIGN_CENTER, "Arrows - movement");
    al_draw_text(font, white, SCREEN_W/2, 400, ALLEGRO_ALIGN_CENTER, "Space - shoot");
    al_flip_display();
    ALLEGRO_EVENT ev;
    while(1)
    {
        al_wait_for_event(event_queue, &ev);
        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            al_destroy_font(font);
            return FALSE;
        }
        if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            break;
        }
    }

    return true;
}
