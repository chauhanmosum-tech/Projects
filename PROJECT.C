#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
#define DATAFILE "expense.dat"  //where expenses are stored in anyformat
#define DESC_LEN 50
#define DATE_LEN 11

struct Expense{
	int id;
	char date[DATE_LEN];
	char desc[DESC_LEN];
	float amount;

};  //written to or read from disk

void add();
void view();
void search();
void delete_exp();
void show();
int get_next_id();


int main()
{
	int ch;
	while (1)
	{
		clrscr();
		printf("===Simple Expense Tracker===\n");
		printf("1.ADD EXPENSE\n");
		printf("2.VIEW EXPENSE\n");
		printf("3.SEARCH EXPENSE\n");
		printf("4.DELETE EXPENSE\n");
		printf("5.SHOW EXPENSE\n");
		printf("6.EXIT\n");
		printf("\nEnter choice: ");
		scanf("%d",&ch);
		getchar();

		switch(ch)
		{
			case 1:add();
			break;
			case 2:view();
			break;
			case 3:search();
			break;
			case 4:delete_exp();
			break;
			case 5:show();
			break;
			case 6:exit(0);
			break;
			default:
			printf("Invalid choice..\n");

		}
		printf("\nPress any key to continue..");
		getchar();


	}
	return 0;
}

       int get_next_id()  //read expense,make a record,track id and give new id to new expense
	{
		FILE *fp;
		struct Expense e;
		int max = 0;      //track of highest id found
		fp=fopen(DATAFILE,"rb"); //open file in readbinary mode
		if(!fp)
		 return 1;
		while(fread(&e, sizeof(struct Expense),1,fp)==1) //read file record and returns 1 if successful
		{
			if(e.id > max)max= e.id;
		}
		fclose(fp);
		return max + 1; //return highestid
	}

	void add()
	{
		FILE *fp;
		struct Expense e;

		clrscr();
		printf("===Add New Expense===\n");
		e.id=get_next_id();  //gets next unique id
		printf("ID: %d\n",e.id);

		printf("Enter date  (dd-mm-yyyy): ");
		gets(e.date);

		printf("Enter Description: ");
		gets(e.desc);

		printf("enter amt: ");
		scanf("%f",&e.amount);
		//fflush(stdin);

		fp=fopen(DATAFILE,"ab");
		if(!fp){
			printf("Unable to open data file for wrting.\n");
			return;
		}
		fwrite(&e,sizeof(struct Expense),1,fp);
		fclose(fp);
		printf("Expense added successfully.\n");
	}

	void view()
	{
		FILE *fp;
		struct Expense e;

		clrscr();
		printf("===ALL EXPENSES===\n");
		fp=fopen(DATAFILE,"rb");
		if(!fp)
		{
			printf("No expense found..\n");
			return;
		}
		printf("%-5s %-12s %-30s %-10s\n","ID", "DATE", "DESCRIPTION", "AMOUNT");
		printf("--------------------------------------------------------------\n");
		while(fread(&e,sizeof(struct Expense),1,fp)==1)
		{

		  printf("%-5d %-12s %-30s %-10.2f\n",e.id,e.date,e.desc,e.amount);
		}
		fclose(fp);
	}
	void search()
	{
		FILE *fp;
		struct Expense e;
		char date[DATE_LEN];
		int found=0;    //check any record matches

		clrscr();

		printf("===Search Expense by Date===\n");
		printf("Enter Date (dd-mm-yyyy): ");
		gets(date);

		fp=fopen(DATAFILE,"rb");
		if(!fp)
		{
			printf("no exp found..\n");
			return;
		}

		printf("\n results for %s:\n",date);
		printf("%-5s %-12s %-30s %-10s\n","ID", "DATE", "DESCRIPTION", "AMOUNT");
		printf("-----------------------------------------------------------------\n");
		while(fread(&e,sizeof(struct Expense),1,fp)==1)
		{
			if(strcmp(e.date,date)==0){
			printf("%-4d %-10s %-30.30s %8.2f\n",e.id,e.date,e.desc,e.amount);
			found =1;
			}

		}

	fclose(fp);
	if(!found)

		printf("no exp found on %s",date);
	}
	void delete_exp()
	{
		FILE *fp, *tmp;
		struct Expense e;
		int id;
		int found=0;
		clrscr();

		printf("===Delete Expense by Id===\n");
		printf("Enter id..");
		scanf("%d",&id);
		getchar();

		fp=fopen(DATAFILE,"rb");
		if(!fp)
		{
			printf("data file not found..\n");
			return;
		}
		tmp=fopen("tmp.dat","wb");
		if(!tmp)
		{
			fclose(fp);
			printf("unable to create tmp file..\n");
			return;
		}
		while(fread(&e,sizeof(struct Expense),1,fp)==1)
		{
			if(e.id==id){
				found = 1;
			}
			else{
				fwrite(&e,sizeof(struct Expense),1,tmp);
			}
		}
		fclose(fp);
		fclose(tmp);
		if(found)
		{
			remove(DATAFILE);
			rename("tmp.dat",DATAFILE);
			printf("Expense id %d delete\n",id);
		}
		else{
			remove("tmp.dat");
			printf("Expense id %d not found..\n",id);
		}
	}
	void show(){
		FILE *fp;
		struct Expense e;
		float total =0.0f;

		clrscr();
		printf("===Total expense===\n");
		fp=fopen(DATAFILE,"rb");
		if(!fp){
			printf("no exp found..\n");
			return;
		}
		while(fread(&e,sizeof(struct Expense),1,fp)==1) {
			total +=e.amount;
		}
		fclose(fp);
		printf("total amt spent: %.2f",total);
	}




