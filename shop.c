#include<stdio.h>
#include<mysql/mysql.h>
#include<string.h>
#include<stdlib.h>
#include<stdint.h>

void InsertGood();
void SelectGood();
void UpdateGood();
void DeleteGood();
void PrintMenu();
void SellGood();
void PrintGood();
void SumMon();
void  LoginShop();
void RegistShop();
MYSQL mysql;

int main()
{
	
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "localhost", "gg", "123456","data",0,NULL,0))
	{
		fprintf(stderr, "Failed to connect to database: Error:%s\n", mysql_error(&mysql));
	}

	mysql_query(&mysql, "set names utf8");

	while(1)
	{
		int a;
		printf("**1.用户登录***********2.用户注册**\n");
		printf("请选择功能：");
		scanf("%d",&a);
		if(a==1)
		{
			LoginShop();
			break;
		}
		if(a==2)
		RegistShop();
	}

	int n;
	PrintMenu();
	printf("请选择功能:");
	scanf("%d",&n);
	while(1)
	{
		switch(n)
		{
			case 1:
				InsertGood();
				break;
			case 2:
				SelectGood();
				break;
			case 3:
				UpdateGood();
				break;
			case 4:
				DeleteGood();
			case 5:
				SellGood();
				break;
			case 6:
				PrintGood();
				break;
			case 7:
				SumMon();
				break;
			case 0:
				return 0;
			default :
				break;
		}
		PrintMenu();
		printf("请选择功能:");
		scanf("%d",&n);

	}

	mysql_close(&mysql);
	return 0;

}
void InsertGood()
{
	char gname[20];
	int gid ,gcount,gbid,gprice;
	char good[100];
	char sell[100];
	int n=0;
	int m=0;
	printf("请输入商品信息：商品编号 商品名字 商品数量 商品进价 商品售价\n");
	scanf("%d %s %d %d %d",&gid,gname,&gcount,&gbid,&gprice);

	sprintf(good,"insert into shop values (%d,'%s',%d,%d,%d)",gid,gname,gcount,gbid,gprice);
	sprintf(sell,"insert into sgood values ('%s',%d,%d)",gname,n,m);

	mysql_query(&mysql,"set foreign_key_checks=0");
	if(mysql_query(&mysql,good))
	{
		fprintf(stderr,"Failed to insert to table shop: Error: %s\n",mysql_error(&mysql));
	
	}


	if(mysql_query(&mysql,sell))
	{
		fprintf(stderr,"Failed to insert to table shop: Error: %s\n",mysql_error(&mysql));
	}
	
	mysql_query(&mysql,"set foreign_key_checks=1");

}
void UpdateGood()
{
	int g_id;
	printf("请输入你要修改的商品的编号：");	
	scanf("%d",&g_id);
	char good[100];
	int g_bid,g_price;
	printf("请输入你想修改的内容：1：进价  2：售价\n");
	int n;
	scanf("%d",&n);
	if(1==n)
	{
		printf("请输入更新后的进价：");
		scanf("%d",&g_bid);
		sprintf(good,"update shop set gbid=%d where gid=%d",g_bid,g_id);
	}
	if(2==n)
	{
		printf("请输入更新后的售价：");
		scanf("%d",&g_price);
		sprintf(good,"update shop set gprice=%d where gid=%d",g_price,g_id);
	}
	if(mysql_query(&mysql,good))
	{
		fprintf(stderr,"Failed to update to table shop: Error: %s\n",mysql_error(&mysql));
	}
	else
	{
		if(mysql_affected_rows(&mysql)==1)
			printf("修改成功!\n");	
		else
			printf("修改失败!\n");
	}
}
void DeleteGood()
{
	int g_id;
	printf("请输入需要删除的商品编号：");
	scanf("%d",&g_id);
	char good[100];
	sprintf(good,"delete from shop where gid=%d",g_id);
	if(mysql_query(&mysql,good))
	{
		fprintf(stderr,"Failed to delete to table shop: Error: %s\n",mysql_error(&mysql));
	}
	else
		printf("删除成功!\n");
}
void SelectGood()
{
	const char* sel="select * from shop";
	if(mysql_query(&mysql,sel))
	{
		fprintf(stderr,"Failed ro select: Error:%s\n",mysql_error(&mysql));
	}
	else
	{
		MYSQL_RES* res;
		res=mysql_store_result(&mysql);

		MYSQL_ROW row;
		while(row=mysql_fetch_row(res))
		{
			printf("%s %s %s %s %s\n",row[0],row[1],row[2],row[3],row[4]);
		}
		mysql_free_result(res);
	}

}
void PrintMenu()
{
	printf("**********超市管理系统*************\n");
	printf("1.增加商品         2.打印商品信息\n");
	printf("3.更新商品         4.删除商品信息\n");
	printf("5.销售商品         6.打印售卖信息\n");
	printf("**************0退出****************\n");
}
void SellGood()
{
	int g_id;
	int cnt;
	char good[100],sell[100];
	printf("请输入出售的商品编号和数量：\n");
	scanf("%d %d",&g_id,&cnt);

	sprintf(good,"update shop set gcount=gcount-%d where gid=%d",cnt,g_id);
	if(mysql_query(&mysql,good))
	{
		fprintf(stderr,"Failed to insert to table shop: Error: %s\n",mysql_error(&mysql));
	}
	sprintf(sell,"update sgood,shop set scount=scount+%d,mon=mon+(gprice-gbid) where sgood.gname=(select gname from shop where gid=%d) and shop.gname=sgood.gname",cnt, g_id);
	if(mysql_query(&mysql,sell))
	{
		fprintf(stderr,"Failed to insert to table shop: Error: %s\n",mysql_error(&mysql));
	}
	
}
void PrintGood()
{
	const char* sel="select * from sgood";
	if(mysql_query(&mysql,sel))
	{
		fprintf(stderr,"Failed ro select: Error:%s\n",mysql_error(&mysql));
	}
	else
	{
		MYSQL_RES* res;
		res=mysql_store_result(&mysql);

		MYSQL_ROW row;
		while(row=mysql_fetch_row(res))
		{
			printf("%s %s %s \n",row[0],row[1],row[2]);
		}
	}
}
void SumMon()
{
	const char* sum="select sum(mon) from sgood";
	if(mysql_query(&mysql,sum))
		fprintf(stderr,"Failed ro select: Error:%s\n",mysql_error(&mysql));
	else
	{
		MYSQL_RES* res;
		res=mysql_store_result(&mysql);
		MYSQL_ROW row=mysql_fetch_row(res);
		printf("当前所有商品销售总利润为：%s￥\n",row[0]);
		mysql_free_result(res);
	}

}
void LoginShop()
{
	int a_id;
	int flag=0;
	char a_passwd[100];
	char pwd[100];
	printf("请输入账号：");
	scanf("%d",&a_id);
	char id[100];
	sprintf(id,"select aid from assistant where aid=%d",a_id);

	if(mysql_query(&mysql,id))
		fprintf(stderr,"Failed ro select: Error:%s\n",mysql_error(&mysql));
	else
	{
		MYSQL_RES* res;
		res=mysql_store_result(&mysql);
		MYSQL_ROW row=mysql_fetch_row(res);
		if(row==0)
		{
			printf("账号不存在!\n");
			exit(0);
		}
	
		else
		{
			flag=1;
		}
		mysql_free_result(res);
	}

	while(flag)
	{
			printf("请输入密码：");
			scanf("%s",a_passwd);
			sprintf(pwd,"select apasswd from assistant where aid=%d",a_id);

			if(mysql_query(&mysql,pwd))
				fprintf(stderr,"Failed ro select: Error:%s\n",mysql_error(&mysql));
			else
			{
				MYSQL_RES* res1;
				res1=mysql_store_result(&mysql);
				MYSQL_ROW row1=mysql_fetch_row(res1);
				

				char *msg;
				size_t len;
				int i;
				uint8_t result[16];
				char md5pwd[100];
				msg=a_passwd;

				len=strlen(msg);

				for(i=0;i<1000000;i++)
				{
					md5((uint8_t*)msg,len,result);
				}
				int j=0;
				for(i=0;i<16;i++)
				{	
					sprintf(md5pwd+j,"%02x",result[i]);
					j+=2;
				}

				//printf("%s  %s",md5pwd,row1[0]);

				if(strcmp(row1[0],md5pwd)==0)
				{
					printf("登录成功!\n");
					flag=0;
				}
				else
				{
					printf("密码错误!\n");
					flag=1;
				}

				mysql_free_result(res1);
			}

	}
	
}
void RegistShop()
{
	int  a_id,i;
    char a_passwd[100];
	char a_pwd[100];
	char at[100];

	printf("请输入一个数字类型的id账号：\n");
	scanf("%d",&a_id);
	while(1)
	{
		printf("请输入密码：\n");
		scanf("%s",a_passwd);
		printf("请再次输入密码：\n");
		scanf("%s",a_pwd);
		if(strcmp(a_passwd,a_pwd)==0)
			break;
		else
			printf("两次输入的密码不一致!\n");
	}
	
	char *msg;
	int j=0;
	size_t len;
    uint8_t  result[16];
	char md5pwd[100];
	msg=a_passwd;

	len=strlen(msg);

	for(i=0;i<1000000;i++)
	{
		md5((uint8_t*)msg,len,result);
	}

//	printf("%x\n",result[0]);
	for(i=0;i<16;i++)
	{
		sprintf(md5pwd+j,"%02x",result[i]);
		j+=2;
	}

//	printf("%s\n",md5pwd);

	sprintf(at,"insert into assistant values (%d,'%s')",a_id,md5pwd);

	if(mysql_query(&mysql,at))
		fprintf(stderr,"Failed to insert to assistant: Error:%s\n",mysql_error(&mysql));
	else
		printf("注册成功!\n");
		
}
