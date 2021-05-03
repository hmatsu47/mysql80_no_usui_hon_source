= @<idx>{レプリケーション}の新機能

== @<idx>{バイナリログ}／@<idx>{リレーログ}@<idx>{暗号化}

MySQL 8.0ではInnoDBのテーブルおよび各種ログファイルの@<idx>{透過的暗号化}サポートが進んでいますが、バイナリログとリレーログの暗号化にも対応しました。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-binlog-encryption.html, https://dev.mysql.com/doc/refman/8.0/ja/replication-binlog-encryption.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/alter-instance.html, https://dev.mysql.com/doc/refman/8.0/ja/alter-instance.html}
 ** マスターキーのローテーション（@<hidx>{ALTER INSTANCE}@<tt>|ALTER INSTANCE|）

==== ブログ記事等

 * @<href>{https://www.s-style.co.jp/blog/2019/03/3771/, https://www.s-style.co.jp/blog/2019/03/3771/}
 * @<href>{https://mysqlhighavailability.com/binary-log-encryption-at-rest/, https://mysqlhighavailability.com/binary-log-encryption-at-rest/}
 * @<href>{https://mysqlhighavailability.com/how-to-manually-decrypt-an-encrypted-binary-log-file/, https://mysqlhighavailability.com/how-to-manually-decrypt-an-encrypted-binary-log-file/}
 * @<href>{https://mysqlhighavailability.com/rotating-binary-log-master-key-online/, https://mysqlhighavailability.com/rotating-binary-log-master-key-online/}
 ** マスターキーのローテーション
 * @<href>{https://mysqlhighavailability.com/binary-log-encryption-encryption-of-temporary-capture-files/, https://mysqlhighavailability.com/binary-log-encryption-encryption-of-temporary-capture-files/}
 ** @<idx>{Temporary Capture Files}（@<idx>{変更キャプチャファイル}）@<fn>{003CCF}の暗号化をサポート（8.0.17）
 * 著者ブログ
 ** @<href>{https://qiita.com/hmatsu47/items/96980d508f79f9510aa2, https://qiita.com/hmatsu47/items/96980d508f79f9510aa2}

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
@<b>{※warningsの内容については著者ブログを参照。}

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

==== ブログ記事等

 * @<href>{https://rabbitfoot141.hatenablog.com/entry/2020/04/29/033627, https://rabbitfoot141.hatenablog.com/entry/2020/04/29/033627}
 * @<href>{https://next4us-ti.hatenablog.com/entry/2020/05/07/161812, https://next4us-ti.hatenablog.com/entry/2020/05/07/161812}
 * @<href>{https://mysql.wisborg.dk/2020/05/07/mysql-compressed-binary-logs/, https://mysql.wisborg.dk/2020/05/07/mysql-compressed-binary-logs/}

== @<idx>{バイナリログ有効期限}の指定方法変更

細かい点ですが、原則として日単位ではなく秒単位で有効期限を設定する仕様になったのでご注意ください。
なお、MySQL 8.0のDMR／RC版を試した経験がある方は、GA前に二度の仕様変更があった点にもご注意ください。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-options-binary-log.html#sysvar_binlog_expire_logs_seconds, https://dev.mysql.com/doc/refman/8.0/ja/replication-options-binary-log.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-options-binary-log.html#sysvar_binlog_expire_logs_seconds, sysvar_binlog_expire_logs_seconds}

==== ブログ記事等

 * @<href>{https://yoku0825.blogspot.com/2018/04/mysql-803-expirelogsdays.html, https://yoku0825.blogspot.com/2018/04/mysql-803-expirelogsdays.html}

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

==== ブログ記事等

 * @<href>{https://www.s-style.co.jp/blog/2018/11/2722/, https://www.s-style.co.jp/blog/2018/11/2722/}
 * @<href>{https://www.s-style.co.jp/blog/2018/11/2890/, https://www.s-style.co.jp/blog/2018/11/2890/}
 * @<href>{https://www.s-style.co.jp/blog/2018/11/2899/, https://www.s-style.co.jp/blog/2018/11/2899/}
 * @<href>{https://www.s-style.co.jp/blog/2018/11/2904/, https://www.s-style.co.jp/blog/2018/11/2904/}
 * @<href>{https://www.s-style.co.jp/blog/2018/12/2962/, https://www.s-style.co.jp/blog/2018/12/2962/}
 * @<href>{https://www.s-style.co.jp/blog/2018/12/3028/, https://www.s-style.co.jp/blog/2018/12/3028/}
 * @<href>{https://www.s-style.co.jp/blog/2019/02/3489/, https://www.s-style.co.jp/blog/2019/02/3489/}
 * @<href>{https://mysqlserverteam.com/mysql-shell-8-0-whats-new/, https://mysqlserverteam.com/mysql-shell-8-0-whats-new/}
 ** InnoDB Cluster Improvements

//embed[latex]{
\clearpage
//}

 * @<href>{https://mysqlserverteam.com/mysql-innodb-cluster-changing-cluster-topology-modes-live/, https://mysqlserverteam.com/mysql-innodb-cluster-changing-cluster-topology-modes-live/}
 ** InnoDB Cluster用のリモートMySQLサーバの設定と再設定
 ** 拡張Clusterステータス表示（複製遅延時間を含む）
 ** InnoDB Clusterでの手動によるプライマリスイッチオーバーとトポロジの再設定
 ** より多くのユースケースと環境のための高度なクラスターのカスタマイズ
 * @<href>{https://www.youtube.com/watch?v=8JqJpVN3XqY, https://www.youtube.com/watch?v=8JqJpVN3XqY}

なお、8.0.17で@<idx>{クローンプラグイン}との組み合わせで@<idx>{自動ノードプロビジョニング}をサポートしました。

==== ブログ記事等

 * @<href>{https://mysqlhighavailability.com/mysql-innodb-cluster-automatic-node-provisioning/, https://mysqlhighavailability.com/mysql-innodb-cluster-automatic-node-provisioning/}
 * @<href>{https://mysqlhighavailability.com/a-breakthrough-in-usability-automatic-node-provisioning/, https://mysqlhighavailability.com/a-breakthrough-in-usability-automatic-node-provisioning/}
 * @<href>{https://dasini.net/blog/2019/09/10/mysql-innodb-cluster-easy-recovering-and-provisioning/, https://dasini.net/blog/2019/09/10/mysql-innodb-cluster-easy-recovering-and-provisioning/}
 * @<href>{https://yoku0825.blogspot.com/2020/02/innodb-cluster.html, https://yoku0825.blogspot.com/2020/02/innodb-cluster.html}
 ** 全ノード正常停止後の復旧方法

== @<idx>{グループレプリケーション}

グループレプリケーションは、Masterサーバの@<idx>{冗長化}を目的としてMySQL 5.7から導入された機能です。
InnoDB Clusterのベースとなる機能の1つです。

詳細は、公式リファレンスマニュアルおよびブログ記事等（注：MySQL 5.7時点のものです）を確認してください。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication.html, https://dev.mysql.com/doc/refman/8.0/ja/group-replication.html}

==== ブログ記事等

 * @<href>{https://mita2db.hateblo.jp/entry/Group_Replication_%E3%82%92%E8%A9%A6%E3%81%99%281%29_%E5%B0%8E%E5%85%A5%E7%B7%A8, https://mita2db.hateblo.jp/entry/Group_Replication_%E3%82%92%E8%A9%A6%E3%81%99}@<br>{}@<href>{https://mita2db.hateblo.jp/entry/Group_Replication_%E3%82%92%E8%A9%A6%E3%81%99%281%29_%E5%B0%8E%E5%85%A5%E7%B7%A8, %281%29_%E5%B0%8E%E5%85%A5%E7%B7%A8}
 * @<href>{https://mita2db.hateblo.jp/entry/Group_Replication_%E3%82%92%E8%A9%A6%E3%81%99%282%29_%E3%83%AD%E3%83%83%E3%82%AF%E3%81%AE%E6%8C%99%E5%8B%95, https://mita2db.hateblo.jp/entry/Group_Replication_%E3%82%92%E8%A9%A6%E3%81%99}@<br>{}@<href>{https://mita2db.hateblo.jp/entry/Group_Replication_%E3%82%92%E8%A9%A6%E3%81%99%282%29_%E3%83%AD%E3%83%83%E3%82%AF%E3%81%AE%E6%8C%99%E5%8B%95, %282%29_%E3%83%AD%E3%83%83%E3%82%AF%E3%81%AE%E6%8C%99%E5%8B%95}
 * @<href>{https://mita2db.hateblo.jp/entry/Group_Replication_%E3%82%92%E8%A9%A6%E3%81%99%283%29_%E9%9A%9C%E5%AE%B3%E6%99%82%E3%81%AE%E6%8C%99%E5%8B%95, https://mita2db.hateblo.jp/entry/Group_Replication_%E3%82%92%E8%A9%A6%E3%81%99}@<br>{}@<href>{https://mita2db.hateblo.jp/entry/Group_Replication_%E3%82%92%E8%A9%A6%E3%81%99%283%29_%E9%9A%9C%E5%AE%B3%E6%99%82%E3%81%AE%E6%8C%99%E5%8B%95, %283%29_%E9%9A%9C%E5%AE%B3%E6%99%82%E3%81%AE%E6%8C%99%E5%8B%95}
 * @<href>{https://mita2db.hateblo.jp/entry/Group_Replication_%E3%82%92%E8%A9%A6%E3%81%99%284%29_%E3%83%AA%E3%82%AB%E3%83%90%E3%83%AA, https://mita2db.hateblo.jp/entry/Group_Replication_%E3%82%92%E8%A9%A6%E3%81%99}@<br>{}@<href>{https://mita2db.hateblo.jp/entry/Group_Replication_%E3%82%92%E8%A9%A6%E3%81%99%284%29_%E3%83%AA%E3%82%AB%E3%83%90%E3%83%AA, %284%29_%E3%83%AA%E3%82%AB%E3%83%90%E3%83%AA}

=== グループレプリケーションの新機能

==== @<idx>{オンライン}およびユーザーによる@<idx>{プライマリ切り替え／選出}（@<idx>{Election}）

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-changing-primary-member.html, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-changing-primary-member.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-functions-for-new-primary.html, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-functions-for-new-primary.html}

==== オンラインおよびユーザーによる@<idx>{シングルプライマリ}／@<idx>{マルチプライマリ}の切り替え

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-changing-group-mode.html, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-changing-group-mode.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-functions-for-mode.html, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-functions-for-mode.html}

==== サーバがグループから削除されたときにサーバを@<idx>{シャットダウン}する

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_exit_state_action, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_exit_state_action, sysvar_group_replication_exit_state_action}
 ** 8.0.16から初期値が変更になっているので注意（8.0.15まで@<tt>|ABORT_SERVER|、8.0.16から@<tt>|READ_ONLY|）
 * @<href>{https://mysqlhighavailability.com/automatic-member-fencing-with-offline_mode-in-group-replication/, https://mysqlhighavailability.com/automatic-member-fencing-with-offline_mode-in-group-replication/}
 ** 8.0.18で@<tt>|OFFLINE_MODE|が追加

==== 応答のないメンバーをグループから追放

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_member_expel_timeout, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_member_expel_timeout, sysvar_group_replication_member_expel_timeout}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_autorejoin_tries, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_autorejoin_tries, sysvar_group_replication_autorejoin_tries}
 ** メンバーのグループ追放・コンタクト不能時自動再試行回数

==== @<idx>{プライマリフェイルオーバ}ー時の一貫読み取り

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_consistency, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_consistency, sysvar_group_replication_consistency}

==== プライマリフェイルオーバー候補の@<idx>{優先順位}設定

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_member_weight, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_member_weight, sysvar_group_replication_member_weight}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-single-primary-mode.html, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-single-primary-mode.html}

===={80230701} 非同期接続フェイルオーバ機能がグループレプリケーション・トポロジをサポート

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-asynchronous-connection-failover.html, https://dev.mysql.com/doc/refman/8.0/ja/replication-asynchronous-connection-failover.html}

==== メンバーの@<idx>{書き込み許可}を自動でOFF（@<hidx>{super_read_only}@<tt>|super_read_only|のチェック）

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-adding-instances.html, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-adding-instances.html}
 ** ヒント「グループレプリケーションが正常に開始され、…」
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/start-group-replication.html, https://dev.mysql.com/doc/refman/8.0/ja/start-group-replication.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/stop-group-replication.html, https://dev.mysql.com/doc/refman/8.0/ja/stop-group-replication.html}

==== 複数バージョンが混在するグループの運用性を向上

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-online-upgrade-combining-versions.html, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-online-upgrade-combining-versions.html}
 * @<href>{https://mysqlhighavailability.com/improved-handling-of-different-member-versions-in-group-replication/, https://mysqlhighavailability.com/improved-handling-of-different-member-versions-in-group-replication/}

==== メンバーの@<idx>{オンラインアップデート}

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-online-upgrade-methods.html, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-online-upgrade-methods.html}

==== グループレプリケーションでの@<idx>{自動プロビジョニング}

 * @<href>{https://mysqlhighavailability.com/automatic-provisioning-in-group-replication/, https://mysqlhighavailability.com/automatic-provisioning-in-group-replication/}

==== グループレプリケーションの整合性レベルについて

 * @<href>{https://www.percona.com/blog/2020/01/23/making-sense-of-group-replication-consistency-levels/, https://www.percona.com/blog/2020/01/23/making-sense-of-group-replication-consistency-levels/}

==== @<idx>{TLS 1.3}のサポート

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-secure-socket-layer-support-ssl.html, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-secure-socket-layer-support-ssl.html}

==== @<idx>{IPv6}のサポート

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/ipv6-support.html, https://dev.mysql.com/doc/refman/8.0/ja/ipv6-support.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-ipv6.html, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-ipv6.html}

==== @<idx>{圧縮}の設定

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_recovery_compression_algorithm, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_recovery_compression_algorithm, sysvar_group_replication_recovery_compression_algorithm}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_recovery_zstd_compression_level, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_recovery_zstd_compression_level, sysvar_group_replication_recovery_zstd_compression_level}

==== @<idx>{フロー制御}を微調整するためのオプション

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-flow-control.html, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-flow-control.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_flow_control_applier_threshold, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_flow_control_applier_threshold, sysvar_group_replication_flow_control_applier_threshold}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_flow_control_certifier_threshold, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_flow_control_certifier_threshold, sysvar_group_replication_flow_control_certifier_threshold}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_flow_control_hold_percent, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_flow_control_hold_percent, sysvar_group_replication_flow_control_hold_percent}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_flow_control_max_commit_quota, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_flow_control_max_commit_quota, sysvar_group_replication_flow_control_max_commit_quota}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_flow_control_member_quota_percent, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_flow_control_member_quota_percent, sysvar_group_replication_flow_control_member_quota_percent}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_flow_control_min_quota, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_flow_control_min_quota, sysvar_group_replication_flow_control_min_quota}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_flow_control_min_recovery_quota, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_flow_control_min_recovery_quota, sysvar_group_replication_flow_control_min_recovery_quota}

==== @<idx>{許可リスト}でのホスト名のサポート

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-ip-address-permissions.html, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-ip-address-permissions.html}

==== @<idx>{メッセージング}関連

 * @<href>{https://mysqlhighavailability.com/group-replication-delivery-message-service/, https://mysqlhighavailability.com/group-replication-delivery-message-service/}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-group-write-consensus.html, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-group-write-consensus.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-functions-for-maximum-consensus.html, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-functions-for-maximum-}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-functions-for-maximum-consensus.html, consensus.html}
 ** 設定可能な@<idx>{メッセージングパイプライン}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_communication_debug_options, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_communication_debug_options, sysvar_group_replication_communication_debug_options}
 ** メッセージ受け渡しのトレース
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_communication_max_message_size, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_communication_max_message_size, sysvar_group_replication_communication_max_message_size}
 ** メッセージの最大サイズ
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_message_cache_size, https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/group-replication-options.html#sysvar_group_replication_message_cache_size, sysvar_group_replication_message_cache_size}
 ** メッセージのキャッシュサイズ

==== @<idx>{トランザクションセーブポイント}のサポート

 * @<href>{https://mysqlserverteam.com/the-complete-list-of-new-features-in-mysql-8-0/, https://mysqlserverteam.com/the-complete-list-of-new-features-in-mysql-8-0/}
 ** Group Replication 1項目目
 ** リンク先のマニュアルに記載なし

==== @<idx>{Performance Schema}項目の追加

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-summary-tables.html,https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-summary-tables.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-threads-table.html, https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-threads-table.html}
 ** threads.INSTRUMENTED
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-memory-summary-tables.html, https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-memory-summary-tables.html}
 ** memory_summary_global_by_event_name

//embed[latex]{
\clearpage
//}

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-setup-instruments-table.html, https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-setup-instruments-table.html}
 ** setup_instruments（列追加）

==== 同上／グループ全体の認証と@<idx>{Applier統計}の@<idx>{モニタリング}

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-replication-group-member-stats-table.html, https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-replication-group-member-stats-table.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-replication-group-members-table.html, https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-replication-group-members-table.html}

==== レプリケーション接続オプション

 * @<href>{https://www.s-style.co.jp/blog/2020/03/5861/, https://www.s-style.co.jp/blog/2020/03/5861/}

===={80210701} バイナリログのチェックサムをサポート

 * @<href>{https://mysqlhighavailability.com/you-can-now-use-binary-log-checksums-with-group-replication/, https://mysqlhighavailability.com/you-can-now-use-binary-log-checksums-with-group-replication/}

===={80210702} 可用性向上のために2つのシステム変数のデフォルト値を変更

 * @<href>{https://mysqlhighavailability.com/mysql-group-replication-default-response-to-network-partitions-has-changed/, https://mysqlhighavailability.com/mysql-group-replication-default-response-to-network-partitions-has-changed/}
 ** @<tt>|group_replication_member_expel_timeout|・@<tt>|group_replication_autorejoin_tries|

== @<idx>{MySQL Router}

MySQL Routerはアプリケーションサーバ～MySQLサーバ間の透過的なルーティングを提供する軽量なミドルウェアです。
前述の通り@<idx>{InnoDB Cluster}の主要コンポーネントの1つです。

 * @<href>{https://www.mysql.com/jp/products/enterprise/router.html, https://www.mysql.com/jp/products/enterprise/router.html}

=== MySQL Routerの新機能

==== 最後に利用したサーバアドレス等の@<idx>{永続化}

 * @<href>{https://dev.mysql.com/doc/mysql-router/8.0/en/mysql-router-conf-options.html#option_mysqlrouter_dynamic_state, https://dev.mysql.com/doc/mysql-router/8.0/en/mysql-router-conf-options.html#}@<br>{}@<href>{https://dev.mysql.com/doc/mysql-router/8.0/en/mysql-router-conf-options.html#option_mysqlrouter_dynamic_state, option_mysqlrouter_dynamic_state}

==== 接続成功時に@<hidx>{max_connect_errors}@<tt>|max_connect_errors|をリセット

 * @<href>{https://dev.mysql.com/doc/mysql-router/8.0/en/mysql-router-conf-options.html#option_mysqlrouter_max_connect_errors, https://dev.mysql.com/doc/mysql-router/8.0/en/mysql-router-conf-options.html#}@<br>{}@<href>{https://dev.mysql.com/doc/mysql-router/8.0/en/mysql-router-conf-options.html#option_mysqlrouter_max_connect_errors, option_mysqlrouter_max_connect_errors}

==== @<hidx>{mysqlrouter_plugin_infoツール}@<tt>|mysqlrouter_plugin_info|ツールを追加

 * @<href>{https://dev.mysql.com/doc/mysql-router/8.0/en/mysqlrouter_plugin_info.html, https://dev.mysql.com/doc/mysql-router/8.0/en/mysqlrouter_plugin_info.html}

==== @<idx>{メタデータキャッシュ}の@<idx>{TTL}を300秒から500ミリ秒に短縮

 * @<href>{https://dev.mysql.com/doc/mysql-router/8.0/en/mysql-router-conf-options.html#option_mysqlrouter_ttl, https://dev.mysql.com/doc/mysql-router/8.0/en/mysql-router-conf-options.html#}@<br>{}@<href>{https://dev.mysql.com/doc/mysql-router/8.0/en/mysql-router-conf-options.html#option_mysqlrouter_ttl, option_mysqlrouter_ttl}

==== @<idx>{ルーティングストラテジ}を追加（@<hidx>{routing_strategy}@<tt>|routing_strategy|オプション）

 * @<href>{https://dev.mysql.com/doc/mysql-router/8.0/en/mysql-router-conf-options.html#option_mysqlrouter_routing_strategy, https://dev.mysql.com/doc/mysql-router/8.0/en/mysql-router-conf-options.html#}@<br>{}@<href>{https://dev.mysql.com/doc/mysql-router/8.0/en/mysql-router-conf-options.html#option_mysqlrouter_routing_strategy, option_mysqlrouter_routing_strategy}

==== 起動オプション追加

 * @<href>{https://dev.mysql.com/doc/mysql-router/8.0/en/mysqlrouter.html#mysqlrouter-option-summary, https://dev.mysql.com/doc/mysql-router/8.0/en/mysqlrouter.html#}@<br>{}@<href>{https://dev.mysql.com/doc/mysql-router/8.0/en/mysqlrouter.html#mysqlrouter-option-summary, mysqlrouter-option-summary}
 ** Table 4.3 Bootstrapping Options（「Introduced」バージョン番号がある項目）

==== プライマリからセカンダリに降格したサーバーノードへのクライアント接続を解除

 * @<href>{https://dev.mysql.com/doc/mysql-router/8.0/en/mysql-router-conf-options.html#option_mysqlrouter_destinations, https://dev.mysql.com/doc/mysql-router/8.0/en/mysql-router-conf-options.html#}@<br>{}@<href>{https://dev.mysql.com/doc/mysql-router/8.0/en/mysql-router-conf-options.html#option_mysqlrouter_destinations, option_mysqlrouter_destinations}

===={80220703} @<idx>{HTTPサーバプラグイン}と@<idx>{REST API}

 * @<href>{https://lefred.be/content/mysql-router-8-0-22-enables-rest-api-at-bootstrap/, https://lefred.be/content/mysql-router-8-0-22-enables-rest-api-at-bootstrap/}
 ** 8.0.17で追加、8.0.22でデフォルト有効に

==== MySQL Serverのソースツリーの一部としてRouterを構築

 * @<href>{https://dev.mysql.com/worklog/task/?id=10799, https://dev.mysql.com/worklog/task/?id=10799}

== MySQL Shell

MySQL 5.7で導入された@<idx>{MySQL Shell}も機能向上しています@<fn>{010MSH}。

==== @<idx>{InnoDB Cluster}に対するMySQL 8.0のサポート

 * 前述（InnoDB Clusterのセクションを参照）。

===={80230702} @<idx>{AdminAPI}

 * @<href>{https://dev.mysql.com/doc/mysql-shell/8.0/ja/admin-api-userguide.html, https://dev.mysql.com/doc/mysql-shell/8.0/ja/admin-api-userguide.html}
 * @<href>{https://mysqlserverteam.com/mysql-shell-adminapi-whats-new-in-8-0-19/, https://mysqlserverteam.com/mysql-shell-adminapi-whats-new-in-8-0-19/}
 ** 8.0.17で機能向上、さらに8.0.19でInnoDB ReplicaSet（後述）などをサポート
 * @<href>{https://mysqlserverteam.com/mysql-shell-8-0-20-whats-new/, https://mysqlserverteam.com/mysql-shell-8-0-20-whats-new/}

//embed[latex]{
\clearpage
//}

 * @<href>{https://mysqlserverteam.com/mysql-shell-adminapi-whats-new-in-8-0-20/, https://mysqlserverteam.com/mysql-shell-adminapi-whats-new-in-8-0-20/}
 ** 8.0.20でInnoDB Cluster・InnoDB ReplicaSetの管理者およびルータアカウント設定などをサポート
 * @<href>{https://mysqlserverteam.com/mysql-shell-adminapi-whats-new-in-8-0-23/, https://mysqlserverteam.com/mysql-shell-adminapi-whats-new-in-8-0-23/}
 ** 8.0.23でクラスタ診断機能が強化

==== @<idx>{InnoDB ReplicaSet}

AdminAPIの機能として8.0.19で追加されました。InnoDB Clusterとは違い、非同期レプリケーションによるクラスタを構成します。

 * @<href>{https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-innodb-replicaset.html, https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-innodb-replicaset.html}
 * @<href>{https://mysqlserverteam.com/introducing-mysql-innodb-replicaset/, https://mysqlserverteam.com/introducing-mysql-innodb-replicaset/}
 * @<href>{https://www.s-style.co.jp/blog/2020/02/5817/, https://www.s-style.co.jp/blog/2020/02/5817/}
 * @<href>{https://qiita.com/hmatsu47/items/6f3d512f092e86b7c1cc, https://qiita.com/hmatsu47/items/6f3d512f092e86b7c1cc}

==== 新しい@<idx>{Shellプラグイン構造}

 * @<href>{https://mysqlserverteam.com/mysql-shell-plugins-introduction/, https://mysqlserverteam.com/mysql-shell-plugins-introduction/}

==== @<idx>{Clone Plugin}（@<idx>{クローンプラグイン}）

別ノードへのデータのオンラインコピーや自動プロビジョニングがサポートされました。

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/clone-plugin.html, https://dev.mysql.com/doc/refman/8.0/ja/clone-plugin.html}
 * @<href>{https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-innodb-cluster-clone-deployment.html, https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-innodb-cluster-clone-deployment.html}
 * @<href>{https://mysqlserverteam.com/clone-create-mysql-instance-replica/, https://mysqlserverteam.com/clone-create-mysql-instance-replica/}
 * @<href>{https://yoku0825.blogspot.com/2019/07/mysql-8017clone.html, https://yoku0825.blogspot.com/2019/07/mysql-8017clone.html}
 * @<href>{https://yoku0825.blogspot.com/2019/07/mysql-8017clone-instance-from.html, https://yoku0825.blogspot.com/2019/07/mysql-8017clone-instance-from.html}
 * @<href>{https://qiita.com/huato/items/5ad03b37313c0b41efbc, https://qiita.com/huato/items/5ad03b37313c0b41efbc}
 * @<href>{https://www.s-style.co.jp/blog/2019/09/5090/, https://www.s-style.co.jp/blog/2019/09/5090/}
 * @<href>{https://www.s-style.co.jp/blog/2019/11/5452/, https://www.s-style.co.jp/blog/2019/11/5452/}

===={80210703} @<idx>{論理ダンプ・リストアツール}

 * @<href>{https://mysqlserverteam.com/mysql-shell-dump-load-part-1-demo/, https://mysqlserverteam.com/mysql-shell-dump-load-part-1-demo/}
 * @<href>{https://mysqlserverteam.com/mysql-shell-dump-load-part-2-benchmarks/, https://mysqlserverteam.com/mysql-shell-dump-load-part-2-benchmarks/}
 * @<href>{https://mysqlserverteam.com/mysql-shell-dump-load-part-3-load-dump/, https://mysqlserverteam.com/mysql-shell-dump-load-part-3-load-dump/}
 * @<href>{https://elephantdolphin.blogspot.com/2019/08/parallel-table-importer-in-mysql-shell.html, https://elephantdolphin.blogspot.com/2019/08/parallel-table-importer-in-mysql-shell.html}
 * @<href>{https://www.s-style.co.jp/blog/2019/10/5248/, https://www.s-style.co.jp/blog/2019/10/5248/}
 * @<href>{https://mysqlserverteam.com/whats-new-in-mysql-shell-8-0-22/, https://mysqlserverteam.com/whats-new-in-mysql-shell-8-0-22/}
 ** 8.0.22でMySQL 5.6からのダンプサポート、ダンプ時@<tt>|FLUSH TALBES WITH READ LOCK|が使えない場合に@<tt>|LOCK TABLES|で代替利用可能に
 * @<href>{https://atsuizo.hatenadiary.jp/entry/2020/09/01/090000, https://atsuizo.hatenadiary.jp/entry/2020/09/01/090000}
 * @<href>{https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-shell-utilities-table-export.html, https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-shell-utilities-table-export.html}
 ** Table Export Utility
 * @<href>{https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-shell-utilities-parallel-table.html, https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-shell-utilities-parallel-table.html}
 ** Parallel Table Import Utility
 ** 8.0.23で複数データファイルからのインポートをサポート
 * @<href>{https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-shell-utilities-dump-instance-schema.html, https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-shell-utilities-dump-instance-schema.html}
 ** Instance Dump Utility・Schema Dump Utility・Table Dump Utility
 * @<href>{https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-shell-utilities-load-dump.html, https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-shell-utilities-load-dump.html}
 ** Dump Loading Utility
 * @<href>{https://mysqlserverteam.com/mysql-shell-8-0-24-whats-new/, https://mysqlserverteam.com/mysql-shell-8-0-24-whats-new/}
 ** 8.0.24で主キーのないテーブルの処理に対するオプションを追加

==== セキュアな@<idx>{パスワード管理}

 * @<href>{https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-shell-pluggable-password-store.html, https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-shell-pluggable-password-store.html}
 * @<href>{https://mysqlserverteam.com/mysql-shell-8-0-12-whats-new/, https://mysqlserverteam.com/mysql-shell-8-0-12-whats-new/}
 ** @<idx>{Pluggable Password Store}

==== Python 3サポート

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/mysql-shell-tutorial-python.html, https://dev.mysql.com/doc/refman/8.0/ja/mysql-shell-tutorial-python.html}

===={80240701} その他の新機能と変更

 *  @<href>{https://mysqlserverteam.com/mysql-shell-8-0-12-whats-new/, https://mysqlserverteam.com/mysql-shell-8-0-12-whats-new/}
 ** Centralized Help System（組み込みヘルプの改善）
 * @<href>{https://mysqlserverteam.com/mysql-shell-8-0-14-whats-new/, https://mysqlserverteam.com/mysql-shell-8-0-14-whats-new/}
 ** クエリ結果におけるカラムタイプ表示
 ** 「Ability to show column type information when executing SQL.」
 * @<href>{https://dev.mysql.com/doc/x-devapi-userguide/en/, https://dev.mysql.com/doc/x-devapi-userguide/en/}
 ** @<idx>{X DevAPI}サポートをアップデート
 * @<href>{https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-shell-using-pager.html, https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-shell-using-pager.html}
 ** スクリーンページング
 * @<href>{https://mysqlserverteam.com/mysql-shell-8-0-whats-new/, https://mysqlserverteam.com/mysql-shell-8-0-whats-new/}
 ** Auto-Completion（自動補完）
 * @<href>{https://mysqlserverteam.com/mysql-shell-8-0-whats-new/, https://mysqlserverteam.com/mysql-shell-8-0-whats-new/}
 ** Prompt Themes（カスタマイズ可能なプロンプト）
 * @<href>{https://mysqlserverteam.com/mysql-shell-8-0-whats-new/, https://mysqlserverteam.com/mysql-shell-8-0-whats-new/}
 ** Command Line History Persistence（コマンド履歴永続化）
 * @<href>{https://mysqlserverteam.com/mysql-shell-api-command-line-integration-for-devops/, https://mysqlserverteam.com/mysql-shell-api-command-line-integration-for-devops/}
 ** シェルAPIの直接コマンドライン実行
 ** 8.0.24でさらに改善
 * @<href>{https://next4us-ti.hatenablog.com/entry/2020/05/07/161812, https://next4us-ti.hatenablog.com/entry/2020/05/07/161812}
 * @<href>{https://mysqlserverteam.com/mysql-shell-8-0-20-whats-new/, https://mysqlserverteam.com/mysql-shell-8-0-20-whats-new/}
 ** Connection Compression Options（コネクション圧縮オプション）

//embed[latex]{
\clearpage
//}

 * @<href>{https://mysqlserverteam.com/mysql-shell-8-0-24-whats-new/, https://mysqlserverteam.com/mysql-shell-8-0-24-whats-new/}
 ** ログ出力の改善

== その他のレプリケーション新機能・変更

==== @<idx>{チャネルフィルタ}毎の@<idx>{マルチソースレプリケーション}

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-rules-channel-based-filters.html, https://dev.mysql.com/doc/refman/8.0/ja/replication-rules-channel-based-filters.html}
 * @<href>{https://gihyo.jp/dev/serial/01/mysql-road-construction-news/0088, https://gihyo.jp/dev/serial/01/mysql-road-construction-news/0088}
 ** 「MySQL8.0では，@<tt>|performance_schema|の@<tt>|replication_applier_global_filters|と…」

==== @<idx>{トランザクション}内@<idx>{テンポラリテーブル}の@<idx>{GTID}サポート

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-gtids-restrictions.html, https://dev.mysql.com/doc/refman/8.0/ja/replication-gtids-restrictions.html}

==== @<idx>{JSON列}の@<idx>{部分アップデート}対応レプリケーション

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-options-binary-log.html#sysvar_binlog_row_value_options, https://dev.mysql.com/doc/refman/8.0/ja/replication-options-binary-log.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-options-binary-log.html#sysvar_binlog_row_value_options, sysvar_binlog_row_value_options}

==== @<hidx>{RESET MASTER TO}@<tt>|RESET MASTER TO|

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/reset-master.html, https://dev.mysql.com/doc/refman/8.0/ja/reset-master.html}

==== @<hidx>{CHANGE REPLICATION SOURCE TO}@<tt>|CHANGE REPLICATION SOURCE TO|@<hidx>{CHANGE MASTER TO}（@<tt>|CHANGE MASTER TO|）のホスト名が255文字まで指定可能に

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/change-replication-source-to.html, https://dev.mysql.com/doc/refman/8.0/ja/change-replication-source-to.html}
 ** @<hidx>{SOURCE_HOST}@<tt>|SOURCE_HOST|@<hidx>{MASTER_HOST}（@<tt>|MASTER_HOST|）オプション

==== 書き込みセットベースの@<idx>{トランザクション依存関係追跡}

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-options-binary-log.html#sysvar_binlog_transaction_dependency_tracking, https://dev.mysql.com/doc/refman/8.0/ja/replication-options-binary-log.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-options-binary-log.html#sysvar_binlog_transaction_dependency_tracking, sysvar_binlog_transaction_dependency_tracking}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-options-replica.html#sysvar_slave_parallel_type, https://dev.mysql.com/doc/refman/8.0/ja/replication-options-replica.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-options-replica.html#sysvar_slave_parallel_type, sysvar_slave_parallel_type}

==== 受信側スレッドと適用側スレッドの間の競合の削減

 * @<href>{https://dev.mysql.com/doc/relnotes/mysql/8.0/en/news-8-0-1.html, https://dev.mysql.com/doc/relnotes/mysql/8.0/en/news-8-0-1.html}
 ** 「To avoid potential race conditions, …」

==== 拡張テーブル@<idx>{メタデータ}の@<idx>{バイナリログ}記録

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-options-binary-log.html#sysvar_binlog_row_metadata, https://dev.mysql.com/doc/refman/8.0/ja/replication-options-binary-log.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-options-binary-log.html#sysvar_binlog_row_metadata, sysvar_binlog_row_metadata}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/mysqlbinlog.html#option_mysqlbinlog_print-table-metadata, https://dev.mysql.com/doc/refman/8.0/ja/mysqlbinlog.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/mysqlbinlog.html#option_mysqlbinlog_print-table-metadata, option_mysqlbinlog_print-table-metadata}

==== @<hidx>{GTID_EXECUTED}@<tt>|GTID_EXECUTED|が空でない場合に設定可能な@<hidx>{GTID_PURGED}@<tt>|GTID_PURGED|

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-options-gtids.html#sysvar_gtid_purged, https://dev.mysql.com/doc/refman/8.0/ja/replication-options-gtids.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-options-gtids.html#sysvar_gtid_purged, sysvar_gtid_purged}

==== 空き容量がなくなったときの安全（@<idx>{ノンブロッキング}）な@<idx>{レプリケーションモニタリング}

 * @<href>{https://dev.mysql.com/doc/relnotes/mysql/8.0/en/news-8-0-2.html, https://dev.mysql.com/doc/relnotes/mysql/8.0/en/news-8-0-2.html}
 ** 「Replication: The receiver thread has been improved to no longer block other thread's …」

==== @<idx>{トランザクション長}のバイナリログへの記録

 * @<href>{https://dev.mysql.com/doc/relnotes/mysql/8.0/en/news-8-0-2.html, https://dev.mysql.com/doc/relnotes/mysql/8.0/en/news-8-0-2.html}
 ** 「Replication: A new transaction length field has been added to the Gtid_log_event …」

==== 各トランザクションの@<idx>{サーババージョン}をバイナリログに記録

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-compatibility.html, https://dev.mysql.com/doc/refman/8.0/ja/replication-compatibility.html}
 ** 「MySQL 8.0.14から、…」
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-options-source.html#sysvar_original_server_version, https://dev.mysql.com/doc/refman/8.0/ja/replication-options-source.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-options-source.html#sysvar_original_server_version, sysvar_original_server_version}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-options-source.html#sysvar_immediate_server_version, https://dev.mysql.com/doc/refman/8.0/ja/replication-options-source.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-options-source.html#sysvar_immediate_server_version, sysvar_immediate_server_version}

==== @<hidx>{START REPLICA UNTIL}@<tt>|START REPLICA UNTIL|@<hidx>{START SLAVE UNTIL}（@<tt>|START SLAVE UNTIL|／@<idx>{マルチスレッドレプリケーション}への対応）

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/start-replica.html, https://dev.mysql.com/doc/refman/8.0/ja/start-replica.html}

==== マルチスレッドレプリカ（スレーブ）で@<tt>|SOURCE_AUTO_POSITION|（@<tt>|MASTER_AUTO_POSITION=1|）のときのリレーログスキップ処理

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-solutions-unexpected-replica-halt.html, https://dev.mysql.com/doc/refman/8.0/ja/replication-solutions-unexpected-replica-halt.html}

===={80230703} マルチスレッドレプリカ（スレーブ）レプリケーションにおけるデッドロック検出機構の改善

 * @<href>{https://dev.mysql.com/worklog/task/?id=13574, https://dev.mysql.com/worklog/task/?id=13574}

==== @<idx>{遅延レプリケーション}のマイクロ秒対応

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-delayed.html, https://dev.mysql.com/doc/refman/8.0/ja/replication-delayed.html}
 ** 「original_commit_timestamp: …」

//embed[latex]{
\clearpage
//}

==== @<hidx>{binlog-row-event-max-size}@<tt>|binlog-row-event-max-size|システム変数

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-options-binary-log.html#option_mysqld_binlog-row-event-max-size, https://dev.mysql.com/doc/refman/8.0/ja/replication-options-binary-log.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-options-binary-log.html#option_mysqld_binlog-row-event-max-size, option_mysqld_binlog-row-event-max-size}

==== @<idx>{バイナリログキャッシュサイズ}の指定

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-options-binary-log.html#sysvar_binlog_cache_size, https://dev.mysql.com/doc/refman/8.0/ja/replication-options-binary-log.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-options-binary-log.html#sysvar_binlog_cache_size, sysvar_binlog_cache_size}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-options-binary-log.html#sysvar_binlog_stmt_cache_size, https://dev.mysql.com/doc/refman/8.0/ja/replication-options-binary-log.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-options-binary-log.html#sysvar_binlog_stmt_cache_size, sysvar_binlog_stmt_cache_size}

==== @<hidx>{mysqlbinlog}@<tt>|mysqlbinlog|で@<idx>{圧縮}をサポート

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/mysqlbinlog.html#option_mysqlbinlog_compression-algorithms, https://dev.mysql.com/doc/refman/8.0/ja/mysqlbinlog.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/mysqlbinlog.html#option_mysqlbinlog_compression-algorithms, option_mysqlbinlog_compression-algorithms}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/mysqlbinlog.html#option_mysqlbinlog_zstd-compression-level, https://dev.mysql.com/doc/refman/8.0/ja/mysqlbinlog.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/mysqlbinlog.html#option_mysqlbinlog_zstd-compression-level, option_mysqlbinlog_zstd-compression-level}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/change-replication-source-to.html, https://dev.mysql.com/doc/refman/8.0/ja/change-replication-source-to.html}
 ** @<hidx>{CHANGE REPLICATION SOURCE TO}@<tt>|CHANGE REPLICATION SOURCE TO|に@<tt>|SOURCE_COMPRESSION_ALGORITHMS|・@<br>{}@<tt>|SOURCE_ZSTD_COMPRESSION_LEVEL|追加

==== 権限を限定したレプリケーション

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-privilege-checks.html, https://dev.mysql.com/doc/refman/8.0/ja/replication-privilege-checks.html}
 * @<href>{https://mysqlhighavailability.com/replication-with-restricted-privileges/, https://mysqlhighavailability.com/replication-with-restricted-privileges/}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/change-replication-source-to.html, https://dev.mysql.com/doc/refman/8.0/ja/change-replication-source-to.html}
 ** @<tt>|CHANGE REPLICATION SOURCE TO|（@<tt>|CHANGE MASTER TO|）に@<tt>|PRIVILEGE_CHECKS_USER|追加

===={80220701} 新しい非同期レプリケーション接続フェイルオーバーメカニズム

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/change-replication-source-to.html, https://dev.mysql.com/doc/refman/8.0/ja/change-replication-source-to.html}
 ** @<tt>|SOURCE_CONNECTION_AUTO_FAILOVER=1|
 * @<href>{https://mysqlhighavailability.com/mysql-8-0-22-replication-enhancements/, https://mysqlhighavailability.com/mysql-8-0-22-replication-enhancements/}
 * @<href>{https://www.percona.com/blog/2020/10/26/mysql-8-0-22-asynchronous-replication-automatic-connection-io-thread-failover/, https://www.percona.com/blog/2020/10/26/mysql-8-0-22-asynchronous-replication-automatic-connection-io-thread-failover/}
 * @<href>{https://www.s-style.co.jp/blog/2020/10/6828/, https://www.s-style.co.jp/blog/2020/10/6828/}

==== レプリカ（スレーブ）での主キーのチェックポリシーが設定可能に

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/change-replication-source-to.html, https://dev.mysql.com/doc/refman/8.0/ja/change-replication-source-to.html}
 ** @<tt>|REQUIRE_TABLE_PRIMARY_KEY_CHECK|

//embed[latex]{
\clearpage
//}

==== @<tt>|auto_increment_increment|・@<tt>|auto_increment_offset|のセッション値設定

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-options-source.html, https://dev.mysql.com/doc/refman/8.0/ja/replication-options-source.html}
 ** 「MySQL 8.0.18では、…」

===={80230704} @<idx>{GTID}を使用しないソースからGTIDを使用するレプリカへのレプリケーションが可能に

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/change-replication-source-to.html, https://dev.mysql.com/doc/refman/8.0/ja/change-replication-source-to.html}
 ** @<hidx>{ASSIGN_GTIDS_TO_ANONYMOUS_TRANSACTIONS}@<tt>|ASSIGN_GTIDS_TO_ANONYMOUS_TRANSACTIONS|オプション

===={80240702} @<hidx>{skip_slave_start}@<tt>|skip_slave_start|システム変数の追加

 * @<href>{https://dev.mysql.com/doc/refman/8.0/en/replication-options-replica.html#sysvar_skip_slave_start, https://dev.mysql.com/doc/refman/8.0/en/replication-options-replica.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/en/replication-options-replica.html#sysvar_skip_slave_start, sysvar_skip_slave_start}

===={80230705} @<idx>{Deprecated}（@<idx>{非推奨}）になった設定等

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-options-replica.html, https://dev.mysql.com/doc/refman/8.0/ja/replication-options-replica.html}
 ** @<tt>|--master-info-file|
 ** @<tt>|master_info_repository|
 ** @<tt>|relay_log_info_file|
 ** @<tt>|relay_log_info_repository|
 ** @<tt>|slave_compressed_protocol|
 ** @<tt>|slave_rows_search_algorithms|
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-options-binary-log.html#sysvar_log_bin_use_v1_row_events, https://dev.mysql.com/doc/refman/8.0/ja/replication-options-binary-log.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/replication-options-binary-log.html#sysvar_log_bin_use_v1_row_events, sysvar_log_bin_use_v1_row_events}
 ** @<tt>|log_bin_use_v1_row_events|
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/gtid-functions.html#function_wait-until-sql-thread-after-gtids, https://dev.mysql.com/doc/refman/8.0/ja/gtid-functions.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/gtid-functions.html#function_wait-until-sql-thread-after-gtids, function_wait-until-sql-thread-after-gtids}
 ** @<tt>|WAIT_UNTIL_SQL_THREAD_AFTER_GTIDS|

===={80220702} 「ホワイトリスト」「マスター」「スレーブ」（用語）の非推奨化による変更

 * @<tt>|group_replication_ip_whitelist|が@<tt>|group_replication_ip_allowlist|に
 * @<tt>|MASTER|が@<tt>|SOURCE|（または@<tt>|REPLICATION SOURCE|）に
 * @<tt>|SLAVE|が@<tt>|REPLICA|に

//footnote[003CCF][バイナリログストリームに入る変更をキャプチャするためにサーバーによって作成された一時ファイル。]
//footnote[004ENC][8.0.24でプラグインからコンポーネントに変わりました（@<href>{https://dev.mysql.com/doc/refman/8.0/en/keyring-file-component.html, https://dev.mysql.com/doc/refman/8.0/en/keyring-file-component.html}）。]
//footnote[010MSH][InnoDB Clusterのコンポーネントの1つですので、この章で説明します。]
