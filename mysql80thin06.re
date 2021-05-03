= GIS（@<idx>{地理情報システム}）の新機能

MySQL 8.0では、@<idx>{地理情報}を扱う@<idx>{GIS機能}がMySQL 5.7と比較して大きく拡張されました。

 * @<idx>{Spatial関数}の追加
 * MySQL 5.7で非推奨になった関数の廃止@<fn>{010DPR}
 * @<idx>{Geography}サポート
 * @<idx>{Spatial Data}・@<idx>{Spatial Index}・Spatial関数の@<idx>{SRID}サポート／@<idx>{地理座標系}サポート

地理座標系をサポートしたことで、MySQL 8.0では地球を@<idx>{回転楕円体}として扱うことができるようになり、GIS機能が利用しやすくなりました。

概要については以下の資料の1つ目、もう少し深く知りたい場合は2つ目をご確認ください。

 * @<href>{https://www.slideshare.net/yoyamasaki/mysql-80gisfoss4g-2018-hokkaido, https://www.slideshare.net/yoyamasaki/mysql-80gisfoss4g-2018-hokkaido}
 * @<href>{https://www.slideshare.net/sakaik/mysql-gis-clubmysql-4, https://www.slideshare.net/sakaik/mysql-gis-clubmysql-4}

=={80230601} GIS関数

MySQL 8.0がサポートするGIS関数は以下の通りです。

==== 【注】

 * @<idx>{WKT}：@<idx>{Well-Known Text}形式
 * @<idx>{WKB}：@<idx>{Well-Known Binary}形式
 * @<idx>{MBR}：@<idx>{Minimum Bounding Rectangle}（@<idx>{最小境界矩形}または@<idx>{最小外接矩形}）

====[column] @<tt>|GA後のGIS関数|

MySQL 8.0がサポートするGIS関数はGA後も追加・機能改善が進んでいます。
@<hidx>{ST_Length()}@<tt>|ST_Length()|は8.0.16で単位の指定ができるようになり、8.0.18で異なるジオメトリタイプ（型）間の距離を計算できるようになりました。

====[/column]

//table{
関数名	説明
------------------------------------------------------------------------------------
@<tt>|GeomCollection()|	@<idx>{ジオメトリ}から@<idx>{ジオメトリコレクション}を構築
@<tt>|GeometryCollection()|	ジオメトリからジオメトリコレクションを構築
@<tt>|LineString()|	@<idx>{Point値}から@<idx>{LineString}を構築
@<hidx>{MBRContains()}@<tt>|MBRContains()|@<br>{}　	あるジオメトリのMBRに別のジオメトリのMBRが含まれて@<br>{}いるか？
@<hidx>{MBRCoveredBy()}@<tt>|MBRCoveredBy()|	あるMBRが別のMBRによって覆われているか？
@<hidx>{MBRCovers()}@<tt>|MBRCovers()|	あるMBRが別のMBRをカバーするか？
@<hidx>{MBRDisjoint()}@<tt>|MBRDisjoint()|	2つの形状のMBRが交差していないか？
@<hidx>{MBREquals()}@<tt>|MBREquals()|	2つの形状のMBRが等しいか？
@<hidx>{MBRIntersects()}@<tt>|MBRIntersects()|	2つの形状のMBRが交差するか？
@<hidx>{MBROverlaps()}@<tt>|MBROverlaps()|	2つの形状のMBRが重複するか？
@<hidx>{MBRTouches()}@<tt>|MBRTouches()|	2つの形状のMBRが接触するか？
@<hidx>{MBRWithin()}@<tt>|MBRWithin()|@<br>{}　	あるジオメトリのMBRが別のジオメトリのMBR内に@<br>{}あるか？
@<tt>|MultiLineString()|	LineString値から@<idx>{MultiLineString}を構築
@<tt>|MultiPoint()|	Point値から@<idx>{MultiPoint}を構築
@<tt>|MultiPolygon()|	@<idx>{Polygon値}から@<idx>{MultiPolygon}を構築
@<tt>|Point()|	座標からPointを構築
@<tt>|Polygon()|	LineString引数からPolygonを構築
@<hidx>{ST_Area()}@<tt>|ST_Area()|	多角形または@<idx>{多角形領域}を返す
@<tt>|ST_AsBinary()|@<br>{}@<tt>|ST_AsWKB()|	@<idx>{内部ジオメトリ形式}からWKBに変換@<br>{}　
@<tt>|ST_AsGeoJSON()|	ジオメトリから@<idx>{GeoJSONオブジェクト}を生成
@<tt>|ST_AsText()|@<br>{}@<tt>|ST_AsWKT()|	内部ジオメトリ形式からWKTに変換@<br>{}　
@<tt>|ST_Buffer()|	ジオメトリから指定距離内にある点のジオメトリを返す
@<tt>|ST_Buffer_Strategy()|	@<tt>|ST_Buffer()|の戦略オプションを生成する
@<tt>|ST_Centroid()|	@<idx>{重心}を点として返す
@<hidx>{ST_Collect()}@<tt>|ST_Collect()|	ジオメトリの値を集約する
@<hidx>{ST_Contains()}@<tt>|ST_Contains()|	あるジオメトリが別のジオメトリを含むか？
@<tt>|ST_ConvexHull()|	ジオメトリの@<idx>{凸包}を返す
@<hidx>{ST_Crosses()}@<tt>|ST_Crosses()|	あるジオメトリが別のジオメトリと交差するか？
@<tt>|ST_Difference()|	2つのジオメトリの違いを@<idx>{Point Set}として返す
@<tt>|ST_Dimension()|	ジオメトリの次元
@<hidx>{ST_Disjoint()}@<tt>|ST_Disjoint()|	あるジオメトリが別のジオメトリと交差しないか？
@<hidx>{ST_Distance()}@<tt>|ST_Distance()|	あるジオメトリから別のジオメトリまでの距離
@<hidx>{ST_Distance_Sphere()}@<tt>|ST_Distance_Sphere()|	2つのジオメトリ間の地球上の最小距離
@<tt>|ST_EndPoint()|	LineStringの終点
@<tt>|ST_Envelope()|	ジオメトリのMBRを返す
@<hidx>{ST_Equals()}@<tt>|ST_Equals()|	あるジオメトリが別のジオメトリと等しいか？
@<tt>|ST_ExteriorRing()|	Polygonの外装リングを返す
@<tt>|ST_FrechetDistance()|	@<idx>{離散フレシェ距離}を返す
@<tt>|ST_GeoHash()|	@<idx>{ジオハッシュ値}を生成する
@<tt>|ST_GeomCollFromText()|@<br>{}@<tt>|ST_GeometryCollectionFromText()|@<br>{}@<tt>|ST_GeomCollFromTxt()|	WKTからジオメトリコレクションを返す@<br>{}　
@<tt>|ST_GeomCollFromWKB()|@<br>{}@<tt>|ST_GeometryCollectionFromWKB()|	WKBからジオメトリコレクションを返す@<br>{}　
//}

//table{
関数名	説明
------------------------------------------------------------------------------------
@<tt>|ST_GeometryN()|	ジオメトリコレクションからN番目のジオメトリを返す
@<tt>|ST_GeometryType()|	ジオメトリタイプの名前を返す
@<tt>|ST_GeomFromGeoJSON()|	GeoJSONオブジェクトからジオメトリを生成する
@<tt>|ST_GeomFromText()|@<br>{}@<tt>|ST_GeometryFromText()|	WKTからジオメトリを返す@<br>{}　　
@<tt>|ST_GeomFromWKB()|@<br>{}@<tt>|ST_GeometryFromWKB()|	WKBからジオメトリを返す@<br>{}　　
@<tt>|ST_HausdorffDistance()|	@<idx>{離散ハウスドルフ距離}を返す
@<tt>|ST_InteriorRingN()|	PolygonのN番目の内部リングを返す
@<tt>|ST_Intersection()|	2つの形状が交差するPoint Setを返す
@<hidx>{ST_Intersects()}@<tt>|ST_Intersects()|	あるジオメトリが別のジオメトリと交差するか？　　　　　　
@<tt>|ST_IsClosed()|	ジオメトリが閉じているか？
@<tt>|ST_IsEmpty()|	プレースホルダー機能
@<hidx>{ST_IsSimple()}@<tt>|ST_IsSimple()|	形状が単純か？
@<hidx>{ST_IsValid()}@<tt>|ST_IsValid()|	ジオメトリが有効か？
@<tt>|ST_LatFromGeoHash()|	ジオハッシュ値から緯度を返す
@<hidx>{ST_Latitude()}@<tt>|ST_Latitude()|	ポイントの緯度を返す
@<hidx>{ST_Length()}@<tt>|ST_Length()|	LineStringの長さを返す
@<tt>|ST_LineFromText()|@<br>{}@<tt>|ST_LineStringFromText()|	WKTからLineStringを構築@<br>{}　
@<tt>|ST_LineFromWKB()|@<br>{}@<tt>|ST_LineStringFromWKB()|	WKBからLineStringを構築@<br>{}　
@<tt>|ST_LineInterpolatePoint()|@<br>{}@<tt>|ST_LineInterpolatePoints()|	線に沿った内挿点を返す@<br>{}　
@<tt>|ST_LongFromGeoHash()|	ジオハッシュ値から経度を返す
@<hidx>{ST_Longitude()}@<tt>|ST_Longitude()|	ポイントの経度を返す
@<tt>|ST_MakeEnvelope()|	2点を囲む四角形
@<tt>|ST_MLineFromText()|@<br>{}@<tt>|ST_MultiLineStringFromText()|　	WKTからMultiLineStringを構築@<br>{}　
@<tt>|ST_MLineFromWKB()|@<br>{}@<tt>|ST_MultiLineStringFromWKB()|	WKBからMultiLineStringを構築@<br>{}　
@<tt>|ST_MPointFromText()|@<br>{}@<tt>|ST_MultiPointFromText()|	WKTからMultiPointを構築@<br>{}　
@<tt>|ST_MPointFromWKB()|@<br>{}@<tt>|ST_MultiPointFromWKB()|	WKBからMultiPointを構築@<br>{}　
@<tt>|ST_MPolyFromText()|@<br>{}@<tt>|ST_MultiPolygonFromText()|	WKTからMultiPolygonを構築@<br>{}　
@<tt>|ST_MPolyFromWKB()|@<br>{}@<tt>|ST_MultiPolygonFromWKB()|	WKBからMultiPolygonを構築@<br>{}　
@<tt>|ST_NumGeometries()|	ジオメトリコレクション内のジオメトリ数を返す
@<tt>|ST_NumInteriorRing()|@<br>{}@<tt>|ST_NumInteriorRings()|	Polygonの内部リングの数を返す@<br>{}　
@<tt>|ST_NumPoints()|	LineStringのポイント数を返す
@<hidx>{ST_Overlaps()}@<tt>|ST_Overlaps()|	あるジオメトリが別のジオメトリと重なるか？
@<tt>|ST_PointAtDistance()|	線の始点から指定された距離の点を返す
@<tt>|ST_PointFromGeoHash()|	ジオハッシュ値をPoint値に変換
@<tt>|ST_PointFromText()|	WKTからポイントを構築
//}

//table{
関数名	説明
------------------------------------------------------------------------------------
@<tt>|ST_PointFromWKB()|	WKBからPointを構築
@<tt>|ST_PointN()|	LineStringからN番目の点を返す
@<tt>|ST_PolyFromText()|@<br>{}@<tt>|ST_PolygonFromText()|	WKTからPolygonを構築@<br>{}　
@<tt>|ST_PolyFromWKB()|@<br>{}@<tt>|ST_PolygonFromWKB()|	WKBからPolygonを構築@<br>{}　
@<tt>|ST_Simplify()|	単純化された形状を返す
@<hidx>{ST_SRID()}@<tt>|ST_SRID()|	ジオメトリのSRIDを返す
@<tt>|ST_StartPoint()|	LineStringの始点
@<hidx>{ST_SwapXY()}@<tt>|ST_SwapXY()|	X / Y座標を入れ替えて引数を返す
@<tt>|ST_SymDifference()|	2つのジオメトリの対称差をPoint Setとして返す
@<hidx>{ST_Touches()}@<tt>|ST_Touches()|	あるジオメトリが別のジオメトリに接するか？
@<hidx>{ST_Transform()}@<tt>|ST_Transform()|	ジオメトリの座標を変換する
@<tt>|ST_Union()|	2つのジオメトリの和集合をPoint Setとして返す　　　　　　
@<hidx>{ST_Validate()}@<tt>|ST_Validate()|　　　　　　　　　	検証済みのジオメトリを返す
@<hidx>{ST_Within()}@<tt>|ST_Within()|	あるジオメトリが別のジオメトリの中にあるか？
@<hidx>{ST_X()}@<tt>|ST_X()|	PointのX座標を返す
@<hidx>{ST_Y()}@<tt>|ST_Y()|	PointのY座標を返す
//}

==== 公式リファレンスマニュアル

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/spatial-reference-systems.html, https://dev.mysql.com/doc/refman/8.0/ja/spatial-reference-systems.html}

==== ブログ記事等

 * @<href>{https://mysqlserverteam.com/spatial-reference-systems-in-mysql-8-0/, https://mysqlserverteam.com/spatial-reference-systems-in-mysql-8-0/}
 * @<href>{https://mysqlserverteam.com/geographic-spatial-reference-systems-in-mysql-8-0/, https://mysqlserverteam.com/geographic-spatial-reference-systems-in-mysql-8-0/}
 * @<href>{https://mysqlserverteam.com/projected-spatial-reference-systems-in-mysql-8-0/, https://mysqlserverteam.com/projected-spatial-reference-systems-in-mysql-8-0/}
 * @<href>{https://mysqlserverteam.com/geography-in-mysql-8-0/, https://mysqlserverteam.com/geography-in-mysql-8-0/}
 * @<href>{https://mysqlserverteam.com/geographic-indexes-in-innodb/, https://mysqlserverteam.com/geographic-indexes-in-innodb/}
 ** @<idx>{Geographic R-treeインデックス}
 * @<href>{https://qiita.com/advent-calendar/2018/rdbms_gis, https://qiita.com/advent-calendar/2018/rdbms_gis}
 ** RDBMS-GIS(MySQL,PostgreSQLなど) Advent Calendar 2018（この中にMySQL 8.0の記事多数）
 * @<href>{https://next4us-ti.hatenablog.com/entry/2019/01/23/100858, https://next4us-ti.hatenablog.com/entry/2019/01/23/100858}
 ** 8.0.14の@<tt>|ST_Distance()|機能強化について
 * @<href>{https://atsuizo.hatenadiary.jp/entry/2018/09/01/161717, https://atsuizo.hatenadiary.jp/entry/2018/09/01/161717}
 ** @<idx>{MySQL Workbench}／結果の図表示について
 * @<href>{https://qiita.com/miyauchi/items/89ae1870c5f611b2558c, https://qiita.com/miyauchi/items/89ae1870c5f611b2558c}
 ** @<idx>{Spatial Index}使用時には@<idx>{SRID}の指定が必要
 * @<href>{https://qiita.com/miyauchi/items/893f12679cb21c12c454, https://qiita.com/miyauchi/items/893f12679cb21c12c454}
 ** 一部の関数を使った検索が遅い不具合（8.0.20にて修正済）
 * @<href>{http://sakaik.hateblo.jp/entry/20191010/dfb2deg_mysql_stored_procedure, http://sakaik.hateblo.jp/entry/20191010/dfb2deg_mysql_stored_procedure}
 ** ストアドプロシージャで度分秒変換
 * 著者ブログ
 ** @<href>{https://qiita.com/hmatsu47/items/97839fd9c3db1d2e9557, https://qiita.com/hmatsu47/items/97839fd9c3db1d2e9557}

==== 実行例

 * 距離の計測（東京駅～大阪駅）
 * ある地点が、複数の地点（政令指定都市の市役所）を結んだ領域の範囲内にあるかどうかの検索@<fn>{020PLY}

を行ってみます。

なお、いずれも@<idx>{測地系}として@<idx>{WGS84}（SRID：4326）を使用しています。
MySQL 8.0の場合、SRID：4326では地点の座標を「緯度 経度」の順に指定します@<fn>{030WGS}。

//emlist{
mysql> @<i>{SELECT ST_Distance(ST_GeomFromText('POINT(35.681236 139.767125)', 4326), ST_GeomFromTex}
@<i>{t('POINT(34.702485 135.495951)', 4326)) AS dist;}
@<b>{※東京駅と大阪駅の間の距離を計測。約403.8km。}
+-------------------+
| dist              |
+-------------------+
| 403826.6344217672 |
+-------------------+
1 row in set (0.12 sec)

mysql> @<i>{CREATE TABLE geom (id INT PRIMARY KEY AUTO_INCREMENT, t TEXT,g GEOMETRY NOT NULL SRID 4}
@<i>{326);}
Query OK, 0 rows affected (0.02 sec)

mysql> @<i>{SET @g = 'POLYGON((43.06208 141.354361,38.268195 140.869418,37.916124 139.036371,43.062}
@<i>{08 141.354361))';}
Query OK, 0 rows affected (0.00 sec)

mysql> @<i>{INSERT INTO geom SET t='札幌・仙台・新潟', g=ST_GeomFromText(@g, 4326);}
@<b>{※札幌市役所～仙台市役所～新潟市役所～札幌市役所の三角形の領域を設定。}
Query OK, 1 row affected (0.01 sec)

mysql> @<i>{SET @g = 'POLYGON((35.861793 139.64551,35.607285 140.106495,35.530807 139.702997,35.443}
@<i>{674 139.637964,35.571257 139.373427,35.861793 139.64551))';}
Query OK, 0 rows affected (0.00 sec)

mysql> @<i>{INSERT INTO geom SET t='さいたま・千葉・川崎・横浜・相模原', g=ST_GeomFromText(@g, 4326);}
@<b>{※さいたま市役所～千葉市役所～川崎市役所～横浜市役所～相模原市役所～さいたま市役所の五角形の領域を設定。}
Query OK, 1 row affected (0.01 sec)

mysql> @<i>{SET @g = 'POLYGON((34.975567 138.382677,34.710865 137.726117,35.181438 136.90642,34.975}
@<i>{567 138.382677))';}
Query OK, 0 rows affected (0.00 sec)

mysql> @<i>{INSERT INTO geom SET t='静岡・浜松・名古屋', g=ST_GeomFromText(@g, 4326);}
@<b>{※静岡市役所～浜松市役所～名古屋市役所～静岡市役所の三角形の領域を設定。}
Query OK, 1 row affected (0.01 sec)

mysql> @<i>{SET @g = 'POLYGON((35.011564 135.768149,34.573362 135.483048,34.689486 135.195739,34.69}
@<i>{3725 135.502254,35.011564 135.768149))';}
Query OK, 0 rows affected (0.00 sec)

mysql> @<i>{INSERT INTO geom SET t='京都・堺・神戸・大阪', g=ST_GeomFromText(@g, 4326);}
@<b>{※京都市役所～堺市役所～神戸市役所～大阪市役所～京都市役所の四角形の領域を設定。}
Query OK, 1 row affected (0.00 sec)

mysql> @<i>{SET @g = 'POLYGON((34.655531 133.919795,32.803216 130.707937,33.590184 130.401689,33.88}
@<i>{3498 130.875177,34.385289 132.455306,34.655531 133.919795))';}
Query OK, 0 rows affected (0.00 sec)

mysql> @<i>{INSERT INTO geom SET t='岡山・熊本・福岡・北九州・広島', g=ST_GeomFromText(@g, 4326);}
@<b>{※岡山市役所～熊本市役所～福岡市役所～北九州市役所～広島市役所～岡山市役所の五角形の領域を設定。}
Query OK, 1 row affected (0.01 sec)

mysql> @<i>{SET @p = ST_GeomFromText('POINT(40.82222 140.747352)', 4326);}
Query OK, 0 rows affected (0.00 sec)

mysql> @<i>{SELECT id, t FROM geom WHERE ST_Contains(g, @p);}
@<b>{※青森市役所の位置は1番目の領域内にある。}
+----+--------------------------+
| id | t                        |
+----+--------------------------+
|  1 | 札幌・仙台・新潟         |
+----+--------------------------+
1 row in set (0.00 sec)

mysql> @<i>{SELECT id, t FROM geom WHERE ST_Within(@p, g);}
@<b>{※ST_Within()は、結果的にST_Containsとは引数が逆になる。}
+----+--------------------------+
| id | t                        |
+----+--------------------------+
|  1 | 札幌・仙台・新潟         |
+----+--------------------------+
1 row in set (0.00 sec)

mysql> @<i>{SET @p = ST_GeomFromText('POINT(33.284461 131.490709)', 4326);}
Query OK, 0 rows affected (0.00 sec)

mysql> @<i>{SELECT id, t FROM geom WHERE ST_Contains(g, @p);}
@<b>{※別府市役所の位置は5番目の領域内にある。}
+----+-----------------------------------------------+
| id | t                                             |
+----+-----------------------------------------------+
|  5 | 岡山・熊本・福岡・北九州・広島                |
+----+-----------------------------------------------+
1 row in set (0.01 sec)

mysql> @<i>{EXPLAIN SELECT id, t FROM geom WHERE ST_Contains(g, @p);}
@<b>{※フルスキャンになっている。}
+----+-------------+-------+------------+------+---------------+------+---------+------+------
+----------+-------------+
| id | select_type | table | partitions | type | possible_keys | key  | key_len | ref  | rows
| filtered | Extra       |
+----+-------------+-------+------------+------+---------------+------+---------+------+------
+----------+-------------+
|  1 | SIMPLE      | geom  | NULL       | ALL  | NULL          | NULL | NULL    | NULL |    5
|   100.00 | Using where |
+----+-------------+-------+------------+------+---------------+------+---------+------+------
+----------+-------------+
1 row in set, 1 warning (0.00 sec)

mysql> @<i>{ALTER TABLE geom ADD SPATIAL INDEX(g);}
@<b>{※R-treeインデックスを作成。}
Query OK, 0 rows affected (0.02 sec)
Records: 0  Duplicates: 0  Warnings: 0

mysql> @<i>{EXPLAIN SELECT id, t FROM geom WHERE ST_Contains(g, @p);}
@<b>{※1行に絞り込まれた。}
+----+-------------+-------+------------+-------+---------------+------+---------+------+-----
-+----------+-------------+
| id | select_type | table | partitions | type  | possible_keys | key  | key_len | ref  | rows
 | filtered | Extra       |
+----+-------------+-------+------------+-------+---------------+------+---------+------+-----
-+----------+-------------+
|  1 | SIMPLE      | geom  | NULL       | range | g             | g    | 34      | NULL |    1
 |   100.00 | Using where |
+----+-------------+-------+------------+-------+---------------+------+---------+------+-----
-+----------+-------------+
1 row in set, 1 warning (0.00 sec)

mysql>  @<i>{SET @p = ST_GeomFromText('POINT(39.701956 141.15433)', 4326);}
Query OK, 0 rows affected (0.00 sec)

mysql> @<i>{SELECT id, t FROM geom WHERE ST_Contains(g, @p);}
@<b>{※盛岡市役所の位置は、どの領域の範囲内にもない（1番目の領域からわずかに東に外れた位置にある）。}

Empty set (0.00 sec)

mysql> @<i>{SELECT id, t FROM geom WHERE MBRContains(g, @p);}
@<b>{※MBRでは外接する矩形を境界に用いて判定するため、「範囲内」にあたる領域が広くなる。}
+----+--------------------------+
| id | t                        |
+----+--------------------------+
|  1 | 札幌・仙台・新潟         |
+----+--------------------------+
1 row in set (0.00 sec)
//}

== その他のGIS新機能

==== @<hidx>{CREATE SPATIAL REFERENCE SYSTEM}@<tt>|CREATE SPATIAL REFERENCE SYSTEM|ステートメント

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/create-spatial-reference-system.html, https://dev.mysql.com/doc/refman/8.0/ja/create-spatial-reference-system.html}
 * @<href>{https://mysqlserverteam.com/creating-your-own-spatial-reference-systems-in-mysql-8-0/, https://mysqlserverteam.com/creating-your-own-spatial-reference-systems-in-mysql-8-0/}

==== @<hidx>{DROP SPATIAL REFERENCE SYSTEM}@<tt>|DROP SPATIAL REFERENCE SYSTEM|ステートメント

 * @<href>{https://dev.mysql.com/doc/refman/8.0/ja/drop-spatial-reference-system.html, https://dev.mysql.com/doc/refman/8.0/ja/drop-spatial-reference-system.html}

===={80240601} @<hidx>{CAST()}@<tt>|CAST()|・@<hidx>{CONVERT()}@<tt>|CONVERT()|関数で地理空間タイプをサポート

 * @<href>{https://dev.mysql.com/doc/refman/8.0/en/cast-functions.html, https://dev.mysql.com/doc/refman/8.0/en/cast-functions.html}
 ** @<tt>|spatial_type|

==== @<idx>{シェープファイル}・@<idx>{GeoJSON}ファイルのインポート

 * @<href>{https://speakerdeck.com/yoshiakiyamasaki/mysql-8-dot-0deqiang-hua-saretagisji-neng-toshi-yong-shi-li-falseshao-jie-tosiepuhuairufalseinpotoshou-shun-nituite, https://speakerdeck.com/yoshiakiyamasaki/mysql-8-dot-0deqiang-hua-saretagisji-neng-toshi-yong-shi-li-falseshao-jie-tosiepuhuairufalseinpotoshou-shun-nituite}
 * @<href>{https://qiita.com/miyauchi/items/c8349e1e6339bdf26a20, https://qiita.com/miyauchi/items/c8349e1e6339bdf26a20}

====[column] @<tt>|MySQL 8.0にシェープファイルをインポートするツール・shp2mysql|

MySQL 8.0の登場以来課題だったシェープファイルのインポートですが、宮内さん作の@<idx>{shp2mysql}によってかなり楽になりました。

 * @<href>{https://qiita.com/miyauchi/items/b4e810b3becf2cf07e2f, https://qiita.com/miyauchi/items/b4e810b3becf2cf07e2f}

====[/column]

//footnote[010DPR][プレフィクスに@<tt>|ST_|・@<tt>|MBR|が付かないGIS関数]
//footnote[020PLY][この例では地点間を直線で結んでいますが、実際には都道府県・市区町村界などを領域として定義し、検索地点がどこに属するか判定する使い方のほうが一般的です。]
//footnote[030WGS][実行例の緯度・経度は、Geocoding（@<href>{https://www.geocoding.jp/, https://www.geocoding.jp/}）で調べたものです。]
