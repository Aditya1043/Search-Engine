#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
int dept=1;
int file_no=1;
char p[100];
struct node *root=NULL;
char *nextUrl;
struct node
{
    char url[1000];
    int dep;
    int isVisited;
    struct node *left;
    struct node *right;
    int key;
};
struct hash
{
    struct node *first;
    int count;
};
void testDir(char* dir,int *flag)
{
    struct stat statbuf;
    if(stat(dir,&statbuf)==-1)
    {
        printf("Invalid directory\n");
        *flag=1;
    }
    if(!S_ISDIR(statbuf.st_mode))
    {
         printf("Not a directory\n");
        *flag=1;
    }
    if((statbuf.st_mode & S_IWUSR)!=S_IWUSR)
    {
        printf("isnt writable\n");
        *flag=1;
    }
}
int crawl(char* seed,int depth,char* dir)
{
    char *s=(char *)malloc(sizeof(seed));
    for(int i=0;seed[i]!='\0';i++)
    {
        if(seed[i]!='/')
        {
            s[i]=seed[i];
        }
        else
        {
            s[i]='\0';
            break;
        }
    }
    int flag=0;
    char seed_url[20]="www.chitkara.edu.in";
    if(strcmp(seed_url,s) != 0)
    {
        printf("url not correct\n");
        flag=1;
    }
    if(depth<=0&&depth>5)
    {
        printf("depth not correct\n");
        flag=1;
    }
    testDir(dir,&flag);
    return flag;
}
void getpage(char *url,char *loc)
{
    char urlbuffer[sizeof(url)+300]={0};
    strcat(urlbuffer,"wget -O ");
    strcat(urlbuffer,loc);
    strcat(urlbuffer," ");
    strcat(urlbuffer,url);
    system(urlbuffer);
}
int getSize()
{
    struct stat st;
    stat("/home/aditya/Desktop/index.html",&st);
    int file_size=st.st_size;
    char itostring[5];
    sprintf(itostring,"%d",file_no);
    char file[]="/temp";
    char f[]=".txt";
    strcpy(p,"/home/aditya/Desktop/seo");
    strcat(p,file);
    strcat(p,itostring);
    strcat(p,f);
    file_no++;
    return file_size;
}
void removeWhiteSpace(char* html)
{
  int i;
  char *buffer = malloc(strlen(html)+1), *p=malloc (sizeof(char)+1);
  memset(buffer,0,strlen(html)+1);
  for (i=0;html[i];i++)
  {
    if(html[i]>32)
    {
      sprintf(p,"%c",html[i]);
      strcat(buffer,p);
    }
  }
  strcpy(html,buffer);
  //free(buffer); free(p);
}

int NormalizeURL(char* URL)
{
  int len = strlen(URL);
  if (len <= 1 )
    return 0;
  //! Normalize all URLs.
  if (URL[len - 1] == '/')
  {
    URL[len - 1] = 0;
    len--;
  }
  int i, j;
  len = strlen(URL);
  //! Safe check.
  if (len < 2)
    return 0;
  //! Locate the URL's suffix.
  for (i = len - 1; i >= 0; i--)
    if (URL[i] =='.')
      break;
  for (j = len - 1; j >= 0; j--)
    if (URL[j] =='/')
      break;
  //! We ignore other file types.
  //! So if a URL link is to a file that are not in the file type of the following
  //! one of four, then we will discard this URL, and it will not be in the URL list.
  if ((j >= 7) && (i > j) && ((i + 2) < len))
  {
    if ((!strncmp((URL + i), ".htm", 4))
        ||(!strncmp((URL + i), ".HTM", 4))
        ||(!strncmp((URL + i), ".php", 4))
        ||(!strncmp((URL + i), ".jsp", 4))
        )
    {
      len = len; // do nothing.
    }
    else
    {
      return 0; // bad type
    }
  }
  return 1;
}

void NormalizeWord(char* word) {
  int i = 0;
  while (word[i]) {
      // NEW
    if (word[i] < 91 && word[i] > 64) // Bounded below so this funct. can run on all urls
      // /NEW
      word[i] += 32;
    i++;
  }
}

int GetNextURL(char* html, char* urlofthispage, char* result, int pos)
{
  char c;
  int len, i, j;
  char* p1;  //!< pointer pointed to the start of a new-founded URL.
  char* p2;  //!< pointer pointed to the end of a new-founded URL.

  // NEW
  // Clean up \n chars
  if(pos == 0) {
    removeWhiteSpace(html);
  }
  // /NEW

  // Find the <a> <A> HTML tag.
  while (0 != (c = html[pos]))
  {
    if ((c=='<') &&
        ((html[pos+1] == 'a') || (html[pos+1] == 'A'))) {
      break;
    }
    pos++;
  }
  //! Find the URL it the HTML tag. They usually look like <a href="www.abc.com">
  //! We try to find the quote mark in order to find the URL inside the quote mark.
  if (c)
  {
    // check for equals first... some HTML tags don't have quotes...or use single quotes instead
    p1 = strchr(&(html[pos+1]), '=');

    if ((!p1) || (*(p1-1) == 'e') || ((p1 - html - pos) > 10))
    {
      // keep going...
      return GetNextURL(html,urlofthispage,result,pos+1);
    }
    if (*(p1+1) == '\"' || *(p1+1) == '\'')
      p1++;

    p1++;

    p2 = strpbrk(p1, "\'\">");
    if (!p2)
    {
      // keep going...
      return GetNextURL(html,urlofthispage,result,pos+1);
    }
    if (*p1 == '#')
    { // Why bother returning anything here....recursively keep going...

      return GetNextURL(html,urlofthispage,result,pos+1);
    }
    if (!strncmp(p1, "mailto:",7))
      return GetNextURL(html, urlofthispage, result, pos+1);
    if (!strncmp(p1, "http", 4) || !strncmp(p1, "HTTP", 4))
    {
      //! Nice! The URL we found is in absolute path.
      strncpy(result, p1, (p2-p1));
      result[p2-p1]='\0';
//printf("1----------%s\n",result);
      return  (int)(p2 - html + 1);
    } else {
      //! We find a URL. HTML is a terrible standard. So there are many ways to present a URL.
      if (p1[0] == '.') {
        //! Some URLs are like <a href="../../../a.txt"> I cannot handle this.
	// again...probably good to recursively keep going..
	// NEW

        return GetNextURL(html,urlofthispage,result,pos+1);
	// /NEW
      }
      if (p1[0] == '/') {
        //! this means the URL is the absolute path
        for (i = 7; i < strlen(urlofthispage); i++)
          if (urlofthispage[i] == '/')
            break;
        strcpy(result, urlofthispage);
        result[i] = '\0';
        strncat(result, p1, (p2 - p1));
        result[(p2-p1)+i]='\0';
//printf("2----------%s\n",result);
        return (int)(p2 - html + 1);
      } else {
        //! the URL is a absolute path.
        len = strlen(urlofthispage);
        for (i = (len - 1); i >= 0; i--)
          if (urlofthispage[i] == '/')
            break;
        for (j = (len - 1); j >= 0; j--)
          if (urlofthispage[j] == '.')
              break;
        if (i == (len -1)) {
          //! urlofthis page is like http://www.abc.com/
            strcpy(result, urlofthispage);
            result[i + 1] = 0;
            strncat(result, p1, p2 - p1);
            result[(p2-p1)+i]='\0';
//printf("3-----------%s\n",result);
            return (int)(p2 - html + 1);
        }
        if ((i <= 6)||(i > j)) {
          //! urlofthis page is like http://www.abc.com/~xyz
          //! or http://www.abc.com
          strcpy(result, urlofthispage);
          result[len] = '/';
          strncat(result, p1, p2 - p1);
          result[(p2-p1)+len+1]='\0';
//printf("4----------%s\n",result);
          return (int)(p2 - html + 1);
        }
        strcpy(result, urlofthispage);
        result[i + 1] = 0;
        strncat(result, p1, p2 - p1);
        result[(p2-p1)+i]='\0';
//printf("5-----------%s\n",result);
        return (int)(p2 - html + 1);
      }
    }
  }
  return -1;
}

int keyGenerator(char *link)
{
    int sum=0;
    for(int i=0;i<strlen(link);i++)
    {
        sum=sum+(int)link[i];
    }
    while(sum>=50)
    {
        sum=sum/10;
    }
    return sum;
}

struct node *get=NULL;
char* getNextUrlToVisit()
{
    if(get->isVisited==0)
    {
        get->isVisited=1;
        return get->url;
    }
}

int main(int argc,char *argv[])
{
    if(argc!=4)
    {
        printf("Invalid argument\n");
        return 0;
    }
    int depth=atoi(argv[2]);
    int flag=crawl(argv[1],depth,argv[3]);
    char check[100]="wget --spider ";
    strcat( check, argv[1]);
    if(!system(check))
    {
        printf("valid url\n");
    }
    else
    {
        flag=1;
        printf("invalid url\n");
    }
    struct hash h[50];
    for(int i=0;i<50;i++)
    {
        h[i].count=0;
    }
    nextUrl=(char *)malloc(1000);
    strcpy(nextUrl,argv[1]);
    do
    {
        getpage(nextUrl,argv[3]);
        int size=getSize();
        char s[size+1];
        FILE *fp;
        fp=fopen("/home/aditya/Desktop/index.html","r");
        int ch = getc(fp);
        int i=0;
        while (ch != EOF)
        {
            s[i]=ch;
            ch = getc(fp);
            i++;
        }
        fclose(fp);
        FILE *f;
        f=fopen(p,"w");
        for(int i=0;nextUrl[i]!='\0';i++)
        {
        putc (nextUrl[i] , f);
        }
        for (int i=0 ; i<size ; i++)
        {
        putc (s[i] , f);
        }
        fclose (f);
        char *links[100];
        int pos=0;
        for(int i=0;i<100;i++)
        {
            links[i]=(char *)malloc(1000);
        int k=GetNextURL(s,nextUrl,links[i],pos);
        NormalizeWord(links[i]);
        int nor=NormalizeURL(links[i]);
            for(int j=0;j<i;j++)
        {
            if(strcmp(links[i],links[j])==0)
            {
            i--;
            }
        }
            pos=k;
            if(k==-1)
            break;
        }
       /*for(int i=0;i<100;i++)
        {
        printf("%d---------------------%s\n",i,links[i]);
        }*/
        for(int i=0;i<100;i++)
        {
        struct node *temp;
        temp=(struct node*) malloc(sizeof(struct node));
        strcpy(temp->url,links[i]);
            temp->key=keyGenerator(links[i]);
            temp->dep=dept;
            temp->isVisited=0;
            temp->left=NULL;
        temp->right=NULL;
            if(root==NULL)
        {
            root=temp;
            get=temp;
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
            int y=0;
            struct node *g=h[temp->key].first;
            for(int c=0;c<(h[temp->key].count);c++)
            {
                if(strcmp(temp->url,g->url)==0)
                {
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
        int r=0;
        struct node *p=root;
        while(p!=NULL)
        {
            printf("%d----------%s\n",r,p->url);
            p=p->right;
            r++;
        }
        dept++;
        get=get->right;
    }while(strcpy(nextUrl,getNextUrlToVisit()));
}
