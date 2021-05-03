= @<idx>{レプリケーション}の新機能

== @<idx>{バイナリログ}／@<idx>{リレーログ}@<idx>{暗号化}

MySQL 8.0ではInnoDBのテーブルおよび各種ログファイルの@<idx>{透過的暗号化}サポートが進んでいますが、バイナリログとリレーログの暗号化にも対応しました。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-binlog-encryption.html, https://dev.mysql.com/doc/refman/8.0/ja/replication-binlog-encryption.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/alter-instance.html, https://dev.mysql.com/doc/refman/8.0/ja/alter-instance.html}
 ** マスターキーのローテーション（@<hidx>{ALTER INSTANCE}@<tt>|ALTER INSTANCE|）

=== 実行例

@<b>{第8章 InnoDBとオプティマイザの新機能 8.2.4 InnoDB「テーブルスペース／Redo・Undoログと一般テーブルスペースの暗号化」}（P.@<pageref>{80230802}）を参考に、あらかじめ@<idx>{キーリング用プラグイン}の導入を行っておきます。@<fn>{004ENC}

次に、@<idx>{Source}（@<idx>{Master}）・@<idx>{Replica}（@<idx>{Slave}）それぞれのサーバの@<hidx>{/etc/my.cnf}@<tt>|/etc/my.cnf|にレプリケーション関連の設定を記述します。

==== Source（Master）側設定（関連部分のみ）

//list[source(master)][/etc/my.cnf]{
server-id=1
binlog_format=MIXED
# バイナリログ形式をMIXEDに変更しているのは暗号化の確認をしやすくするため。実運用ではROW推奨。
binlog_encryption=ON
binlog_rotate_encryption_master_key_at_startup=ON
//}

//embed[latex]{
\clearpage
//}

==== Replica（Slave）側設定（同上）

//list[replica(slave)][/etc/my.cnf]{
server-id=2
binlog_format=MIXED
binlog_encryption=ON
binlog_rotate_encryption_master_key_at_startup=ON
super_read_only
#skip-slave-start
//}

なお、Replica（Slave）をSource（Master）のディスクイメージからコピーして立てた場合、レプリケーション開始時にエラーが発生することがあります。
@<idx>{データディレクトリ}にある@<hidx>{auto.cnf}@<tt>|auto.cnf|の@<hidx>{server-uuid}@<tt>|server-uuid|が重複していることが原因かもしれません。

その場合、@<tt>|auto.cnf|を削除してからサーバを起動すると@<tt>|server-uuid|が自動生成され、正しくレプリケーションを開始することができます。

==== Source（Master）側操作

//emlist{
mysql> @<i>{CREATE DATABASE enc_test;}
Query OK, 1 row affected (0.01 sec)

mysql> @<i>{USE enc_test;}
Database changed
mysql> @<i>{CREATE TABLE enc_test (id int(10) PRIMARY KEY AUTO_INCREMENT, value VARCHAR(100)) ENGIN}
@<i>{E=innodb ENCRYPTION='Y';}
@<b>{※テストテーブルを作成する。ファイルシステム検索で紛らわしくないよう暗号化テーブルで。}
Query OK, 0 rows affected (0.03 sec)

mysql> @<i>{INSERT INTO enc_test SET value='hoge';}
@<b>{※テストデータを挿入する。}
Query OK, 1 row affected (0.00 sec)

mysql> @<i>{INSERT INTO enc_test SET value='fuga';}
Query OK, 1 row affected (0.00 sec)

@<b>{※ここでOS Shellからファイルシステムに対しhoge・fugaをgrep検索しても引っかからないことが確認できる。}

mysql> @<i>{SHOW BINARY LOGS;}
@<b>{※EncryptedがYesになっている。}
+---------------+-----------+-----------+
| Log_name      | File_size | Encrypted |
+---------------+-----------+-----------+
| binlog.000001 |      1855 | Yes       |
+---------------+-----------+-----------+
1 row in set (0.00 sec)

mysql> @<i>{CREATE USER 'repl'@'%' IDENTIFIED BY 'T35+U53r';}
@<b>{※レプリケーション用ユーザを作成する。}
Query OK, 0 rows affected (0.01 sec)

mysql> @<i>{GRANT REPLICATION SLAVE ON *.* TO 'repl'@'%';}
@<b>{※レプリケーションスレーブ権限を付与する。}
Query OK, 0 rows affected (0.01 sec)
//}

==== Replica（Slave）側操作

Source（Master）側と同様に@<tt>|mysql|コマンドを@<tt>|root|ユーザで操作します。

//emlist{
mysql> @<i>{CHANGE REPLICATION SOURCE TO}
    ->     @<i>{SOURCE_HOST='【ホスト名】',}
    ->     @<i>{SOURCE_USER='repl',}
    ->     @<i>{SOURCE_PASSWORD='T35+U53r',}
    ->     @<i>{SOURCE_LOG_FILE='binlog.000001',}
    ->     @<i>{SOURCE_LOG_POS=4;}
Query OK, 0 rows affected, 2 warnings (0.01 sec)

mysql> @<i>{START REPLICA;}
Query OK, 0 rows affected (0.00 sec)

mysql> @<i>{SHOW REPLICA STATUS\G}
*************************** 1. row ***************************
             Replica_IO_State: Waiting for source to send event
@<b>{（中略）}
                   Last_Errno: 1410
                   Last_Error: Error 'You are not allowed to create a user with GRANT' on quer
y. Default database: ''. Query: 'GRANT REPLICATION SLAVE ON *.* TO 'repl'@'%''
@<b>{（中略）}
        Seconds_Behind_Source: NULL
Source_SSL_Verify_Server_Cert: No
@<b>{（中略）}
               Last_SQL_Errno: 1410
               Last_SQL_Error: Error 'You are not allowed to create a user with GRANT' on quer
y. Default database: ''. Query: 'GRANT REPLICATION SLAVE ON *.* TO 'repl'@'%''
  Replicate_Ignore_Server_Ids:
             Source_Server_Id: 1
@<b>{（中略）}
        Get_source_public_key: 0
1 row in set (0.00 sec)
@<b>{※先ほどSource（Master）でユーザを作成したことが原因。Replica（Slave）で同じユーザを作成する必要はないのでスキップする。}

mysql> @<i>{SET GLOBAL SQL_SLAVE_SKIP_COUNTER=1;}
Query OK, 0 rows affected (0.00 sec)

mysql> @<i>{START REPLICA;}
Query OK, 0 rows affected (0.00 sec)

mysql> @<i>{SHOW REPLICA STATUS\G}
*************************** 1. row ***************************
             Replica_IO_State: Waiting for source to send event
@<b>{（中略）}
              Source_Log_File: binlog.000001
          Read_Source_Log_Pos: 1856
@<b>{（中略）}
        Seconds_Behind_Source: 0
@<b>{（中略）}
                Last_IO_Errno: 0
                Last_IO_Error:
               Last_SQL_Errno: 0
               Last_SQL_Error:
@<b>{（中略）}
        Get_source_public_key: 0
1 row in set (0.00 sec)
@<b>{※今度は成功。ここでファイルシステムに対しhoge・fugaをgrep検索しても引っかからないことが確認できる。}
//}

== @<idx>{バイナリログトランザクション圧縮}

8.0.20でバイナリログトランザクション圧縮がサポートされました。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/binary-log-transaction-compression.html, https://dev.mysql.com/doc/refman/8.0/ja/binary-log-transaction-compression.html}

== @<idx>{バイナリログ有効期限}の指定方法変更

細かい点ですが、原則として日単位ではなく秒単位で有効期限を設定する仕様になったのでご注意ください。
なお、MySQL 8.0のDMR／RC版を試した経験がある方は、GA前に二度の仕様変更があった点にもご注意ください。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-options-binary-log.html#sysvar_binlog_expire_logs_seconds, https://dev.mysql.com/doc/refman/8.0/ja/replication-options-binary-log.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-options-binary-log.html#sysvar_binlog_expire_logs_seconds, sysvar_binlog_expire_logs_seconds}

====[column] @<tt>|その他のバイナリログ関連情報|

MySQL 8.0のバイナリログについては、以下の情報も参考になります。

 * @<href>{https://www.s-style.co.jp/blog/2019/11/5440/, https://www.s-style.co.jp/blog/2019/11/5440/}
 * @<href>{https://labs.gree.jp/blog/2019/10/19616/, https://labs.gree.jp/blog/2019/10/19616/}
 * @<href>{https://labs.gree.jp/blog/2019/10/19628/, https://labs.gree.jp/blog/2019/10/19628/}
 * @<href>{https://labs.gree.jp/blog/2019/11/19752/, https://labs.gree.jp/blog/2019/11/19752/}
 * @<href>{https://labs.gree.jp/blog/2019/11/19832/, https://labs.gree.jp/blog/2019/11/19832/}
 * @<href>{https://labs.gree.jp/blog/2019/11/19898/, https://labs.gree.jp/blog/2019/11/19898/}

====[/column]

== @<idx>{InnoDB Cluster}

InnoDB ClusterはMySQL 5.7で導入されたMySQLの@<idx>{高可用性}ソリューションです。
グループレプリケーション・MySQL Router・MySQL Shellの3つのコンポーネントで構成されています。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-innodb-cluster.html, https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-innodb-cluster.html}
 * @<href>{https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-innodb-cluster-working-with-cluster.html, https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-innodb-cluster-working-with-cluster.html}

なお、8.0.17で@<idx>{クローンプラグイン}との組み合わせで@<idx>{自動ノードプロビジョニング}をサポートしました。

 * @<href>{https://mysqlhighavailability.com/mysql-innodb-cluster-automatic-node-provisioning/, https://mysqlhighavailability.com/mysql-innodb-cluster-automatic-node-provisioning/}
 * @<href>{https://mysqlhighavailability.com/a-breakthrough-in-usability-automatic-node-provisioning/, https://mysqlhighavailability.com/a-breakthrough-in-usability-automatic-node-provisioning/}

== @<idx>{グループレプリケーション}

グループレプリケーションは、Masterサーバの@<idx>{冗長化}を目的としてMySQL 5.7から導入された機能です。
InnoDB Clusterのベースとなる機能の1つです。

詳細は、公式リファレンスマニュアルおよびブログ記事等（注：MySQL 5.7時点のものです）を確認してください。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication.html, https://dev.mysql.com/doc/refman/8.0/ja/group-replication.html}

==={80210701} グループレプリケーションの新機能

 * @<idx>{オンライン}およびユーザーによる@<idx>{プライマリ切り替え／選出}（@<idx>{Election}）
 * オンラインおよびユーザーによる@<idx>{シングルプライマリ}／@<idx>{マルチプライマリ}の切り替え
 * サーバがグループから削除されたときにサーバを@<idx>{シャットダウン}する
 * 応答のないメンバーをグループから追放
 * @<idx>{プライマリフェイルオーバ}ー時の一貫読み取り
 * プライマリフェイルオーバー候補の@<idx>{優先順位}設定
 * 非同期接続フェイルオーバ機能がグループレプリケーション・トポロジをサポート
 * メンバーの@<idx>{書き込み許可}を自動でOFF（@<hidx>{super_read_only}@<tt>|super_read_only|のチェック）
 * 複数バージョンが混在するグループの運用性を向上
 * メンバーの@<idx>{オンラインアップデート}
 * グループレプリケーションでの@<idx>{自動プロビジョニング}
 * @<idx>{TLS 1.3}のサポート
 * @<idx>{IPv6}のサポート
 * @<idx>{圧縮}の設定
 * @<idx>{フロー制御}を微調整するためのオプション
 * @<idx>{許可リスト}でのホスト名のサポート

//embed[latex]{
\clearpage
//}

 * @<idx>{メッセージング}関連
 ** 設定可能な@<idx>{メッセージングパイプライン}
 ** メッセージ受け渡しのトレース
 ** メッセージの最大サイズ
 ** メッセージのキャッシュサイズ
 * @<idx>{トランザクションセーブポイント}のサポート
 * @<idx>{Performance Schema}項目の追加
 ** threads.INSTRUMENTED
 ** memory_summary_global_by_event_name
 ** setup_instruments（列追加）
 * 同上／グループ全体の認証と@<idx>{Applier統計}の@<idx>{モニタリング}
 * レプリケーション接続オプション
 * バイナリログのチェックサムをサポート
 * 可用性向上のために2つのシステム変数のデフォルト値を変更
 ** @<tt>|group_replication_member_expel_timeout|・@<tt>|group_replication_autorejoin_tries|

== @<idx>{MySQL Router}

MySQL Routerはアプリケーションサーバ～MySQLサーバ間の透過的なルーティングを提供する軽量なミドルウェアです。
前述の通り@<idx>{InnoDB Cluster}の主要コンポーネントの1つです。

 * @<href>{https://www.mysql.com/jp/products/enterprise/router.html, https://www.mysql.com/jp/products/enterprise/router.html}

==={80220703} MySQL Routerの新機能

 * 最後に利用したサーバアドレス等の@<idx>{永続化}
 * 接続成功時に@<hidx>{max_connect_errors}@<tt>|max_connect_errors|をリセット
 * @<hidx>{mysqlrouter_plugin_infoツール}@<tt>|mysqlrouter_plugin_info|ツールを追加
 * @<idx>{メタデータキャッシュ}の@<idx>{TTL}を300秒から500ミリ秒に短縮
 * @<idx>{ルーティングストラテジ}を追加（@<hidx>{routing_strategy}@<tt>|routing_strategy|オプション）
 * 起動オプション追加
 * プライマリからセカンダリに降格したサーバーノードへのクライアント接続を解除
 * @<idx>{HTTPサーバプラグイン}と@<idx>{REST API}
 ** 8.0.17で追加、8.0.22でデフォルト有効に
 * MySQL Serverのソースツリーの一部としてRouterを構築

== MySQL Shell

MySQL 5.7で導入された@<idx>{MySQL Shell}も機能向上しています@<fn>{010MSH}。

==== @<idx>{InnoDB Cluster}に対するMySQL 8.0のサポート

 * 前述（InnoDB Clusterのセクションを参照）。

===={80230702} @<idx>{AdminAPI}

 * @<href>{https://dev.mysql.com/doc/mysql-shell/8.0/ja/admin-api-userguide.html, https://dev.mysql.com/doc/mysql-shell/8.0/ja/admin-api-userguide.html}
 ** 8.0.17で機能向上、さらに8.0.19でInnoDB ReplicaSet（後述）などをサポート
 ** 8.0.20でInnoDB Cluster・InnoDB ReplicaSetの管理者およびルータアカウント設定などをサポート
 ** 8.0.23でクラスタ診断機能が強化

==== @<idx>{InnoDB ReplicaSet}

AdminAPIの機能として8.0.19で追加されました。InnoDB Clusterとは違い、非同期レプリケーションによるクラスタを構成します。

 * @<href>{https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-innodb-replicaset.html, https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-innodb-replicaset.html}

==== 新しい@<idx>{Shellプラグイン構造}

 * @<href>{https://mysqlserverteam.com/mysql-shell-plugins-introduction/, https://mysqlserverteam.com/mysql-shell-plugins-introduction/}

==== @<idx>{Clone Plugin}（@<idx>{クローンプラグイン}）

別ノードへのデータのオンラインコピーや自動プロビジョニングがサポートされました。

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/clone-plugin.html, https://dev.mysql.com/doc/refman/8.0/ja/clone-plugin.html}
 * @<href>{https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-innodb-cluster-clone-deployment.html, https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-innodb-cluster-clone-deployment.html}

===={80210703} @<idx>{論理ダンプ・リストアツール}

 * @<href>{https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-shell-utilities-table-export.html, https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-shell-utilities-table-export.html}
 ** Table Export Utility
 * @<href>{https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-shell-utilities-parallel-table.html, https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-shell-utilities-parallel-table.html}
 ** Parallel Table Import Utility
 ** 8.0.23で複数データファイルからのインポートをサポート
 * @<href>{https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-shell-utilities-dump-instance-schema.html, https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-shell-utilities-dump-instance-schema.html}
 ** Instance Dump Utility・Schema Dump Utility・Table Dump Utility
 * @<href>{https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-shell-utilities-load-dump.html, https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-shell-utilities-load-dump.html}
 ** Dump Loading Utility
 ** 8.0.24で主キーのないテーブルの処理に対するオプションを追加

==== セキュアな@<idx>{パスワード管理}

 * @<href>{https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-shell-pluggable-password-store.html, https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-shell-pluggable-password-store.html}

==== Python 3サポート

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/mysql-shell-tutorial-python.html, https://dev.mysql.com/doc/refman/8.0/ja/mysql-shell-tutorial-python.html}

===={80240701} その他の新機能と変更

 * Centralized Help System（組み込みヘルプの改善）
 * クエリ結果におけるカラムタイプ表示
 * @<idx>{X DevAPI}サポートをアップデート
 * スクリーンページング
 * Auto-Completion（自動補完）
 * Prompt Themes（カスタマイズ可能なプロンプト）
 * Command Line History Persistence（コマンド履歴永続化）
 * シェルAPIの直接コマンドライン実行
 ** 8.0.24でさらに改善
 * Connection Compression Options（コネクション圧縮オプション）
 * ログ出力の改善

=={80220701} その他のレプリケーション新機能・変更

 * @<idx>{チャネルフィルタ}毎の@<idx>{マルチソースレプリケーション}
 * @<idx>{トランザクション}内@<idx>{テンポラリテーブル}の@<idx>{GTID}サポート
 * @<idx>{JSON列}の@<idx>{部分アップデート}対応レプリケーション
 * @<hidx>{RESET MASTER TO}@<tt>|RESET MASTER TO|
 * @<hidx>{CHANGE REPLICATION SOURCE TO}@<tt>|CHANGE REPLICATION SOURCE TO|@<hidx>{CHANGE MASTER TO}（@<tt>|CHANGE MASTER TO|）のホスト名が255文字まで指定可能に
 * 書き込みセットベースの@<idx>{トランザクション依存関係追跡}
 * 受信側スレッドと適用側スレッドの間の競合の削減
 * 拡張テーブル@<idx>{メタデータ}の@<idx>{バイナリログ}記録
 * @<hidx>{GTID_EXECUTED}@<tt>|GTID_EXECUTED|が空でない場合に設定可能な@<hidx>{GTID_PURGED}@<tt>|GTID_PURGED|
 * 空き容量がなくなったときの安全（@<idx>{ノンブロッキング}）な@<idx>{レプリケーションモニタリング}
 * @<idx>{トランザクション長}のバイナリログへの記録
 * 各トランザクションの@<idx>{サーババージョン}をバイナリログに記録
 * @<hidx>{START REPLICA UNTIL}@<tt>|START REPLICA UNTIL|@<hidx>{START SLAVE UNTIL}（@<tt>|START SLAVE UNTIL|／@<idx>{マルチスレッドレプリケーション}への対応）
 * マルチスレッドレプリカ（スレーブ）で@<tt>|SOURCE_AUTO_POSITION|（@<tt>|MASTER_AUTO_POSITION=1|）のときのリレーログスキップ処理
 * マルチスレッドレプリカ（スレーブ）レプリケーションにおけるデッドロック検出機構の改善
 * @<idx>{遅延レプリケーション}のマイクロ秒対応
 * @<hidx>{binlog-row-event-max-size}@<tt>|binlog-row-event-max-size|システム変数
 * @<idx>{バイナリログキャッシュサイズ}の指定
 * @<hidx>{mysqlbinlog}@<tt>|mysqlbinlog|で@<idx>{圧縮}をサポート
 * 権限を限定したレプリケーション
 * 新しい非同期レプリケーション接続フェイルオーバーメカニズム
 * レプリカ（スレーブ）での主キーのチェックポリシーが設定可能に
 * @<tt>|auto_increment_increment|・@<tt>|auto_increment_offset|のセッション値設定
 * @<idx>{GTID}を使用しないソースからGTIDを使用するレプリカへのレプリケーションが可能に
 * @<hidx>{skip_slave_start}@<tt>|skip_slave_start|システム変数の追加
 * @<idx>{Deprecated}（@<idx>{非推奨}）になった設定等
 ** @<tt>|--master-info-file|
 ** @<tt>|master_info_repository|
 ** @<tt>|relay_log_info_file|
 ** @<tt>|relay_log_info_repository|
 ** @<tt>|slave_compressed_protocol|
 ** @<tt>|slave_rows_search_algorithms|
 ** @<tt>|log_bin_use_v1_row_events|
 ** @<tt>|WAIT_UNTIL_SQL_THREAD_AFTER_GTIDS|
 * 「ホワイトリスト」「マスター」「スレーブ」（用語）の非推奨化による変更
 ** @<tt>|group_replication_ip_whitelist|が@<tt>|group_replication_ip_allowlist|に
 ** @<tt>|MASTER|が@<tt>|SOURCE|（または@<tt>|REPLICATION SOURCE|）に
 ** @<tt>|SLAVE|が@<tt>|REPLICA|に

//footnote[004ENC][8.0.24でプラグインからコンポーネントに変わりました（@<href>{https://dev.mysql.com/doc/refman/8.0/en/keyring-file-component.html, https://dev.mysql.com/doc/refman/8.0/en/keyring-file-component.html}）。]
//footnote[010MSH][InnoDB Clusterのコンポーネントの1つですので、この章で説明します。]
