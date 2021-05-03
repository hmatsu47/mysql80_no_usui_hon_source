= はじめに

== 本書の目的

@<b>{「MySQL 5.7より最大2倍高速」}とOracleがアナウンスしているMySQL 8.0を取り上げた本です@<fn>{001BNC}。

2016年9月にMySQL 8.0.0がはじめてリリースされ、2018年4月リリースのMySQL 8.0.11からGA@<fn>{010GA}となり、MySQL 8.0も徐々にプロダクトへの採用事例が増えてきました。
その間、公式リファレンスマニュアル@<fn>{020ORM}・@<idx>{MySQL Server Team}による@<idx>{MySQL Server Blog}@<fn>{030MSB}のほかMySQLパートナーや個人のブログにMySQL 8.0の新機能を紹介する記事が多数掲載されており、今もその数を増やしています。

この@<b>{「MySQL 8.0の薄い本」}では、MySQL 8.0で導入された新機能をページ数の制約（および著者の能力）の範囲でできるだけ取り上げるとともに、紹介記事のURLを提供します@<fn>{040LP}。

== 想定読者

MySQL 5.7までのバージョンの利用経験があり、MySQL 8.0の新機能に興味がある方です。
なお、この本では従来のMySQLについて丁寧な説明は行いません。
はじめてMySQLに触れる方は、まずMySQL 5.7までの入門書・解説書などを読んでMySQLを実際に起動・操作し、全体像を掴んでおくことをお勧めします。

== ライセンスについて

この作品（本書）は、クリエイティブ・コモンズの 表示 - 継承 4.0 国際 ライセンスで提供されています。
ライセンスの写しをご覧になるには、@<href>{https://creativecommons.org/licenses/by-sa/4.0/, https://creativecommons.org/licenses/by-sa/4.0/}をご覧頂くか、Creative Commons, PO Box 1866, Mountain View, CA 94042, USAまでお手紙をお送りください。

なお、追加の条件として以下1点のみ遵守をお願いします。

 * 原著者名（hmatsu47）とあわせて、原書名（MySQL 8.0の薄い本）を明示すること@<fn>{050CR}

== 商標について

 * @<idx>{Oracle}と@<idx>{Java}、@<idx>{JavaScript}、JDKおよびMySQLは、Oracle Corporation及びその子会社、関連会社の米国及びその他の国における登録商標です。文中の社名、商品名等は各社の商標または登録商標である場合があります@<fn>{060OTG}。
 * その他記載の会社名、製品名等は、それぞれの会社・組織の商標もしくは登録商標です。

== その他免責事項、制限事項等

 * 本書記載の内容は無保証です。本書の利用により生じた一切の損害等を著者は負わないものとします。
 * 本書記載の内容は著者個人の調査等によるものであり、所属する組織とは無関係です。
 * 本書の内容は2021年5月現在の情報をもとに構成しています。
 ** MySQL 8.0は@<idx>{Continuous Delivery Model}（@<idx>{継続提供モデル}）を採用しており、マイナーバージョンが上がるごとに機能が追加されていくことが想定されています。
 ** 本書で紹介する機能は途中のマイナーバージョンで追加・変更されたものを含みますが、煩雑になるため追加・変更されたマイナーバージョンは原則として記載しません。
 *** 8.0.21以降については、主な変更点を「@<title>{mysql80thinR1p}」に記載しています。
 ** 本書では、MySQL NDB Cluster 8.0の機能は原則として紹介しません。
 * 本書の内容に誤りや記載URLのリンク切れ、不適切なURL等が見つかった場合は、こちらにご連絡ください。
 ** E-Mail : hmatsu47@gmail.com
 ** Twitter : @hmatsu47

== 謝辞

本書のレビューを快く引き受けてくださった@taka_yuki_04さん、また執筆中に進捗を見守ってくださったMySQLユーザ会界隈@<fn>{070MOJ}やその他の皆様、ありがとうございました。

//embed[latex]{
\clearpage
//}

== 電子版PDF・最新版ダウンロードURL

MySQL 8.0.20対応版より、印刷版は電子版PDFの要約版（電子版PDFから参考ブログ記事等のURLを省いたもの）になりました。
ブログ記事等のURLは電子版PDFをダウンロードしてご利用ください。

//image[mysql80_ebook][@<href>{https://github.com/hmatsu47/mysql80_no_usui_hon/raw/master/PDF_ebook/book_ebook.pdf, https://github.com/hmatsu47/mysql80_no_usui_hon/raw/master/PDF_ebook/book_ebook.pdf}][scale=0.164]{
  電子版PDF
//}

== 実行例・サンプル

Linuxコマンドライン・SQL実行例がテキストファイルで入っています。

//image[mysql80_examples][@<href>{https://github.com/hmatsu47/mysql80_no_usui_hon/tree/master/examples, https://github.com/hmatsu47/mysql80_no_usui_hon/tree/master/examples}][scale=0.164]{
  実行例・サンプル
//}

//footnote[001BNC][性能・パフォーマンスについて知るには、MySQL界隈で「ベンチマークおじさん」として有名？なDimitriさんのブログや資料がお勧めです。「日本のDimitri（おじ）さん」こと@i_rethiさんによるこちらの解説記事をご確認ください。@<href>{https://hiroi10.hatenablog.com/entry/2018/12/24/000138, http://hiroi10.hatenablog.com/entry/2018/12/24/000138}]
//footnote[010GA][General Availability]
//footnote[020ORM][@<href>{https://dev.mysql.com/doc/refman/8.0/en/, https://dev.mysql.com/doc/refman/8.0/en/}（日本語版は@<href>{https://dev.mysql.com/doc/refman/8.0/ja/, https://dev.mysql.com/doc/refman/8.0/ja/}）]
//footnote[030MSB][@<href>{https://mysqlserverteam.com, https://mysqlserverteam.com} 一部日本語記事あり。また、Yakst | 人力翻訳コミュニティ @<href>{https://yakst.com/ja, https://yakst.com/ja} に日本語訳されている記事もあります。]
//footnote[040LP][URLを入力するのは面倒なので、各章末に関連リンク集へのQRコードを掲載します。]
//footnote[050CR][情報の出所がわからなくなることを避けるため]
//footnote[060OTG][@<href>{https://www.oracle.com/jp/legal/trademarks.html, https://www.oracle.com/jp/legal/trademarks.html}]
//footnote[070MOJ][若い方もいらっしゃるので、「MySQLおじさん」の括りではありません。]
