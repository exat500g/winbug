#include "transformpropertynode.h"
#include <QTransform>

TransformPropertyNode::TransformPropertyNode(QObject *parent):
    PropertyTreeNode(parent)
{
    setExpandable(true);

    PropertyTreeNode    *   node;

    for(int i=0;i<9;i++)
    {
        node = new PropertyTreeNode;
        node->setType(detailedNode);
        node->setValue(0.0);
        fmatrix[i]=node;
        insertChild(node);
        connect(fmatrix[i],SIGNAL(valueChanged(QVariant)),this,SLOT(subValueUpdate(QVariant)));
    }

    fmatrix[0]->setObjectName("m11");
    fmatrix[1]->setObjectName("m12");
    fmatrix[2]->setObjectName("m13");
    fmatrix[3]->setObjectName("m21");
    fmatrix[4]->setObjectName("m22");
    fmatrix[5]->setObjectName("m23");
    fmatrix[6]->setObjectName("m31");
    fmatrix[7]->setObjectName("m32");
    fmatrix[8]->setObjectName("m33");

    setValue(QTransform());
}

QString TransformPropertyNode::stringValue()
{
    QTransform  ftransform = value().value<QTransform>();
    return  QString("[%1 %2 %3][%4 %5 %6][%7 %8 %9]")
            .arg(ftransform.m11()).arg(ftransform.m12()).arg(ftransform.m13())
            .arg(ftransform.m21()).arg(ftransform.m22()).arg(ftransform.m23())
            .arg(ftransform.m31()).arg(ftransform.m32()).arg(ftransform.m33());
}

TreeNode::NodeType TransformPropertyNode::type() const
{
    return  propertyNode;
}

void TransformPropertyNode::subValueUpdate(const QVariant &Value)
{
    QTransform t = value().value<QTransform>();
    PropertyTreeNode *node = dynamic_cast<PropertyTreeNode*>(sender());

    qreal m[9]={t.m11(),t.m12(),t.m13(),t.m21(),t.m22(),t.m23(),t.m31(),t.m32(),t.m33()};

    for(int i=0;i<9;i++){
        if(fmatrix[i]==node){
            m[i]=Value.toDouble();
        }
    }
    t.setMatrix(m[0],m[1],m[2],m[3],m[4],m[5],m[6],m[7],m[8]);
    setValue(QVariant::fromValue(t));
}

void TransformPropertyNode::updateValue()
{
    QTransform   transform = value().value<QTransform>();
    fmatrix[0]->setValue(transform.m11());
    fmatrix[1]->setValue(transform.m12());
    fmatrix[2]->setValue(transform.m13());
    fmatrix[3]->setValue(transform.m21());
    fmatrix[4]->setValue(transform.m22());
    fmatrix[5]->setValue(transform.m23());
    fmatrix[6]->setValue(transform.m31());
    fmatrix[7]->setValue(transform.m32());
    fmatrix[8]->setValue(transform.m33());
}

void TransformPropertyNode::populateProperties()
{
    for(int i=0;i<9;i++)
    {
        fmatrix[i]->setWritable(metaProperty().isWritable());
        fmatrix[i]->setResetable(metaProperty().isResettable());
    }
}
