= MySQL 8.0のインストールと設定パラメータ

== 新規インストール

新規@<idx>{インストール}については公式リファレンスマニュアルに手順が記載されており、基本的にはMySQL 5.7とほぼ同じです。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/installing.html, https://dev.mysql.com/doc/refman/8.0/ja/installing.html}

==== 実行例

参考として、@<idx>{CentOS 8}に新規インストールする場合の例を示しておきます。

//noindent
　@<br>{}@<b>{【注】}@<tt>|SELinux|および@<tt>|firewalld|などの設定は適切に行っておきます。

//emlist{
[root@mysql80cent8 ~]# @<i>{wget https://dev.mysql.com/get/mysql80-community-release-el8-1.noarch.r}
@<i>{pm}
@<b>{※ダウンロードするバージョン（el8-1）はその時点のものを指定}
--2021-05-01 15:56:01--  https://dev.mysql.com/get/mysql80-community-release-el8-1.noarch.rpm
@<b>{（中略）}
2021-05-01 15:56:02 (15.2 MB/s) - ‘mysql80-community-release-el8-1.noarch.rpm’ saved [30388/3
0388]

[root@mysql80cent8 ~]# @<i>{dnf localinstall mysql80-community-release-el8-1.noarch.rpm}
@<b>{（中略）}
Dependencies resolved.
================================================================================
 Package                        Arch        Version     Repository         Size
================================================================================
Installing:
 mysql80-community-release      noarch      el8-1       @commandline       30 k

Transaction Summary
================================================================================
Install  1 Package

Total size: 30 k
Installed size: 29 k
Is this ok [y/N]: @<i>{y}
Downloading Packages:
@<b>{（中略）}
Installed:
  mysql80-community-release-el8-1.noarch

Complete!
[root@mysql80cent8 ~]# @<i>{dnf module disable mysql}
@<b>{※CentOS 8標準のMySQL 8.0のパッケージを無効化する}
@<b>{（中略）}
Dependencies resolved.
================================================================================
 Package           Architecture     Version             Repository         Size
================================================================================
Disabling modules:
 mysql

Transaction Summary
================================================================================

Is this ok [y/N]: @<i>{y}
Complete!
[root@mysql80cent8 ~]# @<i>{dnf install mysql-community-server}
Last metadata expiration check: 0:00:19 ago on Sat 01 May 2021 03:56:59 PM JST.
Dependencies resolved.
================================================================================
 Package                         Arch    Version       Repository          Size
================================================================================
Installing:
 mysql-community-server          x86_64  8.0.24-1.el8  mysql80-community   53 M
Installing dependencies:
 mysql-community-client          x86_64  8.0.24-1.el8  mysql80-community   13 M
 mysql-community-client-plugins  x86_64  8.0.24-1.el8  mysql80-community   98 k
 mysql-community-common          x86_64  8.0.24-1.el8  mysql80-community  618 k
 mysql-community-libs            x86_64  8.0.24-1.el8  mysql80-community  1.4 M

Transaction Summary
================================================================================
Install  5 Packages

Total download size: 68 M
Installed size: 325 M
Is this ok [y/N]: @<i>{y}
Downloading Packages:
@<b>{（中略）}
Importing GPG key 0x5072E1F5:
 Userid     : "MySQL Release Engineering <mysql-build@oss.oracle.com>"
 Fingerprint: A4A9 4068 76FC BD3C 4567 70C8 8C71 8D3B 5072 E1F5
 From       : /etc/pki/rpm-gpg/RPM-GPG-KEY-mysql
Is this ok [y/N]: @<i>{y}
Key imported successfully
@<b>{（中略）}
Installed:
  mysql-community-client-8.0.24-1.el8.x86_64
  mysql-community-client-plugins-8.0.24-1.el8.x86_64
  mysql-community-common-8.0.24-1.el8.x86_64
  mysql-community-libs-8.0.24-1.el8.x86_64
  mysql-community-server-8.0.24-1.el8.x86_64

Complete!
[root@mysql80cent8 ~]# @<i>{systemctl enable mysqld.service}
@<b>{※自動起動ON}
[root@mysql80cent8 ~]# @<i>{systemctl start mysqld.service}
@<b>{※起動}
[root@mysql80cent8 ~]# @<i>{ps aux | fgrep mysqld}
mysql       2145 13.8  9.8 1762448 366972 ?      Ssl  15:59   0:00 /usr/sbin/mysqld
root        2189  0.0  0.0  12112  1068 pts/0    S+   15:59   0:00 grep -F --color=auto mysql
d
[root@mysql80cent8 ~]# @<i>{fgrep assword /var/log/mysqld.log}
@<b>{※ログからサーバ初期パスワードを確認}
2021-05-01T06:59:19.217897Z 6 [Note] [MY-010454] [Server] A temporary password is generated
for root@localhost: 51nz-RX2axWM
//}

@<hidx>{mysql_secure_installation}@<tt>|mysql_secure_installation|も使えます。

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/mysql-secure-installation.html, https://dev.mysql.com/doc/refman/8.0/ja/mysql-secure-installation.html}

//emlist{
[root@mysql80cent8 ~]# @<i>{mysql_secure_installation}

Securing the MySQL server deployment.

Enter password for user root:@<b>{先ほど確認したサーバ初期パスワードを入力}

The existing password for the user account root has expired. Please set a new password.

New password:@<b>{新しいパスワードを入力}

Re-enter new password:@<b>{同じパスワードを入力}
The 'validate_password' component is installed on the server.
The subsequent steps will run with the existing configuration
of the component.
Using existing password for root.

Estimated strength of the password: 100
Change the password for root ? ((Press y|Y for Yes, any other key for No) : @<i>{n}

 ... skipping.
@<b>{（中略）}
Remove anonymous users? (Press y|Y for Yes, any other key for No) : @<i>{y}
Success.
@<b>{（中略）}
Disallow root login remotely? (Press y|Y for Yes, any other key for No) : @<i>{y}
Success.
@<b>{（中略）}
Remove test database and access to it? (Press y|Y for Yes, any other key for No) : @<i>{y}
 - Dropping test database...
Success.

 - Removing privileges on test database...
Success.
@<b>{（中略）}
Reload privilege tables now? (Press y|Y for Yes, any other key for No) : @<i>{y}
Success.

All done!
//}

=== Dedicated Server Mode

MySQL 5.7までは、リソースが乏しいサーバ環境でも動作するよう各種@<idx>{バッファ容量}のデフォルト設定は小さめでした。
MySQL 8.0では、MySQL専用サーバとして設定する場合@<idx>{Dedicated Server Mode}によって、以下の項目の@<idx>{自動設定}を行うことが可能です。

 * @<hidx>{innodb_buffer_pool_size}@<tt>|innodb_buffer_pool_size|
 * @<hidx>{innodb_log_file_size}@<tt>|innodb_log_file_size|
 * @<hidx>{innodb_log_files_in_group}@<tt>|innodb_log_files_in_group|
 * @<hidx>{innodb_flush_method}@<tt>|innodb_flush_method|

@<idx>{起動オプション}として@<hidx>{innodb-dedicated-server}@<tt>|--innodb-dedicated-server=ON|を付けてサーバを起動することで自動設定されます。
具体的な設定値は公式リファレンスマニュアル（以下の1つ目のURL）に記載されています。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-dedicated-server.html, https://dev.mysql.com/doc/refman/8.0/ja/innodb-dedicated-server.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-parameters.html#sysvar_innodb_dedicated_server, https://dev.mysql.com/doc/refman/8.0/ja/innodb-parameters.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/innodb-parameters.html#sysvar_innodb_dedicated_server, sysvar_innodb_dedicated_server}

==== ブログ記事等

 * @<href>{https://yakst.com/ja/posts/4781, https://yakst.com/ja/posts/4781}
 * @<href>{https://www.s-style.co.jp/blog/2018/08/2281/, https://www.s-style.co.jp/blog/2018/08/2281/}

== アップグレードインストール

@<idx>{アップグレードインストール}する方法としては、

 * @<idx>{インプレースアップグレード}する方法
 * 新環境を別途用意し、旧環境で@<hidx>{mysqldump}@<tt>|mysqldump|した内容を@<idx>{リストア}する方法

の2つがあります。

 * @<href>{https://speakerdeck.com/yoshiakiyamasaki/20181201-mysqlbaziyonatupufalseji-chu-zhi-shi?slide=26, https://speakerdeck.com/yoshiakiyamasaki/20181201-mysqlbaziyonatupufalseji-chu-zhi-shi?slide=26}

====[column] @<tt>|Windows環境におけるアップグレード|

こちらが参考になります。

 * @<href>{https://lefred.be/content/upgrading-from-mysql-5-7-to-8-0-on-windows/, https://lefred.be/content/upgrading-from-mysql-5-7-to-8-0-on-windows/}

====[/column]

=== @<idx>{インプレースアップグレード}

こちらの資料の8～17ページを参照してください（要@<idx>{Oracleシングル・サインオンアカウント}@<fn>{010OSA}）。

 * @<href>{https://www.mysql.com/jp/why-mysql/presentations/mysql-80-upgrade-checker-201811-jp/, https://www.mysql.com/jp/why-mysql/presentations/mysql-80-upgrade-checker-201811-jp/}

==== シンプルなケースにおけるインプレースアップグレードの流れ

 * MySQL 5.7系列の最新バージョンまでアップグレードする
 * @<idx>{MySQL Shell} 8.0をインストールして@<idx>{Upgrade Checker}（後述）を実行し、問題点を抽出する
 * 問題となる設定やアプリケーションを修正する
 ** 設定のうち、MySQL 8.0で改名されたパラメータ等については@<hidx>{--loose接頭辞}@<tt>|--loose|接頭辞を付けると良い
 * @<idx>{バックアップ}を取得する
 * MySQL Server 8.0を@<idx>{上書きインストール}して起動する
 * 必要に応じてユーザと権限設定等を修正する
 ** @<idx>{アプリケーション}で使用するユーザを新規に作り直す場合、第2章で説明する@<idx>{認証プラグイン}の指定に注意する

//noindent
@<b>{【注】}8.0.16から@<tt>|mysql_upgrade|が不要になりました。

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/upgrading-what-is-upgraded.html, https://dev.mysql.com/doc/refman/8.0/ja/upgrading-what-is-upgraded.html}

なお、アップグレードインストール後、@<tt>|mysql_upgrade_info|ファイルの所有者・アクセス権が原因でサーバ起動に失敗することがあります。

 * @<href>{https://blog.pinkumohikan.com/entry/could-not-start-mysql8-after-version-up, https://blog.pinkumohikan.com/entry/could-not-start-mysql8-after-version-up}
 * @<href>{https://mgng.mugbum.info/1542, https://mgng.mugbum.info/1542}

//noindent
@<b>{【注】}MySQL 8.0ではダウングレードがサポートされなくなっています。

 * @<href>{https://mita2db.hateblo.jp/entry/MySQL_8.0_%E3%81%AF%E3%83%80%E3%82%A6%E3%83%B3%E3%82%B0%E3%83%AC%E3%83%BC%E3%83%89%E3%81%A7%E3%81%8D%E3%81%AA%E3%81%84, https://mita2db.hateblo.jp/entry/MySQL_8.0_%E3%81%AF%E3%83%80%E3%82%A6}@<br>{}@<href>{https://mita2db.hateblo.jp/entry/MySQL_8.0_%E3%81%AF%E3%83%80%E3%82%A6%E3%83%B3%E3%82%B0%E3%83%AC%E3%83%BC%E3%83%89%E3%81%A7%E3%81%8D%E3%81%AA%E3%81%84, %E3%83%B3%E3%82%B0%E3%83%AC%E3%83%BC%E3%83%89%E3%81%A7%E3%81}@<br>{}@<href>{https://mita2db.hateblo.jp/entry/MySQL_8.0_%E3%81%AF%E3%83%80%E3%82%A6%E3%83%B3%E3%82%B0%E3%83%AC%E3%83%BC%E3%83%89%E3%81%A7%E3%81%8D%E3%81%AA%E3%81%84, %8D%E3%81%AA%E3%81%84}

=== @<tt>|mysqldump|→新環境へのリストアを行う場合の注意点

MySQL 8.0の仕様変更により、旧バージョンで取得した@<idx>{ダンプファイル}を@<idx>{リストア}する際に@<idx>{エラー}が発生する場合があります。

 * @<href>{https://hit.hateblo.jp/entry/MYSQL/MYSQL8/SETTING, https://hit.hateblo.jp/entry/MYSQL/MYSQL8/SETTING}

個人的には、サーバ全体のダンプファイルを一括取得するのではなく、以下のようにするのが良いのではないかと考えています。

 * ユーザはDBのデータとは別に移行する
 ** @<href>{https://speakerdeck.com/yoshiakiyamasaki/20181201-mysqlbaziyonatupufalseji-chu-zhi-shi?slide=70, https://speakerdeck.com/yoshiakiyamasaki/20181201-mysqlbaziyonatupufalseji-chu-zhi-shi?slide=70}
 * DBのデータはスキーマ（DB）別に分割して取得し、移行する
 ** 意図しない情報まで新環境に引き継がないようにする

=== レプリケーションを利用する@<idx>{アップグレード}の注意点

@<tt>|mysqldump|→新環境への@<idx>{リストア}などで移行する場合、システム停止時間の短縮のために@<idx>{レプリケーション}を利用する方法があります。
ところが最近、レプリケーションにおいて@<idx>{複数バージョン}が混在する場合のサポートポリシーが変わり、3バージョン混在@<fn>{020RSC}の環境がサポート外となりました@<fn>{030RSC}。

 * @<href>{https://qiita.com/hmatsu47/items/2cfbb7dec89ce5ddd647, https://qiita.com/hmatsu47/items/2cfbb7dec89ce5ddd647}

=== Upgrade Checker

MySQL 5.7環境からのアップグレード時に@<idx>{互換性}で問題になりそうな箇所を抽出するための@<idx>{Upgrade Checker}があります。

==== 前掲のこちらの資料25～30ページ

 * @<href>{https://www.mysql.com/jp/why-mysql/presentations/mysql-80-upgrade-checker-201811-jp/, https://www.mysql.com/jp/why-mysql/presentations/mysql-80-upgrade-checker-201811-jp/}

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-shell-utilities-upgrade.html, https://dev.mysql.com/doc/mysql-shell/8.0/ja/mysql-shell-utilities-upgrade.html}

==== ブログ記事等

 * @<href>{https://yakst.com/ja/posts/5190, https://yakst.com/ja/posts/5190}
 * @<href>{https://gihyo.jp/dev/serial/01/mysql-road-construction-news/0076, https://gihyo.jp/dev/serial/01/mysql-road-construction-news/0076}
 * @<href>{https://mysqlserverteam.com/upgrade-checker-evolution/, https://mysqlserverteam.com/upgrade-checker-evolution/}

//embed[latex]{
\clearpage
//}

 * @<href>{https://elephantdolphin.blogspot.com/2019/09/upgrading-from-mysql-57-to-mysql-80_25.html, https://elephantdolphin.blogspot.com/2019/09/upgrading-from-mysql-57-to-mysql-80_25.html}
 ** よく出る指摘事項

==== 実行例（CentOS 7上）

//emlist{
[root@mysql57to80 ~]# @<i>{yum-config-manager --disable mysql57-community}
@<b>{※MySQL Shell 8.0をインストールするため、MySQL 5.7のリポジトリを無効にする}
Loaded plugins: fastestmirror
@<b>{（中略）}
username =

[root@mysql57to80 ~]# @<i>{yum-config-manager --enable mysql80-community}
@<b>{※MySQL 8.0のリポジトリを有効にする}
@<b>{（省略）}
[root@mysql57to80 ~]# @<i>{yum install mysql-shell}
@<b>{※MySQL Shell 8.0をインストールする}
Loaded plugins: fastestmirror
@<b>{（中略）}
Dependencies Resolved

================================================================================
 Package         Arch       Version             Repository                 Size
================================================================================
Installing:
 mysql-shell     x86_64     8.0.24-1.el7        mysql-tools-community      31 M

Transaction Summary
================================================================================
Install  1 Package

Total download size: 31 M
Installed size: 140 M
Is this ok [y/d/N]: @<i>{y}
Downloading packages:
@<b>{（中略）}
Installed:
  mysql-shell.x86_64 0:8.0.24-1.el7

Complete!
[root@mysql57to80 ~]# @<i>{mysqlsh -u root -S /var/lib/mysql/mysql.sock}
Please provide the password for 'root@/var%2Flib%2Fmysql%2Fmysql.sock': @<b>{パスワードを入力}
Save password for 'root@/var%2Flib%2Fmysql%2Fmysql.sock'? [Y]es/[N]o/Ne[v]er (default No):
@<b>{[Enter]キーを押す}
MySQL Shell 8.0.24
@<b>{（中略）}
No default schema selected; type \use <schema> to set one.
 MySQL  localhost  JS > @<i>{util.checkForServerUpgrade()}
The MySQL server at /var%2Flib%2Fmysql%2Fmysql.sock, version 5.7.34 - MySQL
Community Server (GPL), will now be checked for compatibility issues for
upgrade to MySQL 8.0.24...

1) Usage of old temporal type
  No issues found
@<b>{（中略）}
21) New default authentication plugin considerations
@<b>{（中略）}
Errors:   0
Warnings: 1
Notices:  1

No fatal errors were found that would prevent an upgrade, but some potential issues were detec
ted. Please ensure that the reported issues are not significant before upgrading.
 MySQL  localhost  JS > @<i>{\q}
Bye!
//}

====[column] @<tt>|Upgrade Checkerのチェック項目|

Upgrade Checkerのチェック項目は、8.0.15時点で15項目だったのが8.0.24時点では21項目になりました。
また、8.0.16からターゲットバージョンを指定してチェックすることができるようになりました。

====[/column]

なお、MySQL 5.5・5.6からの移行で利用可能な@<idx>{非公式Upgrade Checker}（yoku0825さん作）もあります。

 * @<href>{https://github.com/yoku0825/p5-mysql-upgrade-checker, https://github.com/yoku0825/p5-mysql-upgrade-checker}

==== ブログ記事等

 * @<href>{https://yoku0825.blogspot.com/2018/07/mysql-shellupgrade-checkerperl-5.html, https://yoku0825.blogspot.com/2018/07/mysql-shellupgrade-checkerperl-5.html}
 * @<href>{https://next4us-ti.hatenablog.com/entry/2018/12/05/085115, https://next4us-ti.hatenablog.com/entry/2018/12/05/085115}

=== データディクショナリのInnoDB化

前掲の資料にも説明がありましたが、MySQL 8.0から@<idx>{データディクショナリ}が@<idx>{InnoDB}化されました。
@<idx>{トランザクション}対応という触れ込みですが、今のところ@<idx>{DDL}は基本的にトランザクション非対応です。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/data-dictionary.html, https://dev.mysql.com/doc/refman/8.0/ja/data-dictionary.html}

==== ブログ記事等

 * @<href>{https://mysqlserverteam.com/mysql-8-0-data-dictionary-architecture-and-design/, https://mysqlserverteam.com/mysql-8-0-data-dictionary-architecture-and-design/}
 * @<href>{https://lefred.be/content/mysql-8-0-data-dictionary-tables-and-why-they-should-stay-protected/, https://lefred.be/content/mysql-8-0-data-dictionary-tables-and-why-they-should-stay-protected/}

InnoDBテーブル作成時、以前は@<tt>|.ibd|ファイルとともに@<tt>|.frm|ファイルが生成されましたが、MySQL 8.0では@<tt>|.frm|ファイルは生成されません。

MySQL 5.7からの@<idx>{インプレースアップグレード}時、サーバを最初に起動したタイミングで変換が行われます。

== 設定パラメータ・起動パラメータの変更

以下を確認して、設定パラメータの変更を計画します。

==== 前掲のこちらの資料19～24ページ

 * @<href>{https://www.mysql.com/jp/why-mysql/presentations/mysql-80-upgrade-checker-201811-jp/, https://www.mysql.com/jp/why-mysql/presentations/mysql-80-upgrade-checker-201811-jp/}
 ** 特に非推奨化・廃止された機能（21～22ページ）に注意。@<hidx>{sql_mode}@<tt>|sql_mode|、アカウント管理など。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/mysql-nutshell.html, https://dev.mysql.com/doc/refman/8.0/ja/mysql-nutshell.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/upgrading-from-previous-series.html, https://dev.mysql.com/doc/refman/8.0/ja/upgrading-from-previous-series.html}

=== 対象となるサーバ設定パラメータ・起動パラメータ

前述の資料で示されているもののほか、いくつか変更点があります（デフォルトの変更・廃止など）。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/added-deprecated-removed.html, https://dev.mysql.com/doc/refman/8.0/ja/added-deprecated-removed.html}

==== 公式サーババージョンリファレンス

 * @<href>{https://dev.mysql.com/doc/mysqld-version-reference/en/, https://dev.mysql.com/doc/mysqld-version-reference/en/}

==== とみたまさひろさん作・バージョン間パラメータ比較ができるページ

 * @<href>{https://mysql-params.tmtms.net/, https://mysql-params.tmtms.net/}

==== デフォルト値が変更されたパラメータの例

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_explicit_defaults_for_timestamp, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_explicit_defaults_for_timestamp, sysvar_explicit_defaults_for_timestamp}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_max_allowed_packet, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_max_allowed_packet, sysvar_max_allowed_packet}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_table_open_cache, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_table_open_cache, sysvar_table_open_cache}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_event_scheduler, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_event_scheduler, sysvar_event_scheduler}

==== 上限値が変更されたパラメータの例

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_max_prepared_stmt_count, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_max_prepared_stmt_count, sysvar_max_prepared_stmt_count}

==== サーバ変数名が変更されたパラメータの例

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_transaction_read_only, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_transaction_read_only, sysvar_transaction_read_only}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_transaction_isolation, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_transaction_isolation, sysvar_transaction_isolation}

=== その他の変更点

==== @<hidx>{bind-address}@<tt>|bind-address|サーバ変数（起動オプション）で複数のアドレスをサポート

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_bind_address, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_bind_address, sysvar_bind_address}

==== @<idx>{管理専用ポート}の追加

 * @<href>{https://mita2db.hateblo.jp/entry/admin_network_if, https://mita2db.hateblo.jp/entry/admin_network_if}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/administrative-connection-interface.html, https://dev.mysql.com/doc/refman/8.0/ja/administrative-connection-interface.html}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/privileges-provided.html#priv_service-connection-admin, https://dev.mysql.com/doc/refman/8.0/ja/privileges-provided.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/privileges-provided.html#priv_service-connection-admin, priv_service-connection-admin}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_admin_address, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_admin_address, sysvar_admin_address}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_admin_port, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_admin_port, sysvar_admin_port}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_create_admin_listener_thread, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_create_admin_listener_thread, sysvar_create_admin_listener_thread}

===={80210101} 管理用クライアント専用のネットワーク設定が可能に（一般クライアント設定と分離）

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/administrative-connection-interface.html#administrative-interface-encrypted-connections, https://dev.mysql.com/doc/refman/8.0/ja/administrative-connection-interface.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/administrative-connection-interface.html#administrative-interface-encrypted-connections, administrative-interface-encrypted-connections}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-tls-channel-status-table.html, https://dev.mysql.com/doc/refman/8.0/ja/performance-schema-tls-channel-status-table.html}

==== サーバに@<hidx>{mysqld_safe}@<tt>|mysqld_safe|機能を追加

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-options.html#option_mysqld_daemonize, https://dev.mysql.com/doc/refman/8.0/ja/server-options.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-options.html#option_mysqld_daemonize, option_mysqld_daemonize}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-options.html#option_mysqld_initialize, https://dev.mysql.com/doc/refman/8.0/ja/server-options.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-options.html#option_mysqld_initialize, option_mysqld_initialize}

==== @<idx>{TLS 1.3}サポート

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/encrypted-connection-protocols-ciphers.html, https://dev.mysql.com/doc/refman/8.0/ja/encrypted-connection-protocols-ciphers.html}
 ** 8.0.18から@<idx>{グループレプリケーション}もTLS 1.3をサポート
 ** 8.0.19から非同期レプリケーションもTLS 1.3をサポートし、暗号スイート指定を改善

==== サーバステータス変数の追加・廃止

 * @<href>{https://www.s-style.co.jp/blog/2020/02/5847/, https://www.s-style.co.jp/blog/2020/02/5847/}

== @<idx>{キーワード}と予約語

SQLの中で@<idx>{予約語}をテーブル名・カラム名等に使用する場合、バッククォート等で囲む必要があります。
MySQL 8.0で増えた予約語がテーブル名等に使われている場合は要注意です。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/keywords.html, https://dev.mysql.com/doc/refman/8.0/ja/keywords.html}

==== 公式サーババージョンリファレンス

 * @<href>{https://dev.mysql.com/doc/mysqld-version-reference/en/keywords.html, https://dev.mysql.com/doc/mysqld-version-reference/en/keywords.html}

==== ブログ記事等

 * 著者ブログ
 ** @<href>{https://qiita.com/hmatsu47/items/a1da0e06f0597acd6502, https://qiita.com/hmatsu47/items/a1da0e06f0597acd6502}

== キャラクタセットと照合順序

MySQL 8.0では@<idx>{Unicode 9.0}がサポートされるとともに、デフォルトの@<idx>{キャラクタセット}が@<hidx>{utf8mb4}@<tt>|utf8mb4|に変更されました。
あわせて@<idx>{照合順序}（@<hidx>{COLLATION}@<tt>|COLLATION|）も拡張されています。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/charset-charsets.html, https://dev.mysql.com/doc/refman/8.0/ja/charset-charsets.html}

==== ブログ記事等

 * @<href>{https://mysqlserverteam.com/mysql-8-0-1-japanese-collation-for-utf8mb4-ja_jp/, https://mysqlserverteam.com/mysql-8-0-1-japanese-collation-for-utf8mb4-ja_jp/}
 * @<href>{https://mysqlserverteam.com/mysql-8-0-kana-sensitive-collation-for-japanese-ja_jp/, https://mysqlserverteam.com/mysql-8-0-kana-sensitive-collation-for-japanese-ja_jp/}
 * @<href>{https://tmtms.hatenablog.com/entry/201805/mysql-innovation-day-tokyo, https://tmtms.hatenablog.com/entry/201805/mysql-innovation-day-tokyo}

==== 公式リファレンスマニュアル／設定パラメータ等

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_character_set_server, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_character_set_server, sysvar_character_set_server}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_collation_server, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_collation_server, sysvar_collation_server}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_character_set_database, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_character_set_database, sysvar_character_set_database}

//embed[latex]{
\clearpage
//}

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_character_set_client, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_character_set_client, sysvar_character_set_client}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_character_set_connection, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_character_set_connection, sysvar_character_set_connection}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_character_set_results, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_character_set_results, sysvar_character_set_results}
 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_default_collation_for_utf8mb4, https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#}@<br>{}@<href>{https://dev.mysql.com/doc/refman/8.0/ja/server-system-variables.html#sysvar_default_collation_for_utf8mb4, sysvar_default_collation_for_utf8mb4}

デフォルトの変更とあわせて、@<hidx>{utf8mb4}@<tt>|utf8mb4|指定時の処理高速化も行われています。

==== ブログ記事等

 * @<href>{http://dimitrik.free.fr/blog/archives/2018/04/mysql-performance-80-and-utf8-impact.html, http://dimitrik.free.fr/blog/archives/2018/04/mysql-performance-80-and-utf8-impact.html}

==== ブログ記事等／照合順序による差

 * @<href>{https://yoku0825.blogspot.com/2018/12/utf8mb40900aici.html, https://yoku0825.blogspot.com/2018/12/utf8mb40900aici.html}

加えて、8.0.17から照合順序@<hidx>{utf8mb4_0900_bin}@<tt>|utf8mb4_0900_bin|がサポートされ、@<tt>|utf8mb4_bin|と比べてソートが高速化されています@<fn>{040MB4}。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/charset-unicode-sets.html, https://dev.mysql.com/doc/refman/8.0/ja/charset-unicode-sets.html}

==== ブログ記事等

 * 著者ブログ
 ** @<href>{https://qiita.com/hmatsu47/items/d66830c8a00c21f5edad, https://qiita.com/hmatsu47/items/d66830c8a00c21f5edad}

また、@<idx>{正規表現}@<idx>{ライブラリ}の変更とあわせて、正規表現で@<idx>{Unicode}がサポートされました。

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/regexp.html, https://dev.mysql.com/doc/refman/8.0/ja/regexp.html}

==== ブログ記事等

 * @<href>{https://www.s-style.co.jp/blog/2018/09/2519/, https://www.s-style.co.jp/blog/2018/09/2519/}

//footnote[010OSA][無料で登録可能です。登録するとセミナー受講申し込みや、ホワイトペーパー・各種資料の閲覧等が可能になります。]
//footnote[020RSC][マイナーバージョンであっても3バージョン混在はサポート外となります。]
//footnote[030RSC][サポート外ではありますが、必ずしも「できなくなった」わけではありません。]
//footnote[040MB4][但し、条件によっては遅くなることもあるようです。詳細は著者ブログを参照してください。]
