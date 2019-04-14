#include "lib.hpp"
struct scores
{
    char name[10];
    int score;
};

void clearscoreboard(scores data[])
{
    for(int i=0 ; i <11 ; i++ )
    {
        for(int j=0; j<10; j++)
        {
            data[i].name[j] = '_';
        }
        data[i].name[10] = '_';
        data[i].score = 0;
    }
}



void sortscoreboard(scores data[])
{
    scores temp;
    for(int i = 0; i<11 ; i++)
    {
        for(int j = 0; j<11 ;j++)
        {
            if (data[i].score > data[j].score)
            {
                temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
    }
}

void savescoreboard(scores data[])
{
    FILE *fp = fopen("scores.txt", "w");
    char temp[11];
    for (int i = 0; i < 11 ; i++)
    {
        for(int j = 0 ; j<10 ; j++)
            {
                temp[j] = data[i].name[j];
            }
        fprintf(fp,"%s %d\n", temp, data[i].score);
    }
    fclose(fp);

}



scores* loadscoreboard()
{
    FILE *fp = fopen("scores.txt", "r");
    static scores data[11];
    if(!fp)
    {
        clearscoreboard(data);
        savescoreboard(data);
        return data;
    }
    char temp[11];
    for (int i = 0; i < 11 ; i++)
    {
        fscanf(fp,"%s", temp);
        for(int j = 0; j < 10; j++)
        {
            data[i].name[j] = temp[j];
        }
        fscanf(fp,"%d", &data[i].score);
    }
    fclose(fp);
    return data;
}



bool scoreboard (ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer, scores *data)
{
    al_clear_to_color(al_map_rgb(0,0,0));
    ALLEGRO_FONT *font = al_load_font("consola.ttf", 24, NULL);
    sortscoreboard(data);
    int x = 600, y = 100;
    ALLEGRO_BITMAP *kwadrat = al_create_bitmap(20, 20);
    al_set_target_bitmap(kwadrat);
    al_clear_to_color(al_map_rgb(255,255,255));
    al_set_target_bitmap(al_get_backbuffer(display));
    ALLEGRO_EVENT event;
    bool first = 0;
    int counter = 1;
    ALLEGRO_COLOR white = al_map_rgb(255,255,255);
    ALLEGRO_COLOR black = al_map_rgb(0,0,0);
    char temp [10];
    while(1)
    {
        for (int i=0 ; i<10 ; i++)
        {
            for(int j = 0 ; j<10 ; j++)
            {
                temp[j] = data[i].name[j];
            }
            al_draw_text(font, white, 100,y ,0 ,temp);
            sprintf(temp, "%d",data[i].score);                 //sprintf turns integer into char array :)
            al_draw_text(font, white, 100 + 550,y ,0 , temp);
            y+=36;
        }

        y+=36;
        al_draw_text(font, white, 100, y ,0 , "Reset"  );
        al_draw_text(font, white, 100 + 550,y ,0 , "Return");
        al_draw_bitmap(kwadrat, x, y, 0);
        al_flip_display();
        al_wait_for_event(event_queue, &event);
        bool end_this = FALSE;
        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            al_destroy_font(font);
            al_destroy_bitmap(kwadrat);
            return FALSE;
        }
        if(event.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                if(event.keyboard.keycode == ALLEGRO_KEY_LEFT && counter == 1)
                {
                    counter--;
                    x-=550;
                }
                if(event.keyboard.keycode == ALLEGRO_KEY_RIGHT  && counter == 0)
                {
                    counter++;
                    x+=550;
                }
                if(event.keyboard.keycode == ALLEGRO_KEY_ENTER )
                {
                    switch(counter)
                    {
                        case 0:
                            clearscoreboard(data);
                            break;
                        case 1:
                            end_this = TRUE;
                            break;
                    }


                }
            }
        if(end_this)
        {
            savescoreboard(data);
            break;
        }
        y = 100;
        al_clear_to_color(black);

    }
    al_destroy_font(font);
    al_destroy_bitmap(kwadrat);
    return TRUE;
}


bool newhighscore(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer, scores *data, int score)
{
    int SCREEN_W = 800;
    bool shift = 0;
    sortscoreboard(data);
    if (!(score > data[9].score))
    {
        return scoreboard(display, event_queue ,timer ,data);
    }
    ALLEGRO_FONT *font = al_load_font("consola.ttf", 36, NULL);

    ALLEGRO_COLOR white = al_map_rgb(255,255,255);
    ALLEGRO_COLOR black = al_map_rgb(0,0,0);
    ALLEGRO_EVENT event;
    bool first = 0;
    char temp[11] = {' '};
    char temp2[11] = {' '};
    int k = 0;
    for(int i=0; i<10; i++)
    {
        temp2[i] = '_';
    }
    while(1)
    {
        al_clear_to_color(black);
        al_draw_text(font, white, SCREEN_W/2, 100, ALLEGRO_ALIGN_CENTRE, "New Highscore" );
        sprintf(temp, "%d", score);
        al_draw_text(font, white, SCREEN_W/2, 200, ALLEGRO_ALIGN_CENTRE, temp);
        al_draw_text(font, white, SCREEN_W/2, 250, ALLEGRO_ALIGN_CENTRE, "Enter Name:" );
        al_draw_text(font, white, SCREEN_W/2, 300, ALLEGRO_ALIGN_CENTRE, temp2);
        al_wait_for_event(event_queue, &event);
        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                    al_destroy_font(font);
                    return false;
                }
        if(event.type == ALLEGRO_EVENT_KEY_DOWN)
            {

                if(event.keyboard.keycode == ALLEGRO_KEY_ENTER )
                {
                    break;
                }
                if(event.keyboard.keycode > 0 && event.keyboard.keycode < 27 && k < 10 && !shift)
                {
                    temp2[k] = char(event.keyboard.keycode + 'a' - 1);
                    if(k!=9)
                    {
                       k++;
                    }
                }
                if(event.keyboard.keycode > 0 && event.keyboard.keycode < 27 && k < 10 && shift)
                {
                    temp2[k] = char(event.keyboard.keycode + 'A' - 1);
                    if(k!=9)
                    {
                       k++;
                    }
                }
                if(event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
                {
                    temp2[k] = '_';
                    if(k > 0)
                    {
                        k--;
                    }
                }
                if(event.keyboard.keycode == ALLEGRO_KEY_LSHIFT || event.keyboard.keycode == ALLEGRO_KEY_RSHIFT)
                {
                    shift = 1;
                }
            }
        if(event.type == ALLEGRO_EVENT_KEY_UP)
        {
            if(event.keyboard.keycode == ALLEGRO_KEY_LSHIFT || event.keyboard.keycode == ALLEGRO_KEY_RSHIFT)
                {
                    shift = 0;
                }
        }
        al_flip_display();
    }
    for(int i = 0; i<10; i++)
    {
        data[10].name[i] = temp2[i];
    }
    data[10].score = score;

    sortscoreboard(data);

    savescoreboard(data);

    al_destroy_font(font);
    return scoreboard(display, event_queue ,timer ,data);
}
