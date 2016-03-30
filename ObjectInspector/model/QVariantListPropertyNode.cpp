#include "QVariantListPropertyNode.h"

QVariantListPropertyNode::QVariantListPropertyNode(QObject *parent):
    PropertyTreeNode(parent)
{
    setExpandable(true);
    setType(rootNode);
}

void QVariantListPropertyNode::populateProperties()
{
    /*for(int i=0;i<list.length();i++){
        PropertyTreeNode *node=new PropertyTreeNode;
        node->setObjectName(QString("%1").arg(i));
        node->setValue(0.0);
        node->setType(detailedNode);
        node->setWritable(metaProperty().isWritable());
        connect(node,SIGNAL(valueChanged(QVariant)),this,SLOT(subValueUpdate(QVariant)));
        insertChild(node);
        listNode.append(node);
    }*/
}

QString QVariantListPropertyNode::stringValue()
{
    return  QString("[...]");
}

TreeNode::NodeType QVariantListPropertyNode::type() const
{
    return  detailedNode;
}

void QVariantListPropertyNode::subValueUpdate(const QVariant &Value)
{
    Q_UNUSED(Value)
    /*QVariantList   list = value().toList();
    PropertyTreeNode *node = dynamic_cast<PropertyTreeNode*>(sender());
    int i=listNode.indexOf(node);*/
}

void QVariantListPropertyNode::updateValue()
{
    QVariantList   list = value().toList();
    if(listNode.size() < list.size()){
        for(int i=listNode.size() ; i<list.size() ;i++){
            PropertyTreeNode *node=new PropertyTreeNode;
            node->setObjectName(QString("[%1]=").arg(i));
            node->setValue(0.0);
            node->setType(detailedNode);
            node->setWritable(metaProperty().isWritable());
            connect(node,SIGNAL(valueChanged(QVariant)),this,SLOT(subValueUpdate(QVariant)));
            insertChild(node);
            listNode.append(node);
        }
    }
    foreach(PropertyTreeNode* node,listNode){
        node->setValue(list[listNode.indexOf(node)]);
    }
}






