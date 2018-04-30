/*
 *      util.c
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

#include <sys/time.h>

#include "tetris.h"
#include "config.h"

void
clear_term(void)	//스크린을 지우고 커서를  1,1로 옮김(위 왼쪽)
{
     puts("\e[2J");	

     return;
}

void
set_cursor(Bool b)	//Bool b가 l일 경우 hide cursor, h일 경우 showcursor
{
     printf("\e[?25%c", ((b) ? 'h' : 'l'));

     return;
}

void
set_color(int color)	//색을 지정
{
     int bg = 0, fg = 0;	

	 //bg : backgound, fg : foreground

     switch(color)	//각각의 색에 bg를 지정
     {
     default:
     case Black:   bg = 0;  break;
     case Blue:    bg = 44; break;
     case Red:     bg = 41; break;
     case Magenta: bg = 45; break;
     case White:   bg = 47; break;
     case Green:   bg = 42; break;
     case Cyan:    bg = 46; break;
     case Yellow:  bg = 43; break;
     case Test:    bg = 41; break;
     case Test_2:  bg = 41; break;
     case Test_3:  bg = 42; break;
     case Border:  bg = 47; break;
     case Score:   fg = 37; bg = 49; break;
     }

	 printf("\e[%d;%dm", fg, bg);

     return;
}

void
printxy(int color, int x, int y, char *str)	//
{
     set_color(color);	//색 지정
     printf("\e[%d;%dH%s", ++x, ++y, str);
     set_color(0);	//초기화

     return;
}

int 
nrand(int min, int max)	//min에서 max사이의 무작위 정수 설정
{
     return (rand() % (max - min + 1)) + min;
}

void
sig_handler(int sig)	//sig에 따라 지정
{
     switch(sig)
     {
     case SIGTERM:	//안전한 중지일 경우
     case SIGINT:	//종료일 경우
     case SIGSEGV:	//비정상종료일 경우
          running = False;	//running은 False
          break;
     case SIGALRM:	//서로 데이터를 받는지 아닌지 확인 할 수 있는 시그널 SIGALRM일 경우
          tv.it_value.tv_usec -= tv.it_value.tv_usec / 3000;	// 타이머가 만료될 때까지 남은 시간 저장. 이 값이 0 이면 타이머 기능을 멈추는 tv.it_value
          setitimer(0, &tv, NULL);	//타이머를 설정. ovalue 에는 NULL 또는 이전 설정값을 저장하는 setitimer
          break;
     }

     return;
}
