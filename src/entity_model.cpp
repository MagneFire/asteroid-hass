#include "entity_model.h"
#include <QDebug>

EntityModel::EntityModel(QObject *parent) : QAbstractListModel(parent)
{
}

void EntityModel::clear()
{
    beginResetModel();
    m_files.clear();
    endResetModel();
}

void EntityModel::addFile(const Entity &file)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_files << file;
    endInsertRows();
}

int EntityModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_files.count();
}

QVariant EntityModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_files.count())
    {
        return {};
    }

    const Entity &file = m_files[index.row()];
    if (role == DomainRole)
    {
        return file.domain();
    }
    if (role == NameRole)
    {
        return file.name();
    }
    if (role == IdRole)
    {
        return file.id();
    }
    if (role == StateRole)
    {
        return file.state();
    }
    if (role == InDashboardRole)
    {
        return file.inDashboard();
    }
    return {};
}

QHash<int, QByteArray> EntityModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DomainRole] = "domain";
    roles[NameRole] = "name";
    roles[IdRole] = "id";
    roles[StateRole] = "state";
    roles[InDashboardRole] = "inDashboard";
    return roles;
}