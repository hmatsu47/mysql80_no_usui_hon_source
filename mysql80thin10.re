= その他の変更と新機能

第9章までに触れなかったMySQL 8.0の変更点と新機能について簡単に紹介しておきます。

== @<idx>{リソースグループ}

MySQLサーバの@<idx>{スレッド}が使用する@<idx>{リソース}（@<idx>{CPUコア}など）に制限を掛ける機能です。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/resource-groups.html, https://dev.mysql.com/doc/refman/8.0/ja/resource-groups.html}

==== ブログ記事等

 * @<href>{https://www.s-style.co.jp/blog/2018/09/2549/, https://www.s-style.co.jp/blog/2018/09/2549/}
 * @<href>{https://mita2db.hateblo.jp/entry/MySQL_8.0_%E3%81%A7%E5%B0%8E%E5%85%A5%E3%81%95%E3%82%8C%E3%81%9F%E3%83%AA%E3%82%BD%E3%83%BC%E3%82%B9%E3%82%B0%E3%83%AB%E3%83%BC%E3%83%97%E3%82%92%E8%A9%A6%E3%81%99, https://mita2db.hateblo.jp/entry/MySQL_8.0_%E3%81%A7%E5%B0%8E%E5%85%A5%E3}@<br>{}@<href>{https://mita2db.hateblo.jp/entry/MySQL_8.0_%E3%81%A7%E5%B0%8E%E5%85%A5%E3%81%95%E3%82%8C%E3%81%9F%E3%83%AA%E3%82%BD%E3%83%BC%E3%82%B9%E3%82%B0%E3%83%AB%E3%83%BC%E3%83%97%E3%82%92%E8%A9%A6%E3%81%99, %81%95%E3%82%8C%E3%81%9F%E3%83%AA%E3%82%BD%E3%83%BC%E3%82%B9%E3}@<br>{}@<href>{https://mita2db.hateblo.jp/entry/MySQL_8.0_%E3%81%A7%E5%B0%8E%E5%85%A5%E3%81%95%E3%82%8C%E3%81%9F%E3%83%AA%E3%82%BD%E3%83%BC%E3%82%B9%E3%82%B0%E3%83%AB%E3%83%BC%E3%83%97%E3%82%92%E8%A9%A6%E3%81%99, %82%B0%E3%83%AB%E3%83%BC%E3%83%97%E3%82%92%E8%A9%A6%E3%81%99}
 * @<href>{https://mita2db.hateblo.jp/entry/MySQL_8%E3%81%AE%E6%96%B0%E6%A9%9F%E8%83%BD_RESOURCE_GROUP_%E3%81%A8_%E6%A8%A9%E9%99%90, https://mita2db.hateblo.jp/entry/MySQL_8%E3%81%AE%E6%96%B0%E6%A9%9F%E8%83}@<br>{}@<href>{https://mita2db.hateblo.jp/entry/MySQL_8%E3%81%AE%E6%96%B0%E6%A9%9F%E8%83%BD_RESOURCE_GROUP_%E3%81%A8_%E6%A8%A9%E9%99%90, %BD_RESOURCE_GROUP_%E3%81%A8_%E6%A8%A9%E9%99%90}

== @<idx>{DML}の新機能

=== @<hidx>{ORDER BY句}@<tt>|ORDER BY|句／@<hidx>{DICTINCT句}@<tt>|DICTINCT|句と@<hidx>{WITH ROLLUP}@<tt>|WITH ROLLUP|の併用・@<hidx>{GROUPING()}@<tt>|GROUPING()|

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-by-modifiers.html, https://dev.mysql.com/doc/refman/8.0/ja/group-by-modifiers.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/miscellaneous-functions.html#function_grouping, https://dev.mysql.com/doc/refman/8.0/ja/miscellaneous-functions.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/miscellaneous-functions.html#function_grouping, function_grouping}

==== ブログ記事等

 * @<href>{https://mysqlserverteam.com/improvements-to-rollup-in-mysql/, https://mysqlserverteam.com/improvements-to-rollup-in-mysql/}
 * @<href>{https://yakst.com/ja/posts/4564, https://yakst.com/ja/posts/4564}
 * @<href>{https://yoku0825.blogspot.com/2017/04/mysql-801grouping.html, https://yoku0825.blogspot.com/2017/04/mysql-801grouping.html}

=== @<hidx>{LATERAL句}@<tt>|LATERAL|句

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/lateral-derived-tables.html, https://dev.mysql.com/doc/refman/8.0/ja/lateral-derived-tables.html}

==== ブログ記事等

 * @<href>{https://mysqlserverteam.com/support-for-lateral-derived-tables-added-to-mysql-8-0-14/, https://mysqlserverteam.com/support-for-lateral-derived-tables-added-to-mysql-8-0-14/}
 * @<href>{https://mita2db.hateblo.jp/entry/MySQL_8%E3%81%AE%E6%96%B0%E6%A9%9F%E8%83%BD_LATERAL%E5%8F%A5%E3%81%AB%E3%82%88%E3%82%8BSQL%E3%83%81%E3%83%A5%E3%83%BC%E3%83%8B%E3%83%B3%E3%82%B0, https://mita2db.hateblo.jp/entry/MySQL_8%E3%81%AE%E6%96%B0%E6%A9%9F%E8%83}@<br>{}@<href>{https://mita2db.hateblo.jp/entry/MySQL_8%E3%81%AE%E6%96%B0%E6%A9%9F%E8%83%BD_LATERAL%E5%8F%A5%E3%81%AB%E3%82%88%E3%82%8BSQL%E3%83%81%E3%83%A5%E3%83%BC%E3%83%8B%E3%83%B3%E3%82%B0, %BD_LATERAL%E5%8F%A5%E3%81%AB%E3%82%88%E3%82%8BSQL%E3%83%81%E3}@<br>{}@<href>{https://mita2db.hateblo.jp/entry/MySQL_8%E3%81%AE%E6%96%B0%E6%A9%9F%E8%83%BD_LATERAL%E5%8F%A5%E3%81%AB%E3%82%88%E3%82%8BSQL%E3%83%81%E3%83%A5%E3%83%BC%E3%83%8B%E3%83%B3%E3%82%B0, %83%A5%E3%83%BC%E3%83%8B%E3%83%B3%E3%82%B0}
 * @<href>{https://tombo2.hatenablog.com/entry/2019/03/21/210806, https://tombo2.hatenablog.com/entry/2019/03/21/210806}
 * 著者ブログ
 ** @<href>{https://qiita.com/hmatsu47/items/040d65d118d0ecec6381, https://qiita.com/hmatsu47/items/040d65d118d0ecec6381}

=== @<hidx>{派生テーブル}派生（@<idx>{Derived}）テーブルからの外部テーブル参照

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/derived-tables.html, https://dev.mysql.com/doc/refman/8.0/ja/derived-tables.html}
 ** 「MySQL 8.0.14より前は、…」

==== ブログ記事等

 * @<href>{https://mysqlserverteam.com/supporting-all-kinds-of-outer-references-in-derived-tables-lateral-or-not/, https://mysqlserverteam.com/supporting-all-kinds-of-outer-references-in-derived-tables-lateral-or-not/}

=== その他のDML

==== 標準SQL対応など（@<hidx>{TABLEステートメント}@<tt>|TABLE|ステートメント・@<hidx>{VALUESステートメント}@<tt>|VALUES|ステートメントほか）

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/table.html, https://dev.mysql.com/doc/refman/8.0/ja/table.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/values.html, https://dev.mysql.com/doc/refman/8.0/ja/values.html}
 * @<href>{http://blog.kimuradb.com/?eid=877608, http://blog.kimuradb.com/?eid=877608}
 * @<href>{https://tmtms.hatenablog.com/entry/202001/mysql-table-values, https://tmtms.hatenablog.com/entry/202001/mysql-table-values}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/union.html, https://dev.mysql.com/doc/refman/8.0/ja/union.html}
 ** @<hidx>{UNION}@<tt>|UNION SELECT ... INTO OUTFILE|が非推奨に
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/select-into.html, https://dev.mysql.com/doc/refman/8.0/ja/select-into.html}
 ** @<hidx>{INTO}@<tt>|INTO|のロック句前への配置が非推奨に

===={80221001} @<idx>{括弧付きクエリ式}のサポート

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/parenthesized-query-expressions.html, https://dev.mysql.com/doc/refman/8.0/ja/parenthesized-query-expressions.html}

==== @<tt>|INSERT ... ON DUPLICATE KEY UPDATE|で新しい行に対する行エイリアス・列エイリアスをサポート（@<tt>|VALUES()|による参照を非推奨に）

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/insert-on-duplicate.html, https://dev.mysql.com/doc/refman/8.0/ja/insert-on-duplicate.html}
 ** 「MySQL 8.0.19以降では、…」
 ** 「新しい行とカラムを参照するためのVALUES()の使用は、…」

== 関数の変更と新機能

=== @<idx>{正規表現関数}

利用@<idx>{ライブラリ}が@<idx>{ICU}（@<idx>{International Components for Unicode}）に変わるとともに、新しい正規表現関数が追加されました。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/regexp.html, https://dev.mysql.com/doc/refman/8.0/ja/regexp.html}
 ** @<hidx>{REGEXP_INSTR}@<tt>|REGEXP_INSTR|
 ** @<hidx>{REGEXP_LIKE}@<tt>|REGEXP_LIKE|
 ** @<hidx>{REGEXP_REPLACE}@<tt>|REGEXP_REPLACE|
 ** @<hidx>{REGEXP_SUBSTR}@<tt>|REGEXP_SUBSTR|

==== ブログ記事等

 * @<href>{https://www.s-style.co.jp/blog/2018/09/2519/, https://www.s-style.co.jp/blog/2018/09/2519/}
 * @<href>{https://yoku0825.blogspot.com/2018/01/mysql-804mysql.html, https://yoku0825.blogspot.com/2018/01/mysql-804mysql.html}
 * @<href>{https://yoku0825.blogspot.com/2018/02/mysql-804regexpsubstr-regexpinstr.html, https://yoku0825.blogspot.com/2018/02/mysql-804regexpsubstr-regexpinstr.html}
 * @<href>{https://yoku0825.blogspot.com/2018/04/vs-mysql-80.html, https://yoku0825.blogspot.com/2018/04/vs-mysql-80.html}
 ** 危険な正規表現

=== @<hidx>{STATEMENT_DIGEST()}@<tt>|STATEMENT_DIGEST()|／@<hidx>{STATEMENT_DIGEST_TEXT()}@<tt>|STATEMENT_DIGEST_TEXT()|

@<idx>{SQLステートメント}の@<idx>{正規化}（@<idx>{Normalize}）を行う関数です。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/encryption-functions.html#function_statement-digest, https://dev.mysql.com/doc/refman/8.0/ja/encryption-functions.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/encryption-functions.html#function_statement-digest, function_statement-digest}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/encryption-functions.html#function_statement-digest-text, https://dev.mysql.com/doc/refman/8.0/ja/encryption-functions.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/encryption-functions.html#function_statement-digest-text, function_statement-digest-text}

==== ブログ記事等

 * @<href>{https://yoku0825.blogspot.com/2018/04/mysql-80statementdigestsql.html, https://yoku0825.blogspot.com/2018/04/mysql-80statementdigestsql.html}

//embed[latex]{
\clearpage
//}

 * 著者ブログ
 ** @<href>{https://hmatsu47.hatenablog.com/entry/2018/04/19/230021, https://hmatsu47.hatenablog.com/entry/2018/04/19/230021}

=== その他の関数

==== @<idx>{UUID関数}

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/miscellaneous-functions.html, https://dev.mysql.com/doc/refman/8.0/ja/miscellaneous-functions.html}
 ** @<hidx>{UUID_TO_BIN}@<tt>|UUID_TO_BIN|・@<hidx>{BIN_TO_UUID}@<tt>|BIN_TO_UUID|・@<hidx>{IS_UUID}@<tt>|IS_UUID|

==== @<hidx>{BLOB列}@<tt>|BLOB|列に対する@<idx>{ビット処理}

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/bit-functions.html, https://dev.mysql.com/doc/refman/8.0/ja/bit-functions.html}

===={80221002} @<hidx>{CAST()}@<tt>|CAST()|関数または@<hidx>{CONVERT()}@<tt>|CONVERT()|関数による@<hidx>{YEAR型}@<tt>|YEAR|型へのキャスト

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/cast-functions.html#function_convert, https://dev.mysql.com/doc/refman/8.0/ja/cast-functions.html#function_convert}

===={80221003} @<tt>|CAST(value AT TIME ZONE specifier AS DATETIME)|による@<tt>|TIMESTAMP|列値の@<idx>{タイムゾーン変換}

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/cast-functions.html#function_cast, https://dev.mysql.com/doc/refman/8.0/ja/cast-functions.html#function_cast}
 ** 「MySQL 8.0.22以降、…」

== その他各種新機能

=== @<idx>{Query Rewriteプラグイン}

==== Query Rewriteプラグインの@<idx>{書き換え}対応（@<tt>|INSERT|・@<tt>|UPDATE|・@<tt>|DELETE|）

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/rewriter-query-rewrite-plugin.html, https://dev.mysql.com/doc/refman/8.0/ja/rewriter-query-rewrite-plugin.html}
 * 著者ブログ
 ** @<href>{https://qiita.com/hmatsu47/items/a43db9fb8c0504f15a79, https://qiita.com/hmatsu47/items/a43db9fb8c0504f15a79}

=== 新しいメモリ内@<idx>{テンポラリテーブル}@<idx>{ストレージエンジン}

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/internal-temporary-tables.html, https://dev.mysql.com/doc/refman/8.0/ja/internal-temporary-tables.html}
 ** 「テーブル内のBLOBまたはTEXTカラムの存在…」
 * @<href>{https://nippondanji.blogspot.com/2018/06/temptable.html, https://nippondanji.blogspot.com/2018/06/temptable.html}
 * @<href>{https://mysqlserverteam.com/mysql-8-0-support-for-blobs-in-temptable-engine/, https://mysqlserverteam.com/mysql-8-0-support-for-blobs-in-temptable-engine/}

=== @<idx>{エラーロギング}

==== 新しいエラーロギングインフラストラクチャ／エラーロギングの改善

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/error-log-configuration.html, https://dev.mysql.com/doc/refman/8.0/ja/error-log-configuration.html}
 * @<href>{https://www.s-style.co.jp/blog/2018/07/2061/, https://www.s-style.co.jp/blog/2018/07/2061/}

//embed[latex]{
\clearpage
//}

 * @<href>{https://variable.jp/2018/03/14/mysql8-0-%E3%82%A8%E3%83%A9%E3%83%BC%E3%83%AD%E3%82%B0%E3%81%AE%E8%A8%AD%E5%AE%9A/, https://variable.jp/2018/03/14/mysql8-0-%E3%82%A8%E3%83%A9%E3%83%BC%E3%83}@<br>{}@<href>{https://variable.jp/2018/03/14/mysql8-0-%E3%82%A8%E3%83%A9%E3%83%BC%E3%83%AD%E3%82%B0%E3%81%AE%E8%A8%AD%E5%AE%9A/, %AD%E3%82%B0%E3%81%AE%E8%A8%AD%E5%AE%9A/}
 * @<href>{https://yoku0825.blogspot.com/2018/01/mysql-804_25.html, https://yoku0825.blogspot.com/2018/01/mysql-804_25.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/error-log.html, https://dev.mysql.com/doc/refman/8.0/ja/error-log.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/error-log-format.html, https://dev.mysql.com/doc/refman/8.0/ja/error-log-format.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_log_error_verbosity, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_log_error_verbosity, sysvar_log_error_verbosity}
 ** デフォルト変更：@<hidx>{log_error_verbosity}@<tt>|log_error_verbosity=2|
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/error-log-filtering.html, https://dev.mysql.com/doc/refman/8.0/ja/error-log-filtering.html}
 ** エラーログの@<idx>{フィルタリング}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/error-log-json.html, https://dev.mysql.com/doc/refman/8.0/ja/error-log-json.html}
 ** @<idx>{JSON形式}のエラーログ
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_log_error_suppression_list, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_log_error_suppression_list, sysvar_log_error_suppression_list}
 ** @<tt>|WARNINGS|・@<tt>|NOTE|のエラーログを抑制

==== @<idx>{サーバエラーメッセージ}

 * @<href>{https://dev.mysql.com/doc/mysql-errors/8.0/en/server-error-reference.html, https://dev.mysql.com/doc/mysql-errors/8.0/en/server-error-reference.html}

=== ログ関連（エラーログ以外）

==== @<idx>{syslog}・@<idx>{eventlog}関連の@<idx>{システム変数}を@<idx>{コンポーネント変数}に指定

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_syseventlog.facility, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_syseventlog.facility, sysvar_syseventlog.facility}

==== @<idx>{スロークエリログ}への@<hidx>{log-slow-extra}@<tt>|log-slow-extra|の追加

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_log_slow_extra, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_log_slow_extra, sysvar_log_slow_extra}

==== @<idx>{監査ログフィルタ}：@<idx>{ルールベース}の条件でクエリを中断（Enterprise版）

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/audit-log-filtering.html, https://dev.mysql.com/doc/refman/8.0/ja/audit-log-filtering.html}
 ** 「フィルタルールには、…」

==== @<idx>{監査ログ}の@<idx>{JSON形式}化・圧縮・@<idx>{暗号化}（5.7.21と同様／Enterprise版）

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/audit-log-file-formats.html, https://dev.mysql.com/doc/refman/8.0/ja/audit-log-file-formats.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/audit-log-logging-configuration.html#audit-log-file-encryption-old, https://dev.mysql.com/doc/refman/8.0/ja/audit-log-logging-configuration.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/audit-log-logging-configuration.html#audit-log-file-encryption-old, audit-log-file-encryption-old}
 ** 8.0.17での暗号化機能の改良について、8.0.16以前との比較
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/audit-log-reference.html#udf_audit-log-encryption-password-get, https://dev.mysql.com/doc/refman/8.0/ja/audit-log-reference.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/audit-log-reference.html#udf_audit-log-encryption-password-get, udf_audit-log-encryption-password-get}
 ** @<hidx>{audit_log_encryption_password_get()}@<tt>|audit_log_encryption_password_get()|
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/audit-log-reference.html#sysvar_audit_log_password_history_keep_days, https://dev.mysql.com/doc/refman/8.0/ja/audit-log-reference.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/en/audit-log-reference.html#sysvar_audit_log_password_history_keep_days, sysvar_audit_log_password_history_keep_days}
 ** @<hidx>{audit_log_password_history_keep_days}@<tt>|audit_log_password_history_keep_days|

==== 監査ログにデータを挿入するためのSQL関数

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/audit-api-message-emit.html, https://dev.mysql.com/doc/refman/8.0/ja/audit-api-message-emit.html}

===={80221004} 監査プラグインで@<tt>|audit_log_read()|を使用したログ読み込み操作を改善（Enterprise版）

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/audit-log-file-reading.html, https://dev.mysql.com/doc/refman/8.0/ja/audit-log-file-reading.html}
 ** 「start: 最初に読み取るイベントの監査ログ内の位置。」
 * @<href>{https://mysqlserverteam.com/mysql-audit-data-consolidation-made-simple/, https://mysqlserverteam.com/mysql-audit-data-consolidation-made-simple/}

===={80241001} 監査ログファイル（JSON形式）のローテーション後削除（プルーニング）

 * @<href>{https://dev.mysql.com/doc/refman/8.0/en/audit-log-logging-configuration.html#audit-log-space-management, https://dev.mysql.com/doc/refman/8.0/en/audit-log-logging-configuration.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/en/audit-log-logging-configuration.html#audit-log-space-management, audit-log-space-management}

==== @<idx>{データマスキング機能}（5.7.24と同様／Enterprise版）

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/data-masking.html, https://dev.mysql.com/doc/refman/8.0/ja/data-masking.html}

=== その他の変更と新機能

==== @<idx>{クエリキャッシュ}の廃止

 * @<href>{https://yakst.com/ja/posts/4612, https://yakst.com/ja/posts/4612}

==== オンディスク@<idx>{テンポラリテーブル}@<idx>{ストレージエンジン}

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/internal-temporary-tables.html, https://dev.mysql.com/doc/refman/8.0/ja/internal-temporary-tables.html}
 ** InnoDBのみ利用可能に（MyISAM廃止）

==== 255文字までの@<idx>{ホスト名}に対応

 * @<tt>|GRANT|・@<tt>|CHANGE REPLICATION SOURCE TO|（いずれも既出）・@<hidx>{SHOW PROCESSLIST}@<tt>|SHOW PROCESSLIST|・各種ログなど

==== @<idx>{キャスト関数}・@<idx>{オペレータ}（@<idx>{演算子}）の拡張

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/cast-functions.html, https://dev.mysql.com/doc/refman/8.0/ja/cast-functions.html}

==== @<idx>{日時リテラル}が@<idx>{タイムゾーンオフセット付きフォーマット}に対応

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/datetime.html, https://dev.mysql.com/doc/refman/8.0/ja/datetime.html}
 ** 「MySQL 8.0.19では、…」
 * @<href>{https://tmtms.hatenablog.com/entry/202001/mysql-datetime-offset, https://tmtms.hatenablog.com/entry/202001/mysql-datetime-offset}

==== @<idx>{非推奨}（@<idx>{Deprecated}）となった@<idx>{関数}・演算子など（8.0.17以降）

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/numeric-type-syntax.html, https://dev.mysql.com/doc/refman/8.0/ja/numeric-type-syntax.html}
 ** @<hidx>{FLOAT(M,D)}@<tt>|FLOAT(M,D)|・@<hidx>{DOUBLE(M,D)}@<tt>|DOUBLE(M,D)|、@<hidx>{ZEROFILL属性}@<tt>|ZEROFILL|属性、@<tt>|FLOAT|・@<tt>|DOUBLE|・@<hidx>{DECIMAL}@<tt>|DECIMAL|に付加する@<hidx>{UNSIGNED属性}@<tt>|UNSIGNED|属性
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/numeric-type-attributes.html, https://dev.mysql.com/doc/refman/8.0/ja/numeric-type-attributes.html}
 ** @<tt>|FLOAT|・@<tt>|DOUBLE|カラムに対する@<hidx>{AUTO_INCREMENT}@<tt>|AUTO_INCREMENT|
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/string-type-syntax.html, https://dev.mysql.com/doc/refman/8.0/ja/string-type-syntax.html}
 ** @<idx>{文字列データ型}における@<hidx>{BINARY属性}@<tt>|BINARY|属性
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/logical-operators.html, https://dev.mysql.com/doc/refman/8.0/ja/logical-operators.html}
 ** @<idx>{論理演算子}@<hidx>{「&&」}「@<tt>|&&|」・@<hidx>{「||」}@<tt>{「||」}・@<hidx>{「!」}@<tt>|「!」|
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/information-functions.html, https://dev.mysql.com/doc/refman/8.0/ja/information-functions.html}
 ** @<hidx>{SQL_CALC_FOUND_ROWS}@<tt>|SQL_CALC_FOUND_ROWS|、@<hidx>{FOUND_ROWS()}@<tt>|FOUND_ROWS()|
 * @<href>{https://dev.mysql.com/worklog/task/?id=13537, https://dev.mysql.com/worklog/task/?id=13537}
 ** @<hidx>{YEAR型}@<tt>|YEAR|型の表示文字数@<tt>|(n)|と@<tt>|UNSIGNED|属性

===={80221005} @<idx>{InnoDB memcachedプラグイン}が非推奨に

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-memcached.html, https://dev.mysql.com/doc/refman/8.0/ja/innodb-memcached.html}
 ** 「InnoDB memcachedプラグインは、MySQL 8.0.22の時点では非推奨です。」

==== @<hidx>{INTO OUTFILE}@<tt>|INTO OUTFILE|・@<hidx>{INTO DUMPFILE}@<tt>|INTO DUMPFILE|で生成するファイルの@<idx>{パーミッション}を@<tt>|0640|に

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/select-into.html, https://dev.mysql.com/doc/refman/8.0/ja/select-into.html}
 ** 「MySQL 8.0.17では、ファイル作成のumaskは0640です。」

==== @<hidx>{LOAD DATA INFILE LOCAL}@<tt>|LOAD DATA INFILE LOCAL|に関する変更

 * @<href>{https://mita2db.hateblo.jp/entry/2020/01/13/163218, https://mita2db.hateblo.jp/entry/2020/01/13/163218}

==== 新しい@<idx>{バックアップロック}

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/lock-instance-for-backup.html, https://dev.mysql.com/doc/refman/8.0/ja/lock-instance-for-backup.html}

==== コネクション@<idx>{圧縮}関連

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/connection-compression-control.html, https://dev.mysql.com/doc/refman/8.0/ja/connection-compression-control.html}
 ** @<tt>|Compression_algorithm|・@<tt>|Compression_level|
 ** @<tt>|protocol_compression_algorithms|
 ** @<tt>|Compression|（非推奨に）

==== サーバ側@<idx>{キーリング}移行ツール

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/keyring-key-migration.html, https://dev.mysql.com/doc/refman/8.0/ja/keyring-key-migration.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/en/mysql-migrate-keyring.html, https://dev.mysql.com/doc/refman/8.0/en/mysql-migrate-keyring.html}

==== キーリングでサポートするキー長の増加

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/keyring-key-types.html, https://dev.mysql.com/doc/refman/8.0/ja/keyring-key-types.html}

==== @<idx>{AWS KMS}用のキーリングプラグイン（5.7.19と同様）

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/keyring-aws-plugin.html, https://dev.mysql.com/doc/refman/8.0/ja/keyring-aws-plugin.html}

==== @<idx>{HashiCorp Vault}用のキーリングプラグイン

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/keyring-hashicorp-plugin.html, https://dev.mysql.com/doc/refman/8.0/ja/keyring-hashicorp-plugin.html}

===={80221006} @<idx>{Oracle Cloud Infrastructure}（OCI）用のキーリングプラグイン

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/keyring-oci-plugin.html, https://dev.mysql.com/doc/refman/8.0/ja/keyring-oci-plugin.html}

===={80241002} キーリングプラグインのコンポーネント化

 * @<href>{https://dev.mysql.com/doc/refman/8.0/en/keyring-file-component.html, https://dev.mysql.com/doc/refman/8.0/en/keyring-file-component.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/en/keyring-encrypted-file-component.html, https://dev.mysql.com/doc/refman/8.0/en/keyring-encrypted-file-component.html}
 ** Enterprise版
 * @<href>{https://dev.mysql.com/doc/refman/8.0/en/keyring-component-installation.html, https://dev.mysql.com/doc/refman/8.0/en/keyring-component-installation.html}
 ** @<tt>|ALTER INSTANCE RELOAD KEYRING|

==== 復旧／切り離された準備済み@<idx>{XAトランザクション}のMDLロック有効化

 * @<href>{https://dev.mysql.com/doc/relnotes/mysql/8.0/en/news-8-0-13.html#mysqld-8-0-13-xa, https://dev.mysql.com/doc/relnotes/mysql/8.0/en/news-8-0-13.html#}@<br>{}@<href>{https://dev.mysql.com/doc/relnotes/mysql/8.0/en/news-8-0-13.html#mysqld-8-0-13-xa, mysqld-8-0-13-xa}

==== @<idx>{外部キー}のための@<idx>{メタデータロック}のサポート

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/lock-tables.html, https://dev.mysql.com/doc/refman/8.0/ja/lock-tables.html}
 ** 「LOCK TABLESとトリガーで説明されているように、…」

==== @<idx>{The LOCK_ORDER Tool}

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/lock-order-tool.html, https://dev.mysql.com/doc/refman/8.0/ja/lock-order-tool.html}

==== SSLチェックを効率化するための@<hidx>{--ssl-mode}@<tt>|--ssl-mode|クライアントオプション

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/connection-options.html#option_general_ssl-mode, https://dev.mysql.com/doc/refman/8.0/ja/connection-options.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/connection-options.html#option_general_ssl-mode, option_general_ssl-mode}

==== @<hidx>{--binary-as-hex}@<tt>|--binary-as-hex|オプションに関する挙動の変更

 * @<href>{https://next4us-ti.hatenablog.com/entry/2020/02/28/174519, https://next4us-ti.hatenablog.com/entry/2020/02/28/174519}
 * @<href>{http://blog.kimuradb.com/?eid=877614, http://blog.kimuradb.com/?eid=877614}

==== @<idx>{サービスレジストリ}と@<idx>{コンポーネントインフラストラクチャ}

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/components.html, https://dev.mysql.com/doc/refman/8.0/ja/components.html}

==== @<idx>{レプリケーションストリーム}を読み取るためのC API

 * @<href>{https://dev.mysql.com/doc/c-api/8.0/en/c-api-binary-log-function-reference.html, https://dev.mysql.com/doc/c-api/8.0/en/c-api-binary-log-function-reference.html}

==== @<idx>{非同期C API}

 * @<href>{https://dev.mysql.com/doc/c-api/8.0/en/c-api-asynchronous-interface.html, https://dev.mysql.com/doc/c-api/8.0/en/c-api-asynchronous-interface.html}
 ** mysqlプロトコルを利用した非同期API

===={80221007} C APIで@<hidx>{mysql_real_connect_dns_srv()}@<tt>|mysql_real_connect_dns_srv()|をサポート

 * @<href>{https://dev.mysql.com/doc/c-api/8.0/en/mysql-real-connect-dns-srv.html, https://dev.mysql.com/doc/c-api/8.0/en/mysql-real-connect-dns-srv.html}
 * @<href>{https://tmtms.hatenablog.com/entry/202011/mysql-8022-dns-srv, https://tmtms.hatenablog.com/entry/202011/mysql-8022-dns-srv}

===={80231001} @<tt>|mysql_bind_param()| C API関数によるクエリ属性の定義

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/query-attributes.html, https://dev.mysql.com/doc/refman/8.0/ja/query-attributes.html}
 ** ユーザ定義関数を使用してSQL文内から属性値の取得が可能に

==== @<idx>{UDF}自動登録コンポーネントのためのUDF登録サービス

 * @<href>{https://dev.mysql.com/doc/dev/mysql-server/latest/structs__mysql__udf__registration.html, https://dev.mysql.com/doc/dev/mysql-server/latest/}@<br>{}@<href>{https://dev.mysql.com/doc/dev/mysql-server/latest/structs__mysql__udf__registration.html, structs__mysql__udf__registration.html}

==== MySQLサーバー文字列コンポーネントサービス

 * @<href>{https://dev.mysql.com/doc/dev/mysql-server/latest/mysql__string_8h.html, https://dev.mysql.com/doc/dev/mysql-server/latest/mysql__string_8h.html}

==== @<idx>{結果セット}の@<idx>{メタデータ}転送オプション

 * @<href>{https://dev.mysql.com/doc/c-api/8.0/en/c-api-optional-metadata.html, https://dev.mysql.com/doc/c-api/8.0/en/c-api-optional-metadata.html}

==== コンポーネント用の@<idx>{ステータス変数}サービス

 * @<href>{https://dev.mysql.com/doc/dev/mysql-server/latest/structs__mysql__status__variable__registration.html, https://dev.mysql.com/doc/dev/mysql-server/latest/}@<br>{}@<href>{https://dev.mysql.com/doc/dev/mysql-server/latest/structs__mysql__status__variable__registration.html, structs__mysql__status__variable__registration.html}

===={80231002} サーバ変数@<hidx>{temptable_max_mmap}@<tt>|temptable_max_mmap|追加

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/internal-temporary-tables.html#internal-temporary-tables-engines, https://dev.mysql.com/doc/refman/8.0/ja/internal-temporary-tables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/internal-temporary-tables.html#internal-temporary-tables-engines, internal-temporary-tables-engines}

==== @<idx>{プラグイン}が@<idx>{プリペアドステートメント}を使えるように

 * @<href>{https://dev.mysql.com/worklog/task/?id=8413, https://dev.mysql.com/worklog/task/?id=8413}

==== @<idx>{ソートバッファ}の@<idx>{動的割り当て}

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/order-by-optimization.html#order-by-filesort, https://dev.mysql.com/doc/refman/8.0/ja/order-by-optimization.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/order-by-optimization.html#order-by-filesort, order-by-filesort}

==== @<hidx>{NO PAD照合順序}@<tt>|NO PAD|照合順序（COLLATION）用の@<idx>{可変長ソートキー}

 * @<href>{https://dev.mysql.com/worklog/task/?id=9554, https://dev.mysql.com/worklog/task/?id=9554}

==== @<hidx>{lower_case_table_names}@<tt>|lower_case_table_names|の有効化に@<hidx>{debconf-set-selections}@<tt>|debconf-set-selections|を利用

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_lower_case_table_names, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_lower_case_table_names, sysvar_lower_case_table_names}

===={80211001} KEYパーティショニングでカラムインデックスプレフィックスを使ったときに正しく警告・エラーを出力するようになった

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/partitioning-limitations.html#partitioning-limitations-prefixes, https://dev.mysql.com/doc/refman/8.0/ja/partitioning-limitations.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/partitioning-limitations.html#partitioning-limitations-prefixes, partitioning-limitations-prefixes}

===={80211002} 文字列型と数値型・時間型の型変換（キャスト）がどのように行われたかを@<tt>|EXPLAIN ANALYZE|・@<tt>|EXPLAIN FORMAT=JSON|・@<tt>|EXPLAIN FORMAT=TREE|で可視化

 * @<href>{https://dev.mysql.com/doc/relnotes/mysql/8.0/en/news-8-0-21.html, https://dev.mysql.com/doc/relnotes/mysql/8.0/en/news-8-0-21.html}
 ** 「Building on work done in MySQL 8.0.18, …」

===={80221008} @<idx>{ネットワーク名前空間指定子}のサポート

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_bind_address, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_bind_address, sysvar_bind_address}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_admin_address, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_admin_address, sysvar_admin_address}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/x-plugin-options-system-variables.html#sysvar_mysqlx_bind_address, https://dev.mysql.com/doc/refman/8.0/ja/x-plugin-options-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/x-plugin-options-system-variables.html#sysvar_mysqlx_bind_address, sysvar_mysqlx_bind_address}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/mysql-command-options.html#option_mysql_network-namespace, https://dev.mysql.com/doc/refman/8.0/ja/mysql-command-options.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/mysql-command-options.html#option_mysql_network-namespace, option_mysql_network-namespace}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/change-replication-source-to.html, https://dev.mysql.com/doc/refman/8.0/ja/change-replication-source-to.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/show-replica-status.html, https://dev.mysql.com/doc/refman/8.0/ja/show-replica-status.html}

===={80241003} @<hidx>{utf8mb3}@<tt>|utf8mb3|表示を@<tt>|utf8|よりも優先

 * クライアントアプリケーション等で

===={80231003} ユーザ定義関数@<tt>|gen_blacklist()|を@<tt>|gen_blocklist()|に変更（Enterprise版）

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/data-masking-functions.html#udf_gen-blocklist, https://dev.mysql.com/doc/refman/8.0/ja/data-masking-functions.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/data-masking-functions.html#udf_gen-blocklist, udf_gen-blocklist}

===={80231004} MySQL Enterprise Firewallにグループプロファイル機能を追加（Enterprise版）

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/firewall-usage.html, https://dev.mysql.com/doc/refman/8.0/ja/firewall-usage.html}
 ** 「MySQL 8.0.23では、ファイアウォールは…」

==== @<idx>{ソースコード}の改善

 * @<href>{https://mysqlserverteam.com/mysql-8-0-source-code-improvements/, https://mysqlserverteam.com/mysql-8-0-source-code-improvements/}

==== @<idx>{環境変数}@<tt>|MYSQL_PWD|が非推奨に

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/environment-variables.html, https://dev.mysql.com/doc/refman/8.0/ja/environment-variables.html}
