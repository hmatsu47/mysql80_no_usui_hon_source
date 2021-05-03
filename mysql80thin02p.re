= ユーザ管理・認証・権限設定の変更と新機能

== @<idx>{認証プラグイン}

MySQL 8.0では@<idx>{Caching sha2 authenticationプラグイン}が導入され、デフォルトとなりました。
従来の@<idx>{MySQL Native Passwordプラグイン}と比べて、以下の点が優れています。

 * 安全な@<idx>{パスワード}暗号化
 * 高い@<idx>{パフォーマンス}

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/caching-sha2-pluggable-authentication.html, https://dev.mysql.com/doc/refman/8.0/ja/caching-sha2-pluggable-authentication.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/mysql-command-options.html#option_mysql_get-server-public-key, https://dev.mysql.com/doc/refman/8.0/ja/mysql-command-options.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/mysql-command-options.html#option_mysql_get-server-public-key, option_mysql_get-server-public-key}

アプリケーションからの接続に使うコネクタによっては、Caching sha2 authenticationプラグインに対応していないことがあります。
その場合は従来のMySQL Native Passwordプラグインをデフォルトにするか、接続ユーザに対する認証プラグインとして指定します（ブログ記事等の1つ目）。

その他、認証プラグインの注意点についてはブログ記事等の2つ目・3つ目を参照してください。

==== 実行例

MySQL 5.7からアップグレードした環境で確認してみます。

//emlist{
mysql> @<i>{SELECT user, host, plugin, authentication_string FROM mysql.user;}
+------------------+-----------+-----------------------+--------------------------------------
----------------------------------+
| user             | host      | plugin                | authentication_string
                                  |
+------------------+-----------+-----------------------+--------------------------------------
----------------------------------+
| mysql.infoschema | localhost | caching_sha2_password | $A$005$THISISACOMBINATIONOFINVALIDSAL
TANDPASSWORDTHATMUSTNEVERBRBEUSED |
| mysql.session    | localhost | mysql_native_password | *THISISNOTAVALIDPASSWORDTHATCANBEUSED
HERE                              |
@<b>{（中略）}
+------------------+-----------+-----------------------+--------------------------------------
----------------------------------+
5 rows in set (0.00 sec)

mysql> @<i>{CREATE USER `hmatsu47`@`localhost` IDENTIFIED WITH mysql_native_password BY 'H0geFug@';}
Query OK, 0 rows affected (0.01 sec)

mysql> @<i>{SELECT user, host, plugin, authentication_string FROM mysql.user;}
+------------------+-----------+-----------------------+--------------------------------------
----------------------------------+
| user             | host      | plugin                | authentication_string
                                  |
+------------------+-----------+-----------------------+--------------------------------------
----------------------------------+
| hmatsu47         | localhost | mysql_native_password | *5528FA7F88CFC88E779DAE7C94511C249878
B7F8                              |
| mysql.infoschema | localhost | caching_sha2_password | $A$005$THISISACOMBINATIONOFINVALIDSAL
TANDPASSWORDTHATMUSTNEVERBRBEUSED |
@<b>{（中略）}
+------------------+-----------+-----------------------+--------------------------------------
----------------------------------+
6 rows in set (0.00 sec)

mysql> @<i>{ALTER USER `hmatsu47`@`localhost` IDENTIFIED WITH caching_sha2_password;}
Query OK, 0 rows affected (0.00 sec)

mysql> @<i>{SELECT user, host, plugin, authentication_string FROM mysql.user;}
+------------------+-----------+-----------------------+--------------------------------------
----------------------------------+
| user             | host      | plugin                | authentication_string
                                  |
+------------------+-----------+-----------------------+--------------------------------------
----------------------------------+
| hmatsu47         | localhost | caching_sha2_password |
|
| mysql.infoschema | localhost | caching_sha2_password | $A$005$THISISACOMBINATIONOFINVALIDSAL
TANDPASSWORDTHATMUSTNEVERBRBEUSED |
@<b>{（中略）}
+------------------+-----------+-----------------------+--------------------------------------
----------------------------------+
6 rows in set (0.00 sec)
//}

@<hidx>{ALTER USER ～ IDENTIFIED WITH}@<tt>|ALTER USER ～ IDENTIFIED WITH|を実行するとパスワードが消えてしまいます。

== ユーザ・@<idx>{パスワード}と権限の管理

=== @<idx>{ユーザアカウント}ごとに2つの@<idx>{アクティブパスワード}をサポート

ユーザパスワードの変更を行う際、変更ミスがあると認証ができなくなるため、変更には神経を使います。
また、アプリケーションで利用するユーザアカウントでは、パスワードの変更とアプリケーション（もしくはアプリケーション設定）の変更を同じタイミングで実施しないといけないため、レプリカを多数使う環境ではメンテナンス停止なしにパスワードを変更するのが困難でした。

MySQL 8.0では、ユーザーアカウントごとに2つのアクティブパスワードをサポートするようになりました。
最初にユーザパスワードを変更し、全てのアプリケーション（もしくはアプリケーション設定）の変更を段階的に進め、完了後に古いパスワードを無効化する、という運用が可能です。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/password-management.html#dual-passwords, https://dev.mysql.com/doc/refman/8.0/ja/password-management.html#dual-passwords}

=== @<idx>{ランダムパスワード}の設定をサポート

8.0.18から、ユーザ作成・変更およびパスワード変更時にランダムパスワードの設定ができるようになりました。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/password-management.html#random-password-generation, https://dev.mysql.com/doc/refman/8.0/ja/password-management.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/password-management.html#random-password-generation, random-password-generation}

==={80210201} その他のユーザ・パスワード管理、権限管理に関わる変更点

 * @<idx>{ログイン試行回数}（@<tt>|FAILED_LOGIN_ATTEMPTS|）と@<idx>{パスワードロック時間}（@<tt>|PASSWORD_LOCK_TIME|）の追加
 * @<hidx>{SUPER権限}@<tt>|SUPER|権限を動的権限に分割
 * @<idx>{外部キー制約}を作成するには親テーブルに対する@<hidx>{REFERENCES権限}@<tt>|REFERENCES|権限が必要に
 * @<idx>{ストアドルーチン}の定義や属性にアクセスするための@<hidx>{SHOW_ROUTINE権限}@<tt>|SHOW_ROUTINE|権限
 * @<hidx>{RELOAD権限}@<tt>|RELOAD|権限の分割（@<tt>|FLUSH|処理だけを可能に）
 * @<hidx>{GRANTステートメント}@<tt>|GRANT|ステートメントによるユーザ作成の廃止
 * @<hidx>{GRANT TABLE}@<tt>|GRANT TABLE|のホスト名が255文字まで指定可能に
 * ユーザ権限のIPアドレス照合ルール（照合順）の変更
 * データベースオブジェクトに対する@<idx>{部分的な権限の取り消し}（@<hidx>{REVOKE}@<tt>|REVOKE|）
 * @<hidx>{ALTER USER}@<tt>|ALTER USER|／@<hidx>{SET PASSWORD}@<tt>|SET PASSWORD|時に変更前パスワードの入力を要求
 * @<tt>|CREATE USER|・@<tt>|ALTER USER|でJSON形式のユーザコメントが登録可能に
 * @<hidx>{print_identified_with_as_hex}@<tt>|print_identified_with_as_hex|システム変数
 * @<idx>{セキュアセッション変数}の設定（@<hidx>{MYSQL_SESSION_ADMIN権限}@<tt>|MYSQL_SESSION_ADMIN|権限）
 * @<hidx>{--skip-grant-tables}@<tt>|--skip-grant-tables|オプション付きで起動したときに@<hidx>{--skip-networking}@<tt>|--skip-networking|も有効化する
 * @<idx>{ACLステートメント}を@<idx>{アトミック}にする
 * @<idx>{ログイン}失敗時に@<idx>{認証を遅延}させる
 * @<idx>{LDAP認証プラグイン}に関する機能追加（Enterprise版）
 * SASL LDAP認証プラグインがSCRAM-SHA-256をサポート（Enterprise版）
 * 権限テーブルのノンロッキング読み取りが可能に
 * @<tt>|CREATE|・@<tt>|DROP|・@<tt>|RENAME USER|で存在しない@<tt>|DEFINER|に対するチェックを厳格化
 * @<tt>|caching_sha2_password_digest_rounds|システム変数の追加（パスワード保存時のハッシュラウンド数）

== yaSSLから@<idx>{OpenSSL}に移行し@<idx>{動的リンク}化

認証そのものではありませんが、認証機能から利用されるため関連項目としてあげておきます。

@<idx>{SSL/TLSライブラリ}がyaSSLからOpenSSLに変更され、ライブラリのリンク方式が動的になりました。
@<idx>{TLS 1.3}に対応するなど@<idx>{セキュリティ}強化につながっています。

なお、8.0.18からはyaSSL・wolfSSLのサポートは廃止され、OpenSSLのみサポートしています。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/encrypted-connections.html, https://dev.mysql.com/doc/refman/8.0/ja/encrypted-connections.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/fips-mode.html, https://dev.mysql.com/doc/refman/8.0/ja/fips-mode.html}

== @<idx>{ロール}

MySQL 8.0では権限に関わる機能としてロール（@<hidx>{ROLE}@<tt>|ROLE|）がサポートされました。
ロールの基本的な使い方は以下の通りです。

 * 特別な権限（スキーマ・テーブル・ユーザの@<tt>|CREATE|・@<tt>|DROP|などの管理業務に必要な権限）は、ユーザ個人に直接付与するのではなくロールに付与する
 * それぞれのユーザが適用できるロールをあらかじめ指定しておく
 * ユーザは特別な権限を必要とする操作を実行するときに、ロールを適用してから実行する

MySQL 8.0でサポートされた主なロール機能は以下の通りです。

 * ロールの作成と削除
 * ロールに対する@<idx>{権限の付与}と剥奪
 * 適用する@<idx>{ロールの切り替え}
 * ロールに関する情報の表示
 * @<idx>{ログイン}（接続）時に適用される@<idx>{デフォルトロール}の指定
 * @<idx>{必須ロール}（@<hidx>{mandatory_roles}@<tt>|mandatory_roles|）の指定
 * 必須ロールを含めたログイン（接続）時適用ロールの指定

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/roles.html, https://dev.mysql.com/doc/refman/8.0/ja/roles.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_mandatory_roles, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_mandatory_roles, sysvar_mandatory_roles}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_activate_all_roles_on_login, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_activate_all_roles_on_login, sysvar_activate_all_roles_on_login}

==== 実行例

//emlist{
[root@mysql57to80 ~]# @<i>{mysql -u root -p}
Enter password:@<b>{パスワードを入力}
Welcome to the MySQL monitor.  Commands end with ; or \g.
@<b>{（中略）}
mysql> @<i>{GRANT SELECT ON test.sales_person TO `hmatsu47`@`localhost`;}
Query OK, 0 rows affected (0.00 sec)
@<b>{※ユーザにはSELECT権限のみ付与}

mysql> @<i>{CREATE ROLE `account_admin`;}
Query OK, 0 rows affected (0.01 sec)
@<b>{※ロールを作成}

mysql> @<i>{GRANT SELECT, INSERT, UPDATE, DELETE ON test.sales_person TO `account_admin`;}
Query OK, 0 rows affected (0.01 sec)
@<b>{※ロールには更新権限も付与}

mysql> @<i>{GRANT `account_admin` TO `hmatsu47`@`localhost`;}
Query OK, 0 rows affected (0.00 sec)
@<b>{※ユーザにロールを割り当て}

mysql> @<i>{QUIT}
Bye
[root@mysql57to80 ~]# @<i>{mysql -u hmatsu47 -p}
Enter password:@<b>{パスワードを入力}
Welcome to the MySQL monitor.  Commands end with ; or \g.
@<b>{（中略）}
mysql> @<i>{USE test;}
@<b>{（中略）}
Database changed
mysql> @<i>{SELECT * FROM sales_person;}
+----+--------+
| id | name   |
+----+--------+
|  1 | 田中   |
|  2 | 坂井   |
|  3 | 富田   |
|  4 | 三谷   |
+----+--------+
4 rows in set (0.00 sec)
@<b>{※SELECTは可能}

mysql> @<i>{INSERT INTO sales_person SET name='梶山';}
ERROR 1142 (42000): INSERT command denied to user 'hmatsu47'@'localhost' for table 'sales_pers
on'
@<b>{※INSERTはできない}
mysql> @<i>{SET ROLE `account_admin`;}
Query OK, 0 rows affected (0.00 sec)
@<b>{※ロールを有効化}

mysql> @<i>{INSERT INTO sales_person SET name='梶山';}
Query OK, 1 row affected (0.01 sec)
@<b>{※INSERTもできるようになった}

mysql> @<i>{SELECT * FROM sales_person;}
+----+--------+
| id | name   |
+----+--------+
|  1 | 田中   |
|  2 | 坂井   |
|  3 | 富田   |
|  4 | 三谷   |
|  5 | 梶山   |
+----+--------+
5 rows in set (0.00 sec)
//}
