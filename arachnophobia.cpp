#include<iostream>
#include<cmath>
#include<GL/glut.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<string>
#include<stdlib.h>
#include<SDL/SDL.h>
#include<SDL/SDL_mixer.h>
#include<pthread.h>
#define num_spi 10
#define PI 3.14159265
using namespace std;
int windowWidth;
int windowHeight;

int paused;


int speed_det=6000;


float box_len = 4.0f;

int score;
int can_rot;
float mouse_rot_x;
float mouse_rot_y;


clock_t t1,t2,t3,t4,t5,t6;
float xc1 = -0.1f;
float xc2 = 0.1f;
float yc1 = 1.5f;
float yc2 = 1.7f;
float vel = 0.01f;
float cannonx2=0.25f;
float cannonx1=-0.05f;

float gunx1=0.2f;
float gunx2=0.0f;
float gunx3=0.1f;


float laserx1;
float laserx2;
float lasery1;
float lasery2;

float red_x2=-1.15f;
float red_x1=-1.45f;
float green_x1=1.15f;
float green_x2=1.45f;

int fire;

int gameover;

float spidersx1[num_spi];
float spidersx2[num_spi];
float spidersy1[num_spi];
float spidersy2[num_spi];
int spiderscol[num_spi];
float spidersvel[num_spi];
int shot[num_spi];
float laser_vel=0.08f;
pthread_t p1,p2;
char chosen;
float theta = 0.0f;

typedef struct block
{
	int blocked;
	int col;
}block;

block blocked[num_spi];

typedef struct laser
{
	float gx1;
	float gx2;
	float laserx1;
	float laserx2;
	float lasery1;
	float lasery2;

	int fire;
	float theta;
	struct laser *next;
}laser;

laser *beam;

int num_beam;


void update(int value)
{
	
	
	if(paused!=1 && gameover!=1)
	{
		t6=clock();
		if((t6-t5)/CLOCKS_PER_SEC > 3 && speed_det >=500)
		{

			speed_det-=1000;
			t5=clock();
		}
		int i;

		laser * temp=beam;

		while(temp->next!=NULL)
		{
			if(temp->next->fire==1)
			{
				temp->next->lasery1+=cos(temp->next->theta)*laser_vel;
				temp->next->lasery2+=cos(temp->next->theta)*laser_vel;
				temp->next->laserx1+=sin(temp->next->theta)*laser_vel;
				temp->next->laserx2+=sin(temp->next->theta)*laser_vel;
				if(temp->next->lasery2>=box_len/2)
				{
					temp->next->fire=0;

				}

				if(temp->next->laserx2 >= box_len/2 || temp->next->laserx2 <= -1*box_len/2)
				{
					float tempi=temp->next->laserx1;
					temp->next->laserx1=temp->next->laserx2;
					temp->next->laserx2=tempi;

					temp->next->theta*=-1;

				}


				int m;

				for(m=0;m<num_spi;m++)
				{

					if(temp->next->laserx2 >=spidersx1[m] && temp->next->laserx2<=spidersx2[m])
					{
						if(temp->next->lasery2 >= spidersy1[m] && temp->next->lasery2 <= spidersy2[m])
						{
							system("aplay blast1.wav &");
							if(spiderscol[m]==0)
								score++;
							else
								score--;
							int color=rand()%3;
							spiderscol[m]=color;
							spidersy1[m]=box_len/2;
							spidersy2[m]=box_len/2+0.2;
							do
							{
								spidersvel[m]= float(rand()%5)/1000;
							}while(spidersvel[m]==0);
							//spidersy1[m]=-1*box_len/2-1.0f;
							//spidersy2[m]=-1*box_len/2-1.3f;
							temp->next->fire=0;
						}
					}
				}		

			}
			temp=temp->next;
		}

		for(i=0;i<num_spi;i++)
		{
			spidersy1[i]-=spidersvel[i];
			spidersy2[i]-=spidersvel[i];

			if(spidersy1[i]<=-1*box_len/2)
			{
				if(spiderscol[i]==1)
				{
					if(spidersx2[i]<red_x2 && spidersx1[i]>red_x1)
					{
						score++;
					}


					else if(spidersx2[i]<green_x2 && spidersx1[i]>green_x1)
						score--;

					else if((spidersx2[i]<cannonx2 && spidersx1[i]>cannonx1) || (spidersx1[i]<cannonx1 && spidersx2[i]>cannonx1) || (spidersx1[i]<cannonx2 && spidersx2[i]>cannonx2) )
					{
						gameover=1;
						cout<<"gameover"<<endl;
					}

					else
					{
						if(blocked[i].blocked!=1)
							score-=5;

						blocked[i].blocked=1;
						blocked[i].col=1;
					}
				}


				else if(spiderscol[i]==2)
				{
					if(spidersx2[i]<green_x2 && spidersx1[i]>green_x1)
					{
						score++;
					}


					else if(spidersx2[i]<red_x2 && spidersx1[i]>red_x1)
					{
						score--;
					}

					else if((spidersx2[i]<cannonx2 && spidersx1[i]>cannonx1) || (spidersx1[i]<cannonx1 && spidersx2[i]>cannonx1) || (spidersx1[i]<cannonx2 && spidersx2[i]>cannonx2) )
					{
						gameover=1;
						cout<<"gameover"<<endl;
					}


					else
					{
						blocked[i].blocked=1;
						blocked[i].col=2;
					}
				}


				else
				{
					if((spidersx2[i]<cannonx2 && spidersx1[i]>cannonx1) || (spidersx1[i]<cannonx1 && spidersx2[i]>cannonx1) || (spidersx1[i]<cannonx2 && spidersx2[i]>cannonx2) )
					{
						gameover=1;
						cout<<"gameover"<<endl;
					}

					if((spidersx2[i]<red_x2 && spidersx1[i]>red_x1) || (spidersx2[i]<green_x2 && spidersx1[i]>green_x1))
					{
						score--;
					}


					else
					{
						blocked[i].blocked=1;
						blocked[i].col=0;
					}
				}

				int color=rand()%3;
				spiderscol[i]=color;
				spidersy1[i]=box_len/2;
				spidersy2[i]=box_len/2+0.2;
				do
				{
					spidersvel[i]= float(rand()%5)/speed_det;
				}while(spidersvel[i]==0);
				//xc1= float(rand()%300)/100 - 1.5f;
				//xc2=xc1+0.2;

			}


		}

	}
	glutTimerFunc(5, update,0);
}


void drawTriangle() 
{
	glBegin(GL_TRIANGLES);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(gunx1, -1*box_len/2 - 0.01f , 0.0f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(gunx2, -1*box_len/2 - 0.01f, 0.0f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(gunx3, -1*box_len/2 + 0.3f , 0.0f);
	glEnd();


	glLineWidth(3.5);

	glBegin(GL_LINES);
	glColor3f(10.0f, 0.0f, 0.0f);
	glVertex3f(gunx3, -1*box_len/2 + 0.3f , 0.0f);
	glVertex3f((gunx1+gunx2+0.01)/2, -1*box_len/2 - 0.01f , 0.0f);
	glEnd();

	glLineWidth(1);
}



void drawBox(float len) {

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glVertex2f(-len / 2, -len / 2);
	glVertex2f(len / 2, -len / 2);
	glVertex2f(len / 2, len / 2);
	glVertex2f(-len / 2, len / 2);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glVertex2f(len / 2, -len / 2);
	glVertex2f(-len / 2, -len / 2);
	glVertex2f(-len / 2, (-len / 2)-0.4f);
	glVertex2f(len / 2, (-len / 2)-0.4f);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


}


void drawLines() {
	glColor3f(10.0f, 0.0f, 0.0f);
	glPointSize(3.0);  

	laser *temp=beam;
	while(temp->next!=NULL)
	{
		if(temp->next->fire==1)
		{
			glLineWidth(3.5);
			glBegin(GL_LINES);
			glVertex2d(temp->next->laserx1 , temp->next->lasery1);
			glVertex2d(temp->next->laserx2 , temp->next->lasery2);
			glEnd();
			glLineWidth(1);
		}
		temp=temp->next;
	}
} 





void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();

	// Draw Box
	glTranslatef(0.0f, 0.0f, -5.8f);
	glColor3f(1.0f, 0.0f, 0.0f);
	drawBox(box_len);

	if(paused!=1)
		t2=clock();

	if(float(t2-t1)/CLOCKS_PER_SEC>=0.1)
		glColor3f(0.0f, 0.0f, 1.0f);
	else
		glColor3f(0.0f, 1.0f, 1.0f);




	glPushMatrix();
	glTranslatef((cannonx1+cannonx2)/2 , -1*box_len/2,0.0f);
	glRotatef(theta, 0 , 0, -1.0f);
	glTranslatef(-1*(cannonx1+cannonx2)/2 , box_len/2,0.0f);

	glRectf(cannonx1,-1*box_len/2+0.1f, cannonx2, -1*box_len/2-0.1f);
	drawTriangle();


	glLineWidth(12);
	glBegin(GL_LINES);
	glVertex2d(cannonx1+0.05f,-1*box_len/2-0.1f);
	glVertex2d(cannonx1+0.05f, -1*box_len/2-0.2f);
	glVertex2d(cannonx2-0.05f,-1*box_len/2-0.1f);
	glVertex2d(cannonx2-0.05f, -1*box_len/2-0.2f);
	glEnd();
	glLineWidth(1);


	if(chosen=='b')
	{
		glColor3f(0.0f, 0.0f, 0.0f);

		glLineWidth(3);
		glBegin(GL_LINES);

		glVertex2d(cannonx1,-1*box_len/2+0.1f);
		glVertex2d(cannonx1, -1*box_len/2-0.1f);

		glVertex2d(cannonx2, -1*box_len/2-0.1f);
		glVertex2d(cannonx2,-1*box_len/2+0.1f);

		glVertex2d(cannonx1,-1*box_len/2-0.1f);
		glVertex2d(cannonx2, -1*box_len/2-0.1f);

		glVertex2d(cannonx1,-1*box_len/2+0.1f);
		glVertex2d(cannonx2, -1*box_len/2+0.1f);



		glEnd();
		glLineWidth(1);
	}



	glPopMatrix();

	glPushMatrix();
	drawLines();	
	glPopMatrix();




	glPushMatrix();

	glColor3f(1.0f, 1.0f, 1.0f);
	glRectf(red_x1,-1*box_len/2+0.1f, red_x2, -1*box_len/2-0.1f);


	glPushMatrix();	
	glPolygonMode(GL_BACK, GL_LINE);
	if(chosen=='r')
	glColor3f(1.0f, 0.0f, 0.0f);
	
	else
	glColor3f(0.2f, 0.0f, 0.0f);

	glRotatef(90,1.0f,0,0);

	glTranslatef((red_x1+red_x2)/2 , 0, box_len/2 - 0.1f);

	GLUquadricObj *quadratic;
	quadratic=gluNewQuadric();
	
	gluCylinder(quadratic,0.2f,0.15f,0.3f,10,10);

	glTranslatef(-1*(red_x1+red_x2)/2 , 0, -1*box_len/2 + 0.1f );
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPopMatrix();


	glPopMatrix();


	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glRectf(green_x1,-1*box_len/2+0.1f, green_x2, -1*box_len/2-0.1f);

	glPushMatrix();	
	glPolygonMode(GL_BACK, GL_LINE);
	if(chosen=='g')
	glColor3f(0.0f, 1.0f, 0.0f);
	
	else
	glColor3f(0.0f, 0.2f, 0.0f);
	
	
	glRotatef(90,1.0f,0,0);

	glTranslatef((green_x1+green_x2)/2 , 0, box_len/2 - 0.1f);

	quadratic=gluNewQuadric();
	gluCylinder(quadratic,0.2f,0.15f,0.3f,12,12);

	glTranslatef(-1*(green_x1+green_x2)/2 , 0, -1*box_len/2 + 0.1f );
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPopMatrix();


	glPopMatrix();


	int i;
	for(i=0;i<num_spi;i++)
	{


		glColor3f(1.0f, 1.0f, 1.0f);
		//glRectf(spidersx1[i],spidersy1[i],spidersx2[i],spidersy2[i]);

		if(spiderscol[i]==0)
			glColor3f(0.0f, 0.0f, 0.0f);

		if(spiderscol[i]==1)
			glColor3f(1.0f, 0.0f, 0.0f);

		if(spiderscol[i]==2)
			glColor3f(0.0f, 1.0f, 0.0f);


		glRectf(spidersx1[i]+0.05f,spidersy1[i]+0.05f,spidersx2[i]-0.05f,spidersy2[i]-0.05f);

		float cx=(spidersx1[i]+spidersx2[i])/2;
		float cy=(spidersy1[i]+spidersy2[i])/2;

		float cy1=(spidersy1[i]-spidersy2[i])/3;

		glBegin(GL_LINES);
		glVertex2d(spidersx1[i] , spidersy1[i]);
		glVertex2d( cx,cy);
		glVertex2d(spidersx1[i] , spidersy2[i]);
		glVertex2d( cx,cy);
		glVertex2d(spidersx2[i] , spidersy1[i]);
		glVertex2d( cx,cy);
		glVertex2d(spidersx2[i] , spidersy2[i]);
		glVertex2d( cx,cy);
		glVertex2d(spidersx1[i],spidersy1[i]-cy1);
		glVertex2d( cx,cy);
		glVertex2d(spidersx1[i],spidersy1[i]-2*cy1);
		glVertex2d( cx,cy);
		glVertex2d( spidersx2[i],spidersy1[i]-cy1);
		glVertex2d(cx,cy);
		glVertex2d(spidersx2[i],spidersy1[i]-2*cy1);
		glVertex2d( cx,cy);
		glEnd();


		if(blocked[i].blocked==1)
		{
			if(blocked[i].col==0)
			{
				glColor3f(0.0f, 0.0f, 0.0f);
				glRectf(spidersx1[i],-1*box_len/2,spidersx2[i],-1*box_len/2+0.2f);
			}

			else if(blocked[i].col==1)
			{
				glColor3f(1.0f, 0.0f, 0.0f);
				glRectf(spidersx1[i],-1*box_len/2,spidersx2[i],-1*box_len/2+0.2f);
			}

			else if(blocked[i].col==2)
			{
				glColor3f(0.0f, 1.0f, 0.0f);
				glRectf(spidersx1[i],-1*box_len/2,spidersx2[i],-1*box_len/2+0.2f);
			}
		}


	}



	glColor3f(0.0f, 0.0f, 0.0f);

	glRasterPos2f(box_len/2, box_len/2 - 0.2f);	
	char scre[20]="score :";
	char num[20];

	snprintf(num,100,"%d",score);	
	strcat(scre,num);

	int len=strlen(scre);

	for(i=0;i<len;i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, scre[i]);






	//	if(fire==1)
	//glRectf(xc1,yc1,xc2,yc2);

	if(gameover==1)
	{
		glColor3f(0.0f, 0.0f, 0.0f);
		glRasterPos2f(-0.6f, 0);	
		char go[20]="GAMEOVER :(";

		int l=strlen(go);

		for(i=0;i<l;i++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, go[i]);
	}
	glPopMatrix();




	glutSwapBuffers();
}



void initRendering() {

	glEnable(GL_DEPTH_TEST);        // Enable objects to be drawn ahead/behind one another
	glEnable(GL_COLOR_MATERIAL);    // Enable coloring
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);   // Setting a background color
}




void handleResize(int w, int h) {

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)w / (float)h, 0.1f, 200.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void *play(void *arg)
{

				if (SDL_Init(SDL_INIT_AUDIO) != 0)
				{
					std::cerr << "SDL_Init ERROR: " << SDL_GetError() << std::endl;
				}

				// Open Audio device
				if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048) != 0)
				{
					std::cerr << "Mix_OpenAudio ERROR: " << Mix_GetError() << std::endl;
				}

				// Set Volume
				Mix_VolumeMusic(100);

				// Open Audio File
				Mix_Music* music = Mix_LoadMUS("laser.mp3");

				if (music)
				{
					// Start Playback
					if (Mix_PlayMusic(music, 1) == 0)
					{

						// Wait
						while (Mix_PlayingMusic())
						{
							SDL_Delay(10);
						}
					}
					else
					{
						std::cerr << "Mix_PlayMusic ERROR: " << Mix_GetError() << std::endl;
					}

					// Free File
					Mix_FreeMusic(music);
					music = 0;
				}
				else
				{
					std::cerr << "Mix_LoadMuS ERROR: " << Mix_GetError() << std::endl;
     			        }
		           
					          // End
			         Mix_CloseAudio();
				 pthread_exit(NULL); 
				 exit(0);

}





void handleKeypress1(unsigned char key, int x, int y) {

	if (key == 'q') {
		exit(0);     // escape key is pressed
	}

	if(paused!=1)
	{
		if(key == 114)
			chosen='r';
		if(key == 103)
			chosen='g';
		if(key == 98)
			chosen='b';
		if(key == ' ' && chosen=='b')
		{
			t2=clock();

			//cout<<float(t2-t1)/CLOCKS_PER_SEC<<endl;
			if(float(t2-t1)/CLOCKS_PER_SEC >= 0.1 )
			{
				
				if(beam->next!=NULL)
				pthread_join(p1,NULL);
				
				fire=1;

				laser *temp=beam;

				while(temp->next!=NULL)
				{
					temp=temp->next;
				}

				temp->next=new laser;

				temp->next->laserx1=temp->next->gx1=gunx3;
				temp->next->laserx2=temp->next->gx2=gunx3 + 0.4 * sin(theta * PI / 180);
				temp->next->lasery1=-1*box_len/2 ;
				temp->next->lasery2=-1*box_len/2  + 0.4 * cos(theta * PI / 180) ;
				temp->next->fire=1;
				temp->next->theta=theta * PI / 180;
				temp->next->next=NULL;

				laserx1=laserx2=gunx3;
				lasery1=-1*box_len/2+0.1f;
				lasery2=-1*box_len/2+0.5f;
				t1=clock();

				
				pthread_create(&p1, NULL, play, NULL);
	

			}



		}
	}

	if(key == 'p' && paused==0)
	{
		t3=clock();
		paused=1;
	}

	else if(key=='p' && paused==1)
	{
		t4=clock();
		t1+=t4-t3;
		paused=0;
	}


}


void handleKeypress2(int key, int x, int y) 
{

	if(paused!=1 && gameover!=1)
	{
		if(chosen == 'b')
		{
			if (key == GLUT_KEY_LEFT && cannonx1>-1*box_len/2)
			{


				int i;
				int chk=0;
				for(i=0;i<num_spi;i++)
				{
					if(spidersx1[i]>(cannonx1-0.2f) && spidersx2[i]<(cannonx2-0.2f)) 	
					{
						chk=1;
						if(blocked[i].blocked!=1)
						{
							cannonx1-=0.2f;        
							cannonx2-=0.2f;
							gunx1-=0.2f;
							gunx2-=0.2f;
							gunx3-=0.2f;
						}
						break;
					}
				}

				if(chk==0)
				{
					cannonx1-=0.2f;        
					cannonx2-=0.2f;
					gunx1-=0.2f;
					gunx2-=0.2f;
					gunx3-=0.2f;
				}

			}
			if (key == GLUT_KEY_RIGHT && cannonx2<box_len/2)
			{
				int i;
				int chk=0;
				for(i=0;i<num_spi;i++)
				{
					if(spidersx1[i]>(cannonx1+0.2f) && spidersx2[i]<(cannonx2+0.2f)) 	
					{
						chk=1;
						if(blocked[i].blocked!=1)
						{
							cannonx1+=0.2f;        
							cannonx2+=0.2f;
							gunx1+=0.2f;
							gunx2+=0.2f;
							gunx3+=0.2f;
						}
						break;
					}
				}

				if(chk==0)
				{


					cannonx1+=0.2f;        
					cannonx2+=0.2f;	
					gunx1+=0.2f;
					gunx2+=0.2f;
					gunx3+=0.2f;
				}
			}

			if(key == GLUT_KEY_UP && theta<60)
				theta+=5;
			if(key == GLUT_KEY_DOWN && theta>-60)
				theta-=5;


		}
		if(chosen == 'r')
		{
			if (key == GLUT_KEY_LEFT && red_x1>-1*box_len/2)
			{
				int i;
				int chk=0;
				for(i=0;i<num_spi;i++)
				{
					if(spidersx1[i]>(red_x1-0.2f) && spidersx2[i]<(red_x2-0.2f)) 	
					{
						chk=1;
						if(blocked[i].blocked!=1)
						{
							red_x1-=0.2f;        
							red_x2-=0.2f;	
						}
						break;
					}
				}

				if(chk==0)
				{
					red_x1-=0.2f;        
					red_x2-=0.2f;	
				}
			}
			if (key == GLUT_KEY_RIGHT && red_x2<box_len/2)
			{
				int i;
				int chk=0;
				for(i=0;i<num_spi;i++)
				{
					if(spidersx1[i]>(red_x1+0.2f) && spidersx2[i]<(red_x2+0.2f)) 	
					{
						chk=1;
						if(blocked[i].blocked!=1)
						{
							red_x1+=0.2f;        
							red_x2+=0.2f;	
						}
						break;
					}
				}

				if(chk==0)
				{
					red_x1+=0.2f;        
					red_x2+=0.2f;	
				}
			}
		}
		if(chosen == 'g')
		{
			if (key == GLUT_KEY_LEFT && green_x1>-1*box_len/2)
			{
				int i;
				int chk=0;
				for(i=0;i<num_spi;i++)
				{
					if(spidersx1[i]>(green_x1-0.2f) && spidersx2[i]<(green_x2-0.2f)) 	
					{
						chk=1;
						if(blocked[i].blocked!=1)
						{
							green_x1-=0.2f;        
							green_x2-=0.2f;	
						}
						break;
					}
				}

				if(chk==0)
				{
					green_x1-=0.2f;        
					green_x2-=0.2f;	
				}
			}
			if (key == GLUT_KEY_RIGHT && green_x2<box_len/2)
			{
				int i;
				int chk=0;
				for(i=0;i<num_spi;i++)
				{
					if(spidersx1[i]>(green_x1+0.2f) && spidersx2[i]<(green_x2+0.2f)) 	
					{
						chk=1;
						if(blocked[i].blocked!=1)
						{
							green_x1+=0.2f;        
							green_x2+=0.2f;	
						}
						break;
					}
				}

				if(chk==0)
				{
					green_x1+=0.2f;        
					green_x2+=0.2f;
				}
			}
		}
		//if (key == GLUT_KEY_UP)
		//if (key == GLUT_KEY_UP)
		//if (key == GLUT_KEY_UP)
		//if (key == GLUT_KEY_DOWN)
	}
}
void handleMouseclick(int button, int state, int x, int y) 
{

	if(paused!=1 && gameover!=1)
	{
	if (state == GLUT_DOWN)
	{
		float x1=x/float(windowWidth) - 0.5;


		if(button == GLUT_LEFT_BUTTON)
		{
			if (x1*9 <= red_x2 && x1*9 >=red_x1)
				chosen='r';

			if (x1*9 <= green_x2 && x1*9 >= green_x1)
				chosen='g';

			if (x1*9 <= cannonx2 && x1*9 >=cannonx1)
				chosen='b';
		}

		if (button == GLUT_RIGHT_BUTTON && x1*9<=cannonx2 && x1*9>=cannonx1)
		{
			chosen='b';
			can_rot=1;			
		}

		if (button == GLUT_RIGHT_BUTTON && chosen=='b')
		{
			can_rot=1;
		}


	}

	if (state == GLUT_UP)
	{
		if (button == GLUT_RIGHT_BUTTON)
		{
			can_rot=0;			
		}



		if(button == GLUT_LEFT_BUTTON)
		{
			float x1=x/float(windowWidth) - 0.5;
			if(chosen=='r')
			{
				if(x1 * 9 < red_x1)
				{
					while(red_x1 > x1*9 && red_x1>-1*box_len/2)
					{
						int chk=0;
						int i;
						for(i=0;i<num_spi;i++)
						{
							if(spidersx1[i]>(red_x1-0.2f) && spidersx2[i]<(red_x2-0.2f)) 	
							{
								if(blocked[i].blocked==1)
									chk=1;
								break;
							}
						}
					if(chk==1)
						break;
						

						red_x1-=0.2f;
						red_x2-=0.2f;
					}
				}

				else if(x1*9 > red_x2)
				{
					while(red_x2<x1*9 && red_x2 < box_len/2)
					{
						int chk=0;
						int i;
						for(i=0;i<num_spi;i++)
						{
							if(spidersx1[i]>(red_x1+0.2f) && spidersx2[i]<(red_x2+0.2f)) 	
							{
								if(blocked[i].blocked==1)
									chk=1;
								break;
							}
						}
					if(chk==1)
						break;
						red_x2+=0.2f;
						red_x1+=0.2f;
					}
				}

			}
			if(chosen=='g')
			{
				if(x1 * 9 < green_x1)
				{
					while(green_x1 > x1*9 && green_x1>-1*box_len/2)
					{
						int chk=0;
						int i;
						for(i=0;i<num_spi;i++)
						{
							if(spidersx1[i]>(green_x1-0.2f) && spidersx2[i]<(green_x2-0.2f)) 	
							{
								if(blocked[i].blocked==1)
									chk=1;
								break;
							}
						}
					if(chk==1)
						break;
								green_x1-=0.2f;        
								green_x2-=0.2f;	
					}
				}

				else if(x1*9 > green_x2)
				{
					while(green_x2<x1*9 && green_x2 < box_len/2)
					{
						int chk=0;
						int i;
						for(i=0;i<num_spi;i++)
						{
							if(spidersx1[i]>(green_x1+0.2f) && spidersx2[i]<(green_x2+0.2f)) 	
							{
								if(blocked[i].blocked==1)
									chk=1;
								break;
							}
						}
					if(chk==1)
						break;
						
					green_x2+=0.2f;
					green_x1+=0.2f;
					}
				}
			}
			
			if(chosen=='b')
			{
				if(x1 * 9 < cannonx1)
				{
					while(cannonx1 > x1*9 && cannonx1>-1*box_len/2)
					{
						cannonx1-=0.2f;
						cannonx2-=0.2f;
						gunx1-=0.2f;
						gunx2-=0.2f;
						gunx3-=0.2f;
					}
				}

				else if(x1*9 > cannonx2)
				{
					while(cannonx2<x1*9 && cannonx2 < box_len/2)
					{
						cannonx2+=0.2f;
						cannonx1+=0.2f;
						gunx1+=0.2f;
						gunx2+=0.2f;
						gunx3+=0.2f;
					}
				}
			}
		}
	     
	     
	     }
	}

 }

GLdouble ox=0.0,oy=0.0,oz=0.0;
void handleMousedrag(int x, int y) 
{
	if(paused!=1 && gameover!=1)
	{
	if(can_rot==0)	
	{
	        handleMouseclick(GLUT_LEFT_BUTTON,GLUT_UP,x,y);
	}

	else
	{
		float x1=x/float(windowWidth) - 0.5;
		float y1=y/float(windowHeight)-0.5;
		
		theta=atan((x1*9-(cannonx1+cannonx2)/2)/((y1*-5)-(-1*box_len/2))) * 180 / PI;

	
	}
	}
}

int main(int argc, char **argv)
{
	t5=clock();
	t1=clock();
	int i;
	beam=new laser;
	beam->next=NULL;
	float c=-1*box_len/2;
	srand(time(NULL));
	for(i=0;i<num_spi;i++)
	{
		int color=rand()%3;
		spiderscol[i]=color;
		spidersx1[i]=c;
		c+=0.2f;
		spidersx2[i]=c;
		c+=0.2f;
		spidersy1[i]=box_len/2-0.5f;
		spidersy2[i]=box_len/2-0.3f;

		do
		{
		spidersvel[i]= float(rand()%6)/speed_det;
		}while(spidersvel[i]==0);

	}


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	 
	int w = glutGet(GLUT_SCREEN_WIDTH);
	int h = glutGet(GLUT_SCREEN_HEIGHT);
	windowWidth = w * 2 / 3;
        windowHeight = h * 2 / 3;
	 
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition((w - windowWidth) / 2, (h - windowHeight) / 2);
	 
	glutCreateWindow("Arachnophobia");  // Setup the window
	initRendering();
	 
	// Register callbacks
	
	glutDisplayFunc(drawScene);
	glutIdleFunc(drawScene);
	glutReshapeFunc(handleResize);
	glutSpecialFunc(handleKeypress2);
	glutTimerFunc(10, update, 0);
	glutKeyboardFunc(handleKeypress1);
        glutMouseFunc(handleMouseclick);
	glutMotionFunc(handleMousedrag);
	
	glutMainLoop();
	

	cout<<score<<endl;
	return 0;
}
