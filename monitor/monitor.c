#include"read_proc.h"
#include"struct.h"
int main(void)
{
   struct Root * root=read_proc();
   struct Job * buffer;
   int i=0;
   for(;i<root->len;i++)
   {
       buffer=get_from_place(root,i);
       printf("name : %-20s\t PID: %u \t PPID: %u \t status: %s \t UserID: %u\t threadGroupID: %u \t ngid: %u \t tracerpid:%u\n",
	   buffer->name,
	   buffer->pid,
	   buffer->ppid,
	   buffer->status,
	   buffer->uid,
	   buffer->tgid,buffer->ngid,buffer->tracerpid
	   );
   }
   deleteRootComplete(root);
   return 0;
}