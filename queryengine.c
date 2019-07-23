#include<stdio.h>
#include<string.h>
#include<stdlib.h>
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
	struct fileno *ref;
    struct node *left; 
    struct node *right;
};
struct query
{
	char word[100];
	struct fileno *ref;
};
struct disp
{
	int count;
	int id;
	int sum;
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

int main()
{
		struct hash h[51];
   		for(int i=0;i<50;i++)
    	{
			h[i].count=0;
    	}
		char input[1000];
		scanf("%[^\n]%*c",input); 
				
		char arr[10][100];
		int inword=0;
		int z=0;
		int k=0;
		for(int i=0;input[i]!='\0';i++)
		{
			if(input[i]==' ')
			{
				if(inword==1)
				{
					arr[z][k]='\0';
					z++;
					k=0;
				}
				inword=0;
			}
			else
			{
				arr[z][k]=input[i];
				k++;
				inword=1;
			}
		}
		if(inword==1)
   		{
			arr[z][k]='\0';
			z++;
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
		int it=0;
		struct query q[10];
		struct node *p=root;
		while(p!=NULL)
		{
			for(int i=0;i<z;i++)
			{
				if(strcmp(p->word,arr[i])==0)
				{
					strcpy(q[it].word,p->word);
					q[it].ref=p->ref;
					it++;
				}
			}
			p=p->right;
		}
		/*for(int i=0;i<it;i++)
		{
			printf("%s\n",q[i].word);
			struct fileno *no=q[i].ref;
			while(no!=NULL)
	     	{
				printf("%d-----------%d\n",no->id,no->count);
				no=no->right;
	     	}
			printf("--------------------------------------------------------------------\n");
		}*/
		struct disp u[11];
		for(int z=0;z<11;z++)
		{
			u[z].count=0;
			u[z].id=z;
			u[z].sum=0;
		}
		for(int z=0;z<11;z++)
		{
			for(int i=0;i<it;i++)
			{
				struct fileno *no=q[i].ref;
				while(no!=NULL)
			 	{
					if(no->id==u[z].id)
					{
						u[z].sum+=no->count;
						u[z].count++;
						break;
					}
					no=no->right;
			 	}
			}
		}
		for(int i=1;i<11;i++)
		{
			for(int j=i+1;j<11;j++)
			{
				if(u[i].count<u[j].count)
				{
					struct disp temp=u[i];
					u[i]=u[j];
					u[j]=temp;
				}
			}
		}
		for(int i=1;i<11;i++)
		{
			for(int j=i+1;j<11&&(u[i].count==u[j].count);j++)
			{
				if(u[i].sum<u[j].sum)
				{
					struct disp temp=u[i];
					u[i]=u[j];
					u[j]=temp;
				}
			}
		}
		/*for(int z=1;z<11;z++)
		{
			printf("%d\n",u[z].id);
			printf("%d\n",u[z].count);
			printf("%d\n",u[z].sum);
			printf("--------------------------------------------------------------------\n");
		}*/
		for(int i=1;i<11;i++)
		{
			char *file=getFileName(u[i].id);
			FILE *fp;
    		fp=fopen(file,"r");
   			int ch = getc(fp);
   			while (ch != '\n')
			{
		    	printf("%c",ch);
		    	ch = getc(fp);
			}
			printf("\n");
			fclose(fp);
		}
}
