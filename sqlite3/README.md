sqlite3的C/C++接口用法可分为两种：回调形式与非回调形式。  
所谓回调形式其实就是通过回调的方式处理sql语句执行结果，  
非回调形式就是待sql语句执行完毕后再通过返回值和相关函数来判断、获取执行结果。  

一、sqlite3非回调形式接口用法   

1、sqlite3_open   打开sqlite3数据库的连接  
在sqlite3数据库管理系统中，用结构体sqlite3来表示一个打开的数据库对象(sqlite3数据库连接对象)，  
函数原型及用法如下：  
```
int sqlite3_open(const char *filename, sqlite3 **ppDb);
filename:要打开的sqlite3数据库的路径名
ppDb: 二级指针，用来保存打开的数据库的连接对象。
返回值: 成功返回SQLITE_OK,失败返回其他值。
```

2、 sqlite3_close: 关闭数据库连接对象   
```
int sqlite3_close(sqlite3*);
```

3、SQL语句对象   
sqlite3_stmt 这个结构体用来描述一个SQL语句对象。   
我们的应用都是通过SQL语句对象去发送sql指令给数据库管理系统的。   

4、sqlite3_prepare_v2:  编译SQL语句，并创建一个SQL语句对象    
```
int sqlite3_prepare_v2(
      sqlite3 *db,           // 数据库连接对象
      const char *zSql,      // 指向原始sql语句(要编译的sql语句)，可以包含变量名的
      int nByte,             // < 0: 编译zSql到第一个\0为止
                             // >0:  编译zSql中前面nByte个字符
                             // =0:  什么也不编译
      sqlite3_stmt **ppStmt, // *ppStmt用来保存编译好的sql语句对象
      const char **pzTail    // *pzTail如果不为空，则*pzTail指向zSql中编译的第一条完整sql语句后面的第一个字符。                 
    );
    返回值:成功返回SQLITE_OK,失败返回其他值.   

zSql指向原始的SQL语句:
(1) 不包括参数名
     "CREATE TABLE SCORE (\
        NUM INTEGER PRIMARY KEY, \
        NAME VARCHAR(255) NOT NULL,\
        SCORE INTEGER\
        );"

        char *sql = "INSERT INTO STU VALUES(2, 'HULONGTENG', 53) ;";
(2)包括参数名

        parameters: 

        编译的SQL语句中可以包含"变量/参数"，其值可以在运行期间通过特定的参数接口来指定。
        char *sql = "INSERT INTO STU VALUES(变量, 变量, 变量) ;";

有如下几种方式指定变量名:

            :AAAA  命名参数，参数名为AAAA
            @AAAA　命名参数，参数名为@AAAA
            $AAAA 命令参数，参数名为$AAAA, 参数名可以包含一个或多个:: 和().

例子:
            char *sql = "INSERT INTO STU VALUES(@NUM, @NAME, @SCORE) ;";
```

4、给参数赋值   
    (1)    获取参数索引。因为所有的绑定参数的函数，都是通过参数索引去指定参数的。   
     sqlite3_bind_parameter_index用来获取zName在SQL语句对象sqlite3_stmt中的索引。   

函数原型：     
```
int sqlite3_bind_parameter_index(sqlite3_stmt*, const char *zName);
返回值：成功返回索引值(>0)，假如没有匹配的参数名找到，则返回0
```

 (2) 给参数赋值: 绑定值到变量/参数   

参数:  SQL语句对象，参数索引，要设置的参数值   
```
int sqlite3_bind_double(sqlite3_stmt*, int , double);
int sqlite3_bind_int(sqlite3_stmt *, int , int);
int sqlite3_bind_int64(sqlite3_stmt *, int ,  sqlite3_int64);
```

参数: SQL语句对象，参数索引   
        int sqlite3_bind_null(sqlite3_stmt* , int);   

参数: SQL语句对象，参数索引，要设置的字符串指针,字符串的长度，函数指针用来释放字符串(可以为空)   
        int sqlite3_bind_text(sqlite3_stmt *, int,  const char *,  int , void (*) (void *));   

5、sqlite3_step: 用来执行编译好的SQL语句对象(由sqlite3_stmt指定)，每次返回一行执行结果。   

函数原型及用法：   

```
int sqlite3_step(sqlite3_stmt*);
sqlite3_stmt:指向编译好的要执行的SQL语句对象
返回值: SQLITE_BUSY: 没获取到锁，语句没执行。
        SQLITE_DONE: sql语句执行完成。
        SQLITE_ERROR: 出错啦
        SQLITE_MISUSE: 使用方法不当
        SQLITE_ROW:  假如SQL语句执行有返回结果，SQLITE_ROW返回。然后调用者调用sqlite3_column解析结果。    
```
 
6、查询结果分析函数   
若sql语句为SELECT语句，它的返回结果是一个结果表，则需要用额外的函数接口去获取这些结果表中的记录。  
若调用sqlite3_step函数则需要判断返回值，并对执行结果表一行一行的处理，直到返回值为SQLITE_DONE为止。   

```
int sqlite3_column_count(sqlite3_stmt *pStmt);   //返回结果行中有多少列        
int sqlite3_column_type(sqlite3_stmt *pStmt, int iCol);  //返回结果行中第iCol列的数据类型
返回值: SQLITE_INTEGER :整数类型
        SQLITE_FLOAT: 浮点类型
        SQLITE_TEXT: 文本类型，char *
        SQLITE_BLOB: blob
        SQLITE_NULL: 空类型
```
也可以不通过sqlite3_step 函数来获取执行结果，使用sqlite3_get_table 函数直接获取整个结果表。  
函数原型为：
```
int sqlite3_get_table(
  sqlite3 *db,          /* An open database */
  const char *zSql,     /* SQL to be evaluated */
  char ***pazResult,    /* Results of the query */
  int *pnRow,           /* Number of result rows written here */
  int *pnColumn,        /* Number of result columns written here */
  char **pzErrmsg       /* Error msg written here */
);
第1个参数为数据库连接对象。
第2个参数是sql 语句，跟sqlite3_exec 里的sql 是一样的。是一个很普通的以\0结尾的char*字符串。
第3个参数是查询结果（可理解为二维数组的地址）。
第4个参数是查询出多少条记录（即查出多少行，不包括字段名那行）。
第5个参数是多少个字段（多少列）。
第6个参数是错误信息
pazResult返回的字符串数量实际上是(*pnRow+1)*(*pnColumn),因为前(*pnColumn)个是字段名
```
使用sqlite3_get_table 函数后，需要使用对应的sqlite3_free_table函数来释放查询结果所申请的内存。函数原型：   
```
void sqlite3_free_table(char **result); //释放掉查询结果申请的内存空间
```
6、sqlite3_reset:用来复位sql语句对象，以便下一轮的参数赋值   
```
int sqlite3_reset(sqlite3_stmt *pStmt);
```
7、sqlite3_finalize:用来销毁一个SQL语句对象，与sqlite3_prepare_v2创建的SQL语句对象对应。    
```
int sqlite3_finalize(sqlite3_stmt *pStmt);
```


二、sqlite3回调形式接口用法   
一步到位的函数接口: sqlite3_exec   
sqlite3_exec其实是sqlite3_prepare_v2、sqlite3_step、sqlite3_finalize 三个函数的组合。   
函数原型及用法：   
```
int sqlite3_exec( sqlite3* , //指向数据库连接对象
                  const char *sql , //指向要执行的SQL语句,一般不带参数。
                  int (*callback) (void *, int, char **, char **), //回调函数
                  void *arg, //这个函数将作为callback的第一个参数传入
                  char **errmsg //用来保存出错的信息        
                );
第一个参数，数据库连接对象
第二个参数，要执行的sql语句，可以执行多条语句以;分开
第三个参数，函数指针，回调函数。一般在sql语句为select语句时，需要回调。每查询到一条结果时(一行),就调用该回调函数。
int (*callback)(
                  void *,     //sqlite3_exec的第四个参数
                  int,    //结果中有多少列 
                  char **, // char *column_values[],这一行每列的值(转化成字符串)
                  char **  //char *column_names[],这一行每列的名字(字符串)
           ), 
回调函数的返回值: 返回0表示成功，其他值表示失败，回调函数执行失败了(返回非0值),sqlite3_exec就不执行下面的语句了。
第四个参数:将作为回调函数的第一个参数.
第五个参数: *errmsg将保存执行过程中的错误信息。
```

