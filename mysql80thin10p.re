= その他の変更と新機能

第9章までに触れなかったMySQL 8.0の変更点と新機能について簡単に紹介しておきます。

== @<idx>{リソースグループ}

MySQLサーバの@<idx>{スレッド}が使用する@<idx>{リソース}（@<idx>{CPUコア}など）に制限を掛ける機能です。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/resource-groups.html, https://dev.mysql.com/doc/refman/8.0/ja/resource-groups.html}

== @<idx>{DML}の新機能

=== @<hidx>{ORDER BY句}@<tt>|ORDER BY|句／@<hidx>{DICTINCT句}@<tt>|DICTINCT|句と@<hidx>{WITH ROLLUP}@<tt>|WITH ROLLUP|の併用・@<hidx>{GROUPING()}@<tt>|GROUPING()|

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-by-modifiers.html, https://dev.mysql.com/doc/refman/8.0/ja/group-by-modifiers.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/miscellaneous-functions.html#function_grouping, https://dev.mysql.com/doc/refman/8.0/ja/miscellaneous-functions.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/miscellaneous-functions.html#function_grouping, function_grouping}

=== @<hidx>{LATERAL句}@<tt>|LATERAL|句

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/lateral-derived-tables.html, https://dev.mysql.com/doc/refman/8.0/ja/lateral-derived-tables.html}

=== @<hidx>{派生テーブル}派生（@<idx>{Derived}）テーブルからの外部テーブル参照

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/derived-tables.html, https://dev.mysql.com/doc/refman/8.0/ja/derived-tables.html}
 ** 「MySQL 8.0.14より前は、…」

==={80221001} その他のDML

 * 標準SQL対応など（@<hidx>{TABLEステートメント}@<tt>|TABLE|ステートメント・@<hidx>{VALUESステートメント}@<tt>|VALUES|ステートメントほか）
 * @<idx>{括弧付きクエリ式}のサポート
 * @<tt>|INSERT ... ON DUPLICATE KEY UPDATE|で新しい行に対する行エイリアス・列エイリアスをサポート（@<tt>|VALUES()|による参照を非推奨に）

== 関数の変更と新機能

=== @<idx>{正規表現関数}

利用@<idx>{ライブラリ}が@<idx>{ICU}（@<idx>{International Components for Unicode}）に変わるとともに、新しい正規表現関数が追加されました。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/regexp.html, https://dev.mysql.com/doc/refman/8.0/ja/regexp.html}
 ** @<hidx>{REGEXP_INSTR}@<tt>|REGEXP_INSTR|
 ** @<hidx>{REGEXP_LIKE}@<tt>|REGEXP_LIKE|
 ** @<hidx>{REGEXP_REPLACE}@<tt>|REGEXP_REPLACE|
 ** @<hidx>{REGEXP_SUBSTR}@<tt>|REGEXP_SUBSTR|

=== @<hidx>{STATEMENT_DIGEST()}@<tt>|STATEMENT_DIGEST()|／@<hidx>{STATEMENT_DIGEST_TEXT()}@<tt>|STATEMENT_DIGEST_TEXT()|

@<idx>{SQLステートメント}の@<idx>{正規化}（@<idx>{Normalize}）を行う関数です。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/encryption-functions.html#function_statement-digest, https://dev.mysql.com/doc/refman/8.0/ja/encryption-functions.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/encryption-functions.html#function_statement-digest, function_statement-digest}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/encryption-functions.html#function_statement-digest-text, https://dev.mysql.com/doc/refman/8.0/ja/encryption-functions.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/encryption-functions.html#function_statement-digest-text, function_statement-digest-text}

==={80221002} その他の関数

 * @<idx>{UUID関数}
 * @<hidx>{BLOB列}@<tt>|BLOB|列に対する@<idx>{ビット処理}
 * @<hidx>{CAST()}@<tt>|CAST()|関数または@<hidx>{CONVERT()}@<tt>|CONVERT()|関数による@<hidx>{YEAR型}@<tt>|YEAR|型へのキャスト
 * @<tt>|CAST(value AT TIME ZONE specifier AS DATETIME)|による@<tt>|TIMESTAMP|列値の@<idx>{タイムゾーン変換}

== その他各種新機能

=== @<idx>{Query Rewriteプラグイン}

==== Query Rewriteプラグインの@<idx>{書き換え}対応（@<tt>|INSERT|・@<tt>|UPDATE|・@<tt>|DELETE|）

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/rewriter-query-rewrite-plugin.html, https://dev.mysql.com/doc/refman/8.0/ja/rewriter-query-rewrite-plugin.html}

=== 新しいメモリ内@<idx>{テンポラリテーブル}@<idx>{ストレージエンジン}

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/internal-temporary-tables.html, https://dev.mysql.com/doc/refman/8.0/ja/internal-temporary-tables.html}
 ** 「テーブル内のBLOBまたはTEXTカラムの存在…」

=== @<idx>{エラーロギング}

 * 新しいエラーロギングインフラストラクチャ／エラーロギングの改善
 * @<idx>{サーバエラーメッセージ}

==={80221004} ログ関連（エラーログ以外）

 * @<idx>{syslog}・@<idx>{eventlog}関連の@<idx>{システム変数}を@<idx>{コンポーネント変数}に指定
 * @<idx>{スロークエリログ}への@<hidx>{log-slow-extra}@<tt>|log-slow-extra|の追加
 * @<idx>{監査ログフィルタ}：@<idx>{ルールベース}の条件でクエリを中断（Enterprise版）
 * @<idx>{監査ログ}の@<idx>{JSON形式}化・圧縮・@<idx>{暗号化}（5.7.21と同様／Enterprise版）
 * 監査ログにデータを挿入するためのSQL関数
 * 監査プラグインで@<tt>|audit_log_read()|を使用したログ読み込み操作を改善（Enterprise版）
 * 監査ログファイル（JSON形式）のローテーション後削除（プルーニング）
 * @<idx>{データマスキング機能}（5.7.24と同様／Enterprise版）

==={80211001} その他の変更と新機能

 * @<idx>{クエリキャッシュ}の廃止
 * オンディスク@<idx>{テンポラリテーブル}@<idx>{ストレージエンジン}
 * 255文字までの@<idx>{ホスト名}に対応
 * @<idx>{キャスト関数}・@<idx>{オペレータ}（@<idx>{演算子}）の拡張
 * @<idx>{日時リテラル}が@<idx>{タイムゾーンオフセット付きフォーマット}に対応

//embed[latex]{
\clearpage
//}

 * @<idx>{非推奨}（@<idx>{Deprecated}）となった@<idx>{関数}・演算子など（8.0.17以降）
 ** @<hidx>{FLOAT(M,D)}@<tt>|FLOAT(M,D)|・@<hidx>{DOUBLE(M,D)}@<tt>|DOUBLE(M,D)|、@<hidx>{ZEROFILL属性}@<tt>|ZEROFILL|属性、@<tt>|FLOAT|・@<tt>|DOUBLE|・@<hidx>{DECIMAL}@<tt>|DECIMAL|に付加する@<hidx>{UNSIGNED属性}@<tt>|UNSIGNED|属性
 ** @<tt>|FLOAT|・@<tt>|DOUBLE|カラムに対する@<hidx>{AUTO_INCREMENT}@<tt>|AUTO_INCREMENT|
 ** @<idx>{文字列データ型}における@<hidx>{BINARY属性}@<tt>|BINARY|属性
 ** @<idx>{論理演算子}@<hidx>{「&&」}「@<tt>|&&|」・@<hidx>{「||」}@<tt>{「||」}・@<hidx>{「!」}@<tt>|「!」|
 ** @<hidx>{SQL_CALC_FOUND_ROWS}@<tt>|SQL_CALC_FOUND_ROWS|、@<hidx>{FOUND_ROWS()}@<tt>|FOUND_ROWS()|
 ** @<hidx>{YEAR型}@<tt>|YEAR|型の表示文字数@<tt>|(n)|と@<tt>|UNSIGNED|属性
 * @<idx>{InnoDB memcachedプラグイン}が非推奨に
 * @<hidx>{INTO OUTFILE}@<tt>|INTO OUTFILE|・@<hidx>{INTO DUMPFILE}@<tt>|INTO DUMPFILE|で生成するファイルの@<idx>{パーミッション}を@<tt>|0640|に
 * @<hidx>{LOAD DATA INFILE LOCAL}@<tt>|LOAD DATA INFILE LOCAL|に関する変更
 * 新しい@<idx>{バックアップロック}
 * コネクション@<idx>{圧縮}関連
 ** @<tt>|Compression_algorithm|・@<tt>|Compression_level|
 ** @<tt>|protocol_compression_algorithms|
 ** @<tt>|Compression|（非推奨に）
 * サーバ側@<idx>{キーリング}移行ツール
 * キーリングでサポートするキー長の増加
 * @<idx>{AWS KMS}用のキーリングプラグイン（5.7.19と同様）
 * @<idx>{HashiCorp Vault}用のキーリングプラグイン
 * @<idx>{Oracle Cloud Infrastructure}（OCI）用のキーリングプラグイン
 * キーリングプラグインのコンポーネント化
 ** @<tt>|ALTER INSTANCE RELOAD KEYRING|
 * 復旧／切り離された準備済み@<idx>{XAトランザクション}のMDLロック有効化
 * @<idx>{外部キー}のための@<idx>{メタデータロック}のサポート
 * @<idx>{The LOCK_ORDER Tool}
 * SSLチェックを効率化するための@<hidx>{--ssl-mode}@<tt>|--ssl-mode|クライアントオプション
 * @<hidx>{--binary-as-hex}@<tt>|--binary-as-hex|オプションに関する挙動の変更
 * @<idx>{サービスレジストリ}と@<idx>{コンポーネントインフラストラクチャ}
 * @<idx>{レプリケーションストリーム}を読み取るためのC API
 * @<idx>{非同期C API}
 * C APIで@<hidx>{mysql_real_connect_dns_srv()}@<tt>|mysql_real_connect_dns_srv()|をサポート
 * @<tt>|mysql_bind_param()| C API関数によるクエリ属性の定義（ユーザ定義関数を使用してSQL文内から属性値の取得が可能に）
 * @<idx>{UDF}自動登録コンポーネントのためのUDF登録サービス
 * MySQLサーバー文字列コンポーネントサービス
 * @<idx>{結果セット}の@<idx>{メタデータ}転送オプション
 * コンポーネント用の@<idx>{ステータス変数}サービス
 * サーバ変数@<hidx>{temptable_max_mmap}@<tt>|temptable_max_mmap|追加
 * @<idx>{プラグイン}が@<idx>{プリペアドステートメント}を使えるように
 * @<idx>{ソートバッファ}の@<idx>{動的割り当て}
 * @<hidx>{NO PAD照合順序}@<tt>|NO PAD|照合順序（COLLATION）用の@<idx>{可変長ソートキー}
 * @<hidx>{lower_case_table_names}@<tt>|lower_case_table_names|の有効化に@<hidx>{debconf-set-selections}@<tt>|debconf-set-selections|を利用
 * KEYパーティショニングでカラムインデックスプレフィックスを使ったときに正しく警告・エラーを出力するようになった
 * 文字列型と数値型・時間型の型変換（キャスト）がどのように行われたかを@<tt>|EXPLAIN ANALYZE|・@<tt>|EXPLAIN FORMAT=JSON|・@<tt>|EXPLAIN FORMAT=TREE|で可視化
 * @<idx>{ネットワーク名前空間指定子}のサポート
 * @<hidx>{utf8mb3}@<tt>|utf8mb3|表示を@<tt>|utf8|よりも優先
 ** クライアントアプリケーション等で
 * ユーザ定義関数@<tt>|gen_blacklist()|を@<tt>|gen_blocklist()|に変更（Enterprise版）
 * MySQL Enterprise Firewallにグループプロファイル機能を追加（Enterprise版）
 * @<idx>{ソースコード}の改善
 * @<idx>{環境変数}@<tt>|MYSQL_PWD|が非推奨に
