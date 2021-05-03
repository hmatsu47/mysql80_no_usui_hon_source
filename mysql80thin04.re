= CTEとウィンドウ関数

== @<idx>{CTE}（@<idx>{Common Table Expressions}）

CTE（@<idx>{共通テーブル式}）は、@<idx>{主たるSQ}Lの@<idx>{問い合わせ}を実行するために補助的に使う@<idx>{一時テーブル}を定義するものです。
@<tt>|WITH|で記述を始めるので@<hidx>{WITH句}@<tt>|WITH|句とも呼びます。

単純なテーブル構造だけではなく、@<hidx>{WITH RECURSIVE}@<tt>|WITH RECURSIVE|で@<idx>{再帰的}に記述することもできるのがポイントです。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/with.html, https://dev.mysql.com/doc/refman/8.0/ja/with.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/with.html#common-table-expressions-recursive, https://dev.mysql.com/doc/refman/8.0/ja/with.html#common-table-expressions-recursive}

==== ブログ記事等

 * @<href>{https://www.s-style.co.jp/blog/2017/07/884/, https://www.s-style.co.jp/blog/2017/07/884/}
 * @<href>{https://yakst.com/ja/posts/4322, https://yakst.com/ja/posts/4322}
 * @<href>{https://yoku0825.blogspot.com/2018/04/mysql-80ctewith-recursive1000.html, https://yoku0825.blogspot.com/2018/04/mysql-80ctewith-recursive1000.html}
 * @<href>{https://tombo2.hatenablog.com/entry/2019/03/10/222732, https://tombo2.hatenablog.com/entry/2019/03/10/222732}
 * @<href>{https://next4us-ti.hatenablog.com/entry/2019/03/21/003927, https://next4us-ti.hatenablog.com/entry/2019/03/21/003927}
 * @<href>{https://www.percona.com/blog/2020/02/10/introduction-to-mysql-8-0-common-table-expressions-part-1/, https://www.percona.com/blog/2020/02/10/introduction-to-mysql-8-0-common-table-}@<br>{}@<href>{https://www.percona.com/blog/2020/02/10/introduction-to-mysql-8-0-common-table-expressions-part-1/, expressions-part-1/}
 * @<href>{https://www.percona.com/blog/2020/02/13/introduction-to-mysql-8-0-recursive-common-table-expression-part-2/, https://www.percona.com/blog/2020/02/13/introduction-to-mysql-8-0-recursive-common-}@<br>{}@<href>{https://www.percona.com/blog/2020/02/13/introduction-to-mysql-8-0-recursive-common-table-expression-part-2/, table-expression-part-2/}
 * 著者ブログ
 ** @<href>{https://qiita.com/hmatsu47/items/01211556089b19913d05, https://qiita.com/hmatsu47/items/01211556089b19913d05}

==== 実行例

著者ブログで使ったデータのうち、今回は@<tt>|order_id=3|のスタイリッシュパッケージを抽出・更新してみます。

//emlist{
mysql> @<i>{CREATE TABLE order_detail (}
    ->   @<i>{detail_id INT UNSIGNED PRIMARY KEY AUTO_INCREMENT,}
    ->   @<i>{order_id INT UNSIGNED NOT NULL,}
    ->   @<i>{parent_id INT UNSIGNED,}
    ->   @<i>{product_name VARCHAR(100),}
    ->   @<i>{cancel_flag INT UNSIGNED NOT NULL,}
    ->   @<i>{INDEX (order_id),}
    ->   @<i>{INDEX (parent_id)}
    -> @<i>{);}
Query OK, 0 rows affected (0.03 sec)

mysql> @<i>{INSERT INTO order_detail VALUES( 1,1,NULL,'車両本体 Sグレード'        ,0);}
Query OK, 1 row affected (0.01 sec)
@<b>{（中略）}

mysql> @<i>{INSERT INTO order_detail VALUES(22,3,  19,'リアスポイラー'            ,0);}
Query OK, 1 row affected (0.01 sec)

mysql> @<i>{SELECT * FROM order_detail;}
+-----------+----------+-----------+-----------------------------------------+-------------+
| detail_id | order_id | parent_id | product_name                            | cancel_flag |
+-----------+----------+-----------+-----------------------------------------+-------------+
|         1 |        1 |      NULL | 車両本体 Sグレード                      |           0 |
|         2 |        1 |         1 | セーフティーパッケージ                  |           0 |
|         3 |        1 |         2 | 衝突回避ブレーキシステム                |           0 |
|         4 |        1 |         2 | 追加エアバッグセット                    |           0 |
|         5 |        1 |         3 | サイドエアバッグ                        |           0 |
|         6 |        1 |         3 | カーテンエアバッグ                      |           0 |
|         7 |        1 |         1 | 18インチセット                          |           0 |
|         8 |        1 |         7 | 225/40R18ラジアルタイヤ                 |           0 |
|         9 |        1 |         7 | 18インチアルミホイール                  |           0 |
|        10 |        1 |         1 | フロアマット                            |           0 |
|        11 |        2 |      NULL | 車両本体 Bグレード                      |           0 |
|        12 |        2 |        11 | サイドバイザー                          |           0 |
|        13 |        2 |        11 | フロアマット                            |           0 |
|        14 |        3 |      NULL | 車両本体 Xグレード                      |           0 |
|        15 |        3 |        14 | スタイリッシュパッケージ                |           0 |
|        16 |        3 |        15 | 18インチセット                          |           0 |
|        17 |        3 |        16 | 225/40R18ラジアルタイヤ                 |           0 |
|        18 |        3 |        16 | 18インチアルミホイール                  |           0 |
|        19 |        3 |        15 | エアロセットB                           |           0 |
|        20 |        3 |        19 | フロントアンダースポイラー              |           0 |
|        21 |        3 |        19 | サイドステップ                          |           0 |
|        22 |        3 |        19 | リアスポイラー                          |           0 |
+-----------+----------+-----------+-----------------------------------------+-------------+
22 rows in set (0.00 sec)

@<b>{※データの構造は以下の通り（著者ブログより）。}

order#1-+-車両本体 Sグレード
        |
        +-+-セーフティーパッケージ
        | |
        | +---衝突回避ブレーキシステム
        | |
        | +-+-追加エアバッグセット
        |   |
        |   +---サイドエアバッグ
        |   |
        |   +---カーテンエアバッグ
        |
        +-+-18インチセット
        | |
        | +---225/40R18ラジアルタイヤ
        | |
        | +---18インチアルミホイール
        |
        +---フロアマット

order#2-+-車両本体 Bグレード
        |
        +---サイドバイザー
        |
        +---フロアマット

order#3-+-車両本体 Xグレード
        |
        +-+-スタイリッシュパッケージ
          |
          +-+-18インチセット
          | |
          | +---225/40R18ラジアルタイヤ
          | |
          | +---18インチアルミホイール
          |
          +-+-エアロセットB
            |
            +---フロントアンダースポイラー
            |
            +---サイドステップ
            |
            +---リアスポイラー

mysql> @<i>{WITH RECURSIVE product_order AS}
    -> @<i>{(}
    ->   @<i>{SELECT detail_id, parent_id, product_name, cancel_flag}
    ->    @<i>{FROM ctetest.order_detail}
    ->     @<i>{WHERE order_id = 3 AND product_name = 'スタイリッシュパッケージ'}
    ->   @<i>{UNION ALL}
    ->   @<i>{SELECT child.detail_id, child.parent_id, child.product_name, child}
@<i>{.cancel_flag}
    ->    @<i>{FROM ctetest.order_detail AS child, product_order}
    ->     @<i>{WHERE product_order.detail_id = child.parent_id}
    -> @<i>{)}
    -> @<i>{SELECT * FROM product_order;}
+-----------+-----------+-----------------------------------------+-------------+
| detail_id | parent_id | product_name                            | cancel_flag |
+-----------+-----------+-----------------------------------------+-------------+
|        15 |        14 | スタイリッシュパッケージ                |           0 |
|        16 |        15 | 18インチセット                          |           0 |
|        19 |        15 | エアロセットB                           |           0 |
|        17 |        16 | 225/40R18ラジアルタイヤ                 |           0 |
|        18 |        16 | 18インチアルミホイール                  |           0 |
|        20 |        19 | フロントアンダースポイラー              |           0 |
|        21 |        19 | サイドステップ                          |           0 |
|        22 |        19 | リアスポイラー                          |           0 |
+-----------+-----------+-----------------------------------------+-------------+
8 rows in set (0.02 sec)
@<b>{※order_id=3に含まれるスタイリッシュパッケージと、その子・孫にあたる行が抽出された。}

mysql> @<i>{WITH RECURSIVE product_order AS}
    -> @<i>{(}
    ->   @<i>{SELECT detail_id, parent_id, product_name, cancel_flag}
    ->    @<i>{FROM ctetest.order_detail}
    ->     @<i>{WHERE order_id = 3 AND product_name = 'スタイリッシュパッケージ'}
    ->   @<i>{UNION ALL}
    ->   @<i>{SELECT child.detail_id, child.parent_id, child.product_name, child}
@<i>{.cancel_flag}
    ->    @<i>{FROM ctetest.order_detail AS child, product_order}
    ->     @<i>{WHERE product_order.detail_id = child.parent_id}
    -> @<i>{)}
    -> @<i>{UPDATE ctetest.order_detail}
    ->  @<i>{SET cancel_flag = 1}
    ->   @<i>{WHERE detail_id IN}
    ->    @<i>{(SELECT detail_id FROM product_order);}
Query OK, 8 rows affected (0.01 sec)
Rows matched: 8  Changed: 8  Warnings: 0
@<b>{※CTEで直接UPDATEはできないので、CTEをサブクエリで使う。DELETEの場合も同じ。}

mysql> @<i>{SELECT * FROM ctetest.order_detail WHERE cancel_flag = 1;}
+-----------+----------+-----------+-----------------------------------------+-------------+
| detail_id | order_id | parent_id | product_name                            | cancel_flag |
+-----------+----------+-----------+-----------------------------------------+-------------+
|        15 |        3 |        14 | スタイリッシュパッケージ                |           1 |
|        16 |        3 |        15 | 18インチセット                          |           1 |
|        17 |        3 |        16 | 225/40R18ラジアルタイヤ                 |           1 |
|        18 |        3 |        16 | 18インチアルミホイール                  |           1 |
|        19 |        3 |        15 | エアロセットB                           |           1 |
|        20 |        3 |        19 | フロントアンダースポイラー              |           1 |
|        21 |        3 |        19 | サイドステップ                          |           1 |
|        22 |        3 |        19 | リアスポイラー                          |           1 |
+-----------+----------+-----------+-----------------------------------------+-------------+
8 rows in set (0.00 sec)
@<b>{※CTE抽出行のみ、cancel_flagが1に更新されている。}
//}

====[column] @<tt>|再帰CTEのLIMIT句対応|

8.0.19から、再帰CTEでLIMIT句が使えるようになりました。

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/with.html#common-table-expressions-recursive-examples, https://dev.mysql.com/doc/refman/8.0/ja/with.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/with.html#common-table-expressions-recursive-examples, common-table-expressions-recursive-examples}
 ** 「MySQL 8.0.19より前は、…」
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/with.html#common-table-expressions-recursion-limits, https://dev.mysql.com/doc/refman/8.0/ja/with.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/with.html#common-table-expressions-recursion-limits, common-table-expressions-recursion-limits}
 ** 「MySQL 8.0.19以降では、…」
 * @<href>{https://mysqlserverteam.com/a-new-simple-way-to-figure-out-why-your-recursive-cte-is-running-away/, https://mysqlserverteam.com/a-new-simple-way-to-figure-out-why-your-recursive-cte-is-running-away/}

====[/column]

//embed[latex]{
\clearpage
//}

== @<idx>{ウィンドウ関数}（@<idx>{Window Function}）

MySQL 8.0ではウィンドウ関数も利用できるようになりました。
ウィンドウ関数は、テーブルに存在する複数の行を、@<idx>{区間}に@<idx>{分割}して@<idx>{集計}する機能です。
@<idx>{集約関数}（@<hidx>{GROUP BY}@<tt>|GROUP BY|）とは違い、複数の行がまとめられることはなく、個々の行が返却されます。

//table{
関数名	説明
---------------------------------------------------------------
@<hidx>{CUME_DIST()}@<tt>|CUME_DIST()|	@<idx>{累積分布値}
@<hidx>{DENSE_RANK()}@<tt>|DENSE_RANK()|	@<idx>{パーティション}内の現在行の@<idx>{順位}（@<idx>{ギャップ}なし）
@<hidx>{FIRST_VALUE()}@<tt>|FIRST_VALUE()|	@<idx>{ウィンドウフレーム}の最初の行の値
@<hidx>{LAG()}@<tt>|LAG()|	パーティション内の前行の値
@<hidx>{LAST_VALUE()}@<tt>|LAST_VALUE()|	ウィンドウフレームの最終行の値
@<hidx>{LEAD()}@<tt>|LEAD()|	パーティション内の次行の値
@<hidx>{NTH_VALUE()}@<tt>|NTH_VALUE()|	ウィンドウフレームのN行目の値
@<hidx>{NTILE()}@<tt>|NTILE()|	パーティション内の現在行が含まれるバケット番号
@<hidx>{PERCENT_RANK()}@<tt>|PERCENT_RANK()|	@<idx>{パーセントランク値}
@<hidx>{RANK()}@<tt>|RANK()|	パーティション内の現在行の順位（ギャップあり）
@<hidx>{ROW_NUMBER()}@<tt>|ROW_NUMBER()|	パーティション内の現在の行番号
//}

利用可能な関数の詳細については、公式リファレンスマニュアルおよびブログ記事等の2つ目・3つ目を参照してください。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/window-functions.html, https://dev.mysql.com/doc/refman/8.0/ja/window-functions.html}

==== ブログ記事等

 * @<href>{http://blog.kimuradb.com/?eid=877509, http://blog.kimuradb.com/?eid=877509}
 * @<href>{https://next4us-ti.hatenablog.com/entry/2019/03/24/225924, https://next4us-ti.hatenablog.com/entry/2019/03/24/225924}
 * @<href>{https://tombo2.hatenablog.com/entry/2019/03/12/231618, https://tombo2.hatenablog.com/entry/2019/03/12/231618}
 * 著者ブログ
 ** @<href>{https://qiita.com/hmatsu47/items/6cc0e69f3895f3e4a486, https://qiita.com/hmatsu47/items/6cc0e69f3895f3e4a486}
 ** @<href>{https://qiita.com/hmatsu47/items/7976e81100604f8984d2, https://qiita.com/hmatsu47/items/7976e81100604f8984d2}

==== 実行例

著者ブログの記事と同様の集計を、愛知県ではなく静岡県のデータで実行してみました。

//emlist{
mysql> @<i>{CREATE TABLE shizuoka (id INT PRIMARY KEY AUTO_INCREMENT,}
    -> @<i>{ctv_name VARCHAR(50) NOT NULL,}
    -> @<i>{population INT NOT NULL,}
    -> @<i>{ctv_type INT NOT NULL) ENGINE innodb;}
Query OK, 0 rows affected (0.05 sec)
@<b>{※著者ブログで示した例のテーブル構造のうち一部を省略。}

mysql> @<i>{INSERT INTO shizuoka SET ctv_name='静岡市', population=704043, ctv_type=1;}
Query OK, 1 row affected (0.00 sec)
@<b>{（中略）}

mysql> @<i>{INSERT INTO shizuoka SET ctv_name='周智郡森町', population=18507, ctv_type=5;}
Query OK, 1 row affected (0.00 sec)

mysql> @<i>{SELECT * FROM shizuoka;}
+----+-----------------------+------------+----------+
| id | ctv_name              | population | ctv_type |
+----+-----------------------+------------+----------+
|  1 | 静岡市                |     704043 |        1 |
|  2 | 浜松市                |     804989 |        1 |
|  3 | 沼津市                |     196530 |        3 |
|  4 | 熱海市                |      37225 |        4 |
|  5 | 三島市                |     110505 |        4 |
|  6 | 富士宮市              |     133290 |        4 |
|  7 | 伊東市                |      69597 |        4 |
|  8 | 島田市                |      98909 |        4 |
|  9 | 富士市                |     254203 |        3 |
| 10 | 磐田市                |     169931 |        4 |
| 11 | 焼津市                |     140189 |        4 |
| 12 | 掛川市                |     117605 |        4 |
| 13 | 藤枝市                |     145789 |        4 |
| 14 | 御殿場市              |      88494 |        4 |
| 15 | 袋井市                |      87938 |        4 |
| 16 | 下田市                |      21937 |        4 |
| 17 | 裾野市                |      52332 |        4 |
| 18 | 湖西市                |      59861 |        4 |
| 19 | 伊豆市                |      31089 |        4 |
| 20 | 御前崎市              |      32996 |        4 |
| 21 | 菊川市                |      47850 |        4 |
| 22 | 伊豆の国市            |      49082 |        4 |
| 23 | 牧之原市              |      46102 |        4 |
| 24 | 賀茂郡東伊豆町        |      12418 |        5 |
| 25 | 賀茂郡河津町          |       7339 |        5 |
| 26 | 賀茂郡南伊豆町        |       8456 |        5 |
| 27 | 賀茂郡松崎町          |       6768 |        5 |
| 28 | 賀茂郡西伊豆町        |       8083 |        5 |
| 29 | 田方郡函南町          |      37901 |        5 |
| 30 | 駿東郡清水町          |      32606 |        5 |
| 31 | 駿東郡長泉町          |      43185 |        5 |
| 32 | 駿東郡小山町          |      18815 |        5 |
| 33 | 榛原郡吉田町          |      29679 |        5 |
| 34 | 榛原郡川根本町        |       7002 |        5 |
| 35 | 周智郡森町            |      18507 |        5 |
+----+-----------------------+------------+----------+
35 rows in set (0.00 sec)

mysql> @<i>{SELECT RANK() OVER (ORDER BY population DESC) AS pop_rank,}
    -> @<i>{ctv_name, population FROM shizuoka;}
@<b>{※全市町村のランキングを抽出。}
+----------+-----------------------+------------+
| pop_rank | ctv_name              | population |
+----------+-----------------------+------------+
|        1 | 浜松市                |     804989 |
|        2 | 静岡市                |     704043 |
|        3 | 富士市                |     254203 |
@<b>{（中略）}
|       34 | 榛原郡川根本町        |       7002 |
|       35 | 賀茂郡松崎町          |       6768 |
+----------+-----------------------+------------+
35 rows in set (0.02 sec)

mysql> @<i>{SELECT ctv_type,}
    -> @<i>{RANK() OVER (PARTITION BY ctv_type ORDER BY population DESC)}
    ->  @<i>{AS pop_rank,}
    -> @<i>{ctv_name, population FROM shizuoka;}
@<b>{※市町村種類別のランキングを抽出。なお、静岡県には中核市（ctv_type=2）・村（ctv_type=6）は存在しない。}
+----------+----------+-----------------------+------------+
| ctv_type | pop_rank | ctv_name              | population |
+----------+----------+-----------------------+------------+
|        1 |        1 | 浜松市                |     804989 |
|        1 |        2 | 静岡市                |     704043 |
|        3 |        1 | 富士市                |     254203 |
|        3 |        2 | 沼津市                |     196530 |
|        4 |        1 | 磐田市                |     169931 |
|        4 |        2 | 藤枝市                |     145789 |
@<b>{（中略）}
|        5 |       11 | 榛原郡川根本町        |       7002 |
|        5 |       12 | 賀茂郡松崎町          |       6768 |
+----------+----------+-----------------------+------------+
35 rows in set (0.00 sec)

mysql> @<i>{SELECT RANK() OVER w AS pop_rank,}
    -> @<i>{ctv_name, population,}
    -> @<i>{(SUM(population) OVER w / SUM(population) OVER w2)*100}
    ->  @<i>{AS sum_pct,}
    -> @<i>{FORMAT(CUME_DIST() OVER w, 3) AS c_dist FROM shizuoka}
    ->  @<i>{WINDOW w AS (ORDER BY population DESC), w2 AS ();}
@<b>{※sum_pctは、人口が多い市町村から集計した総和が県全体の人口に占める割合。c_dist=0.2のところが「上位2割（35市区町村のうちの7番目）」。愛知県同様、60%台中盤だった。}
+----------+-----------------------+------------+----------+--------+
| pop_rank | ctv_name              | population | sum_pct  | c_dist |
+----------+-----------------------+------------+----------+--------+
|        1 | 浜松市                |     804989 |  21.5743 | 0.029  |
|        2 | 静岡市                |     704043 |  40.4431 | 0.057  |
|        3 | 富士市                |     254203 |  47.2559 | 0.086  |
|        4 | 沼津市                |     196530 |  52.5231 | 0.114  |
|        5 | 磐田市                |     169931 |  57.0774 | 0.143  |
|        6 | 藤枝市                |     145789 |  60.9846 | 0.171  |
|        7 | 焼津市                |     140189 |  64.7418 | 0.200  |
|        8 | 富士宮市              |     133290 |  68.3140 | 0.229  |
|        9 | 掛川市                |     117605 |  71.4659 | 0.257  |
|       10 | 三島市                |     110505 |  74.4275 | 0.286  |
|       11 | 島田市                |      98909 |  77.0784 | 0.314  |
|       12 | 御殿場市              |      88494 |  79.4501 | 0.343  |
|       13 | 袋井市                |      87938 |  81.8069 | 0.371  |
@<b>{（中略）}
|       35 | 賀茂郡松崎町          |       6768 | 100.0000 | 1.000  |
+----------+-----------------------+------------+----------+--------+
35 rows in set (0.00 sec)

mysql> @<i>{SELECT s.pop_rank, s.ctv_name, s.population,}
    -> @<i>{s.sum_pct, s.c_dist FROM}
    -> @<i>{(}
    ->   @<i>{SELECT RANK() OVER w AS pop_rank,}
    ->   @<i>{ctv_name, population,}
    ->   @<i>{(SUM(population) OVER w / SUM(population) OVER w2)*100}
    ->    @<i>{AS sum_pct,}
    ->   @<i>{FORMAT(CUME_DIST() OVER w, 3) AS c_dist FROM shizuoka}
    ->    @<i>{WINDOW w AS (ORDER BY population DESC), w2 AS ()}
    -> @<i>{) AS s}
    ->  @<i>{WHERE s.c_dist <= 0.2;}
@<b>{※ウィンドウ関数はWHERE句に書くことができないので、「上位2割」で打ち切るときはFROM句のサブクエリとして書く。}
+----------+-----------+------------+---------+--------+
| pop_rank | ctv_name  | population | sum_pct | c_dist |
+----------+-----------+------------+---------+--------+
|        1 | 浜松市    |     804989 | 21.5743 | 0.029  |
|        2 | 静岡市    |     704043 | 40.4431 | 0.057  |
|        3 | 富士市    |     254203 | 47.2559 | 0.086  |
|        4 | 沼津市    |     196530 | 52.5231 | 0.114  |
|        5 | 磐田市    |     169931 | 57.0774 | 0.143  |
|        6 | 藤枝市    |     145789 | 60.9846 | 0.171  |
|        7 | 焼津市    |     140189 | 64.7418 | 0.200  |
+----------+-----------+------------+---------+--------+
7 rows in set (0.00 sec)
//}
