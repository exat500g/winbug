#include "PlannerPosPropertyNode.h"
#include "global/type.h"

PlannerPosPropertyNode::PlannerPosPropertyNode(QObject *parent):
    PropertyTreeNode(parent)
{
    setExpandable(true);

    PropertyTreeNode    *   node;

    QStringList key;
    key<<"x"<<"y"<<"z";
    for(int i=0;i<pnum;i++)
    {
        node = new PropertyTreeNode;
        node->setType(detailedNode);
        node->setValue(0.0);
        nodes[i]=node;
        insertChild(node);
        nodes[i]->setObjectName(key[i]);
        connect(nodes[i],SIGNAL(valueChanged(QVariant)),this,SLOT(subValueUpdate(QVariant)));
    }

    PlannerPos r;
    setValue(QVariant::fromValue(r));
    setType(rootNode);
}

QString PlannerPosPropertyNode::stringValue()
{
    PlannerPos  rect = value().value<PlannerPos>();
    return  QString("(%1,%2,%3)").arg(rect.x).arg(rect.y).arg(rect.z);
}

TreeNode::NodeType PlannerPosPropertyNode::type() const
{
    return  propertyNode;
}

void PlannerPosPropertyNode::subValueUpdate(const QVariant &Value)
{
    PlannerPos rect = value().value<PlannerPos>();
    PropertyTreeNode *node = dynamic_cast<PropertyTreeNode*>(sender());
    Q_ASSERT(node);
    if(node==nodes[0]){
        rect.x = Value.toDouble();
    }else if(node==nodes[1]){
        rect.y = Value.toDouble();
    }else if(node==nodes[2]){
        rect.z = Value.toDouble();
    }
    setValue(QVariant::fromValue(rect));
}

void PlannerPosPropertyNode::updateValue()
{
    PlannerPos   rect = value().value<PlannerPos>();
    nodes[0]->setValue(rect.x);
    nodes[1]->setValue(rect.y);
    nodes[2]->setValue(rect.z);
}

void PlannerPosPropertyNode::populateProperties()
{
    for(int i=0;i<pnum;i++)
    {
        nodes[i]->setWritable(metaProperty().isWritable());
        nodes[i]->setResetable(metaProperty().isResettable());
    }
}
