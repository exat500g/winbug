#pragma once

#include "propertytreenode.h"
#include "global/type.h"

class PlannerPosPropertyNode : public PropertyTreeNode
{
    Q_OBJECT
    static const int pnum=3;
    PropertyTreeNode *   nodes[pnum];

    virtual void    updateValue() Q_DECL_OVERRIDE;
    virtual void    populateProperties() Q_DECL_OVERRIDE;
public:
    PlannerPosPropertyNode(QObject *parent = 0);
    QString     stringValue();
    NodeType    type()  const;
private slots:
    void subValueUpdate(const QVariant &Value);
};
