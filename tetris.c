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
//#include <unistd.h>

int increase = 0;


int whichThread[2] = { 0, 1 };
long speenOnLevel[5] = { 100000, 0.7, 0.5, 0.4, 0.3 };
/* Functions */

int first(char * myId, char * myPwd)
{
	int sock, sign;
	char buff[1000];
	struct sockaddr_in serv_addr;
	sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(buff, 0x00, sizeof(buff));
	memset(myId, 0x00, sizeof(myId));
	memset(myPwd, 0x00, sizeof(myPwd));
	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("13.209.29.192");
	serv_addr.sin_port = htons(3090);


	system("clear");
	printf("\n\n\t -------LOGIN--------\n\n");
	printf("\tID : ");
	scanf("%s", myId);

	printf("\tPW : ");
	scanf("%s", myPwd);
	printf("\n\n");

	while (1) {
		printf("\t[1]signup [2]signin : ");
		scanf("%d", &sign);

		if (sign == 1) {
			//for signup
			// serv_addr.sin_addr.s_addr = inet_addr("13.209.29.192");

			if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
				//printf("connect error");
			}

			sprintf(buff, "%c|%s|%s", '1', myId, myPwd);
			write(sock, buff, sizeof(buff)); // send socket to server
											 //printf("%s\n",buff);
			memset(buff, 0x00, sizeof(buff)); // empty buffer
			read(sock, buff, sizeof(buff)); // read socket from server
			printf("\tuser_idx : ");
			printf("%s\n", buff);
			if (buff != NULL) {
				printf("\t succes !");
			}
			break;

		}
		else if (sign == 2) {
			//for login
			if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
				//printf("connect error");

			sprintf(buff, "%c|%s|%s", '2', myId, myPwd);
			write(sock, buff, sizeof(buff)); // send socket to server
											 //printf("%s\n",buff);
			memset(buff, 0x00, sizeof(buff)); // empty buffer
			read(sock, buff, sizeof(buff)); // read socket from server

			printf("\tuser_idx : ");
			printf("%s\n", buff);

			if (buff != NULL) {
				printf("\t success !");
			}
			break;

		}
		else {
			printf("It is wrong");
		}
	}
	close(sock);
	user_idx = (atoi)(buff);
	return user_idx;
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
	term.c_lflag &= ~(ICANON | ECHO);
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
	sigaction(SIGINT, &siga, NULL);
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
	pthread_mutex_lock(&locInfo);
	/*main함수중에 전체 루프중에 필수적으로 거치는 함수이자 입력받은 값에따라 게임 진행이 된다.
	여기서 key_pause와 key_quit는 게임을 계속 진행하는 대에 영향을 준다*/
	switch (c)
	{
	case KEY_MOVE_LEFT:            shape_move(-EXP_FACT);              break;
	case KEY_MOVE_RIGHT:           shape_move(EXP_FACT);               break;

	case KEY_MOVE_DOWN:            ++current.x; break;
	case KEY_CHANGE_POSITION_NEXT: shape_set_position(N_POS);          break;
	case KEY_DROP_SHAPE:           shape_drop();                       break;
	case KEY_PAUSE:                while (getchar() != KEY_PAUSE);      break;
	case KEY_QUIT:                 running = False;                    break;
	case 'Q':                      running = False;                    break; //대문자 Q를 사용할 때 종료
	case 'r':                      if (lifes != 0) revive();             break;
		//case 't':                      sleep(5);                         break; //5초 정지 
		//시간 멈추는 능력 
	}
	pthread_mutex_unlock(&locInfo);
	return;
}
int prescore = 0;
int height = 0;

int prelevel = 0;
void
arrange_score(int l)
{
	prelevel = level;
	/* 클리어한 라인에따라 점수부여. 여기서 의문점이 5줄이상일때 에러가 발생하는지
	테트리스는 5줄이상 못깹니다.  */
	switch (l)
	{
	case 1: score += 10;   break; /* One line */
	case 2: score += 20;  break; /* Two lines */
	case 3: score += 30;  break; /* Three lines */
	case 4: score += 40; break; /* Four lines */
	}

	if (score >= prescore + 100 + (level - 1) * 50)
	{
		prescore = prescore + 100 + (level - 1) * 50;
		level += 1;
		if (prelevel < level)
		{
			lifes += 1;
			increase = 1;
			block_down();
			sound("nope.wav", 400);
		}
		else
		{
			increase = 0;
		}
	}
	else {
		increase = 0;
	}
	lines += l;


	DRAW_SCORE();

	return;
}

void arrange_score2(int height)
{
	prelevel = level;

	score += height * 0.5;

	if (score >= prescore + 100 + (level - 1) * 50)
	{
		prescore = prescore + 100 + (level - 1) * 50;
		level += 1;
		if (prelevel < level)
		{
			lifes += 1;
			increase = 1;
			block_down();
			sound("nope.wav", 400);
		}
		else
		{
			increase = 0;
		}
	}
	else {
		increase = 0;
	}
	DRAW_SCORE();
	return;
}

void arrange_score3(int check)
{
	prelevel = level;
	score += ((check) * 10);

	if (score >= prescore + 100 + (level - 1) * 50)
	{
		prescore = prescore + 100 + (level - 1) * 50;
		level += 1;
		if (prelevel < level)
		{
			lifes += 1;
			increase = 1;
			block_down();
			sound("nope.wav", 400);
		}
		else
		{
			increase = 0;
		}
	}
	else {
		increase = 0;
	}
	DRAW_SCORE();
	return;

}

void
check_plain_line(void)
{
	score += 3;
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		exit;
	}

	int check_line = 0;
	int i, j, k, f, c = 0, nl = 0;

	for (i = 1; i < FRAMEH; ++i)
	{
		for (j = 1; j < FRAMEW; ++j)
			if (frame[i][j] == 0)
			{
				++c;
			}

		if (!c)
		{
			arrange_score2(i + 1);
			++nl;
			++check_line;

			sound("pop.wav", 400);


			for (k = i - 1; k > 1; --k)
				for (f = 1; f < FRAMEW; ++f)
				{
					frame[k + 1][f] = frame[k][f];
				}
		}
		c = 0;
	}

	if (check_line != 0)
	{
		arrange_score3(check);

		check++;

	}
	else {
		check = 0;
	}

	arrange_score(nl);
	frame_refresh();

	return;
}

int
check_possible_pos(int x, int y)
{
	int i, j, c = 0;

	for (i = 0; i < 5; ++i)
		for (j = 0; j < EXP_FACT; ++j)
			if (frame[x + shapes[current.num][current.pos][i][0]]
				[y + shapes[current.num][current.pos][i][1] * EXP_FACT + j] != 0)
				++c;

	return c;
}

int previousScore() {
	int sock, previous_score;
	char buff[1000];
	struct sockaddr_in serv_addr;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(buff, 0x00, sizeof(buff));

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("13.209.29.192");
	serv_addr.sin_port = htons(3090);

	if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
		//printf("connect error");
	}
	//socre get
	sprintf(buff, "%c|%d", '5', user_idx);
	write(sock, buff, sizeof(buff));
	//printf("%s\n", buff);
	memset(buff, 0x00, sizeof(buff));
	read(sock, buff, sizeof(buff));
	//printf("%s\n", buff);
	//printf("current score : ");
	previous_score = (atoi)(buff);
	close(sock);
	return previous_score;
}

void ScoreUpdate()
{
	int sock;
	char buff[1000];
	struct sockaddr_in serv_addr;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(buff, 0x00, sizeof(buff));

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("13.209.29.192");
	serv_addr.sin_port = htons(3090);

	if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
		//  printf("connect error");
	}
	memset(buff, 0x00, sizeof(buff));
	sprintf(buff, "%c|%d|%d", '6', score, user_idx);
	write(sock, buff, sizeof(buff));
	//printf("%s\n", buff);
	memset(buff, 0x00, sizeof(buff));
	read(sock, buff, sizeof(buff));
	close(sock);
}

int rankGet()
{
	int sock, rank;
	char buff[1000];
	struct sockaddr_in serv_addr;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(buff, 0x00, sizeof(buff));

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("13.209.29.192");
	serv_addr.sin_port = htons(3090);

	if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
		//  printf("connect error");
	}
	//rank get
	memset(buff, 0x00, sizeof(buff));
	sprintf(buff, "%c|%d", '7', user_idx);
	write(sock, buff, sizeof(buff));
	//printf("%s\n", buff);
	memset(buff, 0x00, sizeof(buff));
	read(sock, buff, sizeof(buff));
	//printf("%s\n", buff);
	//printf("my Rank : ");
	rank = (atoi)(buff)+1;
	//printf("%c\n",buff[9]);
	// printf("%d\n", myRank);
	close(sock);
	return rank;
}


int firstScoreGet()
{
	int sock, first = 0;
	char buff[1000];
	struct sockaddr_in serv_addr;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(buff, 0x00, sizeof(buff));

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("13.209.29.192");
	serv_addr.sin_port = htons(3090);

	if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
		//printf("connect error");
	}

	memset(buff, 0x00, sizeof(buff));
	sprintf(buff, "%c|", '8');
	write(sock, buff, sizeof(buff));
	//printf("%s\n", buff);
	memset(buff, 0x00, sizeof(buff));
	read(sock, buff, sizeof(buff));
	//printf("%s\n", buff);
	first = (atoi)(buff);
	return first;
}


void saveLifes(int user_idx)
{
	int sock, item;
	struct sockaddr_in serv_addr;
	char buff[1000];

	sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(buff, 0x00, sizeof(buff));

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("13.209.29.192");
	serv_addr.sin_port = htons(3090);

	if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
		//printf("connect error");
	}


	memset(buff, 0x00, sizeof(buff));
	sprintf(buff, "%c|%d|%d", '4', lifes, user_idx);
	write(sock, buff, sizeof(buff));
	//printf("%s\n", buff);
	memset(buff, 0x00, sizeof(buff));
	read(sock, buff, sizeof(buff));
	//printf("%s\n",buff);
	item = (atoi)(buff);
	// printf("%d\n",myItem);
	close(sock);

}

void quit(char * name, int user_idx)
{
	int rank = 0;
	int firstScore = 0;
	char end;
	int previous_score = -1;

	frame_refresh(); /* Redraw a last time the frame */

	set_cursor(True); //이 함수로인해 터미널창 커서가 숨김에서 풀린다
	tcsetattr(0, TCSANOW, &back_attr); //TCSANOW는 즉시속성을 변경을 의미,

	previous_score = previousScore();
	if (score > previous_score) {

		ScoreUpdate();
		rank = rankGet();
		firstScore = firstScoreGet();

		if (rank != 1) {
			printf("\n\n\t축하합니다. %s님이 최고점수 %d점을 달성했습니다.\n", name, score);
			printf("\n\t당신은 현재 %d등 입니다. 1등의 %d점 도전해보세요! \n\n", rank, firstScore);
		}
		else {
			printf("\n\n\t축하합니다. %s님이 최고점수 %d점을 달성했습니다.\n", name, score);
			printf("\n\t당신은 현재 1등 입니다. 본인의 최고 점수를 갱신해보세요! \n\n");
		}
	}
	else {
		//rank get
		printf("\n\n\t실력이 많이 녹슬었군요. 과거의 %s님은 점수 %d점을 달성했습니다.\n\n", name, previous_score);
		printf("\n\t 한번 더 도전해서, 과거의 자신을 뛰어 넘어 보세요!\n");
	}
	//save the lifes (rare items)
	saveLifes(user_idx);
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

void music(const char * filename, int len) {


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

int getLifes(int user_idx)
{
	int sock, item;
	struct sockaddr_in serv_addr;
	char buff[1000];

	sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(buff, 0x00, sizeof(buff));



	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("13.209.29.192");
	serv_addr.sin_port = htons(3090);

	if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
		//   printf("connect error");
	}

	sprintf(buff, "%c|%d", '3', user_idx);
	write(sock, buff, sizeof(buff));
	//printf("%s\n", buff);
	memset(buff, 0x00, sizeof(buff));
	read(sock, buff, sizeof(buff));
	//printf("%s\n", buff);
	//printf("item number : ");
	item = (atoi)(buff);
	//printf("%c\n",buff[9]);
	//printf("%d\n", myItem);
	close(sock);
	return item;
}

void *inputThread(void *param) {  // 사용자의 입력을 받아들일 부분
								  //printf("producer thread got in\n");

	buffer_item item;

	while (running) {
		item = getchar();
		sem_wait(&empty);

		pthread_mutex_lock(&mutex);


		if (count != BUFFER_SIZE) {
			buffer[in] = item;
			in = (in + 1) % BUFFER_SIZE;
			count++;
		}

		pthread_mutex_unlock(&mutex);

		sem_post(&full);
	}
}
void *tetrominoShiftsThread(void *param) { // 사용자의 입력을 반영해 frame을 그릴 부분

	buffer_item output;

	while (running) {

		sem_wait(&full);

		pthread_mutex_lock(&mutex);

		if (count != 0) {
			output = buffer[out];
			out = (out + 1) % BUFFER_SIZE;
			count--;
			//lock
			shape_set_unset(0);
			get_key_event(output);
			if (increase != 1) {
				shape_set_unset(1);

			}
		}

		pthread_mutex_unlock(&mutex);

		sem_post(&empty);
	}
}

void delay(unsigned int sec) {
	clock_t ticks1 = clock();
	clock_t ticks2 = ticks1;
	while ((ticks2 / CLOCKS_PER_SEC - ticks1 / CLOCKS_PER_SEC)<(clock_t)sec)
		ticks2 = clock();
}

// shape set, unset FYCTION

void sound(const char * filename, int len) {
	initAudio();
	Audio * sound = createAudio(filename, 0, SDL_MIX_MAXVOLUME / 2);
	playSoundFromMemory(sound, SDL_MIX_MAXVOLUME);
	SDL_Delay(len);
	endAudio();
	freeAudio(sound);
}

int
main(int argc, char **argv)
{
	/*변수들*/
	level = 1;
	int n = 1;
	current.last_move = False;
	lines = 0;
	char myname[10];

	/*TODO sj
	* for thread*/
	pthread_t producer;
	pthread_t consumer;
	pthread_attr_t attr;

	/*TODO sj
	* initialize for buffer*/
	count = 0;
	in = 0;
	out = 0;

	sem_init(&empty, 0, BUFFER_SIZE);
	sem_init(&full, 0, 0);
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&callFunc, NULL);
	pthread_mutex_init(&locInfo, NULL);

	whichLevel = 0;  // initial level for speed

					 //for signup, loging
	char myId[100];
	char myPwd[100];
	int user_idx = -1;

	/* Initialize only SDL Audio on default device */
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		exit;
	}

	//    first(myname);
	//초기음악

	user_idx = first(myId, myPwd); //
	lifes = getLifes(user_idx); //

	sound("test.wav", 2000);
	init(); //게임 진행중에도 게임 사용법 보여
	frame_init();
	frame_nextbox_init();;
	//여기까지 게임을 초기화하는 부분

	/*TODO sj
	* create thread
	* */
	pthread_attr_init(&attr);
	pthread_create(&producer, &attr, inputThread, &whichThread[0]);
	pthread_create(&consumer, &attr, tetrominoShiftsThread, &whichThread[1]);

	while (running)
	{

		if (increase == 1)
		{
			printxy(0, FRAMEH_NB + 13, FRAMEW + 3, "***블록이 안보입니다***");

		}
		else
		{
			pthread_mutex_lock(&locInfo);

			shape_set_unset(1);
			frame_preview();
			pthread_mutex_unlock(&locInfo);

		}

		printxy(0, FRAMEH_NB + 13, FRAMEW + 3, "                                     ");
		//         else        //레벨 5가 되면 블록이 안보임                     //----------------------seungmin here score modified need!!
		//         {
		//             frame_refresh();
		//             frame_preview();
		//         }

		sleep(1000);
		shape_go_down();


	}	//이것이 게임루프의 주축이

	sound("violin.wav", 9000);
	SDL_Quit();

	quit(myId, user_idx);
	return 0;
}
