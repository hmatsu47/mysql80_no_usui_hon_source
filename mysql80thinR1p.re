= MySQL 8.0.21以降の主な変更点

== 8.0.21（リリース日：2020-07-13）

 * 管理用クライアント専用のネットワーク設定が可能に（一般クライアント設定と分離）（P.@<pageref>{80210101}）
 * @<tt>|CREATE USER|・@<tt>|ALTER USER|でJSON形式のユーザコメントが登録可能に（P.@<pageref>{80210201}）
 * @<tt>|JSON_VALUE()|関数（P.@<pageref>{80210501}）
 * バイナリログのチェックサムをサポート（P.@<pageref>{80210701}）
 * 可用性向上のために2つのシステム変数のデフォルト値を変更（P.@<pageref>{80210701}）
 * 論理ダンプ・リストアツール（P.@<pageref>{80210703}）
 * オプティマイザスイッチ@<tt>|subquery_to_derived|・@<tt>|prefer_ordering_index|の追加（P.@<pageref>{80210801}）
 * @<tt>|UPDATE|・@<tt>|DELETE|でセミジョイン（準結合）・マテリアライズ（実体化）最適化をサポート（P.@<pageref>{80210802}）
 * Redoログの無効化が可能に（P.@<pageref>{80210803}）
 * Undoテーブルスペースの処理性能向上と安定化・ACID Undo DDLのサポート（P.@<pageref>{80210803}）
 * テーブルスペースのパス検証の無効化が可能に（P.@<pageref>{80210803}）
 * KEYパーティショニングでカラムインデックスプレフィックスを使ったときに正しく警告・エラーを出力するようになった（P.@<pageref>{80211001}）
 * 文字列型と数値型・時間型の型変換（キャスト）がどのように行われたかを@<tt>|EXPLAIN ANALYZE|・@<tt>|EXPLAIN FORMAT=JSON|・@<tt>|EXPLAIN FORMAT=TREE|で可視化（P.@<pageref>{80211001}）

== 8.0.22（リリース日：2020-10-19）

 * 権限テーブルのノンロッキング読み取りが可能に（P.@<pageref>{80210201}）
 * @<tt>|CREATE|・@<tt>|DROP|・@<tt>|RENAME USER|で存在しない@<tt>|DEFINER|に対するチェックを厳格化（P.@<pageref>{80210201}）
 * @<tt>|ALTER DATABASE|で読み取り専用オプションをサポート（P.@<pageref>{80220301}）
 * MySQL Router／HTTPサーバプラグインとREST API（8.0.22でデフォルト有効に）（P.@<pageref>{80220703}）
 * 論理ダンプ・リストアツールの拡充（P.@<pageref>{80210703}）
 * 新しい非同期レプリケーション接続フェイルオーバーメカニズム（P.@<pageref>{80220701}）
 * 「ホワイトリスト」「スレーブ」（用語）の非推奨化による変更（P.@<pageref>{80220701}）
 * オプティマイザスイッチ@<tt>|hypergraph_optimizer|の追加（デバッグビルドのみ／P.@<pageref>{80210801}）
 * Derived Condition Pushdown最適化（P.@<pageref>{80220801}）
 * @<tt>|PREPARE|の実行アルゴリズムが変更（@<tt>|ORDER BY ?|での列番号指定などが無視されるように／P.@<pageref>{80220802}）
 * @<tt>|SELECT| ～ @<tt>|FOR SHARE|が@<tt>|SELECT|権限のみで実行可能に（P.@<pageref>{80210803}）
 * Linux環境においてテーブルスペース配置を効率化（@<tt>|innodb_extend_and_initialize|／P.@<pageref>{80210803}）
 * 並列度の低いシステムでdedicated log writer threadsの無効化が可能に（@<tt>|innodb_log_writer_threads|／P.@<pageref>{80210803}）
 * Information Schemaに@<tt>|SCHEMATA_EXTENSIONS|テーブルを追加（P.@<pageref>{80220901}）
 * Information Schemaの@<tt>|TABLESPACES|テーブルが非推奨に（P.@<pageref>{80220902}）
 * Performance Schemaにエラーログテーブルを追加（P.@<pageref>{80220903}）
 * @<tt>|SHOW PROCESSLIST|の性能改善（@<tt>|Performance Schema.processlist|テーブルを利用／P.@<pageref>{80220904}）
 * 括弧付きクエリ式のサポート（P.@<pageref>{80221001}）
 * @<tt>|CAST()|関数または@<tt>|CONVERT()|関数による@<tt>|YEAR|型へのキャスト（P.@<pageref>{80221002}）
 * @<tt>|CAST(value AT TIME ZONE specifier AS DATETIME)|による@<tt>|TIMESTAMP|列値のタイムゾーン変換（P.@<pageref>{80221002}）
 * 監査プラグインで@<tt>|audit_log_read()|を使用したログ読み込み操作を改善（Enterprise版／P.@<pageref>{80221004}）
 * InnoDB memcachedプラグインが非推奨に（P.@<pageref>{80211001}）
 * Oracle Cloud Infrastructure（OCI）用のキーリングプラグイン（P.@<pageref>{80211001}）
 * C APIで@<tt>|mysql_real_connect_dns_srv()|をサポート（P.@<pageref>{80211001}）
 * ネットワーク名前空間指定子のサポート（P.@<pageref>{80211001}）

== 8.0.23（リリース日：2021-01-18）

 * @<tt>|RELOAD|権限の分割（@<tt>|FLUSH|処理だけを可能に）（P.@<pageref>{80210201}）
 * ユーザ権限のIPアドレス照合ルール（照合順）の変更（P.@<pageref>{80210201}）
 * SASL LDAP認証プラグインがSCRAM-SHA-256をサポート（Enterprise版）（P.@<pageref>{80210201}）
 * 不可視カラム（P.@<pageref>{80220301}）
 * @<tt>|FLUSH HOSTS|が非推奨に（P.@<pageref>{80230302}）
 * GIS関数@<tt>|ST_HausdorffDistance()|・@<tt>|ST_FrechetDistance()|を追加（P.@<pageref>{80230601}）
 * 非同期接続フェイルオーバ機能がグループレプリケーション・トポロジをサポート（P.@<pageref>{80210701}）
 * AdminAPIのクラスタ診断機能強化（P.@<pageref>{80230702}）
 * Parallel Table Import Utilityが複数データファイルからのインポートをサポート（P.@<pageref>{80210703}）
 * マルチスレッドレプリカ（スレーブ）レプリケーションにおけるデッドロック検出機構の改善（P.@<pageref>{80220701}）
 * GTIDを使用しないソースからGTIDを使用するレプリカへのレプリケーションが可能に（P.@<pageref>{80220701}）
 * @<tt>|master_info_repository|と@<tt>|relay_log_info_repository|が非推奨に（P.@<pageref>{80220701}）
 * 「マスター」（用語）の非推奨化による変更（P.@<pageref>{80220701}）
 * Hash Join高速化（P.@<pageref>{80230801}）
 * ダブルライトファイル暗号化（P.@<pageref>{80230802}）
 * @<tt>|AUTOEXTEND_SIZE|オプション（P.@<pageref>{80210803}）
 * テーブルスペース@<tt>|DROP|・@<tt>|TRUNCATE|の高速化（P.@<pageref>{80210803}）
 * @<tt>|mysql_bind_param()| C API関数によるクエリ属性の定義（P.@<pageref>{80211001}）
 * サーバ変数@<hidx>{temptable_max_mmap}@<tt>|temptable_max_mmap|追加（P.@<pageref>{80211001}）
 * ユーザ定義関数@<tt>|gen_blacklist()|を@<tt>|gen_blocklist()|に変更（Enterprise版／P.@<pageref>{80211001}）
 * MySQL Enterprise Firewallにグループプロファイル機能を追加（Enterprise版／P.@<pageref>{80211001}）

== 8.0.24（リリース日：2021-04-20）

 * @<tt>|caching_sha2_password_digest_rounds|システム変数の追加（パスワード保存時のハッシュラウンド数／P.@<pageref>{80210201}）
 * @<tt>|ST_Collect()|・@<tt>|ST_LineInterpolatePoint()|・@<tt>|ST_LineInterpolatePoints()|・@<tt>|ST_PointAtDistance()|関数の追加（P.@<pageref>{80230601}）
 * @<tt>|CAST()|・@<tt>|CONVERT()|関数で地理空間タイプをサポート（P.@<pageref>{80240601}）
 * 論理ダンプ・リストアツールに主キーのないテーブルの処理に対するオプションを追加（P.@<pageref>{80210703}）
 * シェルAPIの直接コマンドライン実行の改善（P.@<pageref>{80240701}）
 * MySQL Shellのログ出力の改善（P.@<pageref>{80240701}）
 * @<tt>|skip_slave_start|システム変数の追加（P.@<pageref>{80220701}）
 * 相関スカラーサブクエリ最適化（P.@<pageref>{80220802}）
 * @<tt>|AUTOEXTEND_SIZE|の最大値拡大（P.@<pageref>{80210803}）
 * Performance Schema項目名の一部が変更（P.@<pageref>{80240901}）
 * 監査ログファイル（JSON形式）のローテーション後削除（プルーニング／P.@<pageref>{80221004}）
 * キーリングプラグインのコンポーネント化（P.@<pageref>{80211001}）
 * @<tt>|utf8mb3|表示を@<tt>|utf8|よりも優先（P.@<pageref>{80211001}）
