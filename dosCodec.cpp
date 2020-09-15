#include "dosCodec.h"

dosCodec::dosCodec() {

    for (unsigned i = 0x0; i < 0x80; ++i) {
        _unicodeTOdos[i] = i;
        _dosTOunicode[i] = i;
    }

    _unicodeTOdos[0x00C7] = 0x80;
    _unicodeTOdos[0x00FC] = 0x81;
    _unicodeTOdos[0x00E9] = 0x82;
    _unicodeTOdos[0x00E2] = 0x83;
    _unicodeTOdos[0x00E4] = 0x84;
    _unicodeTOdos[0x016F] = 0x85;
    _unicodeTOdos[0x0107] = 0x86;
    _unicodeTOdos[0x00E7] = 0x87;
    _unicodeTOdos[0x0142] = 0x88;
    _unicodeTOdos[0x00EB] = 0x89;
    _unicodeTOdos[0x0150] = 0x8A;
    _unicodeTOdos[0x0151] = 0x8B;
    _unicodeTOdos[0x00EE] = 0x8C;
    _unicodeTOdos[0x0179] = 0x8D;
    _unicodeTOdos[0x00C4] = 0x8E;
    _unicodeTOdos[0x0106] = 0x8F;
    _unicodeTOdos[0x00C9] = 0x90;
    _unicodeTOdos[0x0139] = 0x91;
    _unicodeTOdos[0x013A] = 0x92;
    _unicodeTOdos[0x00F4] = 0x93;
    _unicodeTOdos[0x00F6] = 0x94;
    _unicodeTOdos[0x013D] = 0x95;
    _unicodeTOdos[0x013E] = 0x96;
    _unicodeTOdos[0x015A] = 0x97;
    _unicodeTOdos[0x015B] = 0x98;
    _unicodeTOdos[0x00D6] = 0x99;
    _unicodeTOdos[0x00DC] = 0x9A;
    _unicodeTOdos[0x0164] = 0x9B;
    _unicodeTOdos[0x0165] = 0x9C;
    _unicodeTOdos[0x0141] = 0x9D;
    _unicodeTOdos[0x00D7] = 0x9E;
    _unicodeTOdos[0x010D] = 0x9F;
    _unicodeTOdos[0x00E1] = 0xA0;
    _unicodeTOdos[0x00ED] = 0xA1;
    _unicodeTOdos[0x00F3] = 0xA2;
    _unicodeTOdos[0x00FA] = 0xA3;
    _unicodeTOdos[0x0104] = 0xA4;
    _unicodeTOdos[0x0105] = 0xA5;
    _unicodeTOdos[0x017D] = 0xA6;
    _unicodeTOdos[0x017E] = 0xA7;
    _unicodeTOdos[0x0118] = 0xA8;
    _unicodeTOdos[0x0119] = 0xA9;
    _unicodeTOdos[0x00AC] = 0xAA;
    _unicodeTOdos[0x017A] = 0xAB;
    _unicodeTOdos[0x010C] = 0xAC;
    _unicodeTOdos[0x015F] = 0xAD;
    _unicodeTOdos[0x00AB] = 0xAE;
    _unicodeTOdos[0x00BB] = 0xAF;
    _unicodeTOdos[0x2591] = 0xB0;
    _unicodeTOdos[0x2592] = 0xB1;
    _unicodeTOdos[0x2593] = 0xB2;
    _unicodeTOdos[0x2502] = 0xB3;
    _unicodeTOdos[0x2524] = 0xB4;
    _unicodeTOdos[0x00C1] = 0xB5;
    _unicodeTOdos[0x00C2] = 0xB6;
    _unicodeTOdos[0x011A] = 0xB7;
    _unicodeTOdos[0x015E] = 0xB8;
    _unicodeTOdos[0x2563] = 0xB9;
    _unicodeTOdos[0x2551] = 0xBA;
    _unicodeTOdos[0x2557] = 0xBB;
    _unicodeTOdos[0x255D] = 0xBC;
    _unicodeTOdos[0x017B] = 0xBD;
    _unicodeTOdos[0x017C] = 0xBE;
    _unicodeTOdos[0x2510] = 0xBF;
    _unicodeTOdos[0x2514] = 0xC0;
    _unicodeTOdos[0x2534] = 0xC1;
    _unicodeTOdos[0x252C] = 0xC2;
    _unicodeTOdos[0x251C] = 0xC3;
    _unicodeTOdos[0x2500] = 0xC4;
    _unicodeTOdos[0x253C] = 0xC5;
    _unicodeTOdos[0x0102] = 0xC6;
    _unicodeTOdos[0x0103] = 0xC7;
    _unicodeTOdos[0x255A] = 0xC8;
    _unicodeTOdos[0x2554] = 0xC9;
    _unicodeTOdos[0x2569] = 0xCA;
    _unicodeTOdos[0x2566] = 0xCB;
    _unicodeTOdos[0x2560] = 0xCC;
    _unicodeTOdos[0x2550] = 0xCD;
    _unicodeTOdos[0x256C] = 0xCE;
    _unicodeTOdos[0x00A4] = 0xCF;
    _unicodeTOdos[0x0111] = 0xD0;
    _unicodeTOdos[0x0110] = 0xD1;
    _unicodeTOdos[0x010E] = 0xD2;
    _unicodeTOdos[0x00CB] = 0xD3;
    _unicodeTOdos[0x010F] = 0xD4;
    _unicodeTOdos[0x0147] = 0xD5;
    _unicodeTOdos[0x00CD] = 0xD6;
    _unicodeTOdos[0x00CE] = 0xD7;
    _unicodeTOdos[0x011B] = 0xD8;
    _unicodeTOdos[0x2518] = 0xD9;
    _unicodeTOdos[0x250C] = 0xDA;
    _unicodeTOdos[0x2588] = 0xDB;
    _unicodeTOdos[0x2584] = 0xDC;
    _unicodeTOdos[0x0162] = 0xDD;
    _unicodeTOdos[0x016E] = 0xDE;
    _unicodeTOdos[0x2580] = 0xDF;
    _unicodeTOdos[0x00D3] = 0xE0;
    _unicodeTOdos[0x00DF] = 0xE1;
    _unicodeTOdos[0x00D4] = 0xE2;
    _unicodeTOdos[0x0143] = 0xE3;
    _unicodeTOdos[0x0144] = 0xE4;
    _unicodeTOdos[0x0148] = 0xE5;
    _unicodeTOdos[0x0160] = 0xE6;
    _unicodeTOdos[0x0161] = 0xE7;
    _unicodeTOdos[0x0154] = 0xE8;
    _unicodeTOdos[0x00DA] = 0xE9;
    _unicodeTOdos[0x0155] = 0xEA;
    _unicodeTOdos[0x0170] = 0xEB;
    _unicodeTOdos[0x00FD] = 0xEC;
    _unicodeTOdos[0x00DD] = 0xED;
    _unicodeTOdos[0x0163] = 0xEE;
    _unicodeTOdos[0x00B4] = 0xEF;
    _unicodeTOdos[0x00AD] = 0xF0;
    _unicodeTOdos[0x02DD] = 0xF1;
    _unicodeTOdos[0x02DB] = 0xF2;
    _unicodeTOdos[0x02C7] = 0xF3;
    _unicodeTOdos[0x02D8] = 0xF4;
    _unicodeTOdos[0x00A7] = 0xF5;
    _unicodeTOdos[0x00F7] = 0xF6;
    _unicodeTOdos[0x00B8] = 0xF7;
    _unicodeTOdos[0x00B0] = 0xF8;
    _unicodeTOdos[0x00A8] = 0xF9;
    _unicodeTOdos[0x02D9] = 0xFA;
    _unicodeTOdos[0x0171] = 0xFB;
    _unicodeTOdos[0x0158] = 0xFC;
    _unicodeTOdos[0x0159] = 0xFD;
    _unicodeTOdos[0x25A0] = 0xFE;
    _unicodeTOdos[0x00A0] = 0xFF;

    _dosTOunicode[0x80] = 0x00C7;
    _dosTOunicode[0x81] = 0x00FC;
    _dosTOunicode[0x82] = 0x00E9;
    _dosTOunicode[0x83] = 0x00E2;
    _dosTOunicode[0x84] = 0x00E4;
    _dosTOunicode[0x85] = 0x016F;
    _dosTOunicode[0x86] = 0x0107;
    _dosTOunicode[0x87] = 0x00E7;
    _dosTOunicode[0x88] = 0x0142;
    _dosTOunicode[0x89] = 0x00EB;
    _dosTOunicode[0x8A] = 0x0150;
    _dosTOunicode[0x8B] = 0x0151;
    _dosTOunicode[0x8C] = 0x00EE;
    _dosTOunicode[0x8D] = 0x0179;
    _dosTOunicode[0x8E] = 0x00C4;
    _dosTOunicode[0x8F] = 0x0106;
    _dosTOunicode[0x90] = 0x00C9;
    _dosTOunicode[0x91] = 0x0139;
    _dosTOunicode[0x92] = 0x013A;
    _dosTOunicode[0x93] = 0x00F4;
    _dosTOunicode[0x94] = 0x00F6;
    _dosTOunicode[0x95] = 0x013D;
    _dosTOunicode[0x96] = 0x013E;
    _dosTOunicode[0x97] = 0x015A;
    _dosTOunicode[0x98] = 0x015B;
    _dosTOunicode[0x99] = 0x00D6;
    _dosTOunicode[0x9A] = 0x00DC;
    _dosTOunicode[0x9B] = 0x0164;
    _dosTOunicode[0x9C] = 0x0165;
    _dosTOunicode[0x9D] = 0x0141;
    _dosTOunicode[0x9E] = 0x00D7;
    _dosTOunicode[0x9F] = 0x010D;
    _dosTOunicode[0xA0] = 0x00E1;
    _dosTOunicode[0xA1] = 0x00ED;
    _dosTOunicode[0xA2] = 0x00F3;
    _dosTOunicode[0xA3] = 0x00FA;
    _dosTOunicode[0xA4] = 0x0104;
    _dosTOunicode[0xA5] = 0x0105;
    _dosTOunicode[0xA6] = 0x017D;
    _dosTOunicode[0xA7] = 0x017E;
    _dosTOunicode[0xA8] = 0x0118;
    _dosTOunicode[0xA9] = 0x0119;
    _dosTOunicode[0xAA] = 0x00AC;
    _dosTOunicode[0xAB] = 0x017A;
    _dosTOunicode[0xAC] = 0x010C;
    _dosTOunicode[0xAD] = 0x015F;
    _dosTOunicode[0xAE] = 0x00AB;
    _dosTOunicode[0xAF] = 0x00BB;
    _dosTOunicode[0xB0] = 0x2591;
    _dosTOunicode[0xB1] = 0x2592;
    _dosTOunicode[0xB2] = 0x2593;
    _dosTOunicode[0xB3] = 0x2502;
    _dosTOunicode[0xB4] = 0x2524;
    _dosTOunicode[0xB5] = 0x00C1;
    _dosTOunicode[0xB6] = 0x00C2;
    _dosTOunicode[0xB7] = 0x011A;
    _dosTOunicode[0xB8] = 0x015E;
    _dosTOunicode[0xB9] = 0x2563;
    _dosTOunicode[0xBA] = 0x2551;
    _dosTOunicode[0xBB] = 0x2557;
    _dosTOunicode[0xBC] = 0x255D;
    _dosTOunicode[0xBD] = 0x017B;
    _dosTOunicode[0xBE] = 0x017C;
    _dosTOunicode[0xBF] = 0x2510;
    _dosTOunicode[0xC0] = 0x2514;
    _dosTOunicode[0xC1] = 0x2534;
    _dosTOunicode[0xC2] = 0x252C;
    _dosTOunicode[0xC3] = 0x251C;
    _dosTOunicode[0xC4] = 0x2500;
    _dosTOunicode[0xC5] = 0x253C;
    _dosTOunicode[0xC6] = 0x0102;
    _dosTOunicode[0xC7] = 0x0103;
    _dosTOunicode[0xC8] = 0x255A;
    _dosTOunicode[0xC9] = 0x2554;
    _dosTOunicode[0xCA] = 0x2569;
    _dosTOunicode[0xCB] = 0x2566;
    _dosTOunicode[0xCC] = 0x2560;
    _dosTOunicode[0xCD] = 0x2550;
    _dosTOunicode[0xCE] = 0x256C;
    _dosTOunicode[0xCF] = 0x00A4;
    _dosTOunicode[0xD0] = 0x0111;
    _dosTOunicode[0xD1] = 0x0110;
    _dosTOunicode[0xD2] = 0x010E;
    _dosTOunicode[0xD3] = 0x00CB;
    _dosTOunicode[0xD4] = 0x010F;
    _dosTOunicode[0xD5] = 0x0147;
    _dosTOunicode[0xD6] = 0x00CD;
    _dosTOunicode[0xD7] = 0x00CE;
    _dosTOunicode[0xD8] = 0x011B;
    _dosTOunicode[0xD9] = 0x2518;
    _dosTOunicode[0xDA] = 0x250C;
    _dosTOunicode[0xDB] = 0x2588;
    _dosTOunicode[0xDC] = 0x2584;
    _dosTOunicode[0xDD] = 0x0162;
    _dosTOunicode[0xDE] = 0x016E;
    _dosTOunicode[0xDF] = 0x2580;
    _dosTOunicode[0xE0] = 0x00D3;
    _dosTOunicode[0xE1] = 0x00DF;
    _dosTOunicode[0xE2] = 0x00D4;
    _dosTOunicode[0xE3] = 0x0143;
    _dosTOunicode[0xE4] = 0x0144;
    _dosTOunicode[0xE5] = 0x0148;
    _dosTOunicode[0xE6] = 0x0160;
    _dosTOunicode[0xE7] = 0x0161;
    _dosTOunicode[0xE8] = 0x0154;
    _dosTOunicode[0xE9] = 0x00DA;
    _dosTOunicode[0xEA] = 0x0155;
    _dosTOunicode[0xEB] = 0x0170;
    _dosTOunicode[0xEC] = 0x00FD;
    _dosTOunicode[0xED] = 0x00DD;
    _dosTOunicode[0xEE] = 0x0163;
    _dosTOunicode[0xEF] = 0x00B4;
    _dosTOunicode[0xF0] = 0x00AD;
    _dosTOunicode[0xF1] = 0x02DD;
    _dosTOunicode[0xF2] = 0x02DB;
    _dosTOunicode[0xF3] = 0x02C7;
    _dosTOunicode[0xF4] = 0x02D8;
    _dosTOunicode[0xF5] = 0x00A7;
    _dosTOunicode[0xF6] = 0x00F7;
    _dosTOunicode[0xF7] = 0x00B8;
    _dosTOunicode[0xF8] = 0x00B0;
    _dosTOunicode[0xF9] = 0x00A8;
    _dosTOunicode[0xFA] = 0x02D9;
    _dosTOunicode[0xFB] = 0x0171;
    _dosTOunicode[0xFC] = 0x0158;
    _dosTOunicode[0xFD] = 0x0159;
    _dosTOunicode[0xFE] = 0x25A0;
    _dosTOunicode[0xFF] = 0x00A0;

}

QByteArray dosCodec::name() const {
    return QByteArray("IBM852", 6);
}

QList<QByteArray> dosCodec::aliases() const {
    QList<QByteArray> result;
    result.append(QByteArray("CP852", 5));
    result.append(QByteArray("Code page 852", 13));
    result.append(QByteArray("CP 852", 6));
    result.append(QByteArray("IBM 852", 7));
    result.append(QByteArray("OEM 852", 7));
    result.append(QByteArray("MS-DOS Latin 2", 14));
    result.append(QByteArray("PC Latin 2", 10));
    result.append(QByteArray("Slavic (Latin II)", 17));
    return result;
}

int dosCodec::mibEnum() const {
    return 2010;
}

QByteArray dosCodec::convertFromUnicode(const QChar *input, int number, QTextCodec::ConverterState *state) const {
    QByteArray result;
    result.reserve(number);
    for (int i = 0; i < number; ++i) {
        const QChar c = input[i];
        if (_unicodeTOdos.find(c) != _unicodeTOdos.end())
            result.append(_unicodeTOdos[c]);
        else
            result.append('?');
    }

    if (state != nullptr) {
        state -> invalidChars = 0;
        state -> remainingChars = 0;
    }

    return result;
}

QString dosCodec::convertToUnicode(const char *chars, int len, QTextCodec::ConverterState *state) const {
    QString result;
    result.reserve(len);
    for (int i = 0; i < len; ++i) {
        const char c = chars[i];
        if (_dosTOunicode.find(c) != _dosTOunicode.end())
            result.append(_dosTOunicode[c]);
        else
            result.append('?');
    }

    if (state != nullptr) {
        state -> invalidChars = 0;
        state -> remainingChars = 0;
    }

    return result;
}
