#include <stdio.h>
#include <time.h>
#include <sys/timeb.h>
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
struct playerinfo {
int start, current;
float score;
} player;
struct node{
char val[20];
struct node * next;
};
int num_of_level (void)
{
    char filename[20], i[2] = {'1', '\0'};
    const char add[] = "levels/level-";
    FILE * f;
    SetColor(79);
    while (1)
    {
        strcpy(filename, add );
        strcat( filename, i);
        strcat( filename, ".txt\0");
        f = fopen ( filename, "r");
        if( f == NULL)
            break;
            i[0]++;
        fclose(f);
    }
    return (int)i[0] - 48;
}
void ommit(struct node * list, int counter)
{
    struct node * current, *new_next;
    int i;
    for ( current = list, i = 0; i < counter-1; i++, current = current->next );
    new_next = current -> next -> next;
    free(current->next);
    current->next = new_next;
}
void delete_end (struct node * list)
{
    if (list->next == NULL)
    {
        free (list);
        return;
    }

    for( ; list->next->next != NULL; list = list->next);
    free(list->next);
    list->next = NULL;

}
void delete_front( struct node ** list )
{
    struct node * current;
    current = (*list)->next;
    free(*list);
    *list = current;
}
struct node * create_list (FILE * f, int * size)
{
    int n;
    *size = 0;
    struct node  * list, * temp;
    list = (struct node *) malloc (sizeof(struct node));
    temp = list;
    char c;
    while (1)
        {
            fscanf( f , "%c", &c);
            if (feof(f))
                break;
            int i;
            for (i=0; c != ' '; i++ )
             {
                 temp -> val[i] = c;
                 fscanf(f, "%c", &c);
                 if (feof(f))
                    break;
             }
             ++ *size;
             temp->val[i] = '\0';
            temp-> next =(struct node *) malloc (sizeof(struct node));
            temp = temp-> next;
        }
    temp = NULL;
        return list;
}
void SetColor(int ForgC)
{
     WORD wColor;
     HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
     CONSOLE_SCREEN_BUFFER_INFO csbi;

     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
          wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
     return;
}
void countdown ()
{
    int j;
     for(j = 3; j > 0; j--)
                {
                    system("cls");
                    printf("resuming in %d", j);
                    Sleep(1500);
                }
}
void save_scores( char name[100], int level_num)
{
    FILE * scoreboard, *player;
    int start, current, levels;
    float score;
    player = fopen(name, "rb");
    fread(&start, sizeof(int), 1, player);
    fread(&current, sizeof(int), 1, player);
    fread(&score, sizeof(float), 1, player);
    levels = current - start;
    if (levels <= 0)
        levels += level_num;
    scoreboard = fopen ("scoreboard.txt", "a+");
    fprintf( scoreboard, "%s %f ", name, score / levels);
    fclose(player);
    fclose(scoreboard);
}
void ready_file (FILE * file, struct playerinfo player)
{
    fseek( file, 0, SEEK_SET);
    fwrite( &player, sizeof( struct playerinfo), 1, file);
    return;
}
struct playerinfo fpinfo ( int levint, int current, float score)
{
    struct playerinfo player;
    player.start = levint;
    player.current = current;
    player.score = score;
     return player;
}
void show_scores (void)
{
    int i, j, z, s, counter;
    float scores[12], max;
    char name [12][100], chtemp[100];
    FILE * fscore;
    fscore = fopen("scoreboard.txt", "r");
    for( i = 0; !feof(fscore); i++)
    {
     fscanf( fscore, "%s %f ", &name[i], &scores[i]);
    }

    for (j = 0; j < i; j++)
    {
        max = scores[j];
        for(z = j + 1,  s = j; z < i; z++)
        {
            if (scores[z] > max)
            {
                max = scores[z];
                s = z;
            }
        }
        scores[s] = scores[j];
        scores[j] = max;
        strcpy(chtemp, name[s]);
        strcpy(name[s], name[j]);
        strcpy(name[j], chtemp);

    }
    for ( j = 0; j < 10; j++)
    {
        SetColor(79);
        printf("%10s", name[j]);
        SetColor(19);
        printf( "%6.2f\n\n", scores[j]);

    }

    fclose (fscore);
    fscore = fopen("scoreboard.txt", "w");
    if (i > 10)
      i = 10;
    for (j = 0; j < i; j++)
        fprintf(fscore, "%s %f ", name[j], scores[j]);
    fclose(fscore);
}
void askscores(void)
{
    char ans;
    do
    {
        system("cls");
        printf("would you like to see the top scores?\n");
        ans = getch();
        if (ans == 'Y' || ans == 'y')
        {
            show_scores();
            return;
        }

    }while(ans != 'N' && ans !='n');
}
float mode1  ( int levint, int level_num, char name[100])
{
    char c, filename[20] = "levels/level-", ans, levch[100];
    int mode, arr, i=0, j=0, r, size, counter, words = 0, speed = 0, letter = 0, wrong = 0;
    float diff = 0, leveltime = 0, score = 0;
    struct node * list, * current;
    struct timeb t1, t2, tempt1, tempt2;
    time_t t;
    FILE * playerfile, * level, *scoreboard;
    player = fpinfo ( levint, levint, score);
    sprintf(levch, "%d", levint);
    strcat( filename, levch);
    strcat( filename, ".txt\0");
    playerfile = fopen( name  , "wb");
    ready_file (playerfile, player);
    level = fopen ( filename , "r");
    system("cls");
    list = create_list(level, &size);
    for ( ; size != 0; size--)
    {
        t = time(NULL);
        srand(t);
        r = rand() %size;
        for(current = list, counter = 0; counter < r; counter++, current = current->next);
            printf("%s", current->val);
        i = 0;
        ftime(&t1);
        letter = 0;
        wrong = 0;
        while (current->val[i] != '\0')
        {
            c = getch();
            if (c == 'Q')
            {
                do
                {
                    ftime(&tempt1);
                    system("cls");
                    printf(" Are you sure? [Y/N]");
                    ans = getch();
                    if (ans == 'Y' || ans == 'y')
                    {
                        do
                        {
                            system("cls");
                            printf("Do you want to save this game[Y/N]?");
                            ans = getch();
                            if (ans == 'Y' || ans == 'y')
                            {
                                save_scores(name, level_num);
                                fclose(playerfile);
                                askscores();
                                exit(0);
                            }
                        }while (ans != 'N' && ans != 'n');
                        player = fpinfo(1, 0, 0);
                        ready_file(playerfile, player);
                        askscores();
                        exit(0);
                    }
                }while (ans != 'N' && ans != 'n');
                system("cls");
                countdown();
                system("cls");
                    SetColor(34);
                    for(j = 0; j < i; j++)
                        printf("%c", current -> val[j]);
                    SetColor(79);
                    for(; current->val[j] != '\0' ; j++)
                        printf("%c", current -> val[j]);
                    ftime(&tempt2);
                    t1.time += tempt2.time - tempt1.time;
            }
            if ( c == 'P')
            {
                ftime(&tempt1);
                do{
                    system("cls");
                    printf("paused\n");
                    c = getch();
                }while(c != 'R' && c != 'Q');
               countdown();
                system("cls");
                SetColor(34);
                    for(j = 0; j < i; j++)
                        printf("%c", current -> val[j]);
                    SetColor(79);
                    for(; current->val[j] != '\0' ; j++)
                        printf("%c", current -> val[j]);
                ftime(&tempt2);
                t1.time += tempt2.time - tempt1.time;
            }
            if (c == current->val[i] )
            {
                system("cls");
                letter ++;
                SetColor(34);
                for(j = 0; j <= i; j++)
                    printf("%c", current -> val[j]);
                SetColor(79);
                for(; current->val[j] != '\0' ; j++)
                    printf("%c", current -> val[j]);
                i++;
            }
            else if (c != 'R' && c != 'Q')
            {
                system("cls");
                wrong++;
                SetColor(34);
                for(j = 0; j < i; j++)
                    printf("%c", current -> val[j]);
                SetColor(60);
                for(; j <= i; j++)
                    printf("%c", current -> val[j]);
                SetColor(79);
                for(; current->val[j] != '\0' ; j++)
                    printf("%c", current -> val[j]);
            }
        }
        ftime(&t2);
        diff = (float)((int) (1000.0 * (t2.time - t1.time) + (t2.millitm - t1.millitm)))/1000;
        leveltime += diff;
        score += (letter * 3 - wrong)/ diff;
        system("cls");
        if (current->next == NULL)
            delete_end(list);
        if(size == 1)
            break;
        if( current == list )
            delete_front(&list);
        else if (current->next != NULL)
            ommit(list, counter);
            words++;
        }
    SetColor(79);
    printf("your speed is ");
    SetColor(19);
    printf("%d wpm\n", (int)(words / (leveltime/60)));
    SetColor(79);
    printf("your score is : ");
    SetColor(19);
    printf("%.2f\n", score / words);
    if(levint == level_num)
        player = fpinfo(level_num, 1, score /  words);
    else
        player = fpinfo(levint, levint + 1, score / words);
    ready_file(playerfile, player);
    SetColor(79);
    fclose(playerfile);
    fclose(level);
    free(list);
    return score / words;
}
float mode2 ( int levint, char name [100], int keep_current_level, float temp, int numoflevels)
{
    FILE * playerfile, * level;
    char c, filename[20] = "levels/level-", ans, levch[100];
    int mode, arr, i=0, r, size, counter, words = 0, speed = 0, letter = 0, wrong = 0, currentlevel = levint;
    float diff = 0, leveltime = 0, score = 0, previous_scores;
    struct node * list, * current;
    struct timeb t1, t2, tempt1, tempt2;
    time_t t;
    int start;
    playerfile = fopen(name, "rb+");
    fread(&start, sizeof(int), 1, playerfile);
   int j;
    for(j = 3; j > 0; j--)
    {
        system("cls");
        printf("level %d will start in %d",levint, j);
        Sleep(1500);
    }
    system ("cls");
        sprintf(levch, "%d", levint);
        strcat( filename, levch);
        strcat( filename, ".txt\0");
        level = fopen(filename, "r");
        list = create_list(level, &size);
        for ( ; size != 0; size--)
        {
            t = time(NULL);
            srand(t);
            r = rand() %size;
            for(current = list, counter = 0; counter < r; counter++, current = current->next);
                printf("%s", current->val);
            i = 0;
            ftime(&t1);
            letter = 0;
            wrong = 0;
            while (current->val[i] != '\0')
            {
                c = getch();
                if (c == 'Q')
                {
                    do
                {
                    ftime(&tempt1);
                    system("cls");
                    printf(" Are you sure? [Y/N]");
                    ans = getch();
                    if (ans == 'Y' || ans == 'y')
                    {
                        do
                        {
                            system("cls");
                            printf("Do you want to save this game[Y/N]?");
                            ans = getch();
                            if (ans == 'Y' || ans == 'y')
                            {
                                save_scores(name, numoflevels);
                                fclose(playerfile);
                                askscores();
                                exit(0);
                            }

                        }while (ans != 'N' && ans != 'n');
                        fseek(playerfile, 2* sizeof(int), SEEK_SET);
                        fread(&score, sizeof(float), 1, playerfile);
                        player = fpinfo(start, keep_current_level, score - temp);
                        ready_file(playerfile, player);
                        askscores();
                        exit(0);
                    }
                }while (ans != 'N' && ans != 'n');
                system("cls");
                countdown();
                system("cls");
                    SetColor(34);
                    for(j = 0; j < i; j++)
                        printf("%c", current -> val[j]);
                    SetColor(79);
                    for(; current->val[j] != '\0' ; j++)
                        printf("%c", current -> val[j]);
                    ftime(&tempt2);
                    t1.time += tempt2.time - tempt1.time;
                }
                if ( c == 'P')
                {
                    system("cls");
                    printf("paused");
                    ftime(&tempt1);
                    do{
                        c = getch();
                    }while(c != 'R');
                    countdown();
                    system("cls");
                    SetColor(34);
                    for(j = 0; j < i; j++)
                        printf("%c", current -> val[j]);
                    SetColor(79);
                    for(; current->val[j] != '\0' ; j++)
                        printf("%c", current -> val[j]);
                    ftime(&tempt2);
                    t1.time += tempt2.time - tempt1.time;
                }
                if (c == current->val[i] )
                {
                    system("cls");
                    letter ++;
                    SetColor(34);
                    for(j = 0; j <= i; j++)
                        printf("%c", current -> val[j]);
                    SetColor(79);
                    for(; current->val[j] != '\0' ; j++)
                        printf("%c", current -> val[j]);
                    i++;
                }
                else if (c != 'R' && c != 'Q')
                {
                    system("cls");
                    wrong++;
                    SetColor(34);
                    for(j = 0; j < i; j++)
                        printf("%c", current -> val[j]);
                    SetColor(60);
                    for(; j <= i; j++)
                        printf("%c", current -> val[j]);
                    SetColor(79);
                    for(; current->val[j] != '\0' ; j++)
                        printf("%c", current -> val[j]);
                }
            }
            ftime(&t2);
            diff = (float)((int) (1000.0 * (t2.time - t1.time) + (t2.millitm - t1.millitm)))/1000;
            leveltime += diff;
            score += (letter * 3 - wrong)/ diff;
            system("cls");
            if (current->next == NULL)
                delete_end(list);
            if(size == 1)
                break;
            if( current == list )
                delete_front(&list);
            else if (current->next != NULL)
                ommit(list, counter);
                words++;
            }
        SetColor(79);
        printf("your speed is ");
        SetColor(19);
        printf("%d wpm\n", (int)(words / (leveltime/60)));
        SetColor(79);
        printf("your score for this level is : ");
        SetColor(19);
        printf("%.2f\n", score / words);
        currentlevel ++;
        fseek(playerfile, 2 * sizeof(int), SEEK_SET);
        fread(&previous_scores, sizeof(float), 1, playerfile);
        player = fpinfo(start, currentlevel, score / words + previous_scores);
        fseek(playerfile, 0, SEEK_SET);
        fwrite(&player, sizeof (struct playerinfo), 1, playerfile);
        fseek(playerfile, 2 * sizeof(int), SEEK_SET);
        fread(&previous_scores, sizeof(float), 1, playerfile);
        SetColor(79);
        printf("total score : ");
        SetColor(19);
        int played;
        played = currentlevel - start;
        if (played <= 0)
            played += numoflevels;
        printf("%.2f\n", previous_scores / played);
        SetColor(79);
        fclose(playerfile);
        fclose(level);
        free(list);
        return score / words;
}
int main()
{
    char name [100], ans, levch[100], filename[20];
    int mode, level_num, start = 0, levint;
    float score = 0, temp, test= 0;
    FILE * n, *level;
    SetColor(79);
    start:  printf("Hello and welcome :)\nplease enter your name ");
    SetColor(19);
    gets(name);
    n = fopen(name, "rb");
    fread(&start, sizeof(int), 1, n);
    fread(&levint, sizeof(int), 1, n);
    fread (&test, sizeof(float), 1, n);
    level_num = num_of_level ();
    if (n == NULL || levint == 0)
    {
        SetColor(79);
        printf( "welcome %s, it is your first time playing. press any key to start a new game.\n", name);
        getch();
        mode = 1;
    }
    else
    {
        mode = 3;
        if((start  == levint && test != 0) || (start == 1 && levint == level_num + 1))
        {
            do{
                SetColor(79);
                printf("You have finished this game once before\nEither you can start a new game with current account by entering: ");
                SetColor(19);
                printf("1\n");
                SetColor(79);
                printf("Or you can sign in with a different one by entering: ");
                SetColor(19);
                printf("2\n");
                SetColor(79);
                scanf("%d", &mode);
            system("cls");
            }while(mode != 2 && mode != 1);
            if (mode == 2)
            {
                gets(name);
                main();
                exit (0);
            }
        }
         if( mode == 3){
            SetColor(79);
            printf("\nIf you wanna start a new game enter ");
            SetColor(19);
            printf("1\n");
            SetColor(79);
            printf("If you wanna resume your old game enter ");
            SetColor(19);
            printf("2\n");
            SetColor(79);
            scanf("%d", &mode);
        }
    }
        if (mode == 1)
    {
        fclose(n);
        printf("There are %d levels. Which level do you wanna play? ", level_num);
        scanf("%d", &levint);
        sprintf(levch, "%d", levint);
        strcpy(filename, "levels/level-");
        strcat( filename, levch);
        strcat( filename, ".txt\0");
        level = fopen(filename, "r");
        while( level == NULL)
    {
        printf("wrong input. there are only %d levels ", level_num);
        strcpy(filename, "levels/level-");
        scanf("%d", &levint);
        sprintf(levch, "%d", levint);
        strcat( filename, levch);
        strcat( filename, ".txt\0");
        level = fopen ( filename , "r");
    }
        start = levint;
        score = mode1(levint, level_num, name);
        do
        {
            if ((levint == start - 1) || (start == 1 && levint == level_num ))
            {
                printf("you have finished the game");
                return 0;
            }
            printf("you finished level %d. do you wanna play the next level? [Y/N]\n", levint);
            ans = getch();
            system("cls");
            if (ans == 'Y' || ans == 'y')
            {
                if (levint == level_num)
                    levint = 0;
                levint++;
               score =  mode2 (levint, name, 0, score, level_num);
            }
        }while (ans != 'N' && ans != 'n');
        do
        {
            system("cls");
            printf("Do you want to save this game[Y/N]?");
            ans = getch();
            if (ans == 'Y' || ans == 'y')
            {
                save_scores(name,level_num);
                askscores();
                exit(0);
            }
        }while (ans != 'N' && ans != 'n');
        n = fopen(name, "wb");
        player = fpinfo(1,0,0);
        fwrite(&player, sizeof(struct playerinfo), 1, n);
        fclose(n);
        askscores();
        return 0;
    }
    int keep_current_level;
    if (mode == 2)
    {
        fseek(n, 0, SEEK_SET);
        fread(&start, sizeof(int), 1, n);
        fread(&levint, sizeof (int), 1, n);
        fread (&test, sizeof(float), 1, n);
        keep_current_level = levint;
        fclose(n);
        if ( start == levint && test == 0)
        {
            printf("you haven't, completed any levels yet. Starting from level %d", start);
        }
        if(levint == level_num+1)
        {
            printf("You've played level %d to level %d\nNow it's time for level %d", start, level_num, 1);
            levint = 1;
        }

        else if (start != levint)
        {
            if(levint - 1 == start )
                printf("you've finished level %d\nNow it's time for level %d", start, levint);
            else
             printf("You've played level %d to level %d\nNow it's time for level %d", start, levint-1, levint);
        }

        Sleep(2000);
        score += mode2(levint, name, keep_current_level, score, level_num);
         do
        {

            if ((levint == start - 1 && test != 0) || (start == 1 && levint == level_num ))
            {
                printf("you have finished the game");
                return 0;
            }
            printf("you finished level %d. do you wanna play the next level? [Y/N]\n", levint);

            if (levint == level_num)
                    levint = 0;

            ans = getch();
            system("cls");
            if (ans == 'Y' || ans == 'y')
            {
                levint++;
                score +=  mode2 (levint, name, keep_current_level, score, level_num);
            }
        }while (ans != 'N' && ans != 'n');
        do
        {
            system("cls");
            printf("Do you want to save this game[Y/N]?");
            ans = getch();
            if (ans == 'Y' || ans == 'y')
            {
                save_scores(name, level_num);
                askscores();
                return 0;
            }
        }while (ans != 'N' && ans != 'n');
        n = fopen(name, "rb+");
        fseek(n, 2 * sizeof(int), SEEK_SET);
        fread(&temp, sizeof(float), 1,  n);
        player = fpinfo(start, keep_current_level, temp - score);
        ready_file(n, player);
        askscores();
        fclose(n);
    }
    return 0;
}
