sqlite3��C/C++�ӿ��÷��ɷ�Ϊ���֣��ص���ʽ��ǻص���ʽ��  
��ν�ص���ʽ��ʵ����ͨ���ص��ķ�ʽ����sql���ִ�н����  
�ǻص���ʽ���Ǵ�sql���ִ����Ϻ���ͨ������ֵ����غ������жϡ���ȡִ�н����  

һ��sqlite3�ǻص���ʽ�ӿ��÷�   

1��sqlite3_open   ��sqlite3���ݿ������  
��sqlite3���ݿ����ϵͳ�У��ýṹ��sqlite3����ʾһ���򿪵����ݿ����(sqlite3���ݿ����Ӷ���)��  
����ԭ�ͼ��÷����£�  
```
int sqlite3_open(const char *filename, sqlite3 **ppDb);
filename:Ҫ�򿪵�sqlite3���ݿ��·����
ppDb: ����ָ�룬��������򿪵����ݿ�����Ӷ���
����ֵ: �ɹ�����SQLITE_OK,ʧ�ܷ�������ֵ��
```

2�� sqlite3_close: �ر����ݿ����Ӷ���   
```
int sqlite3_close(sqlite3*);
```

3��SQL������   
sqlite3_stmt ����ṹ����������һ��SQL������   
���ǵ�Ӧ�ö���ͨ��SQL������ȥ����sqlָ������ݿ����ϵͳ�ġ�   

4��sqlite3_prepare_v2:  ����SQL��䣬������һ��SQL������    
```
int sqlite3_prepare_v2(
      sqlite3 *db,           // ���ݿ����Ӷ���
      const char *zSql,      // ָ��ԭʼsql���(Ҫ�����sql���)�����԰�����������
      int nByte,             // < 0: ����zSql����һ��\0Ϊֹ
                             // >0:  ����zSql��ǰ��nByte���ַ�
                             // =0:  ʲôҲ������
      sqlite3_stmt **ppStmt, // *ppStmt�����������õ�sql������
      const char **pzTail    // *pzTail�����Ϊ�գ���*pzTailָ��zSql�б���ĵ�һ������sql������ĵ�һ���ַ���                 
    );
    ����ֵ:�ɹ�����SQLITE_OK,ʧ�ܷ�������ֵ.   

zSqlָ��ԭʼ��SQL���:
(1) ������������
     "CREATE TABLE SCORE (\
        NUM INTEGER PRIMARY KEY, \
        NAME VARCHAR(255) NOT NULL,\
        SCORE INTEGER\
        );"

        char *sql = "INSERT INTO STU VALUES(2, 'HULONGTENG', 53) ;";
(2)����������

        parameters: 

        �����SQL����п��԰���"����/����"����ֵ�����������ڼ�ͨ���ض��Ĳ����ӿ���ָ����
        char *sql = "INSERT INTO STU VALUES(����, ����, ����) ;";

�����¼��ַ�ʽָ��������:

            :AAAA  ����������������ΪAAAA
            @AAAA������������������Ϊ@AAAA
            $AAAA ���������������Ϊ$AAAA, ���������԰���һ������:: ��().

����:
            char *sql = "INSERT INTO STU VALUES(@NUM, @NAME, @SCORE) ;";
```

4����������ֵ   
    (1)    ��ȡ������������Ϊ���еİ󶨲����ĺ���������ͨ����������ȥָ�������ġ�   
     sqlite3_bind_parameter_index������ȡzName��SQL������sqlite3_stmt�е�������   

����ԭ�ͣ�     
```
int sqlite3_bind_parameter_index(sqlite3_stmt*, const char *zName);
����ֵ���ɹ���������ֵ(>0)������û��ƥ��Ĳ������ҵ����򷵻�0
```

 (2) ��������ֵ: ��ֵ������/����   

����:  SQL�����󣬲���������Ҫ���õĲ���ֵ   
```
int sqlite3_bind_double(sqlite3_stmt*, int , double);
int sqlite3_bind_int(sqlite3_stmt *, int , int);
int sqlite3_bind_int64(sqlite3_stmt *, int ,  sqlite3_int64);
```

����: SQL�����󣬲�������   
        int sqlite3_bind_null(sqlite3_stmt* , int);   

����: SQL�����󣬲���������Ҫ���õ��ַ���ָ��,�ַ����ĳ��ȣ�����ָ�������ͷ��ַ���(����Ϊ��)   
        int sqlite3_bind_text(sqlite3_stmt *, int,  const char *,  int , void (*) (void *));   

5��sqlite3_step: ����ִ�б���õ�SQL������(��sqlite3_stmtָ��)��ÿ�η���һ��ִ�н����   

����ԭ�ͼ��÷���   

```
int sqlite3_step(sqlite3_stmt*);
sqlite3_stmt:ָ�����õ�Ҫִ�е�SQL������
����ֵ: SQLITE_BUSY: û��ȡ���������ûִ�С�
        SQLITE_DONE: sql���ִ����ɡ�
        SQLITE_ERROR: ������
        SQLITE_MISUSE: ʹ�÷�������
        SQLITE_ROW:  ����SQL���ִ���з��ؽ����SQLITE_ROW���ء�Ȼ������ߵ���sqlite3_column���������    
```
 
6����ѯ�����������   
��sql���ΪSELECT��䣬���ķ��ؽ����һ�����������Ҫ�ö���ĺ����ӿ�ȥ��ȡ��Щ������еļ�¼��  
������sqlite3_step��������Ҫ�жϷ���ֵ������ִ�н����һ��һ�еĴ���ֱ������ֵΪSQLITE_DONEΪֹ��   

```
int sqlite3_column_count(sqlite3_stmt *pStmt);   //���ؽ�������ж�����        
int sqlite3_column_type(sqlite3_stmt *pStmt, int iCol);  //���ؽ�����е�iCol�е���������
����ֵ: SQLITE_INTEGER :��������
        SQLITE_FLOAT: ��������
        SQLITE_TEXT: �ı����ͣ�char *
        SQLITE_BLOB: blob
        SQLITE_NULL: ������
```
Ҳ���Բ�ͨ��sqlite3_step ��������ȡִ�н����ʹ��sqlite3_get_table ����ֱ�ӻ�ȡ���������  
����ԭ��Ϊ��
```
int sqlite3_get_table(
  sqlite3 *db,          /* An open database */
  const char *zSql,     /* SQL to be evaluated */
  char ***pazResult,    /* Results of the query */
  int *pnRow,           /* Number of result rows written here */
  int *pnColumn,        /* Number of result columns written here */
  char **pzErrmsg       /* Error msg written here */
);
��1������Ϊ���ݿ����Ӷ���
��2��������sql ��䣬��sqlite3_exec ���sql ��һ���ġ���һ������ͨ����\0��β��char*�ַ�����
��3�������ǲ�ѯ����������Ϊ��ά����ĵ�ַ����
��4�������ǲ�ѯ����������¼������������У��������ֶ������У���
��5�������Ƕ��ٸ��ֶΣ������У���
��6�������Ǵ�����Ϣ
pazResult���ص��ַ�������ʵ������(*pnRow+1)*(*pnColumn),��Ϊǰ(*pnColumn)�����ֶ���
```
ʹ��sqlite3_get_table ��������Ҫʹ�ö�Ӧ��sqlite3_free_table�������ͷŲ�ѯ�����������ڴ档����ԭ�ͣ�   
```
void sqlite3_free_table(char **result); //�ͷŵ���ѯ���������ڴ�ռ�
```
6��sqlite3_reset:������λsql�������Ա���һ�ֵĲ�����ֵ   
```
int sqlite3_reset(sqlite3_stmt *pStmt);
```
7��sqlite3_finalize:��������һ��SQL��������sqlite3_prepare_v2������SQL�������Ӧ��    
```
int sqlite3_finalize(sqlite3_stmt *pStmt);
```


����sqlite3�ص���ʽ�ӿ��÷�   
һ����λ�ĺ����ӿ�: sqlite3_exec   
sqlite3_exec��ʵ��sqlite3_prepare_v2��sqlite3_step��sqlite3_finalize ������������ϡ�   
����ԭ�ͼ��÷���   
```
int sqlite3_exec( sqlite3* , //ָ�����ݿ����Ӷ���
                  const char *sql , //ָ��Ҫִ�е�SQL���,һ�㲻��������
                  int (*callback) (void *, int, char **, char **), //�ص�����
                  void *arg, //�����������Ϊcallback�ĵ�һ����������
                  char **errmsg //��������������Ϣ        
                );
��һ�����������ݿ����Ӷ���
�ڶ���������Ҫִ�е�sql��䣬����ִ�ж��������;�ֿ�
����������������ָ�룬�ص�������һ����sql���Ϊselect���ʱ����Ҫ�ص���ÿ��ѯ��һ�����ʱ(һ��),�͵��øûص�������
int (*callback)(
                  void *,     //sqlite3_exec�ĵ��ĸ�����
                  int,    //������ж����� 
                  char **, // char *column_values[],��һ��ÿ�е�ֵ(ת�����ַ���)
                  char **  //char *column_names[],��һ��ÿ�е�����(�ַ���)
           ), 
�ص������ķ���ֵ: ����0��ʾ�ɹ�������ֵ��ʾʧ�ܣ��ص�����ִ��ʧ����(���ط�0ֵ),sqlite3_exec�Ͳ�ִ�����������ˡ�
���ĸ�����:����Ϊ�ص������ĵ�һ������.
���������: *errmsg������ִ�й����еĴ�����Ϣ��
```

