#include <cstdio>
#include <thread>
#include <conio.h>
using namespace std::literals::chrono_literals;

const int space = 32;
const int solid = 219;
const int usolid = 223;
const int bsolid = 220;
const int lsolid = 221;
const int rsolid = 222;
const int x = 25;
const int y = 45;

class screan
{
private:
    char* grid;
    screan();
public:
    ~screan();
    static screan& get();
    static bool check_bounds(int i ,int j);
    void draw(int i , int j ,int value);
    char check(int i , int j);
    void reset_screan();
    void display();
};
/***************************************************************/


screan::screan()
{
    grid = new char[x * y];
    for(int i = 0; i < x * y; ++i)
        grid[i] = space;
}

screan::~screan()
{
    delete[] grid;
}

screan& screan::get()
{
    static screan instance;
    return instance;
}

bool screan::check_bounds(int i ,int j)
{
    return (i >= 0 && i < x) && (j >= 0 && j < y);
}

 void screan::draw(int i , int j ,int value)
 {
     if(screan::check_bounds(i ,j))
        grid[i * y + j] = value;
 }

 char screan::check(int i , int j)
 {
     if(screan::check_bounds(i ,j))
        return grid[i * y + j];
    return 0;
 }

 void screan::reset_screan()
 {
    for(int i = 0; i < x * y; ++i)
        grid[i] = space;
 }

 void screan::display()
 {
     for(int i = 0; i <= y + 1; ++i)
        printf("%c" ,bsolid);
     printf("\n");
     for(int i = 0; i < x; ++i)
     {
        printf("%c" ,lsolid);
        for(int j = 0; j < y; ++j)
        {
            printf("%c" ,this -> check(i , j));
        }
        printf("%c\n" ,rsolid);
    }
    for(int i = 0; i <= y + 1; ++i)
        printf("%c" ,usolid);
}

/***************************************************************/

const int default_length = 9;

class badel
{
private:
    int posx;
    int posy;
    int length;
    badel(int badel_count);
public:
    static badel& get_first_badel();
    static badel& get_second_badel();
    void move_up();
    void move_down();
    int get_length();
    int get_pos();
};
/***************************************************************/

void draw_line(int i , int j ,int l ,int val)
{
    screan& s = screan::get();
    l += i;
    for(int k = i; k < l; ++k)
    {
        s.draw(k ,j ,val);
    }
}

badel::badel(int badel_count) : length(default_length)
{
    int l = length / 2;
    if(badel_count == 0)
    {
        posx = 0 + l;
        posy = 0;
        draw_line(0 , posy ,length ,solid);
    }
    if(badel_count == 1)
    {
        posx = x - l - 1;
        posy = y - 1;
        draw_line(posx - length / 2 , posy ,length ,solid);
    }
}

badel& badel::get_first_badel()
{
    static badel first_badel(0);
    return first_badel;
}

badel& badel::get_second_badel()
{
    static badel second_badel(1);
    return second_badel;
}

void badel::move_up()
{
    screan& s = screan::get();
    int l = length / 2;
    if(screan::check_bounds(posx - l - 1 , posy))
    {
        s.draw(posx + l ,posy ,space);
        --posx;
        s.draw(posx - l ,posy ,solid);
    }
}
void badel::move_down()
{
    screan& s = screan::get();
    int l = length / 2;
    if(screan::check_bounds(posx + l + 1 , posy))
    {
        s.draw(posx - l ,posy ,space);
        ++posx;
        s.draw(posx + l ,posy ,solid);
    }
}

int badel::get_length()
{
    return length;
}

int badel::get_pos()
{
    return posx;
}

/***************************************************************/

class ball
{
private:
    char back_ground_square;
    int posx;
    int posy;
    int velx;
    int vely;
    ball();
    void badel_reflect(int badel_num);
public:
    static ball& get();
    void move_ball();
    void boundary_colision();
    void badel_colision();
    int score();
};

/***************************************************************/

ball::ball() : posx(x / 2) ,posy(y / 2) ,velx(1) ,vely(1)
{
    screan& s = screan::get();
    back_ground_square = s.check(posx , posy);
    s.draw(posx ,posy ,'O');
}

void ball::badel_reflect(int badel_num)
{
    vely = -vely;
    badel& p1 = badel::get_first_badel();
    badel& p2 = badel::get_second_badel();
    int x;

    if(badel_num == 0)
        x = p1.get_pos();

    if(badel_num == 1)
        x = p2.get_pos();

    x -= posx;

    if(x > 1)
        velx = (velx > 0 ? -velx : velx);

    if(x < -1)
        velx = (velx > 0 ? velx : -velx);
}

ball& ball::get()
{
    static ball b;
    return b;
}

void ball::move_ball()
{
    screan& s = screan::get();
    s.draw(posx ,posy , back_ground_square);
    posx += velx;
    posy += vely;
    back_ground_square = s.check(posx , posy);
    s.draw(posx ,posy , 'O');
}

void ball::boundary_colision()
{
    if(posx == 0 || posx == x - 1) velx = -velx;
}

void ball::badel_colision()
{
    badel& p1 = badel::get_first_badel();
    badel& p2 = badel::get_second_badel();
    ball& b = ball::get();
    int l = default_length / 2;
    if(posy == 1 &&((posx >= p1.get_pos() - l && posx <= p1.get_pos() + l) || (posx + velx == p1.get_pos() - l || posx + velx == p1.get_pos() - l)))
        b.badel_reflect(0);
    if(posy == y - 2 &&((posx >= p2.get_pos() - l && posx <= p2.get_pos() + l) || (posx + velx == p2.get_pos() - l || posx + velx == p2.get_pos() - l)))
        b.badel_reflect(1);
}

int ball::score()
{
    int score_code = 0;
    screan& s = screan::get();
    s.draw(posx , posy , back_ground_square);
    if(posy == 0 || posy == y - 1)
    {

    if(posy == 0)
        score_code = 2;

    if(posy == y - 1)
        score_code = 1;

        velx = -velx;
        vely = -vely;
        posx = x / 2;
        posy = y / 2;
    }
    back_ground_square = s.check(posx , posy);
    return score_code;
}

/***************************************************************/

class game_state
{
 private:
    bool game_running;
    int first_player_score;
    int second_player_score;
    game_state();
 public:
    static game_state& get();
    void update_score(int score_code);
    void quit_game();
    bool is_running();
    void game_end();
    void display_game_score();
};

/***************************************************************/

game_state::game_state() : game_running(true) ,first_player_score(0) ,second_player_score(0)
{}

game_state& game_state::get()
{
    static game_state game;
    return game;
}

void game_state::update_score(int score_code)
{
    switch(score_code)
    {
    case 1:
        ++first_player_score;
        break;
    case 2:
        ++second_player_score;
        break;
    }
}

void game_state::quit_game()
{
    game_running = false;
}

bool game_state::is_running()
{
    return game_running;
}

void game_state::game_end()
{
    if(first_player_score == 10 || second_player_score == 10)
    {
        game_running = false;
        system("cls");
    }
    if(first_player_score == 10)
        printf("player 1 won\n");
    if(second_player_score == 10)
        printf("player 2 won\n");
}

void game_state::display_game_score()
{
    system("cls");
    int n = y / 2 - 2;
    for(int i = 0; i < n; ++i)
        printf(" ");
    printf("%d : %d\n" , first_player_score , second_player_score);
}

/***************************************************************/

void input_controler()
{
    game_state& game = game_state::get();
    badel& p1 = badel::get_first_badel();
    badel& p2 = badel::get_second_badel();
    while(game.is_running())
    {
        if (_kbhit())
            {
                switch (_getch())
                {
                    case 'w':
                        p1.move_up();
                        break;
                    case 's':
                        p1.move_down();
                        break;
                    case 'o':
                        p2.move_up();
                        break;
                    case 'l':
                        p2.move_down();
                        break;
                    case 'q':
                        game.quit_game();
                        break;
                }
            }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

/***************************************************************/

 int main()
 {
     int score_code;
     screan& s = screan::get();
     badel& p1 = badel::get_first_badel();
     badel& p2 = badel::get_second_badel();
     ball& b = ball::get();
     game_state& game = game_state::get();

     std::thread inputThread(input_controler);

     while(game.is_running())
     {
         game.display_game_score();
         s.display();
         score_code = b.score();
         game.update_score(score_code);
         b.badel_colision();
         b.boundary_colision();
         b.move_ball();
         std::this_thread::sleep_for(std::chrono::milliseconds(30));
     }
     inputThread.join();
     return 0;
 }
