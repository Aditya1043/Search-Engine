#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
int t=0;
struct ofWords
{
	char wordd[100];
	int fnumber;
};
struct node *root=NULL;
struct hash
{
    struct node *first;
	int count;
};
struct fileno
{
	int count;
	int id;
	struct fileno *left; 
    struct fileno *right;
};
struct node	
{
    char word[100];
    struct node *left; 
    struct node *right;
    struct fileno *ref;
	int key;
};
char* getFileName(int i)
{
	char *p;
	p=(char*)malloc(100);
	char itostring[5];
    sprintf(itostring,"%d",i);
    char file[]="/temp";
    char f[]=".txt";
    strcpy(p,"/home/aditya/Desktop/seo");
    strcat(p,file);
    strcat(p,itostring);
    strcat(p,f);
	return p;
}
int getFileSize(char *file)
{
	struct stat st;
    stat(file,&st);
    return st.st_size;
}
int findWord(char* html,char* result, int pos)
{
	for(int i=pos;html[i]!='\0';i++)
	{
		if(html[i]=='<'&&html[i+1]=='t'&&html[i+2]=='i'&&html[i+3]=='t'&&html[i+4]=='l'&&html[i+5]=='e')
		{
			i=i+5;
			while(html[i]!='>')
			{
				i++;
			}
			i++;
			int j=0;
			while(html[i]!='<')
			{
				result[j]=html[i];
				i++;
				j++;
			}
			result[j]='\0';
			return i;
		}
		/*else if(html[i]=='<'&&html[i+1]=='m'&&html[i+2]=='e'&&html[i+3]=='t'&&html[i+4]=='a')
		{
			i=i+5;
while(html[i]!='c'&&html[i+1]!='o'&&html[i+2]!='n'&&html[i+3]!='t'&&html[i+4]!='e'&&html[i+5]!='n'&&html[i+6]!='t')
			{
				i++;
			}
			i=i+9;
			int j=0;
			while(html[i]!='/')
			{
				result[j]=html[i];
				i++;
				j++;
			}
			result[j]='\0';
			return i;
		}*/
else if(html[i]=='<'&&html[i+1]=='h'&&(html[i+2]=='1'||html[i+2]=='2'||html[i+2]=='3'||html[i+2]=='4'||html[i+2]=='5'||html[i+2]=='6'))
		{
			i=i+3;
			while(html[i]!='>')
			{
				i++;
			}
			i++;
			int j=0;
			while(html[i]!='<'&&html[i+1]!='/')
			{
				result[j]=html[i];
				i++;
				j++;
			}
			result[j]='\0';
			return i;
		}
	}
	return -1;
}
int keyGenerator(char *word)
{
	int sum=0;
    for(int i=0;i<strlen(word);i++)
    {
	sum=sum+(int)word[i];
    }
    while(sum>=50)
    {
	sum=sum/10;
    }
    return sum;
}

int main()
{
	struct hash h[51];
   	for(int i=0;i<50;i++)
    {
		h[i].count=0;
    }
		FILE *reload;
    	reload=fopen("/home/aditya/Desktop/temp.txt","r");
   		char ch = getc(reload);
		int m=0;
		while(ch != EOF)
		{
			int u=0;
			while(ch != '\n')
			{
				struct node *temp;
				temp=(struct node*) malloc(sizeof(struct node));
				temp->left=NULL;
				temp->right=NULL;
				if(u==0)
				{
					int n=0;
   					while (ch != '|')
    				{
        				h[m].count=h[m].count*10+(ch-48);
						ch = getc(reload);
					}
					u=1;
					ch = getc(reload);
					if(ch=='\n')
					{
						m++;
						break;
					}
					h[m].first=temp;
					m++;
    			}
				if(u==1)
				{
					int n=0;
					while (ch != '|')
    				{
        				temp->word[n]=ch;
						ch = getc(reload);
						n++;
					}
					temp->word[n]='\0';
					u=2;
					ch = getc(reload);
				}
				if(u==2)
				{
					struct fileno *base=NULL;
					while (ch != '|')
    				{
						struct fileno *t;
						t=(struct fileno*) malloc(sizeof(struct fileno));
						t->left=NULL;
						t->right=NULL;
						t->id=0;
						t->count=0;
						while(ch!=' ')
						{
							t->id=t->id*10+(ch-48);
							ch = getc(reload);
						}
						ch = getc(reload);
						while(ch!=' ')
						{
							t->count=t->count*10+(ch-48);
							ch = getc(reload);
						}
						ch = getc(reload);
						if(base==NULL)
						{
							base=t;
						}
						else
						{
							struct fileno *p=base;
							while(p->right!=NULL)
							{
								p=p->right;
							}
							p->right=t;
							t->left=p;
						}
					}
					temp->ref=base;
					u=1;
					ch = getc(reload);
				}
				if(root==NULL)
				{
					root=temp;
				}
				else
				{
					struct node *p=root;
					while(p->right!=NULL)
					{
						p=p->right;
					}
					p->right=temp;
					temp->left=p;
				}
			}
			ch = getc(reload);
		}
    	fclose(reload);
	struct ofWords w[10000];
	int i;
	for(i=1;i<=10;i++)
	{
		char *file=getFileName(i);
		int size=getFileSize(file);
		char html[size+1];
		FILE *fp;
    	fp=fopen(file,"r");
   		int ch = getc(fp);
    	int u=0;
   		while (ch != EOF)
    	{
        	html[u]=ch;
        	ch = getc(fp);
        	u++;
    	}
    	fclose(fp);
		int pos=0;
		int k=0;
		char *words[1000];
		int j=0;
		while(k!=-1)
		{
			words[j]=(char*)malloc(5000);
			k=findWord(html,words[j],pos);
			pos=k;
			int y=0;
			int inword=0;
			for(int d=0;words[j][d]!='\0';d++)
			{
				if(words[j][d]==' '||words[j][d]==','||words[j][d]=='|'||words[j][d]=='\n')
				{
					if(inword==1)
					{
						w[t].wordd[y]='\0';
						w[t].fnumber=i;
						t++;
						y=0;
					}
					inword=0;
				}
				else
				{
					w[t].wordd[y++]=words[j][d];
					inword=1;
				}
			}
			if(inword==1)
    		{
				w[t].wordd[y]='\0';
			}
			j++;
		}
	}
		for(int z=0;z<t;z++)
    	{
			struct node *temp;
			temp=(struct node*) malloc(sizeof(struct node));
			strcpy(temp->word,w[z].wordd);
			temp->key=keyGenerator(w[z].wordd);
        	temp->left=NULL;
			temp->right=NULL;
			struct fileno *temp1;
			temp1=(struct fileno*) malloc(sizeof(struct fileno));
			temp1->left=NULL;
			temp1->right=NULL;
			temp1->count=1;
			temp1->id=w[z].fnumber;
			temp->ref=temp1;
        	if(root==NULL)
			{
	    		root=temp;
	    		h[temp->key].first=temp;
				(h[temp->key].count)++;
			}
			else
			{	
	  			if( h[temp->key].count==0)
	   			{
	   	 			h[temp->key].first=temp;
	    			(h[temp->key].count)++;
	     			struct node *p;
	     			p=root;
	     			while(p->right!=NULL)
	     			{
						p=p->right;
	     			}
	     			p->right=temp;
	     			temp->left=p;
	   			}
	   		 	else
	    		{
					int y=0,flag=0;
					struct node *g=h[temp->key].first;
					for(int c=0;c<(h[temp->key].count);c++)
					{
		   				if(strcmp(temp->word,g->word)==0)
		    			{
							struct fileno *no=g->ref;
							struct fileno *noo=g->ref;
							while(no!=NULL)
	     					{
								if(no->id==temp1->id)
								{
									flag=1;
									(no->count)++;
									break;
								}
								no=no->right;
	     					}
							if(flag==0)
							{
								while(noo->right!=NULL)
	     						{
									noo=noo->right;
	     						}
								noo->right=temp1;
								temp1->left=noo;
							}
							y=1;
							break;
		    			}
		   				 g=g->right;
					}
					if(y==1)
					continue;
					temp->right=(h[temp->key].first)->right;
					(h[temp->key].first)->right=temp;
					(h[temp->key].count)++;
	    		}
			}
   		}
		struct node *p=root;
	    while(p->right!=NULL)
	    {
			printf("%s\n",p->word);
			struct fileno *no=p->ref;
			while(no!=NULL)
	     	{
				printf("%d-----------%d\n",no->id,no->count);
				no=no->right;
			}
			p=p->right;
			printf("--------------------------------------------------------------------\n");
		}

		FILE *save;
    	save=fopen("/home/aditya/Desktop/temp.txt","w");
   		for(int i=0;i<50;i++)
    	{
			if(h[i].count<10)
			putc(h[i].count+48,save);
			else
			{
				putc(h[i].count/10+48,save);
				putc(h[i].count%10+48,save);
			}
			putc('|',save);
			struct node *g=h[i].first;
			for(int j=0;j<h[i].count;j++)
			{
				fputs(g->word,save);
				putc('|',save);
				struct fileno *no=g->ref;
				while(no!=NULL)
	     		{
					if(no->id<10)
					putc(no->id+48,save);
					else
					{
						putc(no->id/10+48,save);
						putc(no->id%10+48,save);
					}
					putc(' ',save);
					if(no->count<10)
					putc(no->count+48,save);
					else
					{
						putc(no->count/10+48,save);
						putc(no->count%10+48,save);
					}
					putc(' ',save);
					no=no->right;
				}
				putc('|',save);
				g=g->right;
			}
			putc('\n',save);
    	}
    	fclose(save);
}
