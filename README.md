Neof5client
===========

プログラミングコンテスト「ネオ富豪」クライアントプログラム

Copyright (C) 2014 W.Maeda <w.maeda@neogenia.co.jp>

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

概要
----

Neof5client は ログラミングコンテスト「ネオ富豪」のためのクライアントプログラムです。
ネオ富豪とは、トランプゲーム大富豪をコンピュータプログラムで対戦する大会です。

詳細は公式ホームページを参照のこと。
http://neof5.neogenia.co.jp/

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

Androidアプリについての説明
---------------------------

NeoftClient/ 以下に置いてあるのは、Androidアプリのソースコードです。
Eclipse でプロジェクトインポートしてビルドすることができます。
ただし、あらかじめ C++ コードを Android NDK でビルドしておく必要があります。

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

C++ コードについての説明
------------------------

C++コードは Neof5Client/jni 以下にあります。
Neof5Client/jni/neof5 が本体です。

### jniディレクトリと内容物の説明

libwebsockets-1.23/ 以下に置いてあるものは、
 https://github.com/warmcat/libwebsockets/releases
よりダウンロードしてきたソースツリーをそのまま置いてあります。

ただし、SSL サポートは必要ないので、CMakeLists.txt の
WITHSSL オプションを OFF に変更しています。
それ以外は何も変更していません。

lwsconfig.android/ 以下に置いてあるものは、Android NDK で
libwebsockets をビルドするために必要な config.h を置いてあります。
このディレクトリは、Android.mk でインクルードパスに設定しています。
config.h は lws_config.h を参考に適当に作成しました。
最小限の設定なので、内容はほとんどありません。

neof5/ 以下に置いてあるものは、ネオ富豪クライアントのソースコードです。


### Android版のビルド方法

Android NDK でビルドします。
アプリのルートディレクトリに cd し、ndk-build するだけです。
なお、Application.mk ではツールチェインに clang を使うように
指定しています。（g++ではビルドが通らないため）


### libwebsocketsのビルド方法

基本的には libwebsocket の README.build に書いてあるとおり。
    $ cd libwebsocket-1.23
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make


### neof5のビルド方法

先に libwebsockets をビルドしておく必要があります。

ビルド方法は単純にmakeするだけです。
    $ make
clang を使う場合は、
    $ make CC=clang
として下さい。
実行ファイルはカレントディレクトリに作成されます。
clientmain というファイルがそうです。

実行するは、ルールと接続先情報を引数で与える必要があります。
詳細は、何も引数を指定せずに実行すると Usage メッセージを出力して
終了するので、それを見て下さい。


