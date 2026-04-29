# CHIP-8 Emulator

C言語と SDL2 で実装した CHIP-8 エミュレータです。

## 概要

CHIP-8 は 1970 年代に設計された仮想マシンで、多くのシンプルなゲームが動作します。このプロジェクトでは fetch → decode → execute サイクルを C で実装し、SDL2 を使ってディスプレイを描画しています。

## 仕様

| 項目 | 内容 |
|------|------|
| メモリ | 4096 バイト |
| 汎用レジスタ | V0〜VF (8bit × 16本) |
| アドレスレジスタ | I (16bit) |
| プログラムカウンタ | PC (16bit、初期値 0x200) |
| スタック | 16段 |
| ディスプレイ | 64×32 モノクロ |
| スケール | 10倍 (640×320 ウィンドウ) |

## 実装済み命令

| オペコード | ニーモニック | 説明 |
|-----------|-------------|------|
| 00E0 | CLS | 画面クリア |
| 00EE | RET | サブルーチンから戻る |
| 1nnn | JP addr | アドレス nnn へジャンプ |
| 2nnn | CALL addr | サブルーチン呼び出し |
| 3xkk | SE Vx, byte | Vx == kk なら次命令スキップ |
| 4xkk | SNE Vx, byte | Vx != kk なら次命令スキップ |
| 5xy0 | SE Vx, Vy | Vx == Vy なら次命令スキップ |
| 6xkk | LD Vx, byte | Vx = kk |
| 7xkk | ADD Vx, byte | Vx += kk |
| 8xy0〜8xyE | 算術/論理演算 | LD/OR/AND/XOR/ADD/SUB/SHR/SUBN/SHL |
| 9xy0 | SNE Vx, Vy | Vx != Vy なら次命令スキップ |
| Annn | LD I, addr | I = nnn |
| Bnnn | JP V0, addr | PC = V0 + nnn |
| Cxkk | RND Vx, byte | Vx = rand() & kk |
| Dxyn | DRW Vx, Vy, n | スプライト描画 (XOR、衝突検出あり) |

## ビルド

SDL2 が必要です。

```bash
# SDL2 インストール (Ubuntu/Debian)
sudo apt install libsdl2-dev

# ビルド
make

# クリーン
make clean
```

## 使い方

```bash
./chip8 <ROM ファイル>
```

例:
```bash
./chip8 cassette/chip8-roms/games/Pong\ \[Paul\ Vervalin\,\ 1990\].ch8
```

ウィンドウを閉じると終了します。

## ディレクトリ構成

```
chip8/
├── main.c          # エントリポイント、SDL2 ループ
├── chip8.c         # fetch / decode / execute / store 実装
├── chip8.h         # Chip8 構造体・プロトタイプ宣言
├── Makefile
└── cassette/
    └── chip8-roms/ # サンプル ROM (games / demos / programs / hires)
```

## 参考

- [CHIP-8 Technical Reference (Cowgod)](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
