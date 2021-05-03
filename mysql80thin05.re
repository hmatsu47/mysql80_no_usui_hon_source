= JSONとドキュメントストアの新機能

=={80210501} @<idx>{JSON関数}

MySQL 5.7でサポートされたJSON関数ですが、MySQL 8.0では新たに以下の関数がサポートされました@<fn>{010B57}。

//table{
関数名	説明
------------------------------------------------------------------------------------
@<hidx>{JSON_ARRAYAGG()}@<tt>|JSON_ARRAYAGG()|	@<hidx>{GROUP BY}@<tt>|GROUP BY|での集約時に結果セットを単一の@<idx>{JSON配列}として返す
@<hidx>{JSON_MERGE_PATCH()}@<tt>|JSON_MERGE_PATCH()|	重複したキー値を置き換えて@<idx>{JSONドキュメント}を結合する
@<hidx>{JSON_OBJECTAGG()}@<tt>|JSON_OBJECTAGG()|	@<hidx>{GROUP BY}@<tt>|GROUP BY|での集約時に結果セットを単一のJSONオブジェクトとして返す
@<hidx>{JSON_PRETTY()}@<tt>|JSON_PRETTY()|	人間が読める形式でJSON文書を表示する
@<hidx>{JSON_STORAGE_FREE()}@<tt>|JSON_STORAGE_FREE()|	部分更新後のJSON列値の@<idx>{バイナリ表記}内の解放容量
@<hidx>{JSON_STORAGE_SIZE()}@<tt>|JSON_STORAGE_SIZE()|	JSONドキュメントのバイナリ表記の格納に使用される容量
@<hidx>{JSON_TABLE()}@<tt>|JSON_TABLE()|	JSON形式の値をリレーショナルテーブルとして返す
@<hidx>{JSON_SCHEMA_VALID()}@<tt>|JSON_SCHEMA_VALID()|	JSONスキーマに対するJSON文書の検証
@<hidx>{JSON_SCHEMA_VALIDATION_REPORT()}@<tt>|JSON_SCHEMA_VALIDATION_REPORT()|	検証に関するレポートをJSON形式で提供
@<hidx>{MEMBER OF()}@<tt>|MEMBER OF()|	検索用オペレータ／値の全部が含まれるか？
@<hidx>{JSON_OVERLAPS()}@<tt>|JSON_OVERLAPS()|	検索用関数／値の一部が含まれるか？
@<hidx>{JSON_VALUE()}@<tt>|JSON_VALUE()|	JSONドキュメントから値を取り出す
//}

なお、@<hidx>{JSON_MERGE()}@<tt>|JSON_MERGE()|は非推奨になりました（代わりに@<hidx>{JSON_MERGE_PRESERVE()}@<tt>|JSON_MERGE_PRESERVE()|を使います）。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/json-utility-functions.html, https://dev.mysql.com/doc/refman/8.0/ja/json-utility-functions.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/json-validation-functions.html, https://dev.mysql.com/doc/refman/8.0/ja/json-validation-functions.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/json-search-functions.html, https://dev.mysql.com/doc/refman/8.0/ja/json-search-functions.html}

==== ブログ記事等

 * @<href>{https://hit.hateblo.jp/entry/MYSQL/8.0/JSON, https://hit.hateblo.jp/entry/MYSQL/8.0/JSON}
 * @<href>{https://symfoware.blog.fc2.com/blog-entry-2140.html, https://symfoware.blog.fc2.com/blog-entry-2140.html}
 * @<href>{https://masayuki14.hatenablog.com/entry/2018/07/25/080000, https://masayuki14.hatenablog.com/entry/2018/07/25/080000}
 * @<href>{https://masayuki14.hatenablog.com/entry/2018/10/17/170000, https://masayuki14.hatenablog.com/entry/2018/10/17/170000}
 * @<href>{https://yoku0825.blogspot.com/2018/04/mysql-80-select-for-update-skip-locked.html, https://yoku0825.blogspot.com/2018/04/mysql-80-select-for-update-skip-locked.html}
 * @<href>{https://elephantdolphin.blogspot.com/2020/07/jsonvalue-now-in-mysql-8021.html, https://elephantdolphin.blogspot.com/2020/07/jsonvalue-now-in-mysql-8021.html}
 * 著者ブログ
 ** @<href>{https://qiita.com/hmatsu47/items/013da2971c8934d867e7, https://qiita.com/hmatsu47/items/013da2971c8934d867e7}

==== 実行例

@<tt>|JSON_OBJECTAGG()|・@<tt>|JSON_STORAGE_SIZE()|・@<tt>|JSON_STORAGE_FREE()|・@<tt>|JSON_TABLE()|の利用例です。

//emlist{
mysql> @<i>{CREATE TABLE agg_test (id INT PRIMARY KEY AUTO_INCREMENT, j_key VARCHAR(20) UNIQUE NOT }
@<i>{NULL, j_val VARCHAR(100));}
Query OK, 0 rows affected (0.03 sec)

mysql> @<i>{INSERT INTO agg_test SET j_key='NEC', j_val='PC-8801';}
Query OK, 1 row affected (0.01 sec)
@<b>{（中略）}

mysql> @<i>{SELECT * FROM agg_test;}
+----+---------+-----------------+
| id | j_key   | j_val           |
+----+---------+-----------------+
|  1 | NEC     | PC-8801         |
|  2 | FUJITSU | FM-8            |
|  3 | SHARP   | MZ-2000         |
|  4 | HITACHI | BASIC MASTER L3 |
+----+---------+-----------------+
4 rows in set (0.00 sec)

mysql> @<i>{SELECT id, JSON_OBJECTAGG(j_key, j_val) AS old_pc FROM agg_test GROUP BY id ORDER BY i}
@<i>{d;}
@<b>{※JSON_OBJECTAGG()でJSONオブジェクトに変換。}
+----+--------------------------------+
| id | old_pc                         |
+----+--------------------------------+
|  1 | {"NEC": "PC-8801"}             |
|  2 | {"FUJITSU": "FM-8"}            |
|  3 | {"SHARP": "MZ-2000"}           |
|  4 | {"HITACHI": "BASIC MASTER L3"} |
+----+--------------------------------+
4 rows in set (0.00 sec)

mysql> @<i>{CREATE TABLE storage_test (id INT PRIMARY KEY AUTO_INCREMENT, j_obj JSON);}
Query OK, 0 rows affected (0.02 sec)

mysql> @<i>{INSERT INTO storage_test SET j_obj=JSON_OBJECT('corp', 'NEC', 'pc', 'PC-8801');}
Query OK, 1 row affected (0.00 sec)
@<b>{（中略）}

mysql> @<i>{SELECT *, JSON_STORAGE_SIZE(j_obj) FROM storage_test;}
@<b>{※JSON_STORAGE_SIZE()でJSON列のサイズを取得。}
+----+----------------------------------------------+--------------------------+
| id | j_obj                                        | JSON_STORAGE_SIZE(j_obj) |
+----+----------------------------------------------+--------------------------+
|  1 | {"pc": "PC-8801", "corp": "NEC"}             |                       37 |
|  2 | {"pc": "FM-8", "corp": "FUJITSU"}            |                       38 |
|  3 | {"pc": "MZ-2000", "corp": "SHARP"}           |                       39 |
|  4 | {"pc": "BASIC MASTER L3", "corp": "HITACHI"} |                       49 |
+----+----------------------------------------------+--------------------------+
4 rows in set (0.00 sec)

mysql> @<i>{UPDATE storage_test SET j_obj=JSON_REPLACE(j_obj, '$.pc', 'MZ-80B') WHERE id=3;}
Query OK, 1 row affected (0.00 sec)
Rows matched: 1  Changed: 1  Warnings: 0

mysql> @<i>{SELECT *, JSON_STORAGE_FREE(j_obj) FROM storage_test;}
@<b>{※JSON_STORAGE_FREE()でJSON列を部分更新した際の空きサイズを取得。id=3の行が1文字減少している。}
+----+----------------------------------------------+--------------------------+
| id | j_obj                                        | JSON_STORAGE_FREE(j_obj) |
+----+----------------------------------------------+--------------------------+
|  1 | {"pc": "PC-8801", "corp": "NEC"}             |                        0 |
|  2 | {"pc": "FM-8", "corp": "FUJITSU"}            |                        0 |
|  3 | {"pc": "MZ-80B", "corp": "SHARP"}            |                        1 |
|  4 | {"pc": "BASIC MASTER L3", "corp": "HITACHI"} |                        0 |
+----+----------------------------------------------+--------------------------+
4 rows in set (0.00 sec)

mysql> @<i>|SELECT * FROM JSON_TABLE(|
    ->   @<i>|'[{"name":"青木","dept":"IT事業部"},{"name":"前田","dept":"コンサル事業部"},{"name":"山|
@<i>|本","dept":["IT事業部","コンサル事業部"]}]',|
    ->   @<i>|"$[*]"|
    ->   @<i>|COLUMNS(|
    ->     @<i>|name VARCHAR(40) PATH "$.name",|
    ->     @<i>|dept VARCHAR(60) PATH "$.dept"|
    ->   @<i>|)|
    -> @<i>|) AS tbl_test;|
@<b>{※JSON_TABLE()でJSONオブジェクトをテーブル形式に変換。name="山本"の非正規列はNULLになっている。}
+--------+-----------------------+
| name   | dept                  |
+--------+-----------------------+
| 青木   | IT事業部              |
| 前田   | コンサル事業部        |
| 山本   | NULL                  |
+--------+-----------------------+
3 rows in set (0.00 sec)
//}

続いて、@<tt>|JSON_VALUE()|の利用例です。
@<idx>{関数インデックス}として使ってみます。

//emlist{
mysql> @<i>{CREATE DATABASE jsontest;}
Query OK, 1 row affected (0.01 sec)

mysql> @<i>{USE jsontest;}
Database changed
mysql> @<i>{CREATE TABLE t1(}
    ->   @<i>{j JSON,}
    ->   @<i>{INDEX i1 ( (JSON_VALUE(j, '$.id' RETURNING UNSIGNED)) )}
    -> @<i>{);}
Query OK, 0 row affected (0.02 sec)

@<b>{※ここでデータを投入。}

mysql> @<i>{SELECT * FROM t1;}
+----------------------------------+
| j                                |
+----------------------------------+
| {"id": 100, "val": [1, 2, 3]}    |
| {"id": 101, "val": [4, 5, 6, 7]} |
| {"id": 110, "val": [8, 9, 0]}    |
| {"id": 120, "val": [1, 2]}       |
| {"id": 122, "val": 3}            |
| {"id": 130, "val": [4, 5]}       |
| {"id": 140, "val": [6, 7, 8]}    |
| {"id": 150, "val": [9, 0, 1, 2]} |
| {"id": 200, "val": [3, 4, 5]}    |
| {"id": 220, "val": [6, 7]}       |
+----------------------------------+
10 rows in set (0.00 sec)

mysql> @<i>{EXPLAIN SELECT * FROM t1 WHERE JSON_VALUE(j, '$.id' RETURNING UNSIGNED) = 150\G}
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
         rows: 10
     filtered: 100.00
        Extra: Using where
1 row in set, 1 warning (0.00 sec)

@<b>{※インデックスが使われなかったのでテーブル定義を確認。}

mysql> @<i>{SHOW CREATE TABLE t1\G}
*************************** 1. row ***************************
       Table: t1
Create Table: CREATE TABLE `t1` (
  `j` json DEFAULT NULL,
  KEY `i1` ((json_value(`j`, _utf8mb4'$.id' returning unsigned)))
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci
1 row in set (0.00 sec)

@<b>{※「_utf8mb4」が補完されていた。}

mysql> @<i>{EXPLAIN SELECT * FROM t1 WHERE json_value(`j`, _utf8mb4'$.id' returning unsigned) = 150}
@<i>{\G}
*************************** 1. row ***************************
           id: 1
  select_type: SIMPLE
        table: t1
   partitions: NULL
         type: ref
possible_keys: i1
          key: i1
      key_len: 9
          ref: const
         rows: 1
     filtered: 100.00
        Extra: NULL
1 row in set, 1 warning (0.00 sec)

@<b>{※インデックスの定義に合わせてWHERE句を書いてみたところ、うまく動いた。}
//}

====[column] @<tt>|JSON_SCHEMA_VALID()でCHECK制約|

8.0.19から、@<hidx>{JSON_SCHEMA_VALID()}@<tt>|JSON_SCHEMA_VALID()|が@<hidx>{CHECK制約}@<tt>|CHECK|制約に対応しています。

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/json-validation-functions.html#json-validation-functions-constraints, https://dev.mysql.com/doc/refman/8.0/ja/json-validation-functions.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/json-validation-functions.html#json-validation-functions-constraints, json-validation-functions-constraints}

====[/column]

== @<idx>{X DevAPI}と@<idx>{ドキュメントストア}

=== X DevAPIの機能向上

X DevAPI自体はMySQL 5.7でサポートされましたが、MySQL 8.0では@<idx>{Connector}の対応も進み、より使いやすくなりました。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/document-store.html, https://dev.mysql.com/doc/refman/8.0/ja/document-store.html}
 ** ドキュメントストア全般
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/x-plugin.html, https://dev.mysql.com/doc/refman/8.0/ja/x-plugin.html}
 ** @<idx>{Xプラグイン}
 * @<href>{https://dev.mysql.com/doc/x-devapi-userguide/en/, https://dev.mysql.com/doc/x-devapi-userguide/en/}
 ** X DevAPIユーザガイド

==== ブログ記事等

 * @<href>{http://blog.64p.org/entry/2018/07/08/233944, http://blog.64p.org/entry/2018/07/08/233944}
 * 著者ブログ
 ** @<href>{https://qiita.com/hmatsu47/items/2de98cd0c9472e72a52a, https://qiita.com/hmatsu47/items/2de98cd0c9472e72a52a}
 ** @<href>{https://qiita.com/hmatsu47/items/bd1634b93bf9c1fbce1e, https://qiita.com/hmatsu47/items/bd1634b93bf9c1fbce1e}
 *** @<idx>{Connector/Node.js 8.0.17}を使ったサンプル

=== コード例／@<idx>{MySQL Connector/J 8.0}を使ったドキュメントストアの利用

X DevAPIによるドキュメントストアの利用例です。MySQL @<idx>{Connector/J 8.0}を使い、@<idx>{Java 8}から実行します@<fn>{020JDB}。

//list[main][DocDbTest.java]{
package site.hmatsu47.DocDbTest;

import java.util.List;

import com.mysql.cj.xdevapi.Collection;
import com.mysql.cj.xdevapi.DbDoc;
import com.mysql.cj.xdevapi.DocResult;
import com.mysql.cj.xdevapi.Schema;
import com.mysql.cj.xdevapi.Session;
import com.mysql.cj.xdevapi.SessionFactory;


public class Main {

	public static void main(String args[]) {
		// サーバに接続
		Session session = new SessionFactory().getSession("mysqlx://localhost:33060/te
st_db?user=testuser&password=T35_U53r");

		// DBに接続
		Schema db = session.getSchema("test_db");

		// コレクション'test_collection'を作成
		Collection col = db.createCollection("test_collection", true);

		// コレクションにドキュメントを追加
		col.add("{\"person_id\":1, \"name\":\"青木\", \"dept\":\"IT事業部\"}")
		.execute();
		col.add("{\"person_id\":2, \"name\":\"前田\", \"dept\":\"コンサル事業部\"}")
		.execute();
		col.add("{\"person_id\":3, \"name\":\"山本\", \"dept\":[\"IT事業部\",\"コンサル事業部\"]}")
		.execute();

		// コレクションの「person_id」列にインデックスを追加
		col.createIndex("pid_index", "{\"fields\": [{\"field\": \"$.person_id\", \"typ
e\": \"INT\"}]}");

		// コレクションから「dept LIKE '%IT事業部%'」を探して表示
		searchDept(col, "IT事業部");

		System.out.println();

		// コレクションから「dept LIKE '%コンサル事業部%'」を探して表示
		searchDept(col, "コンサル事業部");

		System.out.println();

		// コレクションから「person_id=2」を探して表示
		searchPid(col, 2);

		System.out.println();

		// コレクションを削除
		db.dropCollection("test_collection");
	}

	// コレクションから対象ドキュメントの「dept」を文字列検索して表示する
	private static void searchDept(Collection col, String keyword) {

		System.out.println("Search: " + keyword);
		DocResult docs = col.find("dept like :dept")
		        .bind("dept", "%" + keyword + "%").execute();

		// 結果を取得して表示
		List<DbDoc> docl = docs.fetchAll();
		docl.forEach(doc -> System.out.println(doc.toFormattedString()));
	}

	// コレクションから対象ドキュメントの「person_id」を数値検索して表示する
	private static void searchPid(Collection col, long value) {

		System.out.println("Search: " + value);
		DocResult docs = col.find("person_id = :pid")
		        .bind("pid", value).execute();

		// 結果を取得して表示
		System.out.println(docs.fetchOne().toFormattedString());
	}
}
//}

==== コードの実行結果

//emlist{
Search: IT事業部
{
"_id" : "00005c93179d0000000000000001",
"dept" : "IT事業部",
"name" : "青木",
"person_id" : 1
}
{
"_id" : "00005c93179d0000000000000003",
"dept" : ["IT事業部", "コンサル事業部"],
"name" : "山本",
"person_id" : 3
}

Search: コンサル事業部
{
"_id" : "00005c93179d0000000000000002",
"dept" : "コンサル事業部",
"name" : "前田",
"person_id" : 2
}
{
"_id" : "00005c93179d0000000000000003",
"dept" : ["IT事業部", "コンサル事業部"],
"name" : "山本",
"person_id" : 3
}

Search: 2
{
"_id" : "00005c93179d0000000000000002",
"dept" : "コンサル事業部",
"name" : "前田",
"person_id" : 2
}
//}

なお、例では@<tt>|person_id|列に@<idx>{インデックス}を追加していますが、@<idx>{主キー}@<fn>{030MPK}を手動で指定する場合は@<tt>|_id|列に値を入れます。

また、以下は8.0.17で非推奨になりました。

 * @<hidx>{Collection.find().where()}@<tt>|Collection.find().where()|
 * @<hidx>{Collection.modify().where()}@<tt>|Collection.modify().where()|
 * @<hidx>{Collection.remove().where()}@<tt>|Collection.remove().where()|

== その他のJSON新機能

==== MySQL Shell／JSON・@<idx>{BSONデータ}の@<idx>{インポート}

 * @<href>{https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-shell-utilities-json.html, https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-shell-utilities-json.html}
 * @<href>{https://qiita.com/miyamadoKL/items/8d255c5faaeed671b58c, https://qiita.com/miyamadoKL/items/8d255c5faaeed671b58c}
 * @<href>{https://mysqlserverteam.com/mysql-shell-8-0-14-whats-new/, https://mysqlserverteam.com/mysql-shell-8-0-14-whats-new/}

==== @<idx>{JSONパス表現}の拡張

 * @<href>{https://dev.mysql.com/worklog/task/?id=9831, https://dev.mysql.com/worklog/task/?id=9831}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/json.html#json-path-syntax, https://dev.mysql.com/doc/refman/8.0/ja/json.html#json-path-syntax}
 ** 「MySQL 8.0.2以降では、…」

==== JSONオブジェクト値の@<idx>{高速ソート}

 * @<href>{https://dev.mysql.com/worklog/task/?id=8741, https://dev.mysql.com/worklog/task/?id=8741}

==== JSONオブジェクト値の@<idx>{インプレース}更新

 * @<href>{https://dev.mysql.com/worklog/task/?id=10570, https://dev.mysql.com/worklog/task/?id=10570}
 * @<href>{https://dev.mysql.com/worklog/task/?id=8963, https://dev.mysql.com/worklog/task/?id=8963}
 * @<href>{https://labs.gree.jp/blog/2019/10/19553/, https://labs.gree.jp/blog/2019/10/19553/}

==== @<idx>{複数値インデックス}（@<idx>{Multi-Valued Indexes}）

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/create-index.html#create-index-multi-valued, https://dev.mysql.com/doc/refman/8.0/ja/create-index.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/create-index.html#create-index-multi-valued, create-index-multi-valued}
 * @<href>{https://qiita.com/hmatsu47/items/3e49a473bc36aeefc706, https://qiita.com/hmatsu47/items/3e49a473bc36aeefc706}

//footnote[010B57][@<tt>|JSON_STORAGE_FREE()|・@<tt>|JSON_TABLE()|を除きMySQL 5.7系列でもサポートされました（5.7.22）。]
//footnote[020JDB][DBのテーブル定義等は著者ブログ記事中のものと同じです。]
//footnote[030MPK][通常は自動で値が入ります。]
