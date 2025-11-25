// Copyright (c) 2011-2020 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef QUERCUS_QT_QUERCUSADDRESSVALIDATOR_H
#define QUERCUS_QT_QUERCUSADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class QuercusAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit QuercusAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

/** Quercus address widget validator, checks for a valid quercus address.
 */
class QuercusAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit QuercusAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

#endif // QUERCUS_QT_QUERCUSADDRESSVALIDATOR_H
