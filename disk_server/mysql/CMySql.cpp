#include "CMySql.h"


CMySql::CMySql(void)
{
    /*这个函数用来分配或者初始化一个MYSQL对象，用于连接mysql服务端。
    如果你传入的参数是NULL指针，它将自动为你分配一个MYSQL对象，
    如果这个MYSQL对象是它自动分配的，那么在调用mysql_close的时候，会释放这个对象*/
    sock = new MYSQL;
    mysql_init(sock );
    mysql_set_character_set(sock,"utf8"); //gb2312 中华人民共和国简体字标准
}


CMySql::~CMySql(void)
{
    if(sock)
    {
        delete sock;
        sock = NULL;
    }
    
}

void CMySql::DisConnect()
{
    mysql_close(sock);
}

bool CMySql::ConnectMySql(const char *host,const char *user,const char *pass,const char *db)
{
   //localhost   127.0.0.1
   
	if (!mysql_real_connect(sock, host, user, pass, db, 0, NULL, 0))
	{
        //连接错误
		return false;
	}
	
    return true;
}

bool CMySql::SelectMySql(const char* szSql,int nColumn,list<string>& lstStr)
{
    //mysql_query() 函数用于向 MySQL 发送并执行 SQL 语句
     if(mysql_query(sock,szSql))
        return false;

     /*·mysql_store_result 对于成功检索了数据的每个查询(SELECT、SHOW、DESCRIBE、EXPLAIN、CHECK TABLE等)
     返回值:
     . CR_COMMANDS_OUT_OF_SYNC 　　以不恰当的顺序执行了命令。
 　　· CR_OUT_OF_MEMORY 　　内存溢出。
 　　· CR_SERVER_GONE_ERROR 　　MySQL服务器不可用。
 　　· CR_SERVER_LOST 　　在查询过程中，与服务器的连接丢失。
 　　· CR_UNKNOWN_ERROR 　　出现未知错误。*/
	results=mysql_store_result(sock);
    if(NULL == results)return false;
	while (record = mysql_fetch_row(results))
	{
		 for(int i = 0;i < nColumn;i++)
         { 
			 if(NULL == record[i])
			 {
				 record[i] = "null";
			 }
             lstStr.push_back(record[i]);
         }
    

	}

    return true;
}

 bool  CMySql::UpdateMySql(const char* szSql)
 {
    if(!szSql)return false;

    if(mysql_query(sock,szSql))return false;

    return true;
 }


