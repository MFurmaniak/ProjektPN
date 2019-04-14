struct scores
{
    char name[10];
    int score;
};
bool scoreboard (ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer, scores *data);
bool newhighscore(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer, scores *data, int score);
scores* loadscoreboard();
