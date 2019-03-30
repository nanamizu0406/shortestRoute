# RouteSearch
## A* algorithm、dijkstra algorithmを使った最短経路探索
A*algorithmとdijkstra algorithmを使って最短経路を探索します。  
移動にかかるコストは移動距離と地形情報から算出しています。  
算出後、黄色い線によって経路を表示します。  

## dijkstra algorithmを使った自動地形生成
dijkstra algorithmを使って島のような地形を自動生成します。  
何度か更新を行うことにより、島のような形になります。
	
## 操作方法
a…４方向移動可能なA* algorit  
b…８方向移動可能なA* algorit   
c…４方向移動可能なdijkstra algorit  
d…８方向移動可能なdijkstra algorith   
t…diamond squre algorithmによる自動地形生成、更  
q…終了  
左クリックしながらのマウスドラッグで壁の作成

## 動作環境
glutを使用して描画を行います。glutや必要なソフトをインストールしてください。
私はlinuxを使用しているのでlinuxに関するインストール方法を掲載します。
~~~
(Vine)
$ sudo apt-get install freeglut freeglut-devel
(Debian, Ubuntu)
$ sudo apt-get install freeglut3 freeglut3-dev
(RedHat, Fedra)
$ sudo yum install freeglut
$ sudo yum install freeglut-devel
~~~
それでも動かない場合は以下をインストールしてください。
~~~
sudo apt-get install libxmu-dev libxi-dev
~~~

## 実行方法
リポジトリ内にあるshellscriptフィルによってコンパイルします。  
gccglに実行権限を与えてください。その後scriptファイルを使ってコンパイルしてください。  
~~~
$./gccgl main.cpp field.cpp search.cpp tools.cpp diamond.cpp
$./a.out
~~~
