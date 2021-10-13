#include<string>
#include<vector>
#include<cstdlib>
#include<cstdio>
#include<ctime>
#include<curses.h>
using namespace std;

#define width 5
#define n 4
#define s_fail 0
#define s_win 1
#define s_normal 2
#define s_quit 3
#define target 1024
class game{
public:
	game():status(s_normal){
		restart();
	}

	int getstatus(){ return status;}
	
	void processinput()
	{
		char ch=getch();
		if (ch>='a' && ch<='z')
			ch-=32;
		bool updated=false;
		if (status==s_normal)
		{
			if (ch=='A')
			{
				updated=moveleft();
			}
			if (ch=='S')
			{
				rotate();
				rotate();
				rotate();
				updated=moveleft();
				rotate();
			}
			if (ch=='D')
			{
				rotate();
				rotate();
				updated=moveleft();
				rotate();
				rotate();
			}
			if (ch=='W')
			{
				rotate();
				updated=moveleft();
				rotate();
				rotate();
				rotate();
			}
			if (updated)
			{
				randnew();
				if (isover())
					status=s_fail;
			}
	
		}
		if (ch=='Q')
			status=s_quit;
		else if (ch=='R')
			restart();
	}
	void draw(){
		clear();
		const int offset=12;
		for (int i=0;i<=n;i++)
			for (int j=0;j<=n;j++)
			{
				drawitem(i*2,1+j*width+offset,'+');
				if (i!=n){
					drawitem(i*2+1,1+j*width+offset,'|');
				}
				for (int k=1;k<width&&j!=n;k++)
					drawitem(i*2,1+j*width+offset+k,'-');
				if (i!=n&&j!=n)
					drawnum(i*2+1,(j+1)*width+offset,data[i][j]);
			}
		mvprintw(2*n+2,(5*(n-4)-1)/2,"w(up),s(down),a(left),d(right),r(restart),q(quit)");
		if (status==s_win)
			mvprintw(n,5*n/2-1,"you win,press r to continue,press q to quit");
		if (status==s_fail)
			mvprintw(n,5*n/2-1,"you lose,press r to continue,press q to quit");
	}

	
	void setdata(){
		for (int i=0;i<n;i++)
			for (int j=0;j<n;j++)
			{
				data[i][j]=1<<i<<j;
			}
	}
private:
	
	bool isover()
	{
		for (int i=0;i<n;i++)
			for (int j=0;j<n-1;j++)
				if (data[i][j]*data[i][j+1]==0||data[i][j]==data[i][j+1])
					return 0;
		for (int i=0;i<n-1;i++)
			for (int j=0;j<n;j++)
				if (data[i][j]*data[i+1][j]==0||data[i][j]==data[i+1][j])
					return 0;
		return 1;
	}
	bool moveleft()
	{
		int tmp[n][n]={0};
		for (int i=0;i<n;i++)
		{
			int lastnum=0,currentpos=0;
			for (int j=0;j<n;j++)
			{
				tmp[i][j]=data[i][j];
				if (data[i][j]==0)
					continue;
				if (lastnum!=0 && data[i][j]==lastnum)
				{
					data[i][currentpos]=lastnum*2;
					if (lastnum*2==target)
						status=s_win;
					currentpos++;
					lastnum=0;
					data[i][j]=0;
				}
				else
				{
					if (lastnum!=0)
						data[i][currentpos]=lastnum,currentpos++;
					lastnum=data[i][j];
				}
			}
			if (lastnum!=0)
				data[i][currentpos]=lastnum;
			for (int j=currentpos+1;j<n;j++)
				data[i][j]=0;

		}
		for (int i=0;i<n;i++)
			for (int j=0;j<n;j++)
				if (tmp[i][j]!=data[i][j])
					return true;
		return false;
	}
	
	void rotate()
	{
		int tmp[n][n]={0};
		for (int i=0;i<n;i++)
			for (int j=0;j<n;j++)
				tmp[i][j]=data[j][n-i-1];
		for (int i=0;i<n;i++)
			for (int j=0;j<n;j++)
				data[i][j]=tmp[i][j];
	}

	void restart()
	{
		for (int i=0;i<n;i++)
			for (int j=0;j<n;j++)
				data[i][j]=0;
		randnew();
		randnew();
		status=s_normal;
	}
	bool randnew()
	{
		vector<int> empty;
		for (int i=0;i<n;i++)
			for (int j=0;j<n;j++)
				if (data[i][j]==0)
					empty.push_back(i*n+j);
		if (empty.size()==0)
			return false;

		int randpos=empty[rand()%empty.size()];
		data[randpos/n][randpos%n]=rand()%10==1?4:2;
		return true;
	}
	void drawitem(int x,int y,char c)
	{
		move(x,y);
		addch(c);
	}
	void drawnum(int x,int y,int c)
	{
		while (c>0)
		{
			drawitem(x,y,c%10+48);
			c/=10;
			y--;
		}
	}
private:
	int data[n][n];
	int status;

};



void initialize()
{
	initscr();//write without enter
	cbreak(); //hide keys
	noecho(); //hide cursor
	curs_set(0);
	srand(time(0));
}
void shutdown()
{
	endwin();
}
int main()
{
	initialize();

	
	game g;
	do {
		g.draw();
		g.processinput();
	} while(g.getstatus()!=s_quit);

	shutdown();

	return 0;
}
