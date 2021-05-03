= Information Schema・Performance Schemaの変更と新機能

MySQL 8.0における機能追加や変更、および@<idx>{データディクショナリ}のInnoDB化に合わせて、Information Schema・Performance Schemaにも大幅な変更が加えられました。

== @<idx>{Information Schema}

主なものを示します。

=== 全般

==== ブログ記事等

 * @<href>{https://next4us-ti.hatenablog.com/entry/2018/11/14/175647, https://next4us-ti.hatenablog.com/entry/2018/11/14/175647}
 ** 廃止されたものと追加されたもの
 * @<href>{https://mysqlserverteam.com/mysql-8-0-improvements-to-information_schema/, https://mysqlserverteam.com/mysql-8-0-improvements-to-information_schema/}
 * @<href>{https://mysqlserverteam.com/further-improvements-on-information_schema-in-mysql-8-0-3/, https://mysqlserverteam.com/further-improvements-on-information_schema-in-mysql-8-0-3/}

=== @<idx>{データディクショナリ}テーブルとINFORMATION_SCHEMA内テーブルの統合

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/data-dictionary-information-schema.html, https://dev.mysql.com/doc/refman/8.0/ja/data-dictionary-information-schema.html}

=== 新規追加テーブル

===={80220901} 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/information-schema.html, https://dev.mysql.com/doc/refman/8.0/ja/information-schema.html}
 ** @<tt>|VIEW_TABLE_USAGE|
 ** @<tt>|VIEW_ROUTINE_USAGE|
 ** @<tt>|KEYWORDS|
 ** @<tt>|COLUMN_STATISTICS|
 ** @<tt>|ST_GEOMETRY_COLUMNS|
 ** @<tt>|ST_SPATIAL_REFERENCE_SYSTEMS|
 ** @<tt>|ST_UNITS_OF_MEASURE|
 ** @<tt>|APPLICABLE_ROLES|
 ** @<tt>|ADMINISTRABLE_ROLE_AUTHORIZATIONS|
 ** @<tt>|ENABLED_ROLES|
 ** @<tt>|ROLE_TABLE_GRANTS|
 ** @<tt>|ROLE_COLUMN_GRANTS|
 ** @<tt>|ROLE_ROUTINE_GRANTS|
 ** @<tt>|SCHEMATA_EXTENSIONS|

==== ブログ記事等

 * @<href>{https://www.s-style.co.jp/blog/2018/08/2270/, https://www.s-style.co.jp/blog/2018/08/2270/}
 ** @<tt>|INNODB_CACHED_INDEXES|

=== その他のInformation Schema変更

===={80220902} 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/information-schema-innodb-tablespaces-table.html, https://dev.mysql.com/doc/refman/8.0/ja/information-schema-innodb-tablespaces-table.html}
 ** @<tt>|INNODB_TABLESPACES|の@<tt>|SERVER_VERSION|
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-undo-tablespaces.html, https://dev.mysql.com/doc/refman/8.0/ja/innodb-undo-tablespaces.html}
 ** @<tt>|SELECT NAME, SUBSYSTEM, COMMENT FROM INFORMATION_SCHEMA.INNODB_METRICS WHERE NAME LIKE '%truncate%';|
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/information-schema-tablespaces-table.html, https://dev.mysql.com/doc/refman/8.0/ja/information-schema-tablespaces-table.html}
 ** @<tt>|TABLESPACES|テーブルが非推奨に

== @<idx>{Performance Schema}

同様に、主なものを示します。

=== @<idx>{InnoDBロック}関連テーブル等

InnoDBロック関連テーブル・ビューの構成が大きく変更されました。
詳細は著者ブログの記事を確認してください。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/sys-innodb-lock-waits.html, https://dev.mysql.com/doc/refman/8.0/ja/sys-innodb-lock-waits.html}
 ** @<idx>{InnoDBロック関連テーブル}

//embed[latex]{
\clearpage
//}

==== ブログ記事等

 * 著者ブログ
 ** @<href>{https://qiita.com/hmatsu47/items/607d176e885f098262e8, https://qiita.com/hmatsu47/items/607d176e885f098262e8}
 ** @<href>{https://qiita.com/hmatsu47/items/b49bc18d49da5c6029e5, https://qiita.com/hmatsu47/items/b49bc18d49da5c6029e5}

==== 実行例

//emlist{
@<b>{※クライアント1で実行（準備）。}
mysql> @<i>{CREATE DATABASE lock_test;}
Query OK, 1 row affected (0.02 sec)

mysql> @<i>{USE lock_test;}
Database changed
mysql> @<i>{CREATE TABLE lock_test (id int(10) PRIMARY KEY AUTO_INCREMENT, value VARCHAR(100)) ENGI}
@<i>{NE=innodb;}
Query OK, 0 rows affected (0.04 sec)

mysql> @<i>{INSERT INTO lock_test SET value='abc';}
Query OK, 1 row affected (0.00 sec)

mysql> @<i>{INSERT INTO lock_test SET value='def';}
Query OK, 1 row affected (0.00 sec)

mysql> @<i>{INSERT INTO lock_test SET value='ghi';}
Query OK, 1 row affected (0.00 sec)

mysql> @<i>{SET AUTOCOMMIT=0;}
Query OK, 0 rows affected (0.00 sec)

@<b>{※クライアント2で実行（準備）。}
mysql> @<i>{USE lock_test;}
Reading table information for completion of table and column names
You can turn off this feature to get a quicker startup with -A

Database changed

@<b>{※クライアント3で実行。ロックが生じていないことを確認。}
mysql> @<i>{SELECT * FROM sys.innodb_lock_waits\G}
Empty set (0.02 sec)

mysql> @<i>{SELECT * FROM performance_schema.data_locks\G}
Empty set (0.00 sec)

mysql> @<i>{SELECT * FROM performance_schema.data_lock_waits\G}
Empty set (0.00 sec)

@<b>{※クライアント1で実行。UPDATEでロックを発生させる。}
mysql> @<i>{UPDATE lock_test SET value='345' WHERE id>1;}
Query OK, 2 rows affected (0.00 sec)
Rows matched: 2  Changed: 2  Warnings: 0

@<b>{※クライアント3で実行。ロックが生じていることがわかる。}
mysql> @<i>{SELECT * FROM sys.innodb_lock_waits\G}
Empty set (0.00 sec)

mysql> @<i>{SELECT * FROM performance_schema.data_locks\G}
*************************** 1. row ***************************
               ENGINE: INNODB
       ENGINE_LOCK_ID: 139903578867504:1065:139903459448472
ENGINE_TRANSACTION_ID: 2585
            THREAD_ID: 46
             EVENT_ID: 23
        OBJECT_SCHEMA: lock_test
          OBJECT_NAME: lock_test
       PARTITION_NAME: NULL
    SUBPARTITION_NAME: NULL
           INDEX_NAME: NULL
OBJECT_INSTANCE_BEGIN: 139903459448472
            LOCK_TYPE: TABLE
            LOCK_MODE: IX
          LOCK_STATUS: GRANTED
            LOCK_DATA: NULL
*************************** 2. row ***************************
               ENGINE: INNODB
       ENGINE_LOCK_ID: 139903578867504:8:4:1:139903459445432
ENGINE_TRANSACTION_ID: 2585
            THREAD_ID: 46
             EVENT_ID: 23
        OBJECT_SCHEMA: lock_test
          OBJECT_NAME: lock_test
       PARTITION_NAME: NULL
    SUBPARTITION_NAME: NULL
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 139903459445432
            LOCK_TYPE: RECORD
            LOCK_MODE: X
          LOCK_STATUS: GRANTED
            LOCK_DATA: supremum pseudo-record
*************************** 3. row ***************************
               ENGINE: INNODB
       ENGINE_LOCK_ID: 139903578867504:8:4:3:139903459445432
ENGINE_TRANSACTION_ID: 2585
            THREAD_ID: 46
             EVENT_ID: 23
        OBJECT_SCHEMA: lock_test
          OBJECT_NAME: lock_test
       PARTITION_NAME: NULL
    SUBPARTITION_NAME: NULL
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 139903459445432
            LOCK_TYPE: RECORD
            LOCK_MODE: X
          LOCK_STATUS: GRANTED
            LOCK_DATA: 2
*************************** 4. row ***************************
               ENGINE: INNODB
       ENGINE_LOCK_ID: 139903578867504:8:4:4:139903459445432
ENGINE_TRANSACTION_ID: 2585
            THREAD_ID: 46
             EVENT_ID: 23
        OBJECT_SCHEMA: lock_test
          OBJECT_NAME: lock_test
       PARTITION_NAME: NULL
    SUBPARTITION_NAME: NULL
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 139903459445432
            LOCK_TYPE: RECORD
            LOCK_MODE: X
          LOCK_STATUS: GRANTED
            LOCK_DATA: 3
4 rows in set (0.00 sec)

mysql> @<i>{SELECT * FROM performance_schema.data_lock_waits\G}
Empty set (0.00 sec)

@<b>{※クライアント2で実行。INSERTがロック待ちになる。}
mysql> @<i>{SET AUTOCOMMIT=0;}
Query OK, 0 rows affected (0.00 sec)

mysql> @<i>{INSERT INTO lock_test SET value='ghi';}

@<b>{※クライアント3で実行。ロック待ちも確認できる。}
mysql> @<i>{SELECT * FROM sys.innodb_lock_waits\G}
*************************** 1. row ***************************
                wait_started: 2019-05-02 18:35:07
                    wait_age: 00:00:13
               wait_age_secs: 13
                locked_table: `lock_test`.`lock_test`
         locked_table_schema: lock_test
           locked_table_name: lock_test
      locked_table_partition: NULL
   locked_table_subpartition: NULL
                locked_index: PRIMARY
                 locked_type: RECORD
              waiting_trx_id: 2595
         waiting_trx_started: 2019-05-02 18:35:07
             waiting_trx_age: 00:00:13
     waiting_trx_rows_locked: 1
   waiting_trx_rows_modified: 0
                 waiting_pid: 9
               waiting_query: INSERT INTO lock_test SET value='ghi'
             waiting_lock_id: 139903578868400:8:4:1:139903459451384
           waiting_lock_mode: X,INSERT_INTENTION
             blocking_trx_id: 2594
                blocking_pid: 8
              blocking_query: NULL
            blocking_lock_id: 139903578867504:8:4:1:139903459445432
          blocking_lock_mode: X
        blocking_trx_started: 2019-05-02 18:34:54
            blocking_trx_age: 00:00:26
    blocking_trx_rows_locked: 3
  blocking_trx_rows_modified: 2
     sql_kill_blocking_query: KILL QUERY 8
sql_kill_blocking_connection: KILL 8
1 row in set (0.00 sec)

mysql> @<i>{SELECT * FROM performance_schema.data_locks\G}
*************************** 1. row ***************************
               ENGINE: INNODB
       ENGINE_LOCK_ID: 139903578868400:1065:139903459454424
ENGINE_TRANSACTION_ID: 2595
            THREAD_ID: 47
             EVENT_ID: 15
        OBJECT_SCHEMA: lock_test
          OBJECT_NAME: lock_test
       PARTITION_NAME: NULL
    SUBPARTITION_NAME: NULL
           INDEX_NAME: NULL
OBJECT_INSTANCE_BEGIN: 139903459454424
            LOCK_TYPE: TABLE
            LOCK_MODE: IX
          LOCK_STATUS: GRANTED
            LOCK_DATA: NULL
*************************** 2. row ***************************
               ENGINE: INNODB
       ENGINE_LOCK_ID: 139903578868400:8:4:1:139903459451384
ENGINE_TRANSACTION_ID: 2595
            THREAD_ID: 47
             EVENT_ID: 15
        OBJECT_SCHEMA: lock_test
          OBJECT_NAME: lock_test
       PARTITION_NAME: NULL
    SUBPARTITION_NAME: NULL
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 139903459451384
            LOCK_TYPE: RECORD
            LOCK_MODE: X,INSERT_INTENTION
          LOCK_STATUS: WAITING
            LOCK_DATA: supremum pseudo-record
*************************** 3. row ***************************
               ENGINE: INNODB
       ENGINE_LOCK_ID: 139903578867504:1065:139903459448472
ENGINE_TRANSACTION_ID: 2594
            THREAD_ID: 46
             EVENT_ID: 30
        OBJECT_SCHEMA: lock_test
          OBJECT_NAME: lock_test
       PARTITION_NAME: NULL
    SUBPARTITION_NAME: NULL
           INDEX_NAME: NULL
OBJECT_INSTANCE_BEGIN: 139903459448472
            LOCK_TYPE: TABLE
            LOCK_MODE: IX
          LOCK_STATUS: GRANTED
            LOCK_DATA: NULL
*************************** 4. row ***************************
               ENGINE: INNODB
       ENGINE_LOCK_ID: 139903578867504:8:4:1:139903459445432
ENGINE_TRANSACTION_ID: 2594
            THREAD_ID: 46
             EVENT_ID: 30
        OBJECT_SCHEMA: lock_test
          OBJECT_NAME: lock_test
       PARTITION_NAME: NULL
    SUBPARTITION_NAME: NULL
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 139903459445432
            LOCK_TYPE: RECORD
            LOCK_MODE: X
          LOCK_STATUS: GRANTED
            LOCK_DATA: supremum pseudo-record
*************************** 5. row ***************************
               ENGINE: INNODB
       ENGINE_LOCK_ID: 139903578867504:8:4:3:139903459445432
ENGINE_TRANSACTION_ID: 2594
            THREAD_ID: 46
             EVENT_ID: 30
        OBJECT_SCHEMA: lock_test
          OBJECT_NAME: lock_test
       PARTITION_NAME: NULL
    SUBPARTITION_NAME: NULL
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 139903459445432
            LOCK_TYPE: RECORD
            LOCK_MODE: X
          LOCK_STATUS: GRANTED
            LOCK_DATA: 2
*************************** 6. row ***************************
               ENGINE: INNODB
       ENGINE_LOCK_ID: 139903578867504:8:4:4:139903459445432
ENGINE_TRANSACTION_ID: 2594
            THREAD_ID: 46
             EVENT_ID: 30
        OBJECT_SCHEMA: lock_test
          OBJECT_NAME: lock_test
       PARTITION_NAME: NULL
    SUBPARTITION_NAME: NULL
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 139903459445432
            LOCK_TYPE: RECORD
            LOCK_MODE: X
          LOCK_STATUS: GRANTED
            LOCK_DATA: 3
6 rows in set (0.00 sec)

mysql> @<i>{SELECT * FROM performance_schema.data_lock_waits\G}
*************************** 1. row ***************************
                          ENGINE: INNODB
       REQUESTING_ENGINE_LOCK_ID: 139903578868400:8:4:1:139903459451384
REQUESTING_ENGINE_TRANSACTION_ID: 2595
            REQUESTING_THREAD_ID: 47
             REQUESTING_EVENT_ID: 15
REQUESTING_OBJECT_INSTANCE_BEGIN: 139903459451384
         BLOCKING_ENGINE_LOCK_ID: 139903578867504:8:4:1:139903459445432
  BLOCKING_ENGINE_TRANSACTION_ID: 2594
              BLOCKING_THREAD_ID: 46
               BLOCKING_EVENT_ID: 30
  BLOCKING_OBJECT_INSTANCE_BEGIN: 139903459445432
1 row in set (0.00 sec)

@<b>{※クライアント1で実行。COMMITする。}
mysql> @<i>{COMMIT;}
Query OK, 0 rows affected (0.01 sec)

@<b>{※クライアント2で確認。ロック獲得に成功したのでINSERTできた。}
Query OK, 1 row affected (24.45 sec)

@<b>{※クライアント3で実行。クライアント2が獲得したロックが確認できる。}
mysql> @<i>{SELECT * FROM sys.innodb_lock_waits\G}
Empty set (0.00 sec)

mysql> @<i>{SELECT * FROM performance_schema.data_locks\G}
*************************** 1. row ***************************
               ENGINE: INNODB
       ENGINE_LOCK_ID: 139903578868400:1065:139903459454424
ENGINE_TRANSACTION_ID: 2595
            THREAD_ID: 47
             EVENT_ID: 15
        OBJECT_SCHEMA: lock_test
          OBJECT_NAME: lock_test
       PARTITION_NAME: NULL
    SUBPARTITION_NAME: NULL
           INDEX_NAME: NULL
OBJECT_INSTANCE_BEGIN: 139903459454424
            LOCK_TYPE: TABLE
            LOCK_MODE: IX
          LOCK_STATUS: GRANTED
            LOCK_DATA: NULL
*************************** 2. row ***************************
               ENGINE: INNODB
       ENGINE_LOCK_ID: 139903578868400:8:4:1:139903459451384
ENGINE_TRANSACTION_ID: 2595
            THREAD_ID: 47
             EVENT_ID: 15
        OBJECT_SCHEMA: lock_test
          OBJECT_NAME: lock_test
       PARTITION_NAME: NULL
    SUBPARTITION_NAME: NULL
           INDEX_NAME: PRIMARY
OBJECT_INSTANCE_BEGIN: 139903459451384
            LOCK_TYPE: RECORD
            LOCK_MODE: X,INSERT_INTENTION
          LOCK_STATUS: GRANTED
            LOCK_DATA: supremum pseudo-record
2 rows in set (0.01 sec)

mysql> @<i>{SELECT * FROM performance_schema.data_lock_waits\G}
Empty set (0.00 sec)

@<b>{※クライアント2で実行。ROLLBACKする。}
mysql> @<i>{ROLLBACK;}
Query OK, 0 rows affected (0.00 sec)

@<b>{※クライアント3で実行。ロックが解消された。}
mysql> @<i>{SELECT * FROM sys.innodb_lock_waits\G}
Empty set (0.00 sec)

mysql> @<i>{SELECT * FROM performance_schema.data_locks\G}
Empty set (0.00 sec)

mysql> @<i>{SELECT * FROM performance_schema.data_lock_waits\G}
Empty set (0.00 sec)
//}

=== 高速化について

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-optimization.html, https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-optimization.html}

==== ブログ記事等

 * @<href>{https://mysqlserverteam.com/mysql-8-0-performance-schema-now-with-indexes/, https://mysqlserverteam.com/mysql-8-0-performance-schema-now-with-indexes/}

=== 新規追加テーブル

===={80220903} 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-error-summary-tables.html, https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-error-summary-tables.html}
 ** @<idx>{エラー要約テーブル}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-error-log-table.html, https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-error-log-table.html}
 ** @<idx>{エラーログテーブル}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-statement-histogram-summary-tables.html, https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-statement-histogram-summary-tables.html}
 ** @<idx>{ステートメントヒストグラム要約テーブル}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-thread-pool-tables.html, https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-thread-pool-tables.html}
 ** @<idx>{スレッドプールテーブル}（Enterprise版）
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-log-status-table.html, https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-log-status-table.html}
 ** @<idx>{ログステータステーブル}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-keyring-keys-table.html, https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-keyring-keys-table.html}
 ** @<hidx>{keyring_keysテーブル}@<tt>|keyring_keys|テーブル
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-clone-status-table.html, https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-clone-status-table.html}
 ** @<hidx>{clone_statusテーブル}@<tt>|clone_status|テーブル
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-clone-progress-table.html, https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-clone-progress-table.html}
 ** @<hidx>{clone_progressテーブル}@<tt>|clone_progress|テーブル

==== ブログ記事等

 * @<href>{https://yoku0825.blogspot.com/2018/05/mysql-80performanceschemaeventsstatemen.html, https://yoku0825.blogspot.com/2018/05/mysql-80performanceschemaeventsstatemen.html}
 ** @<idx>{ステートメントダイジェスト}
 * @<href>{https://mita2db.hateblo.jp/entry/mysql80-response-time-histgram, https://mita2db.hateblo.jp/entry/mysql80-response-time-histgram}
 ** @<idx>{ヒストグラム}

//embed[latex]{
\clearpage
//}

 * 著者ブログ
 ** @<href>{https://qiita.com/hmatsu47/items/075d8c4f19f7d75a605b, https://qiita.com/hmatsu47/items/075d8c4f19f7d75a605b}
 *** ステートメントダイジェスト
 ** @<href>{https://qiita.com/hmatsu47/items/2e4f7c4a09e4c6d4efe6, https://qiita.com/hmatsu47/items/2e4f7c4a09e4c6d4efe6}
 *** ヒストグラム

=== Performance SchemaのビルトインSQL関数

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-functions.html, https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-functions.html}
 ** @<hidx>{FORMAT_BYTES()}@<tt>|FORMAT_BYTES()|
 ** @<hidx>{FORMAT_PICO_TIME()}@<tt>|FORMAT_PICO_TIME()|
 ** @<hidx>{PS_CURRENT_THREAD_ID()}@<tt>|PS_CURRENT_THREAD_ID()|
 ** @<hidx>{PS_THREAD_ID()}@<tt>|PS_THREAD_ID()|

=== その他のPerformance Schema変更（@<idx>{Sys Schema}を含む）

===={80240901} 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-statement-digests.html, https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-statement-digests.html}
 ** @<idx>{ステートメントダイジェスト}に@<hidx>{QUERY_SAMPLE_TEXT}@<tt>|QUERY_SAMPLE_TEXT|を追加
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-solutions-rbr-monitoring.html, https://dev.mysql.com/doc/refman/8.0/ja/replication-solutions-rbr-monitoring.html}
 ** @<idx>{行ベースレプリケーション}の@<idx>{モニタリング}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-replication-applier-status-by-worker-table.html, https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-replication-applier-status-by-worker-table.html}
 ** @<hidx>{APPLYING_TRANSACTION}@<tt>|APPLYING_TRANSACTION|・@<tt>|APPLYING_TRANSACTION_START_APPLY_TIMESTAMP|など
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-replication-connection-configuration-table.html, https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-replication-connection-configuration-table.html}
 ** @<tt>|COMPRESSION_ALGORITHMS|・@<tt>|ZSTD_COMPRESSION_LEVEL|
 * @<href>{https://dev.mysql.com/doc/relnotes/mysql/8.0/en/news-8-0-24.html#mysqld-8-0-24-performance-schema, https://dev.mysql.com/doc/relnotes/mysql/8.0/en/news-8-0-24.html#}@<br>{}@<href>{https://dev.mysql.com/doc/relnotes/mysql/8.0/en/news-8-0-24.html#mysqld-8-0-24-performance-schema, mysqld-8-0-24-performance-schema}
 ** 8.0.24で項目名の一部が変更
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/sys-ps-is-consumer-enabled.html, https://dev.mysql.com/doc/refman/8.0/ja/sys-ps-is-consumer-enabled.html}
 ** Sys Schemaの@<tt>|ps_is_consumer_enabled()|関数
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/sys-version.html, https://dev.mysql.com/doc/refman/8.0/ja/sys-version.html}
 ** @<tt>|sys.version|ビュー（非推奨に）

//embed[latex]{
\clearpage
//}

== その他の変更と新機能

=== @<hidx>{SHOWステートメント}@<tt>|SHOW|ステートメント

===={80220904} 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/show-columns.html, https://dev.mysql.com/doc/refman/8.0/ja/show-columns.html}
 ** 「オプションの EXTENDED キーワードを使用すると、…」
 ** @<hidx>{SHOW EXTENDED COLUMNS}@<tt>|SHOW EXTENDED COLUMNS|（@<idx>{隠しカラム}の表示）
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/show-index.html, https://dev.mysql.com/doc/refman/8.0/ja/show-index.html}
 ** 「MySQL 8.0.13以上では、…」
 ** @<hidx>{SHOW INDEX}@<tt>|SHOW INDEX|に表示される情報の追加
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-system-variables.html#sysvar_performance_schema_show_processlist, https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-system-variables.html#sysvar_performance_schema_show_processlist, sysvar_performance_schema_show_processlist}
 ** @<hidx>{SHOW PROCESSLIST}@<tt>|SHOW PROCESSLIST|の性能改善（@<tt>|Performance Schema.processlist|テーブルを利用）
