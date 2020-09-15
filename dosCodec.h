#ifndef DOSCODEC_H
#define DOSCODEC_H

#include <QTextCodec>
#include <QMap>

class dosCodec: public QTextCodec {

    QMap<QChar, char> _unicodeTOdos;
    QMap<char, QChar> _dosTOunicode;

public:
    dosCodec();
    QByteArray name() const;
    QList<QByteArray> aliases() const;
    int mibEnum() const;
    QByteArray convertFromUnicode(const QChar *input, int number, QTextCodec::ConverterState *state) const;
    QString convertToUnicode(const char *chars, int len, QTextCodec::ConverterState *state) const;

};

#endif // DOSCODEC_H
