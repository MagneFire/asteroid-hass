#include "entity_model.h"
#include <QDebug>

EntityModel::EntityModel(QObject *parent)
: QAbstractListModel(parent)
{

}

void EntityModel::addFile(const Entity &file)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_files << file;
    endInsertRows();
}

int EntityModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_files.count();
}

QVariant EntityModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_files.count())
        return QVariant();

    const Entity &file = m_files[index.row()];
    if (role == DomainRole)
        return file.domain();
    else if (role == NameRole)
        return file.name();
    else if (role == IdRole)
        return file.id();
    else if (role == StateRole)
        return file.state();
    return QVariant();
}

QHash<int, QByteArray> EntityModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[DomainRole] = "domain";
    roles[NameRole] = "name";
    roles[IdRole] = "id";
    roles[StateRole] = "state";
    return roles;
}