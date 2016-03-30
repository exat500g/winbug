#include "BoundRectPropertyNode.h"
#include "global/type.h"

BoundRectPropertyNode::BoundRectPropertyNode(QObject *parent):
    PropertyTreeNode(parent)
{
    setExpandable(true);

    PropertyTreeNode    *   node;

    QStringList key;
    key<<"x1"<<"x2"<<"y1"<<"y2"<<"z1"<<"z2";
    for(int i=0;i<6;i++)
    {
        node = new PropertyTreeNode;
        node->setType(detailedNode);
        node->setValue(0.0);
        nodes[i]=node;
        insertChild(node);
        nodes[i]->setObjectName(key[i]);
        connect(nodes[i],SIGNAL(valueChanged(QVariant)),this,SLOT(subValueUpdate(QVariant)));
    }

    BoundRect r;
    setValue(QVariant::fromValue(r));
    setType(rootNode);
}

QString BoundRectPropertyNode::stringValue()
{
    BoundRect  rect = value().value<BoundRect>();
    return  QString("x:(%1~%2)  y:(%3~%4)  z:(%5~%6)")
            .arg(rect.x1).arg(rect.x2)
            .arg(rect.y1).arg(rect.y2)
            .arg(rect.z1).arg(rect.z2);
}

TreeNode::NodeType BoundRectPropertyNode::type() const
{
    return  propertyNode;
}

void BoundRectPropertyNode::subValueUpdate(const QVariant &Value)
{
    BoundRect rect = value().value<BoundRect>();
    PropertyTreeNode *node = dynamic_cast<PropertyTreeNode*>(sender());
    Q_ASSERT(node);
    if(node==nodes[0]){
        rect.x1 = Value.toDouble();
    }else if(node==nodes[1]){
        rect.x2 = Value.toDouble();
    }else if(node==nodes[2]){
        rect.y1 = Value.toDouble();
    }else if(node==nodes[3]){
        rect.y2 = Value.toDouble();
    }else if(node==nodes[4]){
        rect.z1 = Value.toDouble();
    }else if(node==nodes[5]){
        rect.z2 = Value.toDouble();
    }
    setValue(QVariant::fromValue(rect));
}

void BoundRectPropertyNode::updateValue()
{
    BoundRect rect = value().value<BoundRect>();
    nodes[0]->setValue(rect.x1);
    nodes[1]->setValue(rect.x2);
    nodes[2]->setValue(rect.y1);
    nodes[3]->setValue(rect.y2);
    nodes[4]->setValue(rect.z1);
    nodes[5]->setValue(rect.z2);
}

void BoundRectPropertyNode::populateProperties()
{
    for(int i=0;i<6;i++)
    {
        nodes[i]->setWritable(metaProperty().isWritable());
        nodes[i]->setResetable(metaProperty().isResettable());
    }
}
