// delegeler.h - GPA ve Bolum icin ozel delege siniflari
#ifndef DELEGELER_H
#define DELEGELER_H

#include <QStyledItemDelegate>
#include <QDoubleSpinBox>
#include <QComboBox>

class GpaDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    using QStyledItemDelegate::QStyledItemDelegate;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override
    {
        Q_UNUSED(option);
        Q_UNUSED(index);
        auto *editor = new QDoubleSpinBox(parent);
        editor->setRange(0.00, 4.00);
        editor->setDecimals(2);
        editor->setSingleStep(0.05);
        return editor;
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const override
    {
        double deger = index.model()->data(index, Qt::EditRole).toDouble();
        auto *spinBox = qobject_cast<QDoubleSpinBox *>(editor);
        if (spinBox)
            spinBox->setValue(deger);
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override
    {
        auto *spinBox = qobject_cast<QDoubleSpinBox *>(editor);
        if (spinBox)
            model->setData(index, spinBox->value(), Qt::EditRole);
    }
};

class BolumDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    using QStyledItemDelegate::QStyledItemDelegate;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override
    {
        Q_UNUSED(option);
        Q_UNUSED(index);
        auto *editor = new QComboBox(parent);
        editor->addItems({
            QString::fromUtf8("Bilgisayar Bilimleri"),
            QString::fromUtf8("Yazılım Mühendisliği"),
            QString::fromUtf8("Elektrik-Elektronik"),
            QString::fromUtf8("Makine Mühendisliği"),
            QString::fromUtf8("İnşaat Mühendisliği")
        });
        return editor;
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const override
    {
        QString deger = index.model()->data(index, Qt::EditRole).toString();
        auto *comboBox = qobject_cast<QComboBox *>(editor);
        if (comboBox) {
            int idx = comboBox->findText(deger);
            if (idx >= 0)
                comboBox->setCurrentIndex(idx);
        }
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override
    {
        auto *comboBox = qobject_cast<QComboBox *>(editor);
        if (comboBox)
            model->setData(index, comboBox->currentText(), Qt::EditRole);
    }
};

#endif // DELEGELER_H
