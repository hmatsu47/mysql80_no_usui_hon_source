= オプティマイザとInnoDBの新機能

== @<idx>{オプティマイザ}

MySQL 8.0では、SQLの@<idx>{実行計画}を最適化するオプティマイザが進化しました。

//blankline
//noindent
@<hidx>{非公式MySQL 8.0オプティマイザガイド}@<b>{【注】「非公式MySQL 8.0オプティマイザガイド」}という非常に有用な資料があります。
ご確認ください。

 * @<href>{https://yakst.github.io/unofficialmysqlguide-ja/, https://yakst.github.io/unofficialmysqlguide-ja/}

=== ヒストグラム

@<idx>{カラム値}の@<idx>{ヒストグラム統計}を使い、@<idx>{インデックス}がないカラムでも値の分布から行の@<idx>{絞り込み}を可能にする機能です。
RDBMSによってはインデックスの一種としてヒストグラムを利用するものがありますが、MySQL 8.0ではインデックスとは別の機能として提供されます。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/optimizer-statistics.html, https://dev.mysql.com/doc/refman/8.0/ja/optimizer-statistics.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/analyze-table.html#analyze-table-histogram-statistics-analysis, https://dev.mysql.com/doc/refman/8.0/ja/analyze-table.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/analyze-table.html#analyze-table-histogram-statistics-analysis, analyze-table-histogram-statistics-analysis}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_histogram_generation_max_mem_size, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_histogram_generation_max_mem_size, sysvar_histogram_generation_max_mem_size}

==== ブログ記事等

 * @<href>{https://yakst.com/ja/posts/4873, https://yakst.com/ja/posts/4873}
 * @<href>{http://masato.ushio.org/blog/index.php/2017/09/25/uco-tech_mysql-8-0-rc-histogram-optimizing/, http://masato.ushio.org/blog/index.php/2017/09/25/uco-tech_mysql-8-0-rc-histogram-optimizing/}
 * @<href>{https://www.percona.com/blog/2019/10/29/column-histograms-on-percona-server-and-mysql-8-0/, https://www.percona.com/blog/2019/10/29/column-histograms-on-percona-server-and-mysql-8-0/}
 * 著者ブログ
 ** @<href>{https://qiita.com/hmatsu47/items/3cfc6762bca766c5d9a1, https://qiita.com/hmatsu47/items/3cfc6762bca766c5d9a1}

=== @<idx>{メモリ}と@<idx>{ディスク}の@<idx>{I/Oコスト}

MySQL 5.7までは、@<idx>{データページ}をメモリ（@<idx>{バッファプール}）から読み取る場合もディスクから読み出す場合も同じコストが掛かるものとしてコスト計算を行っていました。
MySQL 8.0では、メモリとディスクの@<idx>{コスト係数}を別々に設定してコスト計算を行うことができるようになりました。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/cost-model.html, https://dev.mysql.com/doc/refman/8.0/ja/cost-model.html}
 ** メモリとディスクのI/Oコスト係数を分離
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/cost-model.html#cost-model-database, https://dev.mysql.com/doc/refman/8.0/ja/cost-model.html#cost-model-database}
 ** コスト係数テーブルの初期値

==== ブログ記事等

 * 著者ブログ
 ** @<href>{https://qiita.com/hmatsu47/items/d53b0471c8f279130114, https://qiita.com/hmatsu47/items/d53b0471c8f279130114}

==== 実行例

//emlist{
mysql> @<i>{SELECT * FROM mysql.engine_cost\G}
*************************** 1. row ***************************
  engine_name: default
  device_type: 0
    cost_name: io_block_read_cost
   cost_value: NULL
  last_update: 2020-05-02 23:41:29
      comment: NULL
default_value: 1
*************************** 2. row ***************************
  engine_name: default
  device_type: 0
    cost_name: memory_block_read_cost
   cost_value: NULL
  last_update: 2020-05-02 23:41:29
      comment: NULL
default_value: 0.25
2 rows in set (0.00 sec)
@<b>{※ストレージの読み取りコスト=1に対して、メモリの読み取りコスト=0.25がデフォルト。上書き変更したら「FLUSH OPTIMIZER_COSTS;」。}

mysql> @<i>{CREATE DATABASE cost_test;}
Query OK, 1 row affected (0.01 sec)

mysql> @<i>{USE cost_test;}
Database changed
mysql> @<i>{CREATE TABLE t1 (id INT PRIMARY KEY AUTO_INCREMENT, val INT);}
Query OK, 0 rows affected (0.02 sec)

mysql> @<i>{INSERT INTO t1 SET val = 100;}
Query OK, 1 row affected (0.01 sec)
@<b>{（中略）}
mysql> @<i>{SELECT * FROM t1;}
+----+------+
| id | val  |
+----+------+
|  1 |  100 |
@<b>{（中略）}
| 10 | 1000 |
+----+------+
10 rows in set (0.00 sec)

mysql> @<i>{CREATE TABLE t2 (id INT PRIMARY KEY AUTO_INCREMENT, val INT);}
Query OK, 0 rows affected (0.02 sec)
@<b>{（中略）}
mysql> @<i>{SELECT * FROM t2;}
+----+-------+
| id | val   |
+----+-------+
|  1 |  1000 |
@<b>{（中略）}
| 10 | 10000 |
+----+-------+
10 rows in set (0.00 sec)
@<b>{※バッファプールをクリアするため、ここでMySQLサーバを停止。/etc/my.cnfの末尾に「innodb-buffer-pool-load-at-startup=OFF」を追記してから起動。}

mysql> @<i>{USE cost_test;}
Reading table information for completion of table and column names
You can turn off this feature to get a quicker startup with -A

Database changed
mysql> @<i>{CHECKSUM TABLE t1;}
+--------------+------------+
| Table        | Checksum   |
+--------------+------------+
| cost_test.t1 | 3821923307 |
+--------------+------------+
1 row in set (0.00 sec)
@<b>{※t1テーブルのみバッファプールに読み込み。t1テーブルの行読み取りコストがt2テーブルの1/4に。}

mysql> @<i>{EXPLAIN SELECT * FROM t1, t2 WHERE t1.id = t2.id AND t1.id > 2 AND t2.id < 9\G}
*************************** 1. row ***************************
           id: 1
  select_type: SIMPLE
        table: t2
   partitions: NULL
         type: range
possible_keys: PRIMARY
          key: PRIMARY
      key_len: 4
          ref: NULL
         rows: 6
     filtered: 100.00
        Extra: Using where
*************************** 2. row ***************************
           id: 1
  select_type: SIMPLE
        table: t1
   partitions: NULL
         type: eq_ref
possible_keys: PRIMARY
          key: PRIMARY
      key_len: 4
          ref: cost_test.t2.id
         rows: 1
     filtered: 100.00
        Extra: NULL
2 rows in set, 1 warning (0.00 sec)
@<b>{※t2テーブルが駆動表になった。}

mysql> @<i>{CHECKSUM TABLE t2;}
+--------------+------------+
| Table        | Checksum   |
+--------------+------------+
| cost_test.t2 | 1155527278 |
+--------------+------------+
1 row in set (0.00 sec)
@<b>{※t2テーブルもバッファプールに読み込み。t1テーブルとt2テーブルの行読み取りコストが同じに。}

mysql> @<i>{EXPLAIN SELECT * FROM t1, t2 WHERE t1.id = t2.id AND t1.id > 2 AND t2.id < 9\G}
*************************** 1. row ***************************
           id: 1
  select_type: SIMPLE
        table: t1
   partitions: NULL
         type: range
possible_keys: PRIMARY
          key: PRIMARY
      key_len: 4
          ref: NULL
         rows: 6
     filtered: 100.00
        Extra: Using where
*************************** 2. row ***************************
           id: 1
  select_type: SIMPLE
        table: t2
   partitions: NULL
         type: eq_ref
possible_keys: PRIMARY
          key: PRIMARY
      key_len: 4
          ref: cost_test.t1.id
         rows: 1
     filtered: 100.00
        Extra: NULL
2 rows in set, 1 warning (0.00 sec)
@<b>{※駆動表と内部表が逆転した。}
//}

=== @<hidx>{FORCE INDEX}@<tt>|FORCE INDEX|時に不要な@<idx>{インデックスダイブ}を回避

@<tt>|FORCE INDEX|を指定した場合の@<idx>{インデックス走査}が効率的になりました。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/range-optimization.html#equality-range-optimization, https://dev.mysql.com/doc/refman/8.0/ja/range-optimization.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/range-optimization.html#equality-range-optimization, equality-range-optimization}
 ** 「MySQL 8.0 では、…」

==== ブログ記事等

 * @<href>{https://mysqlserverteam.com/optimization-to-skip-index-dives-with-force-index/, https://mysqlserverteam.com/optimization-to-skip-index-dives-with-force-index/}

=== @<idx>{ヒント句}

新しいヒント句が追加されました。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/optimizer-hints.html#optimizer-hints-table-level, https://dev.mysql.com/doc/refman/8.0/ja/optimizer-hints.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/optimizer-hints.html#optimizer-hints-table-level, optimizer-hints-table-level}
 ** @<tt>|HASH_JOIN|, @<tt>|NO_HASH_JOIN| ※8.0.18のみ有効。8.0.20以降では@<tt>|BNL|, @<tt>|NO_BNL|でハッシュジョインをON／OFF
 ** @<hidx>{MERGE}@<tt>|MERGE|, @<tt>|NO_MERGE|
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/optimizer-hints.html#optimizer-hints-index-level, https://dev.mysql.com/doc/refman/8.0/ja/optimizer-hints.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/optimizer-hints.html#optimizer-hints-index-level, optimizer-hints-index-level}
 ** @<hidx>{GROUP_INDEX}@<tt>|GROUP_INDEX|, @<tt>|NO_GROUP_INDEX|
 ** @<hidx>{INDEX}@<tt>|INDEX|, @<tt>|NO_INDEX|
 ** @<hidx>{INDEX_MERGE}@<tt>|INDEX_MERGE|, @<tt>|NO_INDEX_MERGE|
 ** @<hidx>{JOIN_INDEX}@<tt>|JOIN_INDEX|, @<tt>|NO_JOIN_INDEX|
 ** @<hidx>{ORDER_INDEX}@<tt>|ORDER_INDEX|, @<tt>|NO_ORDER_INDEX|
 ** @<hidx>{SKIP_SCAN}@<tt>|SKIP_SCAN|, @<tt>|NO_SKIP_SCAN|
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/optimizer-hints.html#optimizer-hints-join-order, https://dev.mysql.com/doc/refman/8.0/ja/optimizer-hints.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/optimizer-hints.html#optimizer-hints-join-order, optimizer-hints-join-order}
 ** @<hidx>{JOIN_FIXED_ORDER}@<tt>|JOIN_FIXED_ORDER|
 ** @<hidx>{JOIN_ORDER}@<tt>|JOIN_ORDER|
 ** @<hidx>{JOIN_PREFIX}@<tt>|JOIN_PREFIX|
 ** @<hidx>{JOIN_SUFFIX}@<tt>|JOIN_SUFFIX|
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/optimizer-hints.html#optimizer-hints-resource-group, https://dev.mysql.com/doc/refman/8.0/ja/optimizer-hints.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/optimizer-hints.html#optimizer-hints-resource-group, optimizer-hints-resource-group}
 ** @<hidx>{RESOURCE_GROUP}@<tt>|RESOURCE_GROUP|
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/optimizer-hints.html#optimizer-hints-set-var, https://dev.mysql.com/doc/refman/8.0/ja/optimizer-hints.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/optimizer-hints.html#optimizer-hints-set-var, optimizer-hints-set-var}
 ** @<hidx>{SET_VAR}@<tt>|SET_VAR|

==== ブログ記事等

 * @<href>{https://yoku0825.blogspot.com/2017/04/mysql-801joinorder.html, https://yoku0825.blogspot.com/2017/04/mysql-801joinorder.html}

//embed[latex]{
\clearpage
//}

 * @<href>{https://variable.jp/2017/09/28/mysql8-0%E3%81%AB%E3%81%8A%E3%81%91%E3%82%8B%E3%83%92%E3%83%B3%E3%83%88%E5%8F%A5%E3%81%AE%E6%8B%A1%E5%BC%B5/, https://variable.jp/2017/09/28/mysql8-0%E3%81%AB%E3%81%8A%E3%81%91%E3%82%8B}@<br>{}@<href>{https://variable.jp/2017/09/28/mysql8-0%E3%81%AB%E3%81%8A%E3%81%91%E3%82%8B%E3%83%92%E3%83%B3%E3%83%88%E5%8F%A5%E3%81%AE%E6%8B%A1%E5%BC%B5/, %E3%83%92%E3%83%B3%E3%83%88%E5%8F%A5%E3%81%AE%E6%8B%A1%E5%BC%B5/}
 * @<href>{https://mysql.wisborg.dk/2020/04/28/mysql-8-0-20-index-level-optimizer-hints/, https://mysql.wisborg.dk/2020/04/28/mysql-8-0-20-index-level-optimizer-hints/}

==={80210801} オプティマイザスイッチ

@<tt>|subquery_to_derived|・@<tt>|prefer_ordering_index|（および@<tt>|hypergraph_optimizer|：8.0.22時点ではデバッグビルドのみ）が追加されました。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_optimizer_switch, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_optimizer_switch, sysvar_optimizer_switch}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/switchable-optimizations.html, https://dev.mysql.com/doc/refman/8.0/ja/switchable-optimizations.html}

=== @<idx>{Skip Scan Range Access Method}

@<idx>{複合インデックス}の1番目の列が検索条件に入っていない場合に当該インデックスを利用して検索する仕組みです。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/range-optimization.html#range-access-skip-scan, https://dev.mysql.com/doc/refman/8.0/ja/range-optimization.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/range-optimization.html#range-access-skip-scan, range-access-skip-scan}

==== ブログ記事等

 * 著者ブログ
 ** @<href>{https://qiita.com/hmatsu47/items/d83bda0360728d4f585a, https://qiita.com/hmatsu47/items/d83bda0360728d4f585a}

==== 実行例

//emlist{
mysql> @<i>{CREATE DATABASE skipscan_test;}
Query OK, 1 row affected (0.01 sec)

mysql> @<i>{USE skipscan_test;}
Database changed
mysql> @<i>{CREATE TABLE t1 (f1 INT NOT NULL, f2 INT NOT NULL, PRIMARY KEY(f1, f2));}
Query OK, 0 rows affected (0.02 sec)

mysql> @<i>{INSERT INTO t1 VALUES}
    -> @<i>{  (1,1), (1,2), (1,3), (1,4), (1,5),}
    -> @<i>{  (2,1), (2,2), (2,3), (2,4), (2,5);}
Query OK, 10 rows affected (0.01 sec)
Records: 10  Duplicates: 0  Warnings: 0

mysql> @<i>{INSERT INTO t1 SELECT f1, f2 + 5 FROM t1;}
Query OK, 10 rows affected (0.01 sec)
Records: 10  Duplicates: 0  Warnings: 0

mysql> @<i>{INSERT INTO t1 SELECT f1, f2 + 10 FROM t1;}
Query OK, 20 rows affected (0.00 sec)
Records: 20  Duplicates: 0  Warnings: 0

mysql> @<i>{SELECT * FROM t1;}
+----+----+
| f1 | f2 |
+----+----+
|  1 |  1 |
@<b>{（中略）}
|  2 | 20 |
+----+----+
40 rows in set (0.00 sec)

mysql> @<i>{ANALYZE TABLE t1;}
+------------------+---------+----------+----------+
| Table            | Op      | Msg_type | Msg_text |
+------------------+---------+----------+----------+
| skipscan_test.t1 | analyze | status   | OK       |
+------------------+---------+----------+----------+
1 row in set (0.00 sec)
@<b>{※テーブルの統計情報を更新しておく。}

mysql> @<i>{EXPLAIN SELECT f1, f2 FROM t1 WHERE f2 > 10\G}
*************************** 1. row ***************************
           id: 1
  select_type: SIMPLE
        table: t1
   partitions: NULL
         type: range
possible_keys: PRIMARY
          key: PRIMARY
      key_len: 8
          ref: NULL
         rows: 13
     filtered: 100.00
        Extra: Using where; Using index for skip scan
1 row in set, 1 warning (0.00 sec)
@<b>{※「Using index for skip scan」が表示された。}
//}

====[column] @<tt>|暗黙のGROUP BYソートの廃止|

以下のリンクの通りMySQL 5.6の時点で非推奨とされていましたが、@<hidx>{暗黙のGROUP BYソート}暗黙の@<tt>|GROUP BY|ソートはMySQL 8.0で廃止になっています。

 * @<href>{https://dev.mysql.com/doc/refman/5.6/ja/order-by-optimization.html}
 ** 注記「MySQL 5.6 における暗黙の GROUP BY ソートへの依存は…」

====[/column]

=== @<idx>{Hash Join}（@<idx>{ハッシュジョイン}）

8.0.18にて、Hash Joinをサポートしました（8.0.20にて等結合以外の内部結合や外部結合、@<idx>{セミジョイン}・@<idx>{アンチジョイン}にも対応）。

なお、@<hidx>{EXPLAIN FORMAT=TREE}@<tt>|EXPLAIN FORMAT=TREE|または@<hidx>{EXPLAIN ANALYZE}@<tt>|EXPLAIN ANALYZE|でHash Joinを含む実行計画を表示することが可能です。

===={80230801} 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/hash-joins.html, https://dev.mysql.com/doc/refman/8.0/ja/hash-joins.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/explain.html#explain-analyze, https://dev.mysql.com/doc/refman/8.0/ja/explain.html#explain-analyze}
 ** @<tt>|EXPLAIN FORMAT=TREE|・@<tt>|EXPLAIN FORMAT=JSON|・@<tt>|EXPLAIN ANALYZE|では内部的にQuery cast injectionが使用される（8.0.18～）
 * @<href>{https://dev.mysql.com/worklog/task/?id=13459, https://dev.mysql.com/worklog/task/?id=13459}
 ** 8.0.23で高速化

==== ブログ記事等

 * @<href>{https://tombo2.hatenablog.com/entry/2019/10/14/212100, https://tombo2.hatenablog.com/entry/2019/10/14/212100}
 * @<href>{https://atsuizo.hatenadiary.jp/entry/2019/10/18/090000, https://atsuizo.hatenadiary.jp/entry/2019/10/18/090000}
 ** @<tt>|EXPLAIN ANALYZE|の制約（8.0.20で外部結合にも対応）
 * @<href>{https://www.percona.com/blog/2019/10/30/understanding-hash-joins-in-mysql-8/, https://www.percona.com/blog/2019/10/30/understanding-hash-joins-in-mysql-8/}
 * @<href>{https://mysqlserverteam.com/mysql-explain-analyze/, https://mysqlserverteam.com/mysql-explain-analyze/}
 * @<href>{https://mysqlserverteam.com/hash-join-in-mysql-8/, https://mysqlserverteam.com/hash-join-in-mysql-8/}
 * @<href>{https://mita2db.hateblo.jp/entry/2020/05/03/174101, https://mita2db.hateblo.jp/entry/2020/05/03/174101}
 * 著者ブログ
 ** @<href>{https://qiita.com/hmatsu47/items/e9d3d4396fea42c8960e, https://qiita.com/hmatsu47/items/e9d3d4396fea42c8960e}
 ** @<href>{https://qiita.com/hmatsu47/items/e473a3e566b910d61f5b, https://qiita.com/hmatsu47/items/e473a3e566b910d61f5b}

==== 実行例

//emlist{
mysql> @<i>{CREATE DATABASE hashjoin_test;}
Query OK, 1 row affected (0.01 sec)

mysql> @<i>{USE hashjoin_test;}
Database changed
mysql> @<i>{CREATE TABLE t1 (col1 INT);}
Query OK, 0 rows affected (0.09 sec)

mysql> @<i>{CREATE TABLE t2 (col1 INT);}
Query OK, 0 rows affected (0.03 sec)
@<b>{※ここでデータを投入。各テーブル10万行ずつ、ランダムな値で。}

mysql> @<i>{SELECT COUNT(*) FROM t1;}
+----------+
| COUNT(*) |
+----------+
|   100000 |
+----------+
1 row in set (0.01 sec)

mysql> @<i>{SELECT COUNT(*) FROM t2;}
+----------+
| COUNT(*) |
+----------+
|   100000 |
+----------+
1 row in set (0.02 sec)

mysql> @<i>{SELECT * FROM t1 LIMIT 3;}
+------+
| col1 |
+------+
|  895 |
|  944 |
|   36 |
+------+
3 rows in set (0.00 sec)

mysql> @<i>{SELECT * FROM t2 LIMIT 3;}
+------+
| col1 |
+------+
|  964 |
|  237 |
|  296 |
+------+
3 rows in set (0.00 sec)

mysql> @<i>{EXPLAIN FORMAT=tree SELECT * FROM t1 LEFT JOIN t2 ON t1.col1 = t2.col1\G}
*************************** 1. row ***************************
EXPLAIN: -> Left hash join (t2.col1 = t1.col1)  (cost=1005095728.81 rows=10050955650)
    -> Table scan on t1  (cost=10072.15 rows=100159)
    -> Hash
        -> Table scan on t2  (cost=0.10 rows=100350)

1 row in set (0.00 sec)

mysql> @<i>{EXPLAIN SELECT * FROM t1 LEFT JOIN t2 ON t1.col1 = t2.col1\G}
*************************** 1. row ***************************
           id: 1
  select_type: SIMPLE
        table: t1
   partitions: NULL
         type: ALL
possible_keys: NULL
          key: NULL
      key_len: NULL
          ref: NULL
         rows: 100159
     filtered: 100.00
        Extra: NULL
*************************** 2. row ***************************
           id: 1
  select_type: SIMPLE
        table: t2
   partitions: NULL
         type: ALL
possible_keys: NULL
          key: NULL
      key_len: NULL
          ref: NULL
         rows: 100350
     filtered: 100.00
        Extra: Using where; Using join buffer (hash join)
2 rows in set, 1 warning (0.00 sec)
@<b>{※「hash join」が表示されている。}

mysql> @<i>{SELECT COUNT(t1.col1) FROM t1 LEFT JOIN t2 ON t1.col1 = t2.col1;}
+----------------+
| COUNT(t1.col1) |
+----------------+
|       10002205 |
+----------------+
1 row in set (0.68 sec)
@<b>{※8.0.19はLEFT(OUTER)JOINに対応していないので、同じ処理に50秒以上掛かる。}
//}

==={80210802} @<tt>|UPDATE|・@<tt>|DELETE|で@<idx>{セミジョイン}（準結合）・@<idx>{マテリアライズ}（実体化）最適化をサポート

8.0.21にて、@<tt>|IN|・@<tt>|EXISTS|句のサブクエリを使った更新・削除が高速化されました。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/semijoins.html, https://dev.mysql.com/doc/refman/8.0/ja/semijoins.html}
 ** 「MySQL 8.0.21以降では、…」

==== ブログ記事等

 * 著者ブログ
 ** @<href>{https://qiita.com/hmatsu47/items/aa687aa30c4570bac861, https://qiita.com/hmatsu47/items/aa687aa30c4570bac861}

==== 実行例

@<tt>|UPDATE|で試してみます。

//emlist{
mysql> @<i>{USE multitable_test;}
Database changed

@<b>{※テーブル構造は以下の通り。}

mysql> @<i>{SHOW CREATE TABLE t1\G}
*************************** 1. row ***************************
       Table: t1
Create Table: CREATE TABLE `t1` (
  `id` int NOT NULL AUTO_INCREMENT,
  `key` int NOT NULL,
  PRIMARY KEY (`id`),
  KEY `key` (`key`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci
1 row in set (0.00 sec)

mysql> @<i>{SHOW CREATE TABLE t2\G}
*************************** 1. row ***************************
       Table: t2
Create Table: CREATE TABLE `t2` (
  `id` int NOT NULL AUTO_INCREMENT,
  `val` int NOT NULL,
  `key` int NOT NULL,
  PRIMARY KEY (`id`),
  KEY `valkey` (`val`,`key`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci
1 row in set (0.00 sec)

@<b>{※それぞれ50行のデータを投入。}

mysql> @<i>{EXPLAIN UPDATE t1 SET t1.key = 0 WHERE t1.key IN (SELECT t2.key FROM t2 WHERE t2.val IN}
@<i>{(40, 60, 100))\G}
*************************** 1. row ***************************
           id: 1
  select_type: UPDATE
        table: t1
   partitions: NULL
         type: ALL
possible_keys: key
          key: NULL
      key_len: NULL
          ref: NULL
         rows: 44
     filtered: 100.00
        Extra: Using where
*************************** 2. row ***************************
           id: 1
  select_type: SIMPLE
        table: <subquery2>
   partitions: NULL
         type: eq_ref
possible_keys: <auto_distinct_key>
          key: <auto_distinct_key>
      key_len: 4
          ref: multitable_test.t1.key
         rows: 1
     filtered: 100.00
        Extra: NULL
*************************** 3. row ***************************
           id: 2
  select_type: MATERIALIZED
        table: t2
   partitions: NULL
         type: range
possible_keys: valkey
          key: valkey
      key_len: 4
          ref: NULL
         rows: 15
     filtered: 100.00
        Extra: Using where; Using index
3 rows in set, 1 warning (0.00 sec)

@<b>{※実行計画としてサブクエリのマテリアライズが選択されている。8.0.20までは「DEPENDENT SUBQUERY」だった。}

@<b>{※データを約10万行まで増やして時間計測。いずれもデータがバッファプールに載っている状態で実行。}

mysql> @<i>{SET AUTOCOMMIT=0;}
Query OK, 0 rows affected (0.00 sec)

mysql> @<i>{UPDATE t1 SET t1.key = 0 WHERE t1.key IN (SELECT /*+ SUBQUERY(INTOEXISTS) */ t2.key FRO}
@<i>{M t2 WHERE t2.val IN (40, 60, 100));}
Query OK, 24576 rows affected (2.71 sec)
Rows matched: 24576  Changed: 24576  Warnings: 0

@<b>{※マテリアライズを無効化して実行。実行計画は8.0.20以前と同じ。3回実行した平均は2.78秒。}

mysql> @<i>{ROLLBACK;}
Query OK, 0 rows affected (0.95 sec)

mysql> @<i>{SET AUTOCOMMIT=0;}
Query OK, 0 rows affected (0.00 sec)

mysql> @<i>{UPDATE t1 SET t1.key = 0 WHERE t1.key IN (SELECT t2.key FROM t2 WHERE t2.val IN (40, 60,}
@<i>{100));}
Query OK, 24576 rows affected (1.08 sec)
Rows matched: 24576  Changed: 24576  Warnings: 0

@<b>{※マテリアライズ有効（デフォルト）で実行。3回実行した平均は1.07秒。2～3倍高速化した。}

mysql> @<i>{ROLLBACK;}
Query OK, 0 rows affected (0.99 sec)
//}

==={80220801} @<idx>{Derived Condition Pushdown}最適化

8.0.22にて、主クエリ側にある@<tt>|WHERE|句などの条件をサブクエリに直接適用することで処理を効率化できるようになりました。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/derived-condition-pushdown-optimization.html, https://dev.mysql.com/doc/refman/8.0/ja/derived-condition-pushdown-optimization.html}

==== ブログ記事等

 * 著者ブログ
 ** @<href>{https://qiita.com/hmatsu47/items/c15b1c778d050913201d, https://qiita.com/hmatsu47/items/c15b1c778d050913201d}

==== 実行例

//emlist{
mysql> @<i>{CREATE DATABASE subquery_test;}
Query OK, 1 row affected (0.02 sec)

mysql> @<i>{USE subquery_test;}
Database changed
mysql> @<i>{CREATE TABLE t1 (i INT NOT NULL, j INT NOT NULL, k INT NOT NULL);}
Query OK, 0 rows affected (0.12 sec)

mysql> @<i>{INSERT INTO t1 VALUES(1000 * RAND(), 1000 * RAND(), 1000 * RAND());}
@<b>{※これで 2 万行挿入後、INSERT INTO ... SELECT を 5 回繰り返して 64 万行に増やした。}

mysql> @<i>{ALTER TABLE t1 ADD INDEX (i, j);}
@<b>{※INDEXがないと高速化しづらいのでINDEXを作成。}

mysql> @<i>{EXPLAIN SELECT /*+ NO_DERIVED_CONDITION_PUSHDOWN() */ * FROM (SELECT i, j, SUM(k) FROM }
@<i>{t1 GROUP BY i, j) AS dt WHERE i < 10 AND j > 990\G}
*************************** 1. row ***************************
           id: 1
  select_type: PRIMARY
        table: <derived2>
   partitions: NULL
         type: ALL
possible_keys: NULL
          key: NULL
      key_len: NULL
          ref: NULL
         rows: 638976
     filtered: 11.11
        Extra: Using where
*************************** 2. row ***************************
           id: 2
  select_type: DERIVED
        table: t1
   partitions: NULL
         type: index
possible_keys: i
          key: i
      key_len: 8
          ref: NULL
         rows: 638976
     filtered: 100.00
        Extra: NULL
2 rows in set, 1 warning (0.00 sec)

@<b>{※プッシュダウン無効の場合は、サブクエリの処理（id:2）でフルINDEXスキャンが行われる。}

mysql> @<i>{EXPLAIN SELECT * FROM (SELECT i, j, SUM(k) FROM t1 GROUP BY i, j) AS dt WHERE i < 10 AN}
@<i>{D j > 990\G}
*************************** 1. row ***************************
           id: 1
  select_type: PRIMARY
        table: <derived2>
   partitions: NULL
         type: ALL
possible_keys: NULL
          key: NULL
      key_len: NULL
          ref: NULL
         rows: 2058
     filtered: 100.00
        Extra: NULL
*************************** 2. row ***************************
           id: 2
  select_type: DERIVED
        table: t1
   partitions: NULL
         type: range
possible_keys: i
          key: i
      key_len: 4
          ref: NULL
         rows: 6176
     filtered: 33.33
        Extra: Using index condition
2 rows in set, 1 warning (0.00 sec)

@<b>{※プッシュダウン有効の場合はサブクエリの処理（id:2）でINDEXによる絞り込みが行われる。}
//}

=== その他のオプティマイザ新機能

==== デフォルトの@<idx>{オプティマイザトレース}バッファ容量の拡大

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_optimizer_trace_max_mem_size, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_optimizer_trace_max_mem_size, sysvar_optimizer_trace_max_mem_size}

==== @<hidx>{LIKE検索}@<tt>|LIKE|検索時の部分インデックスの適正利用

 * @<href>{https://dev.mysql.com/doc/relnotes/mysql/8.0/en/news-8-0-11.html, https://dev.mysql.com/doc/relnotes/mysql/8.0/en/news-8-0-11.html}
 ** 「When using a partial index, the optimizer performed a more expensive table lookup …」
 * @<href>{https://bugs.mysql.com/bug.php?id=74359, https://bugs.mysql.com/bug.php?id=74359}

==== @<hidx>{INSERT}@<tt>|INSERT|／@<hidx>{UPDATE}@<tt>|UPDATE|／@<hidx>{REPLACE}@<tt>|REPLACE|／@<hidx>{DELETE}@<tt>|DELETE|に対する@<hidx>{EXPLAIN EXTENDED}@<tt>|EXPLAIN EXTENDED|

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/explain-extended.html, https://dev.mysql.com/doc/refman/8.0/ja/explain-extended.html}
 ** 「MySQL 8.0.12では、…」

==== @<hidx>{IN}@<tt>|IN|・@<hidx>{EXISTS}@<tt>|EXISTS|サブクエリの@<idx>{アンチジョイン}・@<idx>{セミジョイン}最適化

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/semijoins.html, https://dev.mysql.com/doc/refman/8.0/ja/semijoins.html}
 * @<href>{https://gihyo.jp/dev/serial/01/mysql-road-construction-news/0103, https://gihyo.jp/dev/serial/01/mysql-road-construction-news/0103}
 * @<href>{https://mysqlserverteam.com/antijoin-in-mysql-8/, https://mysqlserverteam.com/antijoin-in-mysql-8/}

===={80220802} @<hidx>{PREPARE}@<tt>|PREPARE|の実行アルゴリズムが変更（@<hidx>{ORDER BY}@<tt>|ORDER BY ?|での列番号指定などが無視されるように）

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/prepare.html, https://dev.mysql.com/doc/refman/8.0/ja/prepare.html}
 * @<href>{https://tombo2.hatenablog.com/entry/2020/10/29/135053, https://tombo2.hatenablog.com/entry/2020/10/29/135053}
 * @<href>{https://tmtms.hatenablog.com/entry/202011/mysql-8022-prepared-statement, https://tmtms.hatenablog.com/entry/202011/mysql-8022-prepared-statement}
 * @<href>{https://tombo2.hatenablog.com/entry/2020/11/09/125259, https://tombo2.hatenablog.com/entry/2020/11/09/125259}

===={80240801} @<idx>{相関スカラーサブクエリ}最適化

 * @<href>{https://dev.mysql.com/doc/refman/8.0/en/correlated-subqueries.html, https://dev.mysql.com/doc/refman/8.0/en/correlated-subqueries.html}
 * @<href>{https://qiita.com/hmatsu47/items/ff3f68ae0c3f5d988c90, https://qiita.com/hmatsu47/items/ff3f68ae0c3f5d988c90}

== InnoDB

MySQL 8.0では、地味なものが多いですがInnoDBも細かい改良が進んでいます。

=== 新しい@<idx>{ロック}：@<hidx>{NOWAIT}@<tt>|NOWAIT|／@<hidx>{SKIP LOCKED}@<tt>|SKIP LOCKED|

@<hidx>{SELECT ～ FOR UPDATE}@<tt>|SELECT| ～ @<tt>|FOR UPDATE|等によって@<idx>{行ロック}の獲得を試みてすぐに獲得できなかったとき、獲得を待たずに処理を進める機能が追加されました。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-locking-reads.html#innodb-locking-reads-nowait-skip-locked, https://dev.mysql.com/doc/refman/8.0/ja/innodb-locking-reads.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-locking-reads.html#innodb-locking-reads-nowait-skip-locked, innodb-locking-reads-nowait-skip-locked}

==== ブログ記事等

 * @<href>{https://mysqlserverteam.com/mysql-8-0-1-using-skip-locked-and-nowait-to-handle-hot-rows/, https://mysqlserverteam.com/mysql-8-0-1-using-skip-locked-and-nowait-to-handle-hot-rows/}
 * @<href>{https://yoku0825.blogspot.com/2018/04/mysql-80-select-for-update-skip-locked.html, https://yoku0825.blogspot.com/2018/04/mysql-80-select-for-update-skip-locked.html}
 * 著者ブログ
 ** @<href>{https://qiita.com/hmatsu47/items/7675b026e65762d2445f, https://qiita.com/hmatsu47/items/7675b026e65762d2445f}

==== 実行例

//emlist{
@<b>{※クライアント1で実行（準備）。}
mysql> @<i>{CREATE DATABASE skiplock_test;}
Query OK, 1 row affected (0.01 sec)

mysql> @<i>{USE skiplock_test;}
Database changed
mysql> @<i>{CREATE TABLE seat (}
    -> @<i>{  id INT PRIMARY KEY AUTO_INCREMENT,}
    -> @<i>{  flight_date DATE NOT NULL,}
    -> @<i>{  flight_number VARCHAR(10) NOT NULL,}
    -> @<i>{  seat_number VARCHAR(10) NOT NULL,}
    -> @<i>{  reserved BOOLEAN NOT NULL DEFAULT false,}
    -> @<i>{  UNIQUE (flight_date, flight_number, seat_number));}
Query OK, 0 rows affected (0.04 sec)

mysql> @<i>{SELECT * FROM seat ORDER BY id;}
+----+-------------+---------------+-------------+----------+
| id | flight_date | flight_number | seat_number | reserved |
+----+-------------+---------------+-------------+----------+
|  1 | 2020-05-15  | RAC 801       | A2          |        0 |
|  2 | 2020-05-15  | RAC 801       | A3          |        0 |
@<b>{（中略）}
| 12 | 2020-05-15  | RAC 801       | A13         |        0 |
| 13 | 2020-05-15  | RAC 801       | C2          |        0 |
| 14 | 2020-05-15  | RAC 801       | C3          |        0 |
@<b>{（中略）}
| 24 | 2020-05-15  | RAC 801       | C13         |        0 |
| 25 | 2020-05-15  | RAC 801       | H1          |        0 |
| 26 | 2020-05-15  | RAC 801       | H2          |        0 |
| 27 | 2020-05-15  | RAC 801       | H3          |        0 |
@<b>{（中略）}
| 37 | 2020-05-15  | RAC 801       | H13         |        0 |
| 38 | 2020-05-15  | RAC 801       | K1          |        0 |
| 39 | 2020-05-15  | RAC 801       | K2          |        0 |
| 40 | 2020-05-15  | RAC 801       | K3          |        0 |
@<b>{（中略）}
| 50 | 2020-05-15  | RAC 801       | K13         |        0 |
+----+-------------+---------------+-------------+----------+
50 rows in set (0.00 sec)

@<b>{※クライアント1で実行。「K3」席の行ロックを獲得する。}
mysql> @<i>{START TRANSACTION;}
Query OK, 0 rows affected (0.00 sec)

mysql> @<i>{SELECT * FROM seat WHERE flight_date = '2020-05-15' AND flight_number = 'RAC 801'}
    -> @<i>{  AND seat_number = 'K3' AND reserved IS false FOR UPDATE;}
+----+-------------+---------------+-------------+----------+
| id | flight_date | flight_number | seat_number | reserved |
+----+-------------+---------------+-------------+----------+
| 40 | 2020-05-15  | RAC 801       | K3          |        0 |
+----+-------------+---------------+-------------+----------+
1 row in set (0.00 sec)

@<b>{※クライアント2で実行。「K3」席の行ロック獲得を「NOWAIT」で試みる。}
mysql> @<i>{USE skiplock_test;}
Database changed
mysql> @<i>{START TRANSACTION;}
Query OK, 0 rows affected (0.00 sec)

mysql> @<i>{SELECT * FROM seat WHERE flight_date = '2020-05-15' AND flight_number = 'RAC 801'}
    -> @<i>{  AND seat_number = 'K3' AND reserved IS false FOR UPDATE NOWAIT;}
ERROR 3572 (HY000): Statement aborted because lock(s) could not be acquired immediately and NO
WAIT is set.
@<b>{※行ロックが獲得できなかったので即座ににエラーが返る。}

@<b>{※クライアント3で実行。「A3」「C3」「H3」「K3」席をまとめて、行ロック獲得を「SKIP LOCKED」で試みる。}
mysql> @<i>{USE skiplock_test;}
Database changed
mysql> @<i>{START TRANSACTION;}
Query OK, 0 rows affected (0.00 sec)

mysql> @<i>{SELECT * FROM seat WHERE flight_date = '2020-05-15' AND flight_number = 'RAC 801'}
    -> @<i>{  AND seat_number LIKE '_3' AND reserved IS false FOR UPDATE SKIP LOCKED;}
+----+-------------+---------------+-------------+----------+
| id | flight_date | flight_number | seat_number | reserved |
+----+-------------+---------------+-------------+----------+
|  2 | 2020-05-15  | RAC 801       | A3          |        0 |
| 14 | 2020-05-15  | RAC 801       | C3          |        0 |
| 27 | 2020-05-15  | RAC 801       | H3          |        0 |
+----+-------------+---------------+-------------+----------+
3 rows in set (0.00 sec)
@<b>{※「K3」席以外の行ロックが即座に獲得できた。}
//}

=== @<idx>{ノンロッキング並列読み取り}

8.0.15時点では@<hidx>{CHECK TABLE}@<tt>|CHECK TABLE|・@<hidx>{SELECT COUNT(*)}@<tt>|SELECT COUNT(*)|など利用可能なケースがかなり限られますが、@<idx>{並列読み取り}（@<idx>{パラレルスキャン}）に対応しました。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/check-table.html#check-table-innodb, https://dev.mysql.com/doc/refman/8.0/ja/check-table.html#check-table-innodb}
 ** 「MySQL 8.0.14では、…」
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-parameters.html#sysvar_innodb_parallel_read_threads, https://dev.mysql.com/doc/refman/8.0/ja/innodb-parameters.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-parameters.html#sysvar_innodb_parallel_read_threads, sysvar_innodb_parallel_read_threads}

==== ブログ記事等

 * @<href>{https://atsuizo.hatenadiary.jp/entry/2019/01/23/112608, https://atsuizo.hatenadiary.jp/entry/2019/01/23/112608}
 * @<href>{https://atsuizo.hatenadiary.jp/entry/2019/01/24/090000, https://atsuizo.hatenadiary.jp/entry/2019/01/24/090000}
 * @<href>{https://atsuizo.hatenadiary.jp/entry/2019/01/26/090000, https://atsuizo.hatenadiary.jp/entry/2019/01/26/090000}
 * @<href>{https://atsuizo.hatenadiary.jp/entry/2019/01/28/090000, https://atsuizo.hatenadiary.jp/entry/2019/01/28/090000}
 * @<href>{https://atsuizo.hatenadiary.jp/entry/2019/01/29/090000, https://atsuizo.hatenadiary.jp/entry/2019/01/29/090000}
 ** グループ化せずに@<tt>|SELECT COUNT(*)|を高速化

=== @<hidx>{AUTO_INCREMENT値}@<tt>|AUTO_INCREMENT|値の@<idx>{永続化}

MySQL 5.7まではサーバを再起動すると各テーブルの@<tt>|AUTO_INCREMENT|値が「最大の値を持つ行の次の値」@<fn>{010AIV}に自動的に設定されていました。
これにより、@<idx>{行削除}や@<idx>{トランザクション}の@<idx>{ロールバック}などが原因で@<tt>|AUTO_INCREMENT|列の値に空き番号が存在した場合に番号が巻き戻ることがありましたが、MySQL 8.0では正しく@<tt>|AUTO_INCREMENT|値を保持するようになりました。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-auto-increment-handling.html#innodb-auto-increment-initialization, https://dev.mysql.com/doc/refman/8.0/ja/innodb-auto-increment-handling.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-auto-increment-handling.html#innodb-auto-increment-initialization, innodb-auto-increment-initialization}

==== ブログ記事等

 * @<href>{https://www.s-style.co.jp/blog/2018/08/2284/, https://www.s-style.co.jp/blog/2018/08/2284/}
 * 著者ブログ
 ** @<href>{https://qiita.com/hmatsu47/items/4429171c1bbaba564774, https://qiita.com/hmatsu47/items/4429171c1bbaba564774}

==== 実行例

//emlist{
mysql> @<i>{CREATE DATABASE autoinc_test;}
Query OK, 1 row affected (0.00 sec)

mysql> @<i>{USE autoinc_test;}
Database changed
mysql> @<i>{CREATE TABLE t1 (id int(10) PRIMARY KEY AUTO_INCREMENT, name VARCHAR(100)) ENGINE=innod}
@<i>{b;}
Query OK, 0 rows affected (0.02 sec)

mysql> @<i>{INSERT INTO t1 SET name = 'first';}
Query OK, 1 row affected (0.01 sec)

mysql> @<i>{INSERT INTO t1 SET name = 'second';}
Query OK, 1 row affected (0.00 sec)

mysql> @<i>{INSERT INTO t1 SET name = 'third';}
Query OK, 1 row affected (0.00 sec)

mysql> @<i>{SELECT * FROM t1;}
+----+--------+
| id | name   |
+----+--------+
|  1 | first  |
|  2 | second |
|  3 | third  |
+----+--------+
3 rows in set (0.00 sec)

mysql> @<i>{DELETE FROM t1 WHERE id = 3;}
Query OK, 1 row affected (0.00 sec)
@<b>{※id=3の行を削除したところで、MySQLサーバを再起動。}

mysql> @<i>{USE autoinc_test;}
Reading table information for completion of table and column names
You can turn off this feature to get a quicker startup with -A

Database changed
mysql> @<i>{SELECT * FROM t1;}
+----+--------+
| id | name   |
+----+--------+
|  1 | first  |
|  2 | second |
+----+--------+
2 rows in set (0.00 sec)

mysql> @<i>{INSERT INTO t1 SET name = 'fourth';}
Query OK, 1 row affected (0.00 sec)

mysql> @<i>{SELECT * FROM t1;}
+----+--------+
| id | name   |
+----+--------+
|  1 | first  |
|  2 | second |
|  4 | fourth |
+----+--------+
3 rows in set (0.00 sec)
@<b>{※MySQL 5.7までとは異なり、再起動後にid=3は再利用されずにid=4の行が挿入された。}
//}

==={80230802} テーブルスペース／Redo・Undoログ／一般テーブルスペース／システムテーブル等の暗号化

MySQL 5.7では@<idx>{テーブルスペース}だけが対象だった@<idx>{透過的暗号化}機能が、MySQL 8.0では

 * @<idx>{Redoログ}
 * @<idx>{Undoログ}
 * @<idx>{一般テーブルスペース}
 * @<idx>{バイナリログ}／@<idx>{リレーログ}（第7章参照）
 * @<idx>{システムテーブル}（@<tt>|mysql|スキーマ）
 * @<idx>{ダブルライトファイル}

まで対象が増えました。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-data-encryption.html, https://dev.mysql.com/doc/refman/8.0/ja/innodb-data-encryption.html}
 ** @<idx>{透過的暗号化}（@<idx>{TDE}）全体の解説
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-data-encryption.html#innodb-data-encryption-redo-log, https://dev.mysql.com/doc/refman/8.0/ja/innodb-data-encryption.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-data-encryption.html#innodb-data-encryption-redo-log, innodb-data-encryption-redo-log}
 ** @<idx>{Redoログ暗号化}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-data-encryption.html#innodb-data-encryption-undo-log, https://dev.mysql.com/doc/refman/8.0/ja/innodb-data-encryption.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-data-encryption.html#innodb-data-encryption-undo-log, innodb-data-encryption-undo-log}
 ** @<idx>{Undoログ暗号化}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-data-encryption.html#innodb-general-tablespace-encryption-enabling-disabling, https://dev.mysql.com/doc/refman/8.0/ja/innodb-data-encryption.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-data-encryption.html#innodb-general-tablespace-encryption-enabling-disabling, innodb-general-tablespace-encryption-enabling-disabling}
 ** @<idx>{一般テーブルスペース暗号化}

//embed[latex]{
\clearpage
//}

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-data-encryption.html#innodb-mysql-tablespace-encryption-enabling-disabling, https://dev.mysql.com/doc/refman/8.0/ja/innodb-data-encryption.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-data-encryption.html#innodb-mysql-tablespace-encryption-enabling-disabling, innodb-mysql-tablespace-encryption-enabling-disabling}
 ** @<idx>{システムテーブル暗号化}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-data-encryption.html#innodb-doublewrite-file-encryption, https://dev.mysql.com/doc/refman/8.0/ja/innodb-data-encryption.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-data-encryption.html#innodb-doublewrite-file-encryption, innodb-doublewrite-file-encryption}
 ** @<idx>{ダブルライトファイル}

==== ブログ記事等

 * @<href>{https://mysqlserverteam.com/mysql-8-0-13-innodb-transparent-tablespace-encryption-for-general-tablespaces/, https://mysqlserverteam.com/mysql-8-0-13-innodb-transparent-tablespace-encryption-for-general-tablespaces/}
 * @<href>{https://mita2db.hateblo.jp/entry/MySQL_default_table_encryption_%E3%83%91%E3%83%A9%E3%83%A1%E3%83%BC%E3%82%BF%E3%81%AE%E6%8C%99%E5%8B%95, https://mita2db.hateblo.jp/entry/MySQL_default_table_encryption_%E3%83%91%E3%83}@<br>{}@<href>{https://mita2db.hateblo.jp/entry/MySQL_default_table_encryption_%E3%83%91%E3%83%A9%E3%83%A1%E3%83%BC%E3%82%BF%E3%81%AE%E6%8C%99%E5%8B%95, %A9%E3%83%A1%E3%83%BC%E3%82%BF%E3%81%AE%E6%8C%99%E5%8B%95}
 ** @<tt>|default_table_encryption|パラメータの挙動
 * @<href>{https://mita2db.hateblo.jp/entry/MySQL_table_encryption_privilege_check_%E3%83%91%E3%83%A9%E3%83%A1%E3%83%BC%E3%82%BF%E3%81%AE%E6%8C%99%E5%8B%95, https://mita2db.hateblo.jp/entry/MySQL_table_encryption_privilege_check_%E3%83%91}@<br>{}@<href>{https://mita2db.hateblo.jp/entry/MySQL_table_encryption_privilege_check_%E3%83%91%E3%83%A9%E3%83%A1%E3%83%BC%E3%82%BF%E3%81%AE%E6%8C%99%E5%8B%95, %E3%83%A9%E3%83%A1%E3%83%BC%E3%82%BF%E3%81%AE%E6%8C%99%E5%8B%95}
 ** @<tt>|table_encryption_privilege_check|パラメータの挙動
 * 著者ブログ
 ** @<href>{https://qiita.com/hmatsu47/items/bae53fd0f6d09511732c, https://qiita.com/hmatsu47/items/bae53fd0f6d09511732c}
 ** @<href>{https://qiita.com/hmatsu47/items/f3519532c134ba0018af, https://qiita.com/hmatsu47/items/f3519532c134ba0018af}
 ** @<href>{https://qiita.com/hmatsu47/items/596496b814b6905e1463, https://qiita.com/hmatsu47/items/596496b814b6905e1463}

==== 実行例

テーブルスペース／Redo・Undoログ／システムテーブル／ダブルライトファイル暗号化の実行例です。
一般テーブルスペース暗号化についてはブログ記事等の1つ目または著者ブログの2つ目をご確認ください。
@<idx>{バイナリログ}／@<idx>{リレーログ}暗号化については@<b>{第7章 レプリケーションの新機能 7.1 バイナリログ／リレーログ暗号化}をご確認ください。

まず、@<tt>|/etc/my.cnf|に設定を追加します。@<fn>{020ENC}

//list[master][/etc/my.cnf追記部分]{
early-plugin-load=keyring_file.so
keyring_file_data=/var/lib/mysql-keyring/keyring
# innodb_doublewrite=0
# 8.0.23からダブルライトを無効にしなくても正式にダブルライトファイルに平文で書き出されることがなくなった（↑の1行が不要に）。
innodb_redo_log_encrypt=1
innodb_undo_log_encrypt=1
//}

次にサーバを再起動します。
再起動後、暗号化テーブルを作成してみます。

//emlist{
mysql> @<i>{CREATE TABLE enc_test (id int(10) PRIMARY KEY AUTO_INCREMENT, value VARCHAR(100)) ENGIN}
@<i>{E=innodb ENCRYPTION='Y';}
Query OK, 0 rows affected (0.02 sec)

mysql> @<i>{INSERT INTO enc_test SET value='1234567890ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst12345}
@<i>{67890ABCDEFGHIJKLMNOPQRSTabcdefghijklmnopqrst';}
Query OK, 1 row affected (0.01 sec)
@<b>{※複数行入れておく。}

mysql> @<i>{SET AUTOCOMMIT=0;}
Query OK, 0 rows affected (0.00 sec)

mysql> @<i>{UPDATE enc_test SET value='ENCRYPTED' WHERE id=1;}
Query OK, 1 row affected (0.00 sec)
Rows matched: 1  Changed: 1  Warnings: 0

mysql> @<i>{COMMIT;}
Query OK, 0 rows affected (0.01 sec)
@<b>{※ここでOS Shellからファイルシステムに対しENCRYPTEDをgrep検索しても見つからないことが確認できる。}

mysql> @<i>{ALTER TABLESPACE mysql ENCRYPTION = 'Y';}
Query OK, 0 rows affected (0.21 sec)
@<b>{※ここでOS Shellからファイルシステムに対しenc_testをgrep検索すると見つからなくなったことが確認できる。}
//}

====[column] @<tt>|InnoDBテーブルスペース管理について|

こちらが参考になります。

 * @<href>{https://mysqlserverteam.com/innodb-tablespace-space-management/, https://mysqlserverteam.com/innodb-tablespace-space-management/}

====[/column]

=== その他のInnoDB新機能

==== @<hidx>{CREATE UNDO TABLESPACE}@<tt>|CREATE|／@<hidx>{ALTER UNDO TABLESPACE}@<tt>|ALTER|／@<hidx>{DROP UNDO TABLESPACE}@<tt>|DROP UNDO TABLESPACE|

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-undo-tablespaces.html#innodb-add-undo-tablespaces, https://dev.mysql.com/doc/refman/8.0/ja/innodb-undo-tablespaces.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-undo-tablespaces.html#innodb-add-undo-tablespaces, innodb-add-undo-tablespaces}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-undo-tablespaces.html#innodb-drop-undo-tablespaces, https://dev.mysql.com/doc/refman/8.0/ja/innodb-undo-tablespaces.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-undo-tablespaces.html#innodb-drop-undo-tablespaces, innodb-drop-undo-tablespaces}
 * @<href>{https://mysqlserverteam.com/new-in-mysql-8-0-14-create-undo-tablespace/, https://mysqlserverteam.com/new-in-mysql-8-0-14-create-undo-tablespace/}

==== @<idx>{ダブルライトバッファ}がInnoDB@<idx>{システムテーブルスペース}から分離可能に

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-doublewrite-buffer.html, https://dev.mysql.com/doc/refman/8.0/ja/innodb-doublewrite-buffer.html}
 * @<href>{https://next4us-ti.hatenablog.com/entry/2020/05/07/161812, https://next4us-ti.hatenablog.com/entry/2020/05/07/161812}

==== 適応型の@<idx>{スキャンバッファ}サイズ調整

 * @<href>{https://dev.mysql.com/worklog/task/?id=7093, https://dev.mysql.com/worklog/task/?id=7093}

==== @<idx>{ストレージエンジン}APIにおけるサンプリングインターフェース

 * @<href>{https://dev.mysql.com/doc/dev/mysql-server/8.0.14/classhandler.html#a684f7429844b6a5061d5942e6f12b573, https://dev.mysql.com/doc/dev/mysql-server/8.0.14/classhandler.html#}@<br>{}@<href>{https://dev.mysql.com/doc/dev/mysql-server/8.0.14/classhandler.html#a684f7429844b6a5061d5942e6f12b573, a684f7429844b6a5061d5942e6f12b573}
 * @<href>{https://dev.mysql.com/doc/dev/mysql-server/8.0.14/classhandler.html#a2e052d45aa09050fdfff6e10c67cbd1e, https://dev.mysql.com/doc/dev/mysql-server/8.0.14/classhandler.html#}@<br>{}@<href>{https://dev.mysql.com/doc/dev/mysql-server/8.0.14/classhandler.html#a2e052d45aa09050fdfff6e10c67cbd1e, a2e052d45aa09050fdfff6e10c67cbd1e}
 * @<href>{https://dev.mysql.com/doc/dev/mysql-server/8.0.14/classhandler.html#a167d4bd2f1c5f353d77ed08dbed4c04a, https://dev.mysql.com/doc/dev/mysql-server/8.0.14/classhandler.html#}@<br>{}@<href>{https://dev.mysql.com/doc/dev/mysql-server/8.0.14/classhandler.html#a167d4bd2f1c5f353d77ed08dbed4c04a, a167d4bd2f1c5f353d77ed08dbed4c04a}
 * @<href>{https://dev.mysql.com/doc/dev/mysql-server/8.0.14/sql_2handler_8h.html#ae954cfd46ba0b8300368dccf2bebc842, https://dev.mysql.com/doc/dev/mysql-server/8.0.14/sql_2handler_8h.html#}@<br>{}@<href>{https://dev.mysql.com/doc/dev/mysql-server/8.0.14/sql_2handler_8h.html#ae954cfd46ba0b8300368dccf2bebc842, ae954cfd46ba0b8300368dccf2bebc842}

==== @<idx>{ラッチ}フリーでスケーラブルなRedoログ

 * @<href>{https://dev.mysql.com/worklog/task/?id=10310, https://dev.mysql.com/worklog/task/?id=10310}
 * @<href>{https://mysqlserverteam.com/mysql-8-0-new-lock-free-scalable-wal-design/, https://mysqlserverteam.com/mysql-8-0-new-lock-free-scalable-wal-design/}

==== @<idx>{Redoログアーカイブ}処理

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-redo-log.html#innodb-redo-log-archiving, https://dev.mysql.com/doc/refman/8.0/ja/innodb-redo-log.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-redo-log.html#innodb-redo-log-archiving, innodb-redo-log-archiving}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-parameters.html#sysvar_innodb_redo_log_archive_dirs, https://dev.mysql.com/doc/refman/8.0/ja/innodb-parameters.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-parameters.html#sysvar_innodb_redo_log_archive_dirs, sysvar_innodb_redo_log_archive_dirs}
 * @<href>{https://www.s-style.co.jp/blog/2019/08/5060/, https://www.s-style.co.jp/blog/2019/08/5060/}

===={80210803} Redoログの無効化が可能に

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-redo-log.html#innodb-disable-redo-logging, https://dev.mysql.com/doc/refman/8.0/ja/innodb-redo-log.html#innodb-disable-redo-logging}
 ** 初期データロード（バックアップデータのリストアなど）の時間短縮が可能に（通常稼働時の性能向上を目的とした機能ではない）
 * @<href>{https://atsuizo.hatenadiary.jp/entry/2020/07/16/140000, https://atsuizo.hatenadiary.jp/entry/2020/07/16/140000}

===={80210804} Undoテーブルスペースの処理性能向上と安定化・ACID Undo DDLのサポート

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-undo-tablespaces.html, https://dev.mysql.com/doc/refman/8.0/ja/innodb-undo-tablespaces.html}

==== @<idx>{LOB列}の再設計・改良による高速化

 * @<href>{https://dev.mysql.com/worklog/task/?id=8960, https://dev.mysql.com/worklog/task/?id=8960}
 * @<href>{https://mysqlserverteam.com/mysql-8-0-innodb-introduces-lob-index-for-faster-updates/, https://mysqlserverteam.com/mysql-8-0-innodb-introduces-lob-index-for-faster-updates/}
 * @<href>{https://mysqlserverteam.com/mysql-8-0-mvcc-of-large-objects-in-innodb/, https://mysqlserverteam.com/mysql-8-0-mvcc-of-large-objects-in-innodb/}
 * @<href>{https://mysqlserverteam.com/mysql-8-0-optimizing-small-partial-update-of-lob-in-innodb/, https://mysqlserverteam.com/mysql-8-0-optimizing-small-partial-update-of-lob-in-innodb/}

==== @<idx>{CATS}（the Contention-Aware Transaction Scheduling＝新しい@<idx>{ロックスケジューラ}）

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-transaction-scheduling.html, https://dev.mysql.com/doc/refman/8.0/ja/innodb-transaction-scheduling.html}
 ** 8.0.20より、FIFOアルゴリズムでのトランザクションスケジューリングは廃止されてCATSに一本化
 * @<href>{http://masato.ushio.org/blog/index.php/2018/03/04/uco-tech_mysql-8-0_trx_scheduling_cats/, http://masato.ushio.org/blog/index.php/2018/03/04/uco-tech_mysql-8-0_trx_scheduling_}@<br>{}@<href>{http://masato.ushio.org/blog/index.php/2018/03/04/uco-tech_mysql-8-0_trx_scheduling_cats/, cats/}
 * @<href>{https://mysqlserverteam.com/contention-aware-transaction-scheduling-arriving-in-innodb-to-boost-performance/, https://mysqlserverteam.com/contention-aware-transaction-scheduling-arriving-in-innodb-to-boost-performance/}

==== InnoDB@<idx>{テーブルスペース}@<idx>{バージョン管理}サポート（アップグレード用）

 * @<href>{https://dev.mysql.com/worklog/task/?id=5989, https://dev.mysql.com/worklog/task/?id=5989}

===={80210805} テーブルスペースのパス検証の無効化が可能に

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-disabling-tablespace-path-validation.html, https://dev.mysql.com/doc/refman/8.0/ja/innodb-disabling-tablespace-path-validation.html}

===={80230803} @<hidx>{AUTOEXTEND_SIZE}@<tt>|AUTOEXTEND_SIZE|オプション

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-tablespace-autoextend-size.html, https://dev.mysql.com/doc/refman/8.0/ja/innodb-tablespace-autoextend-size.html}
 ** 8.0.24で最大値が4GBに拡大
 * @<href>{http://buildup-db.blogspot.com/2021/01/mysqlinnodb.html, http://buildup-db.blogspot.com/2021/01/mysqlinnodb.html}

==== @<idx>{シリアライズ辞書情報}（@<idx>{SDI}）を持つ自己記述型テーブルスペースと管理ツール

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/glossary.html#glos_serialized_dictionary_information, https://dev.mysql.com/doc/refman/8.0/ja/glossary.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/glossary.html#glos_serialized_dictionary_information, glos_serialized_dictionary_information}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/ibd2sdi.html, https://dev.mysql.com/doc/refman/8.0/ja/ibd2sdi.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/serialized-dictionary-information.html, https://dev.mysql.com/doc/refman/8.0/ja/serialized-dictionary-information.html}

==== @<idx>{バッファプール}のMutex削除

 * @<href>{https://dev.mysql.com/worklog/task/?id=8423, https://dev.mysql.com/worklog/task/?id=8423}

==== @<idx>{パージ}の改善

 * @<href>{https://dev.mysql.com/worklog/task/?id=9387, https://dev.mysql.com/worklog/task/?id=9387}

===={80230804} テーブルスペース@<hidx>{DROP}@<tt>|DROP|・@<hidx>{TRUNCATE}@<tt>|TRUNCATE|の高速化

 * @<href>{https://mysqlserverteam.com/a-250x-improvement-to-tablespace-truncation-in-mysql-server-8-0-23/, https://mysqlserverteam.com/a-250x-improvement-to-tablespace-truncation-in-mysql-server-8-0-23/}
 ** バッファプールが大きい場合・適応型ハッシュインデックス（AHI）使用時など

==== @<idx>{デッドロック}検出を自動的に有効化／無効化

 * @<href>{https://dev.mysql.com/worklog/task/?id=9383, https://dev.mysql.com/worklog/task/?id=9383}

==== オフラインでの@<idx>{DBポータビリティ}提供（.islファイル不要化）

 * @<href>{https://dev.mysql.com/worklog/task/?id=8619, https://dev.mysql.com/worklog/task/?id=8619}

==== より小さな@<idx>{コアファイル}を生成するための新設定

 * @<href>{https://mysqlserverteam.com/mysql-8-0-excluding-the-buffer-pool-from-a-core-file/, https://mysqlserverteam.com/mysql-8-0-excluding-the-buffer-pool-from-a-core-file/}

==== @<idx>{パーティションテーブル}の@<idx>{共有テーブル領域}を非推奨に

 * @<href>{https://dev.mysql.com/worklog/task/?id=11571, https://dev.mysql.com/worklog/task/?id=11571}

==== @<idx>{テンポラリテーブル}が占有している@<idx>{オンラインディスクスペース}をオンラインで再利用

 * @<href>{https://dev.mysql.com/worklog/task/?id=11613, https://dev.mysql.com/worklog/task/?id=11613}

==== @<idx>{XAトランザクション}@<idx>{ロールバック}時の権限チェック

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/xa.html, https://dev.mysql.com/doc/refman/8.0/ja/xa.html}

==== アイドル時書き込み@<idx>{IOPS}の調整

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-parameters.html#sysvar_innodb_idle_flush_pct, https://dev.mysql.com/doc/refman/8.0/ja/innodb-parameters.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-parameters.html#sysvar_innodb_idle_flush_pct, sysvar_innodb_idle_flush_pct}

===={80220803} @<hidx>{SELECT ～ FOR SHARE}@<tt>|SELECT| ～ @<tt>|FOR SHARE|が@<tt>|SELECT|権限のみで実行可能に

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-locking-reads.html, https://dev.mysql.com/doc/refman/8.0/ja/innodb-locking-reads.html}
 ** 「MySQL 8.0.22では、…」

===={80220804} Linux環境においてテーブルスペース配置を効率化（@<hidx>{innodb_extend_and_initialize}@<tt>|innodb_extend_and_initialize|）

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-optimize-tablespace-page-allocation.html, https://dev.mysql.com/doc/refman/8.0/ja/innodb-optimize-tablespace-page-allocation.html}

===={80220805} 並列度の低いシステムでdedicated log writer threadsの無効化が可能に（@<hidx>{innodb_log_writer_threads}@<tt>|innodb_log_writer_threads|）

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/optimizing-innodb-logging.html, https://dev.mysql.com/doc/refman/8.0/ja/optimizing-innodb-logging.html}
 ** 「MySQL 8.0.22では、…」

//footnote[010AIV][Cluster構成でない通常の状態では最大値+1です。]
//footnote[020ENC][8.0.24でプラグインからコンポーネントに変わりました（@<href>{https://dev.mysql.com/doc/refman/8.0/en/keyring-file-component.html, https://dev.mysql.com/doc/refman/8.0/en/keyring-file-component.html}）。]
