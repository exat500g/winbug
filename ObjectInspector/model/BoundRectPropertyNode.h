#pragma once

#include "propertytreenode.h"
#include "global/type.h"

class BoundRectPropertyNode : public PropertyTreeNode
{
Q_OBJECT

    PropertyTreeNode *   nodes[6];

    virtual void    updateValue() Q_DECL_OVERRIDE;
    virtual void    populateProperties() Q_DECL_OVERRIDE;
public:
    BoundRectPropertyNode(QObject *parent = 0);
    QString     stringValue();
    NodeType    type()  const;
private slots:
    void subValueUpdate(const QVariant &Value);
};
