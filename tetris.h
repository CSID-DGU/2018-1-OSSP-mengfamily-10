/*
 *      tetris.h
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

#ifndef TETRIS_H
#define TETRIS_H
/* Libs */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include <string.h>
#include <sys/time.h>
#include <SDL2/SDL.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "audio.h"
#include "buffer.h"
/*TODO sj*/
#include <pthread.h>
#include <semaphore.h>
  
/* Expension factor of shapes */
#define EXP_FACT 2

/* Frame dimension */
#define FRAMEW 23
#define FRAMEH 20
#define FRAMEW_NB 18
#define FRAMEH_NB 8

/* Shapes position */
#define N_POS ((current.pos < 3) ? current.pos + 1 : 0)
#define P_POS ((current.pos > 0) ? current.pos - 1 : 3)

/* Draw the score.. */
#define DRAW_SCORE() set_color(Score);                              \
     printf("\033[%d;%dH %d", FRAMEH_NB + 2, FRAMEW + 10, level);   \
     printf("\033[%d;%dH %d", FRAMEH_NB + 3, FRAMEW + 10, score);   \
     printf("\033[%d;%dH %d", FRAMEH_NB + 4, FRAMEW + 10, lines);   \
     printf("\033[%d;%dH %d", FRAMEH_NB + 12, FRAMEW + 10, lifes);   \
     set_color(0);

/* Bool type */
typedef enum { False, True } Bool;

/* Shape structure */
typedef struct
{
     int num;
     int next;
     int pos;
     int x, y;
     Bool last_move;
} shape_t;

/* Color enum */
enum { Black, Blue, Red, Magenta, White, Green, Cyan, Yellow, Test, Test_2, Test_3, Border, Score, ColLast };

/* Prototypes */

/* util.c */
void clear_term(void);
void set_cursor(Bool);
void printxy(int, int, int, char*);
void set_color(int);
int nrand(int, int);
void sig_handler(int);

/* frame.c */
void frame_init(void);
void frame_nextbox_init(void);
void frame_refresh(void);
void frame_preview(void);
void frame_nextbox_refresh(void);
void view(void);

/* shapes.c */
void shape_set_unset(int n);
void shape_unset(void);
void shape_unset2(void);
void shape_new(void);
void shape_go_down(void);
void shape_set_position(int);
void shape_move(int);
void shape_drop(void);
void block_down(void);
void revive(void);
int height;

/* tetris.c */

int height;
int check;
//char* first(char * name);
int first(char * myId, char * myPwd);
void init(void);
void arrange_score(int l);
void arrange_score2(int h);
void check_plain_line(void);
int check_possible_pos(int, int);

/*TODO sj
 * changed param*/
void get_key_event(int c);
void quit(char * name,int user_idx);

void music(const char * filename, int len);
void sound(const char * filename, int len);

/*TODO sj
 * 스레드를 위한 함수들*/
//void *runner(void *param);
void *inputThread(void *param);
void *tetrominoShiftsThread(void *param);
void delay(unsigned int sec);

buffer_item buffer[BUFFER_SIZE];
int count, in, out;
sem_t empty, full;
pthread_mutex_t mutex;
pthread_mutex_t callFunc;
pthread_mutex_t locInfo;

//int whichThread[2] = {0, 1};
///* Variables */

const int shapes[10][4][5][2];
struct itimerval tv;
struct termios back_attr;
shape_t current;
int frame[FRAMEH + 1][FRAMEW + 1];
int frame_nextbox[FRAMEH_NB][FRAMEW_NB];
int score;
int lines;
int level;
int lifes;
int user_idx;

Bool running;

//////int speenOnLevel[5] = {1, 0.7, 0.5, 0.4, 0.3}; // parameter for sleep()
/*TODO sj 승민아 이 배열 바꿔서 적용시키면 레벨별로 내려오는 시간 다르게 할 수 있어*/
int whichLevel;

#endif