#include"read_proc.h"

/*
   read_proc.c
   Library to read_proc.h, see this file for descriptions.

   (C) 2014 Daniel Knuettel

   GNU General Public License v3
   
   This program comes with ABSOLUTLY NO WARRANTY!
   See the GNU GPL v3 for more information.
 */

/* DEBUG mode, you all know this*/
//#define DEBUG

int xyz(char s)
{
	
	if(s<'0'||s>'9')return 0;
	return 1;
}
int is_uint(char input[])
{
	#ifdef DEBUG
	printf("DEBUG: running is_uint(%s).\n",input);
	printf("DEBUG: strlen(%s)= %u\n",input,strlen(input));
	#endif
	int i;
	for (i=0;i<strlen(input);i++)
	{
		#ifdef DEBUG
		printf("DEBUG: is_uint(): testing char no %d\n",i);
		#endif
		if(!xyz(input[i]))
		{
			#ifdef DEBUG
			printf("DEBUG: is_uint(): char no %d is invalid\n",i);
			#endif
			return 0;
		}
		#ifdef DEBUG
		printf("DEBUG: is_uint(): char no %d is valid\n",i);
		#endif
	}
	#ifdef DEBUG
	printf("DEBUG: valid.\n");
	#endif
	return 1;
}
struct Job * get_job(char * path)
{
	#ifdef DEBUG
	printf("DEBUG: running get_job(%s)\n",path);
	#endif
	char * pth=malloc(sizeof(char)*90);
	char 
	name[BUF_LEN],	// the name of the job
	/*
	   there are descriptions for human readers
	   we throw them away with this.
	 */
        null[BUF_LEN],
		test[BUF_LEN],	
		st[BUF_LEN],	
	state;


	unsigned int pid,ppid,uid,ngid,tgid,tracerpid;
	

	strcpy(pth,path);
	strcat(pth,"status");
	FILE * file=fopen(pth,"r");
	if(!file)
	{
		printf("ERROR: FATAL: cannot open %s for read\n",pth);
		exit(-2);
	}

	fscanf( file, "%s ", null);//name word for print
        fscanf( file, "%s ",name);//name of the process
        fscanf( file, "%s ",null);//state word for print
        fscanf( file, "%c ",&state);//char for state example: s
        fscanf( file, "%s ",st);// full state word example:sleeping
        fscanf( file, "%s ",null);//tgrid word 
        fscanf( file, "%u ",&tgid);//tgid->thread group id
        fscanf( file, "%s ",null);//ngid
        fscanf( file, "%u ",&ngid);//ngid
        fscanf( file, "%s ",null);//pid word
        fscanf( file, "%u ",&pid);//pid
        fscanf( file, "%s ",null);//ppid word
        fscanf( file, "%u ",&ppid);// parent pid
        fscanf( file, "%s ",null);//trace pid 
        fscanf( file, "%u ",&tracerpid);//trace pid
	fscanf( file, "%s ",null);//user id word
	fscanf( file, "%u ",&uid);// user id
	fclose(file);

	#ifdef DEBUG
	printf("%s %c %u %u %u\n",name,state,pid,ppid,uid);
	#endif
	
	struct Job * jb=newJob(name,pid,ppid,uid,st,tgid,ngid,tracerpid);
	free(pth);
	return jb;
}

struct Root * read_proc(void)
{
	#ifdef DEBUG
	printf("DEBUG: running read_proc()\n");
	#endif
	char * dirname_buf, * curr_path=malloc(sizeof(char)*10);
	struct Root * list=newRoot();	// to save the Jobs

	DIR * directory;
	struct dirent * dir;
	directory=opendir("/proc");
	/*
	   this is fix here, because /proc is always the proc directory.
	  */
	if(directory==NULL)
	{
		printf("\n\tERROR: unable to open /proc for read\n\n");
		exit(-23);
	}

	#ifdef DEBUG
	printf("DEBUG: opened /proc for read\n");
	#endif
	while((dir=readdir(directory))!=NULL)
	{

		dirname_buf=dir->d_name;
		if(!is_uint(dirname_buf))
		{
			continue;
		}
		strcpy(curr_path,"/proc/");
		strcat(curr_path,dirname_buf);
		strcat(curr_path,"/");
		append(list,get_job(curr_path));

	}
	free(curr_path);
	return list;
}

short int is_present(char * name)
{
	struct Root * root=read_proc();
	if(!root)
	{
		return -1;
	}
	struct Knot * varknot=root->first;
	if(!varknot)
	{
		return -1;
	}

	while((varknot->got_next)&FULL)
	{
		if(strcmp(varknot->job->name,name)==0)
		{
			deleteRootComplete(root);
			return 1;
		}
		varknot=varknot->next;
	}
	deleteRootComplete(root);
	return 0;
}
