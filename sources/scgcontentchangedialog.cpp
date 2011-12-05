/*
-----------------------------------------------------------------------------
This source file is part of OSTIS (Open Semantic Technology for Intelligent Systems)
For the latest info, see http://www.ostis.net

Copyright (c) 2010 OSTIS

OSTIS is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OSTIS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with OSTIS.  If not, see <http://www.gnu.org/licenses/>.
-----------------------------------------------------------------------------
*/

#include "scgcontentchangedialog.h"

#include "scgcontentfactory.h"
#include "scgcontentdialog.h"
#include "scgnode.h"

#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

SCgContentChangeDialog::SCgContentChangeDialog(SCgNode *node, QWidget *parent) :
    QDialog(parent),
    mNode(node),
    mDialog(0),
    mCenterLayout(0)
{

    QHBoxLayout *topLayout = new QHBoxLayout;

    QComboBox *comboFormats = new QComboBox(this);
    connect(comboFormats, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeFromat(QString)));
    topLayout->addWidget(comboFormats, 0, Qt::AlignLeft);

    QHBoxLayout *bottomLayout = new QHBoxLayout;

    buttonApply = new QPushButton(tr("Apply"), this);
    connect(buttonApply, SIGNAL(clicked()), this, SLOT(apply()));
    bottomLayout->addWidget(buttonApply, 1, Qt::AlignRight);

    mCenterLayout = new QHBoxLayout;

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(mCenterLayout);
    mainLayout->addLayout(bottomLayout);

    setLayout(mainLayout);

    QList<QString> list = SCgContentFactory::supportedFormats();
    comboFormats->addItems(list);

    int curIndex = list.indexOf(mNode->contentFormat());
    if (curIndex > 0)
    	comboFormats->setCurrentIndex(curIndex);
}

SCgContentChangeDialog::~SCgContentChangeDialog()
{
    if (mDialog != 0)
    {
        delete mDialog;
        mDialog = 0;
    }
}

void SCgContentChangeDialog::changeFromat(QString format)
{
    if (mDialog)
    {
        delete mDialog;
        mDialog = 0;
    }

    mDialog = SCgContentFactory::createDialog(format, mNode);
    mDialog->setParent(this);
    if (format == "numeric")
    {
        buttonApply->setEnabled(false);
        connect(mDialog, SIGNAL(enableApplyButton(QValidator::State)), this, SLOT(slotEnableApplyButton(QValidator::State)));
    }
    else
    {
        buttonApply->setEnabled(true);
    }
    mCenterLayout->addWidget(mDialog);
    setFixedSize(mDialog->sizeHint() + QSize(40, 70));//resize(mDialog->sizeHint());
}

void SCgContentChangeDialog::apply()
{
    Q_ASSERT(mDialog);
    mDialog->apply();
    done(QDialog::Accepted);
}

void SCgContentChangeDialog::contentInfo(SCgContent::ContInfo &info)
{
    return mDialog->contentInfo(info);
}

void SCgContentChangeDialog::slotEnableApplyButton(QValidator::State state)
{
    if (state == QValidator::Acceptable)
    {
        buttonApply->setEnabled(true);
    }
    else
    {
        buttonApply->setEnabled(false);
    }
}
