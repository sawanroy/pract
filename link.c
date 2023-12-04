#include<stdio.h>
#include <stdlib.h>

struct link
{
    int data;
    struct link *next;
};

struct link *head =NULL;
struct link *list;
void create(int node);
void display();
void insert_begin(int data);

void main()
{
    int menu ,node, data;
while (1)
{
    printf("1: CREATE LIST\n");
    printf("2: DISPLAY\n");
    printf("3: INSERT AT BEGINING\n");

    printf("Chose the option for operation: \n");
    scanf("%d",&menu);

    switch (menu)
    {
    case 1:
        printf("Enter the number of nodes\n");
        scanf("%d",&node);
        create(node);
        break;
    case 2:
        display(head);
        break;   
    case 3:
        printf("Enter data\n");
        scanf("%d",&data);
        insert_begin(data);      
    
    default:
        break;
    }  
}


}

void create(int node)
{
    int d,i=0;
    if(head!=NULL)
    {
        printf("list already created\n");
    }
    else{
        struct link *temp;
        list = (struct link*)calloc(node,sizeof(struct link));
        //temp = (struct link*)malloc(sizeof(struct link));
        while (i<node)
        {
            printf("Enter the data: \n");
            scanf("%d",&d);
            printf("i = %d\n", i);
            list[i].data = d;
            list[i].next = NULL;
            if(head == NULL)
            {
                head = list;
            }
            else
            {
                temp = head;
               //    printf("temp->next = %p\n", temp->next);
                while (temp->next!=NULL)
                {
                    temp=temp->next;
                }
                temp->next=&list[i];
            }
           i++;
        }           
    }
}

void display()
{
   if(head == NULL)
   {
    printf("list is empty\n");
   } 
    struct link *temp;
    temp = head;
    while (temp != NULL)
    {
        printf("%d->",temp->data);
        temp = temp->next;
    }
    printf("NULL");
    
}

void insert_begin(int data){
    list = (struct link*)malloc(sizeof(struct link));
    //struct link *temp = head;
    list->data = data;
    list->next = NULL;
    if(head==NULL){ 
        head = list;
    }
    else
    {
        list->next = head;
        head=list;
    }
}

