/**
 * Ozel Delegeler - GpaDelegate ve BolumDelegate
 *
 * QStyledItemDelegate'den tureyen ozel editor widget'lari.
 * GPA icin QDoubleSpinBox, bolum icin QComboBox kullanir.
 *
 * Bolum: 06 - Qt Model/View Mimarisi
 * Unite: 3 - Veri Duzenleme ve Delegeler
 */

#ifndef DELEGELER_H
#define DELEGELER_H

#include <QStyledItemDelegate>
#include <QDoubleSpinBox>
#include <QComboBox>

// GPA duzenlemesi icin ozel delege
class GpaDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    using QStyledItemDelegate::QStyledItemDelegate;

    QWidget* createEditor(QWidget *parent,
            const QStyleOptionViewItem&,
            const QModelIndex&) const override {
        auto *editor = new QDoubleSpinBox(parent);
        editor->setMinimum(0.00);
        editor->setMaximum(4.00);
        editor->setSingleStep(0.05);
        editor->setDecimals(2);
        return editor;
    }

    void setEditorData(QWidget *editor,
            const QModelIndex& index) const override {
        double deger = index.model()->data(index, Qt::EditRole).toDouble();
        auto *spinBox = qobject_cast<QDoubleSpinBox*>(editor);
        if (spinBox)
            spinBox->setValue(deger);
    }

    void setModelData(QWidget *editor,
            QAbstractItemModel *model,
            const QModelIndex& index) const override {
        auto *spinBox = qobject_cast<QDoubleSpinBox*>(editor);
        if (spinBox) {
            spinBox->interpretText();
            model->setData(index, spinBox->value(), Qt::EditRole);
        }
    }

    void updateEditorGeometry(QWidget *editor,
            const QStyleOptionViewItem& option,
            const QModelIndex&) const override {
        editor->setGeometry(option.rect);
    }
};

// Bolum secimi icin ozel delege
class BolumDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    using QStyledItemDelegate::QStyledItemDelegate;

    QWidget* createEditor(QWidget *parent,
            const QStyleOptionViewItem&,
            const QModelIndex&) const override {
        auto *combo = new QComboBox(parent);
        combo->addItems({
            "Bilgisayar Bilimleri",
            "Yazilim Muhendisligi",
            "Elektrik-Elektronik",
            "Makine Muhendisligi",
            "Insaat Muhendisligi"
        });
        return combo;
    }

    void setEditorData(QWidget *editor,
            const QModelIndex& index) const override {
        QString deger = index.model()->data(index, Qt::EditRole).toString();
        auto *combo = qobject_cast<QComboBox*>(editor);
        if (combo) {
            int idx = combo->findText(deger);
            if (idx >= 0)
                combo->setCurrentIndex(idx);
        }
    }

    void setModelData(QWidget *editor,
            QAbstractItemModel *model,
            const QModelIndex& index) const override {
        auto *combo = qobject_cast<QComboBox*>(editor);
        if (combo)
            model->setData(index, combo->currentText(), Qt::EditRole);
    }

    void updateEditorGeometry(QWidget *editor,
            const QStyleOptionViewItem& option,
            const QModelIndex&) const override {
        editor->setGeometry(option.rect);
    }
};

#endif // DELEGELER_H
