#pragma GCC optimize("O0")
#include <errno.h>
#include <iconv.h>
#include <stdlib.h>
#include <string.h>
#include "../include/KanjiHelper.h"
#pragma GCC diagnostic ignored "-Wunused-variable"
/**
 * @file KanjiHelper.cpp
 * @brief フォントテーブルデータ（KanjiHelper.hで、シンボルによりインクルードされる）から、特定のフォントを検索する
 * 処理を行う、KanjiHelperクラスを定義する。KanjiHelperクラスはすべてstaticメソッドで、インスタンス化することはできない。
 */

/// @brief フォントテーブルからUTF-8を使って特定のフォントをバイナリサーチで探し出す際のサブ関数。<br/>
/// フォントへのポインタを受け取り、大小を比較する。第一円数の方が
/// @param a 比較するUTF-8コード
/// @param b 比較するUTF-8コード
/// @return a が b より大きければ1、小さければ-1、等しければ0を返す。
int KanjiHelper::compareUnicode(const void *a, const void *b)
{
	uint32_t unicodeA = ((KanjiFont *)a)->Unicode;
	uint32_t unicodeB = ((KanjiFont *)b)->Unicode;
	return (unicodeA > unicodeB) - (unicodeA < unicodeB);
}
/// @brief フォントテーブルからUTF-8を使って特定のフォントをバイナリサーチで探し出す。<br/>
/// 漢字テーブルにはUTF-8がソートされて格納されている。<br/>
/// UTF-8は非連続、バイト長も１～４とバラバラなので、計算でピンポイントに見つけ出すのは難しい。そこで漢字テーブルからバイナリサーチで探し出す。
/// 検索回数は、6000文字でも13回程度なので良しとする。
/// @param array 検索対象の漢字テーブル
/// @param size 検索対象の漢字テーブルの要素数
/// @param unicode 検索するUTF-8コード
/// @return 検索された漢字データのポインタ。見つからなかったときはNULL。
const KanjiFont* KanjiHelper::binarySearchUnicode(const KanjiFont array[], size_t size, uint32_t unicode)
{
	size_t left = 0;
	size_t right = size - 1;

	while (left <= right) {
		size_t middle = left + (right - left) / 2;
		if (array[middle].Unicode == unicode) {
			return (KanjiFont *)&array[middle];
		} else if (array[middle].Unicode < unicode) {
			left = middle + 1;
		} else {
			right = middle - 1;
		}
	}
	return NULL;
}
/// @brief フォントテーブルからASCII文字を検索する。<br/>
/// ASCII文字は、0x00を除く255が必ずあるので、検索ではなく計算でピンポイントで返す
/// @param array 検索対象のASCIIテーブル
/// @param asciicode 検索対象のASCIIコード
/// @return 検索されたASCIIデータのポインタ
const AsciiFont* KanjiHelper::SearchAscii(const AsciiFont array[],uint8_t asciicode)
{
	return (AsciiFont *)&array[asciicode-1];
	
}

/// @brief フォントテーブルからUTF-8を使って特定のフォントをバイナリサーチで探し出す。<br/>
/// 漢字テーブルにはUTF-8がソートされてはいっている。
/// UTF-8は非連続、バイト長も１～４とバラバラなので、計算でピンポイントに見つけ出すのは難しい。そこで漢字テーブルからバイナリサーチで探し出す。
/// 検索回数は、6000文字でも13回程度なので良しとする。
/// @param targetCodeUTF 検索対象のUTF-8コード
/// @return　検索された漢字データのポインタ。見つからなかったときはNULL。
const KanjiFont* KanjiHelper::FindKanji(uint32_t targetCodeUTF)
{
//	uint32_t targetCodeUTF = 0x0000C2A8;  // 探すUnicode値
	size_t arraySize = sizeof(KFont) / sizeof(KFont[0]);

	const KanjiFont *result = binarySearchUnicode(KFont, arraySize, targetCodeUTF);
	return result;
}
/// @brief フォントテーブルからASCII文字を検索する。<br/>
/// ASCII文字は、0x00を除く255が必ずあるので、検索ではなく計算でピンポイントで返す
/// @param targetCodeAscii 検索対象のASCIIコード
/// @return 検索されたASCIIデータのポインタ
const AsciiFont *KanjiHelper::FindAscii(uint8_t targetCodeAscii)
{
	//	uint32_t targetCodeUTF = 0x0000C2A8;  // 探すUnicode値
	size_t arraySize = sizeof(AFont) / sizeof(AFont[0]);

	const AsciiFont *result = SearchAscii(AFont, targetCodeAscii);
	return result;
}
