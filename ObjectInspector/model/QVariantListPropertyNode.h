#pragma once

#include "propertytreenode.h"

class QVariantListPropertyNode : public PropertyTreeNode
{
    Q_OBJECT

    QVector<PropertyTreeNode*>   listNode;

    virtual void    updateValue() Q_DECL_OVERRIDE;
    virtual void    populateProperties() Q_DECL_OVERRIDE;
public:
    QVariantListPropertyNode(QObject *parent = 0);
    QString     stringValue();
    NodeType    type()  const;
private slots:
    void subValueUpdate(const QVariant &Value);
};






