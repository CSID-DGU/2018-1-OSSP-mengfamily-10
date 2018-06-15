/*
 *      shapes.c
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

#include "tetris.h"

/*  Shapes data */
/*  각각의 4X4X2 배열은 블록 모양의 4가지 경우를 표현한다. 따라서 현재는 7가지 종류의 블록을 가진다.
 *  예를들어, I 모양의 블록을 상하좌우로 돌리는 경우 각각의 상태가 배열에 저장되어 있는것.
 *  X와 Y축이 0~3인 좌표계를 생각하고, {0~3,0~3}의 좌표로 도형을 나타낸다.
 */


const int shapes[10][4][5][2] =
{
     /* O */
     /* 정사각형 */
     {
          {{0,0},{1,0},{0,1},{1,1},{1,1}},
          {{0,0},{1,0},{0,1},{1,1},{1,1}},
          {{0,0},{1,0},{0,1},{1,1},{1,1}},
          {{0,0},{1,0},{0,1},{1,1},{1,1}}
     },
     /* I */
     /* 일자형 막대 */
     {
          {{1,0},{1,1},{1,2},{1,3},{1,3}},
          {{0,1},{1,1},{2,1},{3,1},{3,1}},
          {{1,0},{1,1},{1,2},{1,3},{1,3}},
          {{0,1},{1,1},{2,1},{3,1},{3,1}}
     },
     /* L */
     /* 오른쪽 방향 ㄴ자 블록 */
     {
          {{0,1},{1,1},{2,1},{2,2},{2,2}},
          {{1,0},{1,1},{1,2},{2,0},{2,0}},
          {{0,0},{0,1},{1,1},{2,1},{2,1}},
          {{1,0},{1,1},{1,2},{0,2},{0,2}}
     },
     /* J */
     /* 왼쪽 방향 ㄱ자 블록 */
     {
          {{1,0},{1,1},{1,2},{2,2},{2,2}},
          {{0,2},{1,2},{2,2},{2,1},{2,1}},
          {{0,0},{1,0},{1,1},{1,2},{1,2}},
          {{0,1},{0,2},{1,1},{2,1},{2,1}}
     },
     /* S */
     /* S자 블록 */
     {
          {{1,1},{1,2},{2,0},{2,1},{2,1}},
          {{0,1},{1,1},{1,2},{2,2},{2,2}},
          {{1,1},{1,2},{2,0},{2,1},{2,1}},
          {{0,1},{1,1},{1,2},{2,2},{2,2}}
     },
     /* Z */
     /* Z자 블록 */
     {
          {{0,0},{0,1},{1,1},{1,2},{1,2}},
          {{0,2},{1,1},{2,1},{1,2},{1,2}},
          {{0,0},{0,1},{1,1},{1,2},{1,2}},
          {{0,2},{1,1},{2,1},{1,2},{1,2}}
     },
     /* T */
     /* T 블록 */
     {
          {{0,1},{1,0},{1,1},{1,2},{1,2}},
          {{0,1},{1,1},{1,2},{2,1},{2,1}},
          {{1,0},{1,1},{1,2},{2,1},{2,1}},
          {{1,0},{0,1},{1,1},{2,1},{2,1}}
     },
     /* + 블록 */
     {
          {{1,0},{0,1},{2,1},{1,2},{1,1}},
          {{1,0},{0,1},{2,1},{1,2},{1,1}},
          {{1,0},{0,1},{2,1},{1,2},{1,1}},
          {{1,0},{0,1},{2,1},{1,2},{1,1}}
     }, 
     /* . 블록 */
     {
          {{1,1},{1,1},{1,1},{1,1},{1,1}},
          {{1,1},{1,1},{1,1},{1,1},{1,1}},
          {{1,1},{1,1},{1,1},{1,1},{1,1}},
          {{1,1},{1,1},{1,1},{1,1},{1,1}}
     },
    
     {
          {{0,0},{0,1},{1,1},{0,1},{0,1}},
          {{1,0},{1,1},{0,1},{0,1},{0,1}},
          {{1,1},{0,1},{0,0},{0,0},{0,0}},
          {{1,0},{0,0},{0,1},{0,1},{0,1}}
     },
};


void shape_set_unset(int n){ //chhhhanged
    //pthread_mutex_lock(&callFunc);
    //pthread_mutex_lock(&locInfo);
    int i,j;

    if(n != 1){
        //printf("\nunset\n");
        //printf("%d %d %d %d\n", current.x, current.y, current.num, current.pos);
        for(i = 0; i < 5; ++i){
            for(j = 0; j < EXP_FACT; ++j) {
                frame[current.x + shapes[current.num][current.pos][i][0]]
                [current.y + shapes[current.num][current.pos][i][1] * EXP_FACT + j] = 0;
//                printf("%d ", frame[current.x + shapes[current.num][current.pos][i][0]]
//                [current.y + shapes[current.num][current.pos][i][1] * EXP_FACT + j]);
            }
            //printf("\n");
        }
    // erase it

        if(current.x < 1)
          for(i = 0; i < FRAMEW + 1; ++i)
            frame[0][i] = Border;
    }
    if(n != 0){
        //printf("\nset\n");
        //printf("%d %d %d %d", current.x, current.y, current.num, current.pos);
        for(i = 0; i < 5; ++i) {
            for (j = 0; j < EXP_FACT; ++j) {
                frame[current.x + shapes[current.num][current.pos][i][0]] /* 1 + 1 */
                [current.y + shapes[current.num][current.pos][i][1] * EXP_FACT + j] /* 24 + 0 * 2 + 0 */
                        = current.num + 1; /* frame[0~5][0~54] = 0~6 + 1 */
                //printf("%d ", frame[current.x + shapes[current.num][current.pos][i][0]]
                //[current.y + shapes[current.num][current.pos][i][1] * EXP_FACT + j]);
            }
            //printf("\n");
        }

        if(current.x < 1)
            for(i = 0; i < FRAMEW + 1; ++i)
                frame[0][i] = Border;

        frame_refresh();
        }
    //pthread_mutex_unlock(&callFunc);
    //pthread_mutex_unlock(&locInfo);
}

void
shape_set(void)
{
    //printf("shape set\n");


    //pthread_mutex_lock(&callFunc);
     int i, j;


//    for(i = 0; i < 5; ++i)
//        for(j = 0; j < EXP_FACT; ++j)
//            frame[current.x + shapes[current.num][current.pos][i][0]]
//            [current.y + shapes[current.num][current.pos][i][1] * EXP_FACT + j] = 0;
//    // erase it
//
//    if(current.x < 1)
//        for(i = 0; i < FRAMEW + 1; ++i)
//            frame[0][i] = Border;
     
     /* frame[FRAMEH+1][FRAMEW+1]
        FRAMEH: 프레임의 높이, FRAMEW: 프레임의 넓이
        아래의 반복문에서
        FRAMEH = current.x + shpaes[current.num][current.pos][i][o]
        프레임의 높이는 current.x 값에 현재 출력되고 있는 블록의 값을 더하는 것.
	current.num은 0~6까지 무작위 정수. 
        (ex) currnet.num = 1, current.pos = 2 이면 1자형 막대이다.
	
	본 함수에서는 frame[x][y] 배열에 값을 채워넣는다.
	EXP_FACT = 2이고, 이 값은 frame의 열에 곱해진다.
	정해진 frame[x][y] 위치에 current.num + 1의 값을 저장한다.
	current.y = (FRAMEW / 2) - 1 = 24, #define FRAMEW (int)(10*2.3)
     */
     for(i = 0; i < 5; ++i)
          for(j = 0; j < EXP_FACT; ++j)
               frame[current.x + shapes[current.num][current.pos][i][0]] /* 1 + 1 */
                    [current.y + shapes[current.num][current.pos][i][1] * EXP_FACT + j] /* 24 + 0 * 2 + 0 */
                    = current.num + 1; /* frame[0~5][0~54] = 0~6 + 1 */
    printf("%d %d %d %d", current.x, current.y, current.num, current.pos);
     /* 
	Current.num은 현재 블록의 모양을 정의, +1을 해주면 다음 블록 모양
	Currnet.pos는 현재 블록의 포지션을 정의, 0~3까지의 숫자로 상하좌우를 표현
	Current.x, Current.y는 정확한 역할을 모르겠음...
        Current.x값을 지우면 블록이 아래로 내려가지 않음.
	Current.y값을 지우면 블록이 아래로 내려가지만, 잔상이 남는다.
	EXP_FACT값을 제외하면 블록이 내려가지않고 게임이 끝남.
	j값을 제외하면 블록의 모양이 변형됨.
     */
	
     /* 
	current.x의 값이 1보다 작으면
	배열 frame의 모든 첫번째 행에 Border라는 값을 저장한다.
	Border는 색의 값을 저장하는 변수이다.
     */

     if(current.x < 1)
          for(i = 0; i < FRAMEW + 1; ++i)
               frame[0][i] = Border;

     //pthread_mutex_unlock(&callFunc);
     return;
}

void
shape_unset(void)
{
    //printf("shape unset\n");
     /* 이미 Set되어있는 Shapes를 제거하는 함수
	(ex) 블록을 모두 채워서 한개의 줄이 사라져야하는 경우

	배열 frame의 모든 속성값을 0으로 설정하여준다.
	마찬가지로 current.x값이 1보다 작으면 프레임 보더로 인식한다.
     */
     //pthread_mutex_lock(&callFunc);
     int i, j;

     for(i = 0; i < 5; ++i)
          for(j = 0; j < EXP_FACT; ++j)
               frame[current.x + shapes[current.num][current.pos][i][0]]
                    [current.y + shapes[current.num][current.pos][i][1] * EXP_FACT + j] = 0;
     // erase it

     if(current.x < 1)
          for(i = 0; i < FRAMEW + 1; ++i)
               frame[0][i] = Border;
     //pthread_mutex_unlock(&callFunc);
     return;
}

void
shape_new(void)
{
     int i;

     /* Draw the previous shape for it stay there */
    shape_set_unset(1);

     check_plain_line();

     /* Set the new shape property */
     /*
        next 값을 num으로 넘겨 다음 Shape를 설정한다.
        새로운 Shape를 만드는 함수이기 때문에 current.x값을 1로 설정해준다.
        다음 next 값은 0~7 사이의 무작위 정수로 설정된다.
     */
     current.num = current.next;
     current.x = 1; // 새로운 블록이 생성되는 위치
     current.y = (FRAMEW / 2) - 1;
     current.next = nrand(0, 9);
     // current.next = nrand(0, 8);을 하면 투명 블록이 생성된다.
     /* 다음 블록을 표시해주는 프레임을 초기화시킨다 */
     frame_nextbox_refresh();
     /* 
	current.x 값이 1보다 크면 
	배열 frame의 두번째 행의 모든 값을 0으로 지정한다
     */        
     if(current.x > 1)
          for(i = 2; i < FRAMEW - 1; ++i)
               frame[1][i] = 0;

     return;
}

void 
block_down() 
{
        /*
        현재까지 있는 블럭을 모두 한칸 위로 올린 후 
        맨 아랫줄에 랜덤으로 한칸이 비어있는 블럭 
        한줄을 생성해준다. 
        */
         int ranNum = nrand(2,21);
                for(int i = 1; i < FRAMEH ; ++i) //이거는 블럭이 있는지 찾아서
                {                                //있으면 한줄을 올려준다.
                        for(int j = 2; j <FRAMEW - 1 ; ++j)
                        {
                        if(frame[i][j]!=0&& i>1)
                              {
                                        frame[i-1][j] = 3;  
                                        frame[i][j] = 0;
                               }
                        }
                        
                } 
                   for(int j = 2; j <FRAMEW - 1 ; ++j) //맨 아랫줄을 모두 채워준다.
                        {              
                                frame[19][j] = 3;
                        }
                        
                if(ranNum%2 == 1) 
                 {
                         /*
                         맨 아랫줄 중 랜덤으로 하나를 비우게 한다.
                         블럭이 2칸 기준이므로 홀수 짝수 나눠서 해준다.
                         */
                         frame[19][ranNum] = 0;
                         frame[19][ranNum-1] = 0;
                 }
                 else
                 {
                         frame[19][ranNum] = 0;
                         frame[19][ranNum+1] = 0;
                 }
} 

void
revive(void) //테트리스 안에 모든 블럭을 초기화 해주는 함수
{
        for(int i = 1; i < FRAMEH ; ++i){
          for(int j = 2; j <FRAMEW  -1 ; ++j){
                frame[i][j] = 0;
          }
        }

        
 
        lifes --;
}

void
shape_go_down(void)
{

    pthread_mutex_lock(&locInfo);
    shape_set_unset(0);

     /* Fall the shape else; collision with the ground or another shape
      * then stop it and create another */

     /* 
	생성된 Shapes를 아래로 이동시키는 함수이다.
	우선 현재 Shape의 위치를 체크하여 이동이 가능한지 파악하고, 가능하면 current.x 값을 1 증가.
	이동이 불가능한 상태라면 이동을 중지하고 새로운 Shape를 생성한다.
     */
     if(!check_possible_pos(current.x + 1, current.y))
          {
                  ++current.x;
          }
     else
          if(current.x > 2){
         //printf("semi done\n");
              shape_new();
     }
          /*
		current.x값이 1이거나 그보다 작다면, Shape가 이동하지 않은 것
		따라서 게임이 종료된 것으로 간주된다.
	  */
          else
          {
              printf("shapegodownRUNNING FALSE\n");
               shape_new();
               frame_refresh();
               sleep(2);
               running = False;
          }



          pthread_mutex_unlock(&locInfo);
          //shape_set();
     return;
}

void
shape_set_position(int p)
{
     /*
	현재의 위치를 Old라는 변수에 저장한다 (Temporary)
     */
     int old = current.pos;

     shape_set_unset(0);

     /*
	P값을 현재의 위치로 설정한다
     */
     current.pos = p ;

     if(check_possible_pos(current.x, current.y))
          current.pos = old;

     return;
}


void
shape_move(int n)
{

     shape_set_unset(0);

     if(!check_possible_pos(current.x, current.y + n))
          current.y += n;

     return;
}

void
shape_drop(void)
{
        if(SDL_Init(SDL_INIT_AUDIO) < 0)
    {
       exit;
    }
    
     while(!check_possible_pos(current.x + 1, current.y))
     {
          //shape_set_unset(0);
          ++current.x;
     }
     score += (FRAMEH - current.x)*2;
sound("slip.wav",300);
     return;
}
