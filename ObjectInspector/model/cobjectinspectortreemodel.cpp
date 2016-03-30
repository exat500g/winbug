#include    "cobjectinspectortreemodel.h"
#include    <QMetaProperty>
#include    <QStringList>
#include    <QColor>
#include    "flagpropertynode.h"
#include    "sizepropertynode.h"
#include    "enumproperty.h"
#include    "floatsizepropertynode.h"
#include    "cpropertycommand.h"
#include    "cpropertyresetcommand.h"
#include    "transformpropertynode.h"
#include    "BoundRectPropertyNode.h"
#include    "PlannerPosPropertyNode.h"
#include    "QVariantListPropertyNode.h"
#include    <QDebug>
#include    "global/type.h"

const static qint32 BoundRectType = qMetaTypeId<BoundRect>();
const static qint32 PlannerPosType = qMetaTypeId<PlannerPos>();

CObjectInspectorTreeModel::CObjectInspectorTreeModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    frootNode   =   new TreeNode(this);
    frootNode->setType(TreeNode::rootNode);
    fmode = pmDesignableAll;
    fsupportedTypes<<QVariant::Int;
    fsupportedTypes<<QVariant::LongLong;
    fsupportedTypes<<QVariant::UInt;
    fsupportedTypes<<QVariant::ULongLong;
    fsupportedTypes<<QVariant::Size;
    fsupportedTypes<<QVariant::SizeF;
    fsupportedTypes<<QVariant::String;
    fsupportedTypes<<QVariant::Color;
    fsupportedTypes<<QVariant::Font;
    fsupportedTypes<<QVariant::Bool;
    fsupportedTypes<<QVariant::Double;
    fsupportedTypes<<QVariant::Transform;
    fsupportedTypes<<BoundRectType;
    fsupportedTypes<<PlannerPosType;
    fsupportedTypes<<QVariant::Invalid;
    fsupportedTypes<<QVariant::List;
}

CObjectInspectorTreeModel::~CObjectInspectorTreeModel()
{
    delete  frootNode;
}

Qt::ItemFlags CObjectInspectorTreeModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags   oldFlags = QAbstractItemModel::flags(index);
    if(index.isValid())
    {
        TreeNode * node = nodeForIndex(index);
        //qDebug()<<"flags name="<<node->objectName()<<"childint="<<node->childCount()<<"type="<<node->type();
        if(index.column()==1 && node->type()>=TreeNode::propertyNode){
            oldFlags |= Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
        }else{
            oldFlags &= ~Qt::ItemIsEditable;
        }
    }
    return  oldFlags;
}

QVariant CObjectInspectorTreeModel::data(const QModelIndex &index, int role) const
{
    if(!frootNode || !index.isValid() || index.column()<0 || index.column()>=maxColumn)
        return  QVariant();

    TreeNode * node = nodeForIndex(index);

    if(node && node!=frootNode)
    {
        switch (role) {
        case Qt::DisplayRole:
        switch (index.column())
        {
            case    columnName:
                return  node->objectName();
            case    columnValue:
                return  node->stringValue();
            default:
                break;
        }
        break;
        case Qt::EditRole:
        {
            if(index.column()==columnValue)
            {
                PropertyTreeNode    *   propertyNode = qobject_cast<PropertyTreeNode*>(node);
                if(propertyNode)
                {
                    return  propertyNode->value();
                }
            }
        }
        break;
        case CObjectInspectorTreeModel::TreeNodeRole:
        switch (index.column())
        {
        case    columnValue:
            {
                PropertyTreeNode    *   propertyNode = qobject_cast<PropertyTreeNode*>(node);
                return  propertyNode?QVariant::fromValue(propertyNode):QVariant();
            }
        default:
            break;
        }
        break;
        case    Qt::ToolTipRole:
        {
            PropertyTreeNode    *   propertyNode = qobject_cast<PropertyTreeNode*>(node);
            if(propertyNode)
                return  QString(propertyNode->value().typeName());
        }
            break;
        default:
            break;
        }
    }

    return  QVariant();
}

QVariant CObjectInspectorTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation==Qt::Horizontal && role==Qt::DisplayRole && section>=0 && section<maxColumn)
    {
        static  const   QString headers[maxColumn] = {trUtf8("Property"),trUtf8("Value")};
        return  headers[section];
    }
    return  QVariant();
}

int CObjectInspectorTreeModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid() && parent.column()!=0)
        return  0;
    TreeNode * node = nodeForIndex(parent);
    return  node?node->childCount():0;
}

int CObjectInspectorTreeModel::columnCount(const QModelIndex &parent) const
{
    return  parent.isValid() && parent.column()!=0 ? 0: maxColumn;
}

QModelIndex CObjectInspectorTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!frootNode || row<0 || column<0 || column>=maxColumn || (parent.isValid() && parent.column()!=0))
        return  QModelIndex();

    TreeNode    *   parentNode = nodeForIndex(parent);
    Q_ASSERT(parentNode);
    if(TreeNode * item = dynamic_cast<TreeNode*>(parentNode->childAt(row)))
    {
        QModelIndex createdIndex = createIndex(row,column,item);
        item->setModelIndex(createdIndex);
        connect(item,SIGNAL(updateModel(QModelIndex,QModelIndex)),this,SIGNAL(dataChanged(QModelIndex,QModelIndex)));
        return  createdIndex;
    }
    return  QModelIndex();
}

QModelIndex CObjectInspectorTreeModel::parent(const QModelIndex &child) const
{
    if(!child.isValid())
        return  QModelIndex();
    if(TreeNode * childNode = nodeForIndex(child))
    {
        if(TreeNode * parentNode = dynamic_cast<TreeNode*>(childNode->parent()))
        {
            if(parentNode == frootNode)
                return  QModelIndex();
            if(TreeNode * grandParentNode = dynamic_cast<TreeNode*>(parentNode->parent()))
            {
                int row = grandParentNode->childRow(parentNode);
                return  createIndex(row,0,parentNode);
            }
        }
    }
    return  QModelIndex();
}

bool CObjectInspectorTreeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    Q_UNUSED(section)
    Q_UNUSED(orientation)
    Q_UNUSED(value)
    Q_UNUSED(role)
    return  false;
}

bool CObjectInspectorTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && index.column()==columnValue)
    {
        if(value.isValid())
        switch (role)
        {
        case Qt::EditRole:
        {
            PropertyTreeNode    *   propertyNode = qobject_cast<PropertyTreeNode*>(nodeForIndex(index));
            if(propertyNode && !propertyNode->isExpandable())
            {
                propertyNode->setValue(value);
                emit    dataChanged(index,index);
                return  true;
            }
        }
            break;
        default:
            break;
        }
    }
    return  false;
}

TreeNode * CObjectInspectorTreeModel::nodeForIndex(const QModelIndex &index) const
{
    if(index.isValid())
    {
        if(TreeNode * node = static_cast<TreeNode*>(index.internalPointer()))
            return  node;
    }
    return  frootNode;
}

void CObjectInspectorTreeModel::setObjects(const QList<QObject*> & ObjectList)
{
    if(fobjects!=ObjectList)
    {
        beginResetModel();
        frootNode->clear();
        fobjects = ObjectList;
        if(!fobjects.isEmpty())
        {
            filterProps();
        }
        endResetModel();
    }
}

const QList<QObject*> & CObjectInspectorTreeModel::objects() const
{
    return  fobjects;
}

TreeNode *CObjectInspectorTreeModel::rootNode() const
{
    return  frootNode;
}

void CObjectInspectorTreeModel::clear()
{
    beginResetModel();
    fobjects.clear();
    frootNode->clear();
    endResetModel();
}

void CObjectInspectorTreeModel::update()
{
    beginResetModel();
    endResetModel();
}

void CObjectInspectorTreeModel::setMode(CObjectInspectorTreeModel::PropertyMode Mode)
{
    if(fmode!=Mode)
    {
        beginResetModel();
        fmode = Mode;
        frootNode->clear();
        filterProps();
        endResetModel();
    }
}

void CObjectInspectorTreeModel::setValueForObjects(const QVariant &Value)
{
    PropertyTreeNode    *   senderNode = qobject_cast<PropertyTreeNode*>(sender());
    if(senderNode && !fobjects.isEmpty())
    {
        const QString & propertyName = senderNode->targetPropertyName();
        foreach (QObject * object, fobjects)
        {
            object->setProperty(qPrintable(propertyName),Value);
        }
    }
}

void CObjectInspectorTreeModel::setValueForObjects(const QString &propertyName, const QVariant &propertyValue)
{
    if(fobjects.isEmpty())
        return;

    const   char    *   property_c_string = qPrintable(propertyName);
    foreach (QObject * object, fobjects)
    {
        object->setProperty(property_c_string,propertyValue);
    }
}

void CObjectInspectorTreeModel::resetValuesForObjects()
{
    PropertyTreeNode    *   senderNode = qobject_cast<PropertyTreeNode*>(sender());
    if(senderNode && !fobjects.isEmpty())
    {
        int index;
        const QString & propertyName = senderNode->targetPropertyName();
        foreach (QObject * object, fobjects)
        {
            index = object->metaObject()->indexOfProperty(qPrintable(propertyName));
            if(index>=0)
            {
                object->metaObject()->property(index).reset(object);
            }
        }
    }
}

void CObjectInspectorTreeModel::enumerateProps(QObject *object, CObjectInspectorTreeModel::PropertyMode Mode, QStringList &propNameList)
{
    const   QMetaObject *   metaObject = object->metaObject();
    switch (Mode)
    {
    case pmAll:
        for(int propIndex = 0;propIndex<metaObject->propertyCount();++propIndex)
        {
            QMetaProperty   currentMetaProperty = metaObject->property(propIndex);
                propNameList<<currentMetaProperty.name();
        }
        break;
    case    pmDesignableAll:
        for(int propIndex = 0;propIndex<metaObject->propertyCount();++propIndex)
        {
            QMetaProperty   currentMetaProperty = metaObject->property(propIndex);
            if(currentMetaProperty.isDesignable())
                propNameList<<currentMetaProperty.name();
        }
        break;
    case    pmDesignableThisOnly:
        for(int propIndex = metaObject->propertyOffset();propIndex<metaObject->propertyCount();++propIndex)
        {
            QMetaProperty   currentMetaProperty = metaObject->property(propIndex);
            if(currentMetaProperty.isDesignable())
                propNameList<<currentMetaProperty.name();
        }
        break;
    case    pmThisOnly:
        for(int propIndex = metaObject->propertyOffset();propIndex<metaObject->propertyCount();++propIndex)
        {
            QMetaProperty   currentMetaProperty = metaObject->property(propIndex);
                propNameList<<currentMetaProperty.name();
        }
        break;
    }
}

void CObjectInspectorTreeModel::filterProps()
{
    if(fobjects.isEmpty())
        return;

    QList<QStringList>    fpropsList;
    QStringList           fresultList;
    //fill fpropsList by mode
    foreach (QObject * object, fobjects)
    {
        QStringList   list;
        enumerateProps(object,fmode,list);
        fpropsList<<list;
    }

    //Now we cut the differing, so that only matching
    bool    shouldAdd = false;

    for(QList<QStringList>::const_iterator it = fpropsList.constBegin(); it!=fpropsList.constEnd(); ++it)
    {
        const QStringList & currentPropList = *it;
        if(currentPropList.isEmpty())
            continue;

        foreach(const QString & currentProperty, currentPropList)
        {
            for(QList<QStringList>::const_iterator jt=it; jt!=fpropsList.constEnd(); ++jt)
            {
                shouldAdd = true;

                if(jt == it)
                    continue;

                const QStringList &   oppositeList = *jt;

                if(!oppositeList.contains(currentProperty))
                {
                    shouldAdd = false;
                    break;
                }
            }

            if(shouldAdd && !fresultList.contains(currentProperty))
            {
                fresultList<<currentProperty;
            }
        }
    }
    //qDebug()<<"fresultList = "<<fresultList;

    //now take place on all sites and find the properties that have the same value
    QStringList filteredList;
    QVariant    prevValue;
    QObject *   object;
    QString     propertyName;
    int         indexOfProperty;

    for(QStringList::iterator it=fresultList.begin();it!=fresultList.end();it++)
    {
        shouldAdd = true;
        QList<QObject*>::iterator first = fobjects.begin();

        object = *first;

        const QMetaObject   *   firstMetaObject = object->metaObject();
        propertyName = *it;
        indexOfProperty = firstMetaObject->indexOfProperty(qPrintable(propertyName));
        if(indexOfProperty<0)
            continue;

        QMetaProperty   metaProperty = firstMetaObject->property(indexOfProperty);
        if(!fsupportedTypes.contains(metaProperty.userType())){
            qDebug()<<"unsupported types ="<<propertyName<<metaProperty.userType();
            continue;
        }

        prevValue = object->property(qPrintable(propertyName));
        first++;
        for(QList<QObject*>::iterator jt=first;jt!=fobjects.end();jt++)
        {
            if((*jt)->property(qPrintable(propertyName))!=prevValue)
            {
                shouldAdd = false;
                break;
            }
        }
        if(shouldAdd)
            filteredList<<*it;
    }

    fresultList.clear();

    //Now you can publish properties
    int index;
    const QMetaObject *   firstObject = fobjects.first()->metaObject();

    PropertyTreeNode    *   propertyNode;
    QObject *   fobject = objects().first();
    foreach (const QString & propName, filteredList)
    {
        index = firstObject->indexOfProperty(qPrintable(propName));
        if(index>=0)
        {
            QMetaProperty           metaProp = firstObject->property(index);
            if(!metaProp.isWritable())
                continue;

            propertyNode = NULL;
            switch (metaProp.userType())
            {
            case    QVariant::Invalid:
            case    QVariant::Int:
            case    QVariant::LongLong:
            case    QVariant::UInt:
            case    QVariant::ULongLong:
            {
                if(metaProp.isFlagType())
                {
                    propertyNode = new FlagPropertyNode;
                }
                else
                if(metaProp.isEnumType())
                {
                    propertyNode = new EnumProperty;
                }
                else
                {
                    propertyNode = new PropertyTreeNode;
                    propertyNode->setEditable();
                }
            }
                break;
            case    QVariant::Size:
                propertyNode    =   new SizePropertyNode;
                break;
            case    QVariant::SizeF:
                propertyNode    =   new FloatSizePropertyNode;
                break;
            case    QVariant::String:
            case    QVariant::Color:
            case    QVariant::Font:
            {
                propertyNode    =   new PropertyTreeNode;
                propertyNode->setEditable();
            }
                break;
            case    QVariant::Bool:
            case    QVariant::Double:
                propertyNode    =   new PropertyTreeNode;
                break;
            case    QVariant::Transform:
                propertyNode    =   new TransformPropertyNode;
                break;
            case QVariant::List:
                propertyNode    =   new QVariantListPropertyNode;
                break;
            default:
                if(metaProp.userType()==BoundRectType){
                    propertyNode    =   new BoundRectPropertyNode;
                    break;
                }else if(metaProp.userType()==PlannerPosType){
                    propertyNode    =   new PlannerPosPropertyNode;
                    break;
                }
                continue;
                break;
            }
            if(propertyNode)
            {
                propertyNode->setEditable(metaProp.isWritable());
                propertyNode->setResetable(metaProp.isResettable());
                propertyNode->setType(TreeNode::propertyNode);
                propertyNode->setObjectName(propName);
                propertyNode->setTargetPropertyName(propName);
                propertyNode->setMetaProperty(metaProp);
                QVariant v = fobject->property(qPrintable(propName));
                propertyNode->setValue(v);
                connect(propertyNode,SIGNAL(valueChanged(QVariant)),this,SLOT(setValueForObjects(QVariant)));
                connect(propertyNode,SIGNAL(resetProperty()),this,SLOT(resetValuesForObjects()));
                frootNode->insertChild(propertyNode);
            }
        }
    }
}
