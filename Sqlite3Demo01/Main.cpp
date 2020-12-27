#include <stdio.h>
#include <string.h>
#include "../sqlite3/sqlite3.h"

/*
SQLite的5个主要的函数：

sqlite3_open()， 打开数据库
sqlite3_exec()，执行非查询的sql语句
sqlite3_prepare()，准备sql语句，执行select语句或者要使用parameter bind时，用这个函数（封装了sqlite3_exec）.
sqlite3_step()，在调用sqlite3_prepare后，使用这个函数在记录集中移动。
sqlite3_close()，关闭数据库文件
*/


static int callback(void *NotUsed, int argc, char **argv, char **azColName);
void doDemo1();
void doDemo2();
void doDemo3();

int main()
{
	int ret = -1;
	sqlite3 *pdb1 = NULL;
	const char* data = "Callback function called";
	// step 1: 打开数据库连接对象
	ret = sqlite3_open("test.db", &pdb1);
	if (ret != SQLITE_OK)
	{
		fprintf(stderr, "Can't open database:%s", sqlite3_errmsg(pdb1));
		return 0;
	}
	else
	{
		fprintf(stderr, "Opened database successfully\n");
	}
	sqlite3 *pdb2 = NULL;
	// step 1: 打开数据库连接对象
	ret = sqlite3_open("test.db", &pdb2);
	if (ret != SQLITE_OK)
	{
		fprintf(stderr, "Can't open database:%s", sqlite3_errmsg(pdb2));
		return 0;
	}
	else
	{
		fprintf(stderr, "Opened database successfully\n");
	}

	//doDemo1();
	//doDemo2();
	//doDemo3();
	return 0;
}

static int callback(void *data, int argc, char *column_values[], char *column_names[])
{
	int i;
	fprintf(stderr, "%s: \n", (const char*)data);
	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", column_names[i], column_values[i] ? column_values[i] : "NULL");
	}
	printf("\n");

	return 0;
}

void doDemo1()
{
	int ret = -1;
	const char *sql = NULL;
	char *zErrMsg = NULL;
	sqlite3 *pdb = NULL;
	const char* data = "Callback function called";
	// step 1: 打开数据库连接对象
	ret = sqlite3_open("test.db", &pdb);
	if (ret != SQLITE_OK)
	{
		fprintf(stderr, "Can't open database:%s", sqlite3_errmsg(pdb));
		return;
	}
	else
	{
		fprintf(stderr, "Opened database successfully\n");
	}
	/*
	// Create Table SQL statement
	sql = "CREATE TABLE IF NOT EXISTS TSUDENT("  \
		"ID INT PRIMARY KEY     NOT NULL," \
		"NAME           TEXT    NOT NULL," \
		"AGE            INT     NOT NULL," \
		"ADDRESS        CHAR(50)," \
		"SALARY         REAL );";

	// Execute SQL statement
	ret = sqlite3_exec(pdb, sql, callback, 0, &zErrMsg);
	if (ret != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else 
	{
		fprintf(stdout, "Table created successfully\n");
	}
	*/
	/*
	// INSERT SQL statement
	sql = "INSERT INTO TSUDENT (ID,NAME,AGE,ADDRESS,SALARY) "  \
		"VALUES (1, 'Paul', 32, 'California', 20000.00 ); " \
		"INSERT INTO TSUDENT (ID,NAME,AGE,ADDRESS,SALARY) "  \
		"VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
		"INSERT INTO TSUDENT (ID,NAME,AGE,ADDRESS,SALARY)" \
		"VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
		"INSERT INTO TSUDENT (ID,NAME,AGE,ADDRESS,SALARY)" \
		"VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";
	// Execute SQL statement
	ret = sqlite3_exec(pdb, sql, callback, 0, &zErrMsg);
	if (ret != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		fprintf(stdout, "Records created successfully\n");
	}
	*/

	/*
	// SELECT SQL statement
	sql = "SELECT * from TSUDENT";
	// Execute SQL statement
	ret = sqlite3_exec(pdb, sql, callback, (void*)data, &zErrMsg);
	if (ret != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		fprintf(stdout, "Operation done successfully\n");
	}
	*/

	/*
	// UPDATE merged SQL statement
	sql = "UPDATE TSUDENT set SALARY = 25000.00 where ID=1; " \
		"SELECT * from TSUDENT";

	// Execute SQL statement
	ret = sqlite3_exec(pdb, sql, callback, (void*)data, &zErrMsg);
	if (ret != SQLITE_OK) 
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else 
	{
		fprintf(stdout, "Operation done successfully\n");
	}
	*/

	// DELETE merged SQL statement
	sql = "DELETE from TSUDENT where ID=2; " \
		"SELECT * from TSUDENT";

	// Execute SQL statement
	ret = sqlite3_exec(pdb, sql, callback, (void*)data, &zErrMsg);
	if (ret != SQLITE_OK) 
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else 
	{
		fprintf(stdout, "Operation done successfully\n");
	}

	//step 3: 关闭数据库连接对象
	sqlite3_close(pdb);
}

// sqlite3_step 方式获取执行结果：
void doDemo2()
{
	int ret = -1;
	const char *sql = NULL;
	char *zErrMsg = NULL;
	sqlite3 *pdb = NULL;
	const char* data = "Callback function called";
	// step 1: 打开数据库连接对象
	ret = sqlite3_open("test.db", &pdb);
	if (ret != SQLITE_OK)
	{
		fprintf(stderr, "Can't open database:%s", sqlite3_errmsg(pdb));
		return;
	}
	else
	{
		fprintf(stderr, "Opened database successfully\n");
	}

	// step 2: sql语句对象
	sqlite3_stmt *pStmt;
	sql = "SELECT * FROM TSUDENT WHERE ID = ? AND AGE = ? ;";
	ret = sqlite3_prepare_v2(
		pdb, //数据库连接对象
		sql, //指向原始sql语句字符串
		strlen(sql), //
		&pStmt,
		NULL
	);
	if (ret != SQLITE_OK)
	{
		fprintf(stderr, "sqlite3_prepare_v2 error:%s", sqlite3_errmsg(pdb));
		return ;
	}
	//int bindcount = sqlite3_bind_parameter_count(pStmt); //通过名称获得项的索引号
	//const char * name = sqlite3_bind_parameter_name(pStmt, 1);
	//name = sqlite3_bind_parameter_name(pStmt, 2);
	//int index = sqlite3_bind_parameter_index(pStmt, ":AGE");
	//index = sqlite3_bind_parameter_index(pStmt, ":ID");
	ret = sqlite3_bind_int(pStmt, 1, 1);
	ret = sqlite3_bind_int(pStmt, 2, 32);
	//ret = sqlite3_bind_int(pStmt, sqlite3_bind_parameter_index(pStmt, ":AGE"), 32);
	
	while (1)
	{
		static int first = 1;
		int i;
		// 2.2 执行SQL语句
		ret = sqlite3_step(pStmt);
		if (ret == SQLITE_DONE)
		{
			printf("o la\n");
			break;//执行完成
		}
		else if (ret == SQLITE_ROW) //获取到一行的结果
		{
			int cNum = 0;//结果集中有多少列
			cNum = sqlite3_column_count(pStmt);
			if (first)
			{
				for (i = 0; i < cNum; i++)
				{
					//返回结果集中第i列的名字
					const char *p = sqlite3_column_name(pStmt, i);
					printf("%s\t", p);
				}
				printf("\n---------------------------\n");
				first = 0;
			}
			int iType; //第i列的数据类型
			for (i = 0; i < cNum; i++)
			{
				//获取结果集中第i列的数据类型
				iType = sqlite3_column_type(pStmt, i);
				if (iType == SQLITE_INTEGER)
				{
					int iValue;
					//获取结果集中第i列的数据值
					iValue = sqlite3_column_int(pStmt, i);
					printf("%d\t", iValue);
				}
				else if (iType == SQLITE_FLOAT)
				{
					double iValue;
					//获取结果集中第i列的数据值
					iValue = sqlite3_column_double(pStmt, i);
					printf("%g\t", iValue);


				}
				else if (iType == SQLITE_TEXT)
				{
					const unsigned char * iValue;
					//获取结果集中第i列的数据值
					iValue = sqlite3_column_text(pStmt, i);
					printf("%s\t", iValue);
				}
			}
			printf("\n");
		}
	}
	sqlite3_reset(pStmt);
	//销毁一个SQL语句对象
	sqlite3_finalize(pStmt);

	//step 3: 关闭数据库连接对象
	sqlite3_close(pdb);
}

// sqlite3_get_table 方式获取执行结果
void doDemo3()
{
	int ret = -1;
	const char *sql = NULL;
	char *zErrMsg = NULL;
	sqlite3 *pdb = NULL;
	const char* data = "Callback function called";
	// step 1: 打开数据库连接对象
	ret = sqlite3_open("test.db", &pdb);
	if (ret != SQLITE_OK)
	{
		fprintf(stderr, "Can't open database:%s", sqlite3_errmsg(pdb));
		return;
	}
	else
	{
		fprintf(stderr, "Opened database successfully\n");
	}

	// step 2: sql语句对象。
	sqlite3_stmt *pStmt = NULL;
	sql = "SELECT * FROM TSUDENT WHERE ID=1";
	ret = sqlite3_prepare_v2(
		pdb, //数据库连接对象
		sql, //指向原始sql语句字符串
		strlen(sql), //
		&pStmt,
		NULL
	);
	if (ret != SQLITE_OK)
	{
		fprintf(stderr, "sqlite3_prepare_v2 error:%s", sqlite3_errmsg(pdb));
		return ;
	}
	printf("The SQL CMD:%s\n", sql);
	int nrow = 0;       //查询到数据的行数
	int ncolumn = 0;   //查询到数据的列数
	char **azResult;   //二维数组存放结果
	ret = sqlite3_get_table(pdb, sql, &azResult, &nrow, &ncolumn, &zErrMsg); //查询数据库
	if (ret != SQLITE_OK)
	{
		fprintf(stderr, "Can't get table: %s", sqlite3_errmsg(pdb));
		return ;
	}

	printf("nrow = %d ,ncolumn = %d\n", nrow, ncolumn); //打印查询结果表的行数和列数
	if (nrow != 0 && ncolumn != 0)     //有查询结果,不包含表头所占行数
	{
		int i = 0, j = 0;
		for (i = 0; i <= nrow; i++)           //打印查询结果
		{
			for (j = 0; j < ncolumn; j++)
			{
				if (i == 0) //第0行为数据表头
				{
					printf("%s\t", azResult[i + j]);
				}
				else
					printf("%s\t", azResult[i + j]);
			}
			printf("\n");
		}

	}

	sqlite3_free_table(azResult);     //释放掉 azResult的内存空间
	sqlite3_finalize(pStmt);     //销毁一个SQL语句对象
	//step 3: 关闭数据库连接对象
	sqlite3_close(pdb);
}