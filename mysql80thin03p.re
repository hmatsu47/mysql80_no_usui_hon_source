= @<idx>{DDL}と管理用SQLの新機能

== DDL

MySQL 8.0では@<idx>{インスタントDDL}のサポートなど、DDL関連の機能が向上しています。

=== インスタントDDL

@<hidx>{ALTER TABLE}@<tt>|ALTER TABLE|でカラムの追加等を行う際、実データの更新を行わず@<idx>{メタデータ}@<fn>{010MDT}の更新のみを行う機能です。
なお、全てのDDL処理がインスタントDDLとして処理できるわけではありません。
インスタントDDLに対応している処理については、公式マニュアルで確認してください。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-online-ddl-operations.html, https://dev.mysql.com/doc/refman/8.0/ja/innodb-online-ddl-operations.html}

==== 実行例

//emlist{
mysql> @<i>{ALTER TABLE test ADD COLUMN str VARCHAR(100), ALGORITHM=INSTANT;}
Query OK, 0 rows affected (0.05 sec)
Records: 0  Duplicates: 0  Warnings: 0
//}

=== カラムのデフォルト値指定の拡張（関数・式の利用）

以前は日付型カラムにおいて@<tt>|CURRENT_TIMESTAMP|を指定できる程度でしたが、MySQL 8.0からカラムのデフォルト値として関数や式を指定できるようになりました。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/data-type-defaults.html#data-type-defaults-explicit, https://dev.mysql.com/doc/refman/8.0/ja/data-type-defaults.html#data-type-defaults-explicit}

==== 実行例

//emlist{
@<b>{※sql_require_primary_key=0の環境で実行した結果です。}

mysql> @<i>{CREATE TABLE def_test (org_str VARCHAR(100), sha_str VARCHAR(64) DEFAULT (SHA2(org_str}
@<i>{, 256)));}
Query OK, 0 rows affected (0.03 sec)
@<b>{※関数を使う}

mysql> @<i>{INSERT INTO def_test SET org_str='abc';}
Query OK, 1 row affected (0.01 sec)
@<b>{※中略}

mysql> @<i>{SELECT * FROM def_test;}
+---------+------------------------------------------------------------------+
| org_str | sha_str                                                          |
+---------+------------------------------------------------------------------+
| abc     | ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad |
| 123     | a665a45920422f9d417e4867efdc4fb8a04a1f3fff1fa07e998e86f7f7a27ae3 |
| XYZ     | ade099751d2ea9f3393f0f32d20c6b980dd5d3b0989dea599b966ae0d3cd5a1e |
+---------+------------------------------------------------------------------+
3 rows in set (0.00 sec)

mysql> @<i>{CREATE TABLE def_test2 (val INT NOT NULL, calc BIGINT DEFAULT (val*(val+1)));}
Query OK, 0 rows affected (0.02 sec)
@<b>{※式を使う}

mysql> @<i>{INSERT INTO def_test2 SET val=10;}
Query OK, 1 row affected (0.01 sec)
@<b>{※中略}

mysql> @<i>{SELECT * FROM def_test2;}
+-----+-------+
| val | calc  |
+-----+-------+
|  10 |   110 |
| 100 | 10100 |
+-----+-------+
2 rows in set (0.00 sec)
//}

=== @<idx>{不可視インデックス}

不可視インデックス（@<idx>{Invisible Index}）は、@<idx>{インデックス}を@<idx>{オプティマイザ}から使われないようにする機能です。

インデックスを運用していると、データの増加や値の偏りなどによって有効に利用されなくなることがありますが、非効率なインデックスだからといっていきなり削除してしまうと、削除に時間が掛かったり意図しない実行計画の変化をもたらす場合があります。

不可視インデックスを使うと、インデックスを削除する前に（インデックスが削除された状態での）オプティマイザの判断を確認することができます。

反対に、インデックスを追加する際にいきなり有効にするのではなく無効（不可視）の状態で追加し、影響を確認してから有効（可視）化する使い方も可能です。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/invisible-indexes.html, https://dev.mysql.com/doc/refman/8.0/ja/invisible-indexes.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/switchable-optimizations.html, https://dev.mysql.com/doc/refman/8.0/ja/switchable-optimizations.html}
 ** オプティマイザスイッチ@<hidx>{use_invisible_indexes}@<tt>|use_invisible_indexes|

==== 実行例

//emlist{
mysql> @<i>{CREATE TABLE iv_test (id INT PRIMARY KEY AUTO_INCREMENT, val INT, INDEX idx_val (val));}
Query OK, 0 rows affected (0.03 sec)
@<b>{※通常のインデックスを作成}

mysql> @<i>{INSERT INTO iv_test SET val=FLOOR(RAND()*100);}
Query OK, 1 row affected (0.01 sec)
@<b>{※中略}

mysql> @<i>{SELECT * FROM iv_test ORDER BY id;}
+----+------+
| id | val  |
+----+------+
|  1 |   39 |
|  2 |   93 |
@<b>{※中略}
| 50 |   73 |
+----+------+
50 rows in set (0.00 sec)

mysql> @<i>{EXPLAIN SELECT * FROM iv_test WHERE val BETWEEN 40 AND 59;}
+----+-------------+---------+------------+-------+---------------+---------+---------+------+
------+----------+--------------------------+
| id | select_type | table   | partitions | type  | possible_keys | key     | key_len | ref  |
 rows | filtered | Extra                    |
+----+-------------+---------+------------+-------+---------------+---------+---------+------+
------+----------+--------------------------+
|  1 | SIMPLE      | iv_test | NULL       | range | idx_val       | idx_val | 5       | NULL |
   10 |   100.00 | Using where; Using index |
+----+-------------+---------+------------+-------+---------------+---------+---------+------+
------+----------+--------------------------+
1 row in set, 1 warning (0.00 sec)
@<b>{※インデックスidx_valが効いている}

mysql> @<i>{ALTER TABLE iv_test ALTER INDEX idx_val INVISIBLE;}
Query OK, 0 rows affected (0.01 sec)
Records: 0  Duplicates: 0  Warnings: 0
@<b>{※インデックスidx_valを不可視にする。可視化するときはVISIBLE}

mysql> @<i>{EXPLAIN SELECT * FROM iv_test WHERE val BETWEEN 40 AND 59;}
+----+-------------+---------+------------+------+---------------+------+---------+------+----
--+----------+-------------+
| id | select_type | table   | partitions | type | possible_keys | key  | key_len | ref  | row
s | filtered | Extra       |
+----+-------------+---------+------------+------+---------------+------+---------+------+----
--+----------+-------------+
|  1 | SIMPLE      | iv_test | NULL       | ALL  | NULL          | NULL | NULL    | NULL |   5
0 |    11.11 | Using where |
+----+-------------+---------+------------+------+---------------+------+---------+------+----
--+----------+-------------+
1 row in set, 1 warning (0.00 sec)
@<b>{※インデックスidx_valが効かなくなった}

mysql> @<i>{SET optimizer_switch='use_invisible_indexes=on';}
Query OK, 0 rows affected (0.00 sec)
@<b>{※オプティマイザスイッチuse_invisible_indexesをonに変更}

mysql> @<i>{EXPLAIN SELECT * FROM iv_test WHERE val BETWEEN 40 AND 59;}
+----+-------------+---------+------------+-------+---------------+---------+---------+------+
------+----------+--------------------------+
| id | select_type | table   | partitions | type  | possible_keys | key     | key_len | ref  |
 rows | filtered | Extra                    |
+----+-------------+---------+------------+-------+---------------+---------+---------+------+
------+----------+--------------------------+
|  1 | SIMPLE      | iv_test | NULL       | range | idx_val       | idx_val | 5       | NULL |
   10 |   100.00 | Using where; Using index |
+----+-------------+---------+------------+-------+---------------+---------+---------+------+
------+----------+--------------------------+
1 row in set, 1 warning (0.00 sec)
@<b>{※インデックスidx_valが効くようになった}
//}

=== @<idx>{降順インデックス}

従来、MySQLでは@<idx>{インデックス}作成時に@<tt>|DESC|を指定しても無視されましたが、MySQL 8.0から降順インデックスを作成できるようになりました。
通常は複合インデックスで昇順ソートしたいカラムと降順ソートしたいカラムが混在する場合に使用します。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/descending-indexes.html, https://dev.mysql.com/doc/refman/8.0/ja/descending-indexes.html}

==== 実行例

//emlist{
mysql> @<i>{CREATE TABLE di_test (id INT PRIMARY KEY AUTO_INCREMENT, val1 INT, val2 INT, INDEX id}
@<i>{x_val(val1 ASC, val2 ASC));}
Query OK, 0 rows affected (0.02 sec)
@<b>{※複合インデックスidx_valを両カラムとも昇順で作成}

mysql> @<i>{INSERT INTO di_test SET val1=FLOOR(RAND()*100), val2=FLOOR(RAND()*100);}
Query OK, 1 row affected (0.01 sec)
@<b>{※中略}

mysql> @<i>{SELECT * FROM di_test ORDER BY id;}
+----+------+------+
| id | val1 | val2 |
+----+------+------+
|  1 |   62 |   91 |
|  2 |   71 |   82 |
@<b>{※中略}
| 50 |   66 |   41 |
+----+------+------+
50 rows in set (0.00 sec)

mysql> @<i>{EXPLAIN SELECT * FROM di_test ORDER BY val1 ASC, val2 ASC;}
+----+-------------+---------+------------+-------+---------------+---------+---------+------+
------+----------+-------------+
| id | select_type | table   | partitions | type  | possible_keys | key     | key_len | ref  |
 rows | filtered | Extra       |
+----+-------------+---------+------------+-------+---------------+---------+---------+------+
------+----------+-------------+
|  1 | SIMPLE      | di_test | NULL       | index | NULL          | idx_val | 10      | NULL |
   50 |   100.00 | Using index |
+----+-------------+---------+------------+-------+---------------+---------+---------+------+
------+----------+-------------+
1 row in set, 1 warning (0.00 sec)

mysql> @<i>{EXPLAIN SELECT * FROM di_test ORDER BY val1 ASC, val2 DESC;}
+----+-------------+---------+------------+-------+---------------+---------+---------+------+
------+----------+-----------------------------+
| id | select_type | table   | partitions | type  | possible_keys | key     | key_len | ref  |
 rows | filtered | Extra                       |
+----+-------------+---------+------------+-------+---------------+---------+---------+------+
------+----------+-----------------------------+
|  1 | SIMPLE      | di_test | NULL       | index | NULL          | idx_val | 10      | NULL |
   50 |   100.00 | Using index; Using filesort |
+----+-------------+---------+------------+-------+---------------+---------+---------+------+
------+----------+-----------------------------+
1 row in set, 1 warning (0.00 sec)
@<b>{※Using filesortが表示された}

mysql> @<i>{ALTER TABLE di_test DROP INDEX idx_val, ADD INDEX idx_val(val1 ASC, val2 DESC);}
Query OK, 0 rows affected (0.02 sec)
Records: 0  Duplicates: 0  Warnings: 0

mysql> @<i>{EXPLAIN SELECT * FROM di_test ORDER BY val1 ASC, val2 DESC;}
+----+-------------+---------+------------+-------+---------------+---------+---------+------+
------+----------+-------------+
| id | select_type | table   | partitions | type  | possible_keys | key     | key_len | ref  |
 rows | filtered | Extra       |
+----+-------------+---------+------------+-------+---------------+---------+---------+------+
------+----------+-------------+
|  1 | SIMPLE      | di_test | NULL       | index | NULL          | idx_val | 10      | NULL |
   50 |   100.00 | Using index |
+----+-------------+---------+------------+-------+---------------+---------+---------+------+
------+----------+-------------+
1 row in set, 1 warning (0.00 sec)
@<b>{※Using filesortが表示されなくなった}
//}

=== @<hidx>{関数インデックス}@<hidx>{式インデックス}関数・式インデックス

MySQL 8.0より、@<idx>{インデックス}の定義として関数や式を使うことができるようになりました。
MySQL 5.7でも@<idx>{生成列}（@<hidx>{Generated Column}@<tt>|Generated Column|）によって同様の機能を利用することができましたが、列ではなくインデックスとして定義できるのがポイントです@<fn>{020FID}。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/create-index.html#create-index-functional-key-parts, https://dev.mysql.com/doc/refman/8.0/ja/create-index.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/create-index.html#create-index-functional-key-parts, create-index-functional-key-parts}

==== 実行例

//emlist{
@<b>{※関数インデックスの例。降順インデックスの実行例で使ったテーブルを流用。}

mysql> @<i>{EXPLAIN SELECT * FROM di_test WHERE MOD(val1, 10) < 3;}
+----+-------------+---------+------------+-------+---------------+---------+---------+------+
------+----------+--------------------------+
| id | select_type | table   | partitions | type  | possible_keys | key     | key_len | ref  |
 rows | filtered | Extra                    |
+----+-------------+---------+------------+-------+---------------+---------+---------+------+
------+----------+--------------------------+
|  1 | SIMPLE      | di_test | NULL       | index | NULL          | idx_val | 10      | NULL |
   50 |   100.00 | Using where; Using index |
+----+-------------+---------+------------+-------+---------------+---------+---------+------+
------+----------+--------------------------+
1 row in set, 1 warning (0.00 sec)
@<b>{※val1がidx_valの1列目に定義されているのでインデックスフルスキャンになっている。}

mysql> @<i>{ALTER TABLE di_test ADD INDEX idx_func((MOD(val1, 10)));}
Query OK, 0 rows affected (0.05 sec)
Records: 0  Duplicates: 0  Warnings: 0
@<b>{※関数インデックスidx_funcを定義。}

mysql> @<i>{EXPLAIN SELECT * FROM di_test WHERE MOD(val1, 10) < 3;}
+----+-------------+---------+------------+-------+---------------+----------+---------+------
+------+----------+-------------+
| id | select_type | table   | partitions | type  | possible_keys | key      | key_len | ref
| rows | filtered | Extra       |
+----+-------------+---------+------------+-------+---------------+----------+---------+------
+------+----------+-------------+
|  1 | SIMPLE      | di_test | NULL       | range | idx_func      | idx_func | 5       | NULL
|   14 |   100.00 | Using where |
+----+-------------+---------+------------+-------+---------------+----------+---------+------
+------+----------+-------------+
1 row in set, 1 warning (0.00 sec)
@<b>{※idx_funcのrangeスキャンに変わった。}
//}

=== 主キーのないテーブルの禁止（sql_require_primary_key）

MySQL 8.0より、主キーのないテーブルの作成を禁止するサーバシステム変数@<hidx>{sql_require_primary_key}@<tt>|sql_require_primary_key|が新設されました。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_sql_require_primary_key, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_sql_require_primary_key, sysvar_sql_require_primary_key}

=== @<hidx>{CHECK制約}@<tt>|CHECK|制約

8.0.16より、@<tt>|CHECK|制約がサポートされました@<fn>{030CHK}。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/create-table-check-constraints.html, https://dev.mysql.com/doc/refman/8.0/ja/create-table-check-constraints.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/information-schema-table-constraints-table.html, https://dev.mysql.com/doc/refman/8.0/ja/information-schema-table-constraints-table.html}
 ** @<hidx>{TABLE_CONSTRAINTSテーブル}@<tt>|INFORMATION_SCHEMA|.@<tt>|TABLE_CONSTRAINTS|テーブル
 *** @<hidx>{CONSTRAINT_TYPE列}@<tt>|CONSTRAINT_TYPE|列
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/alter-table.html#alter-table-foreign-key, https://dev.mysql.com/doc/refman/8.0/ja/alter-table.html#alter-table-foreign-key}
 ** 「MySQL 8.0.19の時点で、…」
 *** @<tt>|ALTER TABLE|での変更・削除をサポート

==== 実行例

//emlist{
mysql> @<i>{CREATE TABLE t1 (}
    ->   @<i>{id INT PRIMARY KEY AUTO_INCREMENT,}
    ->   @<i>{prefecture VARCHAR(4) NOT NULL,}
    ->   @<i>{city_town_village VARCHAR(10),}
    ->   @<i>{ward VARCHAR(10),}
    ->   @<i>{CONSTRAINT ctv_ward_blank CHECK (}
    ->   @<i>{  (prefecture = '東京都' AND city_town_village IS NULL AND ward IS NOT NULL) OR}
    ->   @<i>{  (prefecture = '東京都' AND city_town_village IS NOT NULL AND ward IS NULL) OR}
    ->   @<i>{  (prefecture <> '東京都' AND city_town_village IS NOT NULL)));}
Query OK, 0 rows affected (0.02 sec)

mysql> @<i>{INSERT INTO t1 SET prefecture = '東京都', ward = '千代田区';}
Query OK, 1 row affected (0.00 sec)

mysql> @<i>{INSERT INTO t1 SET prefecture = '東京都', city_town_village = '八王子市';}
Query OK, 1 row affected (0.01 sec)

mysql> @<i>{INSERT INTO t1 SET prefecture = '愛知県', city_town_village = '豊田市';}
Query OK, 1 row affected (0.01 sec)

mysql> @<i>{INSERT INTO t1 SET prefecture = '愛知県', city_town_village = '名古屋市', ward = '中区';}
Query OK, 1 row affected (0.00 sec)

mysql> @<i>{INSERT INTO t1 SET prefecture = '東京都', city_town_village = '町田市', ward = '多摩区';}
ERROR 3819 (HY000): Check constraint 'ctv_ward_blank' is violated.
@<b>{※エラーの原因は「町田は神奈川」ではなく、東京都なのに市町村と区が同時に指定されたから。}
mysql> @<i>{INSERT INTO t1 SET prefecture = '東京都';}
ERROR 3819 (HY000): Check constraint 'ctv_ward_blank' is violated.
@<b>{※東京都では市町村または区のいずれかが指定されていないとエラーになる。}
mysql> @<i>{INSERT INTO t1 SET prefecture = '愛知県';}
ERROR 3819 (HY000): Check constraint 'ctv_ward_blank' is violated.
@<b>{※東京都以外の場合は市町村が指定されていないとエラーになる。}
mysql> @<i>{SELECT * FROM t1 ORDER BY id;}
+----+------------+-------------------+--------------+
| id | prefecture | city_town_village | ward         |
+----+------------+-------------------+--------------+
|  1 | 東京都     | NULL              | 千代田区     |
|  2 | 東京都     | 八王子市          | NULL         |
|  3 | 愛知県     | 豊田市            | NULL         |
|  4 | 愛知県     | 名古屋市          | 中区         |
+----+------------+-------------------+--------------+
4 rows in set (0.00 sec)
//}

==={80220301} その他のDDL新機能

 * @<idx>{アトミック}なDDL・@<idx>{バイナリログ}からのアトミックなDDL@<idx>{リカバリ}
 * インプレース処理でのキャラクタセット変換
 * @<hidx>{ADD DATAFILE}@<tt>|ADD DATAFILE|を伴わない@<hidx>{CREATE TABLESPACE}@<tt>|CREATE TABLESPACE|
 * @<hidx>{LOCK TABLES}@<tt>|LOCK TABLES|を伴う@<hidx>{RENAME TABLE}@<tt>|RENAME TABLE|
 * @<hidx>{ALTER DATABASE}@<tt>|ALTER DATABASE|で@<idx>{読み取り専用オプション}をサポート
 * @<idx>{The ddl_rewriter Plugin}
 * @<idx>{不可視カラム}

== @<idx>{管理用SQL}

MySQL 8.0では、MySQL 5.7で始まった@<b>{「OSレベルではなくSQLレベルの操作でサーバの管理を行う」}機能の実装がさらに進みました。

=== @<hidx>{RESTARTステートメント}@<tt>|RESTART|ステートメント

MySQL 5.7で導入された@<hidx>{SHUTDOWNステートメント}@<tt>|SHUTDOWN|ステートメントに続いて、MySQL 8.0では@<tt>|RESTART|ステートメントが使えるようになりました。
OS操作レベルではなく、MySQLに接続してSQL操作レベルでの@<idx>{サーバ再起動}が可能です。

なお、実行には@<hidx>{SHUTDOWN権限}@<tt>|SHUTDOWN|権限が必要です。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/restart.html, https://dev.mysql.com/doc/refman/8.0/ja/restart.html}

=== @<hidx>{SET PERSISTステートメント}@<tt>|SET PERSIST|ステートメント

従来、@<hidx>{SETステートメント}@<tt>|SET|ステートメントで設定した設定値は、サーバを再起動すると消えてしまっていました。
MySQL 8.0では、@<tt>|SET PERSIST|ステートメントにより設定値が保存され、サーバを再起動しても維持されるようになりました。

また、@<hidx>{SET PERSIST_ONLYステートメント}@<tt>|SET PERSIST_ONLY|ステートメントによって、動作中のサーバには影響を与えず、次回（再）起動時に有効になる形で設置値を変更できるようになりました。

そして、@<hidx>{RESET PERSISTステートメント}@<tt>|RESET PERSIST|ステートメントによって設定値をデフォルトに戻すことができます。

必要な権限など細かい仕様については、公式リファレンスマニュアルを参照してください。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/persisted-system-variables.html, https://dev.mysql.com/doc/refman/8.0/ja/persisted-system-variables.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_persist_only_admin_x509_subject, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_persist_only_admin_x509_subject, sysvar_persist_only_admin_x509_subject}
 ** @<tt>|SET PERSIST_ONLY|するユーザに対する追加の認証設定について

==={80230302} その他の管理用SQL変更点

 * @<hidx>{FLUSH HOSTS}@<tt>|FLUSH HOSTS|が非推奨に

//footnote[010MDT][テーブルの設計情報など]
//footnote[020FID][MySQL 5.7の生成列は「更新できない列」であり、@<idx>{ORM}（@<idx>{オブジェクト関係マッピング}）との相性が悪い、という問題があります。]
//footnote[030CHK][過去のバージョンでは制約として定義した内容がエラーにならない場合でも、処理上は無視されていました。]
