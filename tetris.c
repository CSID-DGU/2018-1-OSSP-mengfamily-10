/*
 *      tetris.c TTY-TETRIS Main file.
 *      Copyright © 2008 Martin Duquesnoy <xorg62@gmail.com>
 *      All rights reserved.
 *
 *      Redistribution and use in source and binary forms, with or without
 *      modification, are permitted provided that the following conditions are
 *      met:
 *
 *      * Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *      * Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following disclaimer
 *        in the documentation and/or other materials provided with the
 *        distribution.
 *      * Neither the name of the  nor the names of its
 *        contributors may be used to endorse or promote products derived from
 *        this software without specific prior written permission.
 *
 *      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *      "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *      LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *      A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *      OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *      SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *      LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *      DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *      THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *      (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *      OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <SDL2/SDL.h>
#include "tetris.h"
#include "config.h"
#include "buffer.h"

int whichThread[2] = {0, 1};
int speenOnLevel[5] = {1, 0.7, 0.5, 0.4, 0.3};
/* Functions */

char* first(char * name)
{
  
  char start;
  printf("\n\n\t당신의 이름은? ");
  scanf("%s",name);
  printf("\n\n\t\t\tpress enter to enter game!");	 //tab세번이 적절
  while (1) {
    start = getchar();
    if (start == '\n')break;
  }
  return name;
}
void init(void)
{
  
     struct sigaction siga;
     struct termios term;

     /* Clean term */
     clear_term();
     set_cursor(False);
     /* Init terminal (for non blocking & noecho getchar();
      이것을 위로 옮긴 이유는 처음부터 껌벅커서를 지우기 위해*/
     tcgetattr(STDIN_FILENO, &term);
     tcgetattr(STDIN_FILENO, &back_attr);
     term.c_lflag &= ~(ICANON|ECHO);
     tcsetattr(0, TCSANOW, &term);
    /*스타트함수를 init에 통합했다*/
     //set_cursor(False); //커서없애줌
 
     printxy(0, FRAMEH_NB + 2, FRAMEW + 3, "Score :");
     printxy(0, FRAMEH_NB + 3, FRAMEW + 3, "Lines :");
     printxy(0, FRAMEH_NB + 4, FRAMEW + 3, "Left  : ←"); 
     printxy(0, FRAMEH_NB + 5, FRAMEW + 3, "Right : →"); 
     printxy(0, FRAMEH_NB + 6, FRAMEW + 3, "Change: ↑");
     printxy(0, FRAMEH_NB + 7, FRAMEW + 3, "Down  : ↓"); 
     printxy(0, FRAMEH_NB + 8, FRAMEW + 3, "Drop  : Space Bar");
     printxy(0, FRAMEH_NB + 9, FRAMEW + 3, "Pause : p"); 
     printxy(0, FRAMEH_NB + 10, FRAMEW + 3, "Revive : r");
     printxy(0, FRAMEH_NB + 11, FRAMEW + 3, "Quit  : q");  
    //게임 시작하기 전에 안내를 한번 해줌
    
     clear_term(); //화면 지움
     /* Make rand() really random :) */
     srand(getpid());

     /* Init variables */
     score = 0;
      
     running = True;	//true일 경우에 게임의 축이 되는 루프가 계속 돌아가고 false일 경우 루프가 break되고 quit함수가 호출되어 종료된다
     current.y = (FRAMEW / 2) - 1; 
     current.num = nrand(0, 7); //7가지블록랜덤
     current.next = nrand(0, 7);//다음 블록의 종류를 정함

     /* Score(오른쪽에 표시되는 안내사항을 보여주는 */
     printxy(0, FRAMEH_NB + 1, FRAMEW + 3, "Level :");
     printxy(0, FRAMEH_NB + 2, FRAMEW + 3, "Score :");
     printxy(0, FRAMEH_NB + 3, FRAMEW + 3, "Lines :");
     printxy(0, FRAMEH_NB + 4, FRAMEW + 3, "Left  : ←"); 
     printxy(0, FRAMEH_NB + 5, FRAMEW + 3, "Right : →"); 
     printxy(0, FRAMEH_NB + 6, FRAMEW + 3, "Change: ↑");
     printxy(0, FRAMEH_NB + 7, FRAMEW + 3, "Down  : ↓"); 
     printxy(0, FRAMEH_NB + 8, FRAMEW + 3, "Drop  : Space Bar");
     printxy(0, FRAMEH_NB + 9, FRAMEW + 3, "Pause : p"); 
     printxy(0, FRAMEH_NB + 10, FRAMEW + 3, "Revive : r");
     printxy(0, FRAMEH_NB + 11, FRAMEW + 3, "Life : ");
     printxy(0, FRAMEH_NB + 12, FRAMEW + 3, "Quit  : q"); 
     DRAW_SCORE();

     /* Init signal */
     sigemptyset(&siga.sa_mask);
     siga.sa_flags = 0;
     siga.sa_handler = sig_handler;
     sigaction(SIGALRM, &siga, NULL);
     sigaction(SIGTERM, &siga, NULL);
     sigaction(SIGINT,  &siga, NULL);
     sigaction(SIGSEGV, &siga, NULL);

     /* Init timer */
     tv.it_value.tv_usec = TIMING;
     sig_handler(SIGALRM);

     
 
     return;
}

/*TODO sj todo
 * 사용자 입력이 들어오면 isChange를 바꿔줘야함 // flag 역할을 함
 * CHECK KEY_MOVE_DOWN*/
void
get_key_event(int c)
{
    /*TODO sj
     * consider this*/
     if(c > 0)
          --current.x;
	 /*main함수중에 전체 루프중에 필수적으로 거치는 함수이자 입력받은 값에따라 게임 진행이 된다.
    여기서 key_pause와 key_quit는 게임을 계속 진행하는 대에 영향을 준다*/
     switch(c)
     {
     case KEY_MOVE_LEFT:            shape_move(-EXP_FACT);              break;
     case KEY_MOVE_RIGHT:           shape_move(EXP_FACT);               break;
     /*TODO sj
      * ++scoreeeeeeeeeee?????*/
     case KEY_MOVE_DOWN:            ++current.x; ++score; DRAW_SCORE(); break;
     case KEY_CHANGE_POSITION_NEXT: shape_set_position(N_POS);          break;
     /*TODO sj
      * whhhat is the different between KEY_MOVE_DOWN and KEY_DRPO_SHHHAPE*/
     case KEY_DROP_SHAPE:           shape_drop();                       break;
     case KEY_PAUSE:                while(getchar() != KEY_PAUSE);      break;
     case KEY_QUIT:                 running = False;                    break;
     case 'Q':                      running = False;                    break; //대문자 Q를 사용할 때 종료
     case 'r':                      if(lifes != 0) revive();             break;
     //case 't':                      sleep(5);                         break; //5초 정지 
     //시간 멈추는 능력 
     }  

     return;
}

void
arrange_score(int l)
{
     /* 클리어한 라인에따라 점수부여. 여기서 의문점이 5줄이상일때 에러가 발생하는지
     테트리스는 5줄이상 못깹니다.  */
     switch(l)
     {
     case 1: score += 40;   break; /* One line */
     case 2: score += 100;  break; /* Two lines */
     case 3: score += 300;  break; /* Three lines */
     case 4: score += 1200; break; /* Four lines */
     }

     
     if (score >=100)  //레벨 추가
      level=2;
     if (score >=400)
      level=3;
     if (score >=700)
      level=4;
     if(score >= 1000)
      level =5;
     lines += l;

     DRAW_SCORE();

     return;
}

void
check_plain_line(void)
{
if(SDL_Init(SDL_INIT_AUDIO) < 0)
    {
       exit;
    }
    
     int i, j, k, f, c = 0, nl = 0;

     for(i = 1; i < FRAMEH; ++i)
     {
          for(j = 1; j < FRAMEW; ++j)
               if(frame[i][j] == 0)
                    ++c;
          if(!c)
          {
               ++nl;
               sound("pop.wav",400);
               for(k = i - 1; k > 1; --k)
                    for(f = 1; f < FRAMEW; ++f)
                         frame[k + 1][f] = frame[k][f];
          }
          c = 0;
     }
     arrange_score(nl);
     frame_refresh();

     return;
}

int
check_possible_pos(int x, int y)
{
     int i, j, c = 0;

     for(i = 0; i < 5; ++i)
          for(j = 0; j < EXP_FACT; ++j)
               if(frame[x + shapes[current.num][current.pos][i][0]]
                  [y + shapes[current.num][current.pos][i][1] * EXP_FACT + j] != 0)
                    ++c;

     return c;
}

void quit(char * name)
{
  FILE *rp;
    rp = fopen ("score.txt","a+");
    int best_sc;
    fscanf(rp,"%d",&best_sc);
  

	 char end;
     frame_refresh(); /* Redraw a last time the frame */

     set_cursor(True); //이 함수로인해 터미널창 커서가 숨김에서 풀린다
     tcsetattr(0, TCSANOW, &back_attr); //TCSANOW는 즉시속성을 변경을 의미, 
  
     if(best_sc<score)
     {
        printf("\n\n\n%d ",best_sc);
      if(best_sc != 0)
        {
          FILE *wp;
          wp = fopen ("score.txt","w");
          fprintf(wp,"%d %s",score,name);
          fclose(wp);
        }
      else
          {
            fprintf(rp,"%d %s",score,name);
            printf("\n\n\nsad");
          }
      printf("\n\n\t축하합니다. %s님이 레벨 %d, 최고점수 %d 점을 달성했습니다.\n\n",name,level,score);
     }
    else
       {
        printf("\n\n\t수고하셨습니다. %s님의 레벨 %d, 점수는: %d입니다.\n\n",name,level, score);
       }
       fclose(rp);
       
	 printf("\n\n\t\t\tpress enter to end the game!\n");
	 while (1) {
		 end = getchar();
		 if (end == '\n')break;
   }
   set_cursor(True); 
   tcsetattr(0, TCSANOW, &back_attr); //TCSANOW는 즉시속성을 변경을 의미, 터미널 세팅을 되돌리기
    system("clear"); //입력창이 다 밑으로 내려가서 이걸로하면 다시위로감


     return;
}

void music(const char * filename, int len){
 

    /* Init Simple-SDL2-Audio */
    initAudio();
    Audio * music = createAudio(filename, 1, SDL_MIX_MAXVOLUME);
    playMusicFromMemory(music, SDL_MIX_MAXVOLUME);
    SDL_Delay(len);

    /* End Simple-SDL2-Audio */
    endAudio();

    /* Important to free audio after ending Simple-SDL2-Audio because they might be referenced still */
    freeAudio(music);

    
}
void sound(const char * filename, int len){
  initAudio();
  Audio * sound = createAudio(filename, 0, SDL_MIX_MAXVOLUME / 2);
  playSoundFromMemory(sound, SDL_MIX_MAXVOLUME);
  SDL_Delay(len);
  endAudio();
  freeAudio(sound);
}

/*TODO sj
 * 새롭게 만들어진 스레드를 위한 것*/
void *runner(void *param){
    int checkThread = atoi(param);
    //int checkThread = param;
    if(checkThread == 0)    // 전달된 param을 써서 구분
        inputThread();
    else if(checkThread == 1)
        tetrominoShiftsThread();
}

/*TODO sj todo
 * 이거 tetris.h에 나중에 추가하기*/
void inputThread(){  // 사용자의 입력을 받아들일 부분
    while(running){
        int c = getchar();
        sem_wait(&empty);

        pthread_mutex_lock(&mutex);

        if(count != BUFFER_SIZE){
            buffer[in] = c;
            in = (in+1)%BUFFER_SIZE;
            count++;
        }

        pthread_mutex_unlock(&mutex);

        sem_post(&full);
    }
}
void tetrominoShiftsThread(){ // 사용자의 입력을 반영해 frame을 그릴 부분
    while(running){
        int output;
        sem_wait(&full);

        pthread_mutex_lock(&mutex);

        if(count!=0){
            output = buffer[out];
            out = (out+1)%BUFFER_SIZE;
            count--;
        }

        pthread_mutex_unlock(&mutex);

        sem_post(&empty);

        get_key_event(output);
    }
}

int
main(int argc, char **argv)
{
    /*변수들*/
    level = 1;
    int n =1;
     current.last_move = False;
     lifes = 2;
     lines = 0;
     char myname[10]; 
	
    /*TODO sj
     * for thread*/
    //pthread_t tid[3];
    pthread_t producer[2];
    pthread_t consumer[4];
    pthread_attr_t attr;

    /*TODO sj
     * initialize for buffer*/
    count =0;
    in =0;
    out =0;

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    whichLevel =0;  // initial level for speed
	
    /* Initialize only SDL Audio on default device */
    if(SDL_Init(SDL_INIT_AUDIO) < 0)
    {
       exit;
    }
         
     first(myname);
      //초기음악
     sound("test.wav", 2000);
     init(); //게임 진행중에도 게임 사용법 보여
     frame_init();
     frame_nextbox_init();;
      //여기까지 게임을 초기화하는 부분
    
    /*TODO sj
     * create thread
     * and call runner*/
    pthread_attr_init(&attr);
    for(int i=0; i<2; i++)
        pthread_create(&producer[i], &attr, runner, &whichThread[0]);
    for(int i=0; i<4; i++)
        pthread_create(&consumer[i], &attr, runner, &whichThread[1]);
	
      while(running)
     {
	      /*TODO sj todo
      	 * 여기 루프 싹 다 바꿈
      	 * 이것들을 다 runner로 빼버릴거임*/
      	sleep(speenOnLevel[whichLevel]);
      	/*TODO sj
      	 **/

          /*TODO sj todo
           * why
           * duplicated shape_set()
           * shape_go_down()->shape_set()*/
     	  shape_go_down();

     }	//이것이 게임루프의 주축이 되는 부분
     
     sound("violin.wav",9000);
      SDL_Quit();
      quit(myname);
     return 0;
}
